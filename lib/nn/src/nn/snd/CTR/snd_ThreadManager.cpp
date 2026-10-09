#include "nn/snd/CTR/snd_ThreadManager.h"
#include "nn/applet/CTR/CTR_Api.h"
#include "nn/os/os_Atomic.h"
#include "nn/snd/CTR/CTR_Api.h"
#include "nn/snd/CTR/snd_MasterManager.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace snd {
namespace CTR {
namespace {
// Result 0x0820ABF9: already done (info level)
const bit32 RESULT_ALREADY_DONE = 0x0820ABF9;
// Result 0xE0E0ABEE: invalid state
const bit32 RESULT_INVALID_STATE = 0xE0E0ABEE;
// the priority of the sound thread of a system applet on the system core
const s32 SYSTEM_APPLET_PRIORITY = 0x5109D500;
} // namespace

// 0x00AECE34 (name is ours)
bool g_IsSoundThreadOnApplicationCore;

// (inline in GetInstance)
nn::snd::CTR::ThreadManager::ThreadManager() : m_Tick(0)
{
    m_IsStarted = false;
    m_IsUserThreadStarted = false;
    m_IsTickCounterEnabled = false;
    m_CoreNo = 0;
}

// 0x00461F60 | nintendogs:callgraph [confirmed by fefates] [tier A]
ThreadManager* nn::snd::CTR::ThreadManager::GetInstance()
{
    // 0x00AECE38 (name is ours)
    static ThreadManager s_Instance;
    return &s_Instance;
}

// 0x00462008 | nintendogs:callgraph [confirmed by fefates] [tier A]
nn::Result nn::snd::CTR::ThreadManager::StartSoundThread(Callback userCallback, uptr userArg, uptr stackBuffer, u32 stackSize, s32 priority,
                                                          s32 coreNo)
{
    if (m_IsStarted) {
        return nn::Result(RESULT_ALREADY_DONE);
    }
    const uptr stackBottom = stackBuffer + stackSize;
    if (nn::applet::CTR::IsSystemApplet() && coreNo == 1) {
        priority = SYSTEM_APPLET_PRIORITY;
    }
    m_Lock.Initialize();
    nn::Result result = m_Thread.TryStart(&SoundThreadFunc, static_cast<uptr>(0), stackBottom, priority, coreNo);
    if (result.IsFailure()) {
        m_Lock.Finalize();
        return result;
    }
    m_Callback = NULL;
    m_IsStarted = true;
    m_CallbackArg = 0;
    m_UserCallback = userCallback;
    m_UserCallbackArg = userArg;
    g_IsSoundThreadOnApplicationCore = coreNo == 0;
    m_Tick = 0;
    m_CoreNo = coreNo;
    return result;
}

// 0x0046212C | nintendogs:bytes [confirmed by fefates] [tier A]
nn::Result nn::snd::CTR::ThreadManager::StartSoundThread(const ThreadParameter* parameter, Callback callback, uptr arg,
                                                          const ThreadParameter* userParameter, Callback userCallback, uptr userArg,
                                                          int coreNo)
{
    nn::Result result = StartSoundThread(userCallback, userArg, parameter->m_StackBuffer, parameter->m_StackSize, parameter->m_Priority, coreNo);
    if (result.IsFailure()) {
        return result;
    }
    m_Callback = callback;
    m_CallbackArg = arg;
    if (userParameter != NULL) {
        result = StartUserSoundThread(userParameter->m_StackBuffer, userParameter->m_StackSize, userParameter->m_Priority);
        if (result.IsFailure()) {
            FinalizeSoundThread();
            return result;
        }
    }
    return nn::Result();
}

// 0x004621D8 (name is ours)
nn::os::Tick nn::snd::CTR::ThreadManager::GetSoundThreadTick() const
{
    return nn::os::Tick(m_Tick);
}

// 0x004621F4 | nintendogs:callgraph [confirmed by fefates] [tier A]
void nn::snd::CTR::ThreadManager::FinalizeSoundThread()
{
    if (m_IsUserThreadStarted) {
        m_IsUserThreadRunning = false;
        m_UserThread.Join();
        m_UserThread.Finalize();
        m_IsUserThreadStarted = false;
        nn::os::detail::DataSynchronizationBarrier();
        m_DoneEvent.Signal();
    }
    if (m_IsStarted) {
        m_IsRunning = false;
        m_Thread.Join();
        m_Thread.Finalize();
        m_Callback = NULL;
        m_UserCallback = NULL;
        m_CoreNo = 0;
        m_Lock.Finalize();
        g_IsSoundThreadOnApplicationCore = true;
        m_IsStarted = false;
    }
}

// 0x004622D8 (name is ours)
void nn::snd::CTR::ThreadManager::SoundThreadFunc(uptr arg)
{
    GetInstance()->SoundThreadFuncImpl(arg);
}

// 0x004622F0 | fefates:bytes [tier B]
void nn::snd::CTR::ThreadManager::SoundThreadFuncImpl(uptr)
{
    m_IsRunning = true;
    do {
        nn::os::Tick syncTick(0);
        s64 start = 0;
        if (m_IsTickCounterEnabled) {
            WaitForDspSync(&syncTick);
            start = nn::svc::GetSystemTick();
        } else {
            WaitForDspSync();
        }
        // the user thread takes the frame if it has something to do
        const bool hasUserCallback = m_UserCallback != NULL;
        bool isUserThread = m_IsUserThreadStarted;
        bool hasAuxCallback = false;
        if (isUserThread) {
            AuxCallback auxCallback[AUX_BUS_COUNT];
            uptr auxArg[AUX_BUS_COUNT];
            g_MasterManager.GetAuxCallback(AUX_BUS_0, &auxCallback[0], &auxArg[0]);
            g_MasterManager.GetAuxCallback(AUX_BUS_1, &auxCallback[1], &auxArg[1]);
            hasAuxCallback = auxCallback[0] != NULL || auxCallback[1] != NULL;
        }
        isUserThread = isUserThread && (hasAuxCallback || hasUserCallback);
        if (isUserThread) {
            nn::os::detail::DataSynchronizationBarrier();
            m_UserThreadEvent.Signal();
        }
        if (m_CoreNo == 0 && m_UserCallback != NULL) {
            m_UserCallback(m_UserCallbackArg);
        }
        if (m_Callback != NULL) {
            m_Lock.Enter();
            m_Callback(m_CallbackArg);
            m_Lock.Exit();
        }
        if (isUserThread) {
            m_DoneEvent.Wait();
        }
        m_Lock.Enter();
        SendParameterToDsp();
        m_Lock.Exit();
        if (m_IsTickCounterEnabled) {
            m_Tick = syncTick + (nn::svc::GetSystemTick() - start);
        }
    } while (m_IsRunning);
}

// 0x00462474 | nintendogs:bytes [confirmed by fefates] [tier A]
nn::Result nn::snd::CTR::ThreadManager::StartUserSoundThread(uptr stackBuffer, u32 stackSize, s32 priority)
{
    if (!m_IsStarted) {
        return nn::Result(RESULT_INVALID_STATE);
    }
    if (m_IsUserThreadStarted) {
        return nn::Result(RESULT_ALREADY_DONE);
    }
    // only with the sound thread on the system core
    if (m_CoreNo != 1) {
        return nn::Result(RESULT_INVALID_STATE);
    }
    m_DoneEvent.Initialize(false);
    m_UserThreadEvent.Initialize(false);
    nn::Result result = m_UserThread.TryStart(&UserSoundThreadFunc, static_cast<uptr>(0), stackBuffer + stackSize, priority, 0);
    m_IsUserThreadStarted = result.IsSuccess();
    return result;
}

// 0x00462584 (name is ours)
void nn::snd::CTR::ThreadManager::EnableSoundThreadTickCounter(bool isEnabled)
{
    if (m_CoreNo == 0) {
        m_IsTickCounterEnabled = isEnabled;
    }
}

// 0x00462594 | nintendogs:bytes [confirmed by fefates] [tier A]
nn::snd::CTR::ThreadManager::~ThreadManager()
{
}

} // namespace CTR
} // namespace snd
} // namespace nn
