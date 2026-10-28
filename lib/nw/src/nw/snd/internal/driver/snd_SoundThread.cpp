#include "nw/snd/internal/driver/snd_SoundThread.h"

#include <cstring>

#include "nn/snd/CTR/CTR_Api.h"
#include "nw/snd/internal/Util/Util_Api.h"
#include "nw/snd/internal/driver/snd_ChannelManager.h"
#include "nw/snd/internal/driver/snd_HardwareManager.h"
#include "nw/snd/internal/driver/snd_SoundThread_PlayerCallback.h"
#include "nw/snd/internal/driver/snd_SoundThread_SoundFrameCallback.h"
#include "nw/snd/internal/driver/snd_VoiceManager.h"
#include "nw/snd/internal/snd_DriverCommandManager.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
namespace {
// the load in percent of a microsecond of a frame
const f32 LOAD_PERCENT_PER_USEC = 100.0f / SoundThread::FRAME_LENGTH_USEC;
// the core of the user thread
const int USER_THREAD_CORE_NO = 1;

// (inline) the sound frame callbacks
template <typename TFunc>
void ForEachSoundFrameCallback(SoundThread::SoundFrameCallbackList& list, TFunc func)
{
    for (ut::internal::LinkListImpl::iterator it = list.begin(); it != list.end();) {
        SoundThread::SoundFrameCallback* pCallback = SoundThread::SoundFrameCallbackList::GetPointerFromNode(it.GetNode());
        ++it;
        func(pCallback);
    }
}
} // namespace

// 0x00140F34 | nintendogs:callseq-callee [tier A]
void nw::snd::internal::driver::SoundThread::UnregisterPlayerCallback(PlayerCallback* pCallback)
{
    m_PlayerCallbackList.Erase(pCallback);
}

// 0x004CB3DC | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SoundThread::Initialize()
{
    m_CriticalSection.Initialize();
}

// 0x004CB3E4 | nintendogs:callgraph [tier A]
nw::snd::internal::driver::SoundThread& nw::snd::internal::driver::SoundThread::GetInstance()
{
    // 0x00AE58B8
    static SoundThread s_SoundThread;
    return s_SoundThread;
}

// 0x004CB43C | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SoundThread::SoundThreadFunc(uptr arg)
{
    SoundThread* pThread = reinterpret_cast<SoundThread*>(arg);
    if (pThread->m_IsEnableGetTick) {
        nn::os::Tick tick;
        tick = nn::snd::CTR::GetSoundThreadTick();
        if (pThread->m_ProfileIndex < pThread->m_ProfileCount) {
            pThread->m_pProfiles[pThread->m_ProfileIndex].tick = tick;
            ++pThread->m_ProfileIndex;
        }
        pThread->CalcProcessCost(tick);
    }
    pThread->FrameProcess();
}

// 0x004CB4B8 | fefates:bytes [tier B]
void nw::snd::internal::driver::SoundThread::FrameProcess()
{
    nn::os::CriticalSection::ScopedLock lock(m_CriticalSection);

    for (ut::internal::LinkListImpl::iterator it = m_SoundFrameCallbackList.begin(); it != m_SoundFrameCallbackList.end();) {
        SoundFrameCallback* pCallback = SoundFrameCallbackList::GetPointerFromNode(it.GetNode());
        ++it;
        pCallback->OnBeginSoundFrame();
    }
    for (ut::internal::LinkListImpl::iterator it = m_PlayerCallbackList.begin(); it != m_PlayerCallbackList.end();) {
        PlayerCallback* pCallback = PlayerCallbackList::GetPointerFromNode(it.GetNode());
        ++it;
        pCallback->OnUpdateBuffer();
    }

    HardwareManager::GetInstance().Update();
    DriverCommandManager::GetInstance().ProcessCommand();
    DriverCommandManager::GetInstanceForTaskThread().ProcessCommand();

    for (ut::internal::LinkListImpl::iterator it = m_PlayerCallbackList.begin(); it != m_PlayerCallbackList.end();) {
        PlayerCallback* pCallback = PlayerCallbackList::GetPointerFromNode(it.GetNode());
        ++it;
        pCallback->OnUpdateFrame();
    }
    ChannelManager::GetInstance().UpdateAllChannel();
    Util::CalcRandom();
    VoiceManager::GetInstance().UpdateAllVoices();

    for (ut::internal::LinkListImpl::iterator it = m_SoundFrameCallbackList.begin(); it != m_SoundFrameCallbackList.end();) {
        SoundFrameCallback* pCallback = SoundFrameCallbackList::GetPointerFromNode(it.GetNode());
        ++it;
        pCallback->OnEndSoundFrame();
    }
}

// 0x004CB620 | nintendogs:callgraph [tier A]
void nw::snd::internal::driver::SoundThread::CalcProcessCost(const nn::os::Tick& tick)
{
    if (!m_IsEnableGetTick) {
        return;
    }
    m_ProcessTick += static_cast<s64>(tick);
    ++m_ProcessCount;
    m_DspCycles += nn::snd::CTR::GetDspCycles();
    if (!m_IsLoadStatEnabled) {
        return;
    }

    const f32 load = static_cast<f32>(tick.ToTimeSpan().GetMicroSeconds()) * LOAD_PERCENT_PER_USEC;
    if (load < 0.0f || load > 100.0f) {
        ++m_LoadStat.overloadCount;
    }
    if (m_LoadStat.maxLoad < load) {
        m_LoadStat.maxLoad = load;
    }
    const int percent = static_cast<int>(load);
    if (percent < LOAD_HISTOGRAM_SIZE) {
        ++m_LoadStat.histogram[percent];
    }
    ++m_LoadStat.frameCount;
    m_LoadStat.totalLoad += load;

    if (m_pLoadProfiles != nullptr && m_LoadProfileCount > m_LoadProfileIndex) {
        LoadProfile& profile = m_pLoadProfiles[m_LoadProfileIndex];
        profile.voiceCount = VoiceManager::GetInstance().GetVoiceCount();
        profile.noteOnCount = m_NoteOnCount;
        profile.load = load;
        ++m_LoadProfileIndex;
    }
    m_NoteOnCount = 0;
}

// 0x004CB854 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SoundThread::SetProfileBuffer(SoundThreadProfile* pProfiles, int count)
{
    nn::os::CriticalSection::ScopedLock lock(m_CriticalSection);
    if (!m_IsEnableGetTick || pProfiles == nullptr) {
        return;
    }
    m_ProfileIndex = 0;
    m_pProfiles = pProfiles;
    m_ProfileCount = count;
}

// 0x004CB8A8 | nintendogs:callseq [tier A]
bool nw::snd::internal::driver::SoundThread::CreateSoundThread(uptr stackBuffer, u32 stackSize, int priority, uptr userStackBuffer, u32 userStackSize,
                                                               int userPriority, int coreNo, bool isEnableGetTick)
{
    if (!m_IsCreated) {
        m_IsCreated = true;
        m_IsEnableGetTick = isEnableGetTick;
        m_IsFinalizing = false;
    }
    m_CoreNo = coreNo;

    nn::snd::CTR::ThreadParameter parameter;
    parameter.m_StackBuffer = stackBuffer;
    parameter.m_StackSize = stackSize;
    parameter.m_Priority = priority;
    nn::snd::CTR::ThreadParameter userParameter;
    userParameter.m_StackBuffer = userStackBuffer;
    userParameter.m_StackSize = userStackSize;
    userParameter.m_Priority = userPriority;

    const nn::snd::CTR::ThreadParameter* pUserParameter = nullptr;
    if (coreNo == USER_THREAD_CORE_NO && userStackBuffer != 0) {
        pUserParameter = &userParameter;
    }
    const nn::Result result = nn::snd::CTR::StartSoundThread(&parameter, SoundThreadFunc, reinterpret_cast<uptr>(&GetInstance()), pUserParameter,
                                                             UserThreadCallback, reinterpret_cast<uptr>(&GetInstance()), coreNo);
    nn::snd::CTR::EnableSoundThreadTickCounter(isEnableGetTick);
    return result.IsSuccess();
}

// 0x004CB9E4 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SoundThread::ClearProfileBuffer()
{
    nn::os::CriticalSection::ScopedLock lock(m_CriticalSection);
    m_pProfiles = nullptr;
    m_ProfileCount = 0;
    m_ProfileIndex = 0;
}

// 0x004CBA18 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SoundThread::UserThreadCallback(uptr arg)
{
    SoundThread* pThread = reinterpret_cast<SoundThread*>(arg);
    if (pThread->m_pUserCallback != nullptr) {
        pThread->m_pUserCallback(pThread->m_UserCallbackArg);
    }
}

// 0x004CBA38 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SoundThread::RegisterPlayerCallback(PlayerCallback* pCallback)
{
    m_PlayerCallbackList.PushBack(pCallback);
}

// 0x004CBA4C | fefates:bytes [tier B]
void nw::snd::internal::driver::SoundThread::RegisterSoundFrameCallback(SoundFrameCallback* pCallback)
{
    nn::os::CriticalSection::ScopedLock lock(m_CriticalSection);
    m_SoundFrameCallbackList.PushBack(pCallback);
}

// 0x004CBA80 | fefates:bytes [tier B]
void nw::snd::internal::driver::SoundThread::UnregisterSoundFrameCallback(SoundFrameCallback* pCallback)
{
    nn::os::CriticalSection::ScopedLock lock(m_CriticalSection);
    m_SoundFrameCallbackList.Erase(pCallback);
}

// 0x004CBAB0 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SoundThread::Destroy()
{
    if (!m_IsCreated) {
        return;
    }
    m_IsFinalizing = true;
    nn::snd::CTR::FinalizeSoundThread();
    m_IsCreated = false;
}

// 0x004CBADC | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SoundThread::Finalize()
{
    Destroy();
    for (ut::internal::LinkListImpl::iterator it = m_PlayerCallbackList.begin(); it != m_PlayerCallbackList.end();) {
        PlayerCallback* pCallback = PlayerCallbackList::GetPointerFromNode(it.GetNode());
        ++it;
        pCallback->OnShutdown();
    }
    m_CriticalSection.Finalize();
}

// 0x004CBB44 | nintendogs:bytes [tier A]
nw::snd::internal::driver::SoundThread::SoundThread()
    : m_ProcessTick(0), m_ProcessCount(0), m_DspCycles(0), m_IsLoadStatEnabled(false), m_pProfiles(nullptr), m_ProfileCount(0), m_ProfileIndex(0),
      m_pUserCallback(nullptr), m_UserCallbackArg(0), m_CoreNo(0), m_IsCreated(false), m_Unknown0x1F5(false), m_IsEnableGetTick(false),
      m_IsFinalizing(false), m_pLoadProfiles(nullptr), m_NoteOnCount(0), m_LoadProfileCount(0), m_LoadProfileIndex(0)
{
    m_LoadStat.totalLoad = 0.0f;
    m_LoadStat.maxLoad = 0.0f;
    m_LoadStat.overloadCount = 0;
    m_LoadStat.frameCount = 0;
    std::memset(m_LoadStat.histogram, 0, sizeof(m_LoadStat.histogram));
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
