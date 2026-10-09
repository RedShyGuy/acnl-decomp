// snd_Api.cpp in the original (__sti___11_snd_Api_cpp)

#include "nn/snd/CTR/CTR_Api.h"
#include "nn/dsp/CTR/CTR_Api.h"
#include "nn/fnd/fnd_TimeSpan.h"
#include "nn/os/os_Atomic.h"
#include "nn/os/os_LightEvent.h"
#include "nn/os/os_Thread.h"
#include "nn/snd/CTR/snd_DspFxManager.h"
#include "nn/snd/CTR/snd_Dspsnd.h"
#include "nn/snd/CTR/snd_MasterManager.h"
#include "nn/snd/CTR/snd_ThreadManager.h"
#include "nn/snd/CTR/snd_VoiceManager.h"
#include "nn/srv/srv_Api.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace snd {
namespace CTR {
namespace {
// Result 0x0820ABF9: already done (info level)
const bit32 RESULT_ALREADY_INITIALIZED = 0x0820ABF9;

// the headphones of the shared page (1: connected)
volatile bool* const HEADPHONE_STATUS = reinterpret_cast<volatile bool*>(0x1FF810C0);

// the state of the library (names are ours)
struct State
{
    bool m_IsInitialized;           // 0x0
    bool m_IsSleeping;              // 0x1
    bool m_IsSleepRequested;        // 0x2, the DSP stops at the next frame
    bool m_IsWaitingForFinalize;    // 0x3
    bool m_HeadphoneStatus;         // 0x4
    u8 m_SendState;                 // 0x5, 1: the parameters are sent, the frame not synchronized yet
};

// 0x00975FA0 (name is ours)
State s_State;
// signalled at the wake up
// 0x00975FA8 (name is ours)
nn::os::LightEvent s_WakeUpEvent;

inline void Wait()
{
    nn::os::Thread::SleepImpl(nn::fnd::TimeSpan::FromNanoSeconds(4888000));
}

inline void UpdateHeadphoneStatus()
{
    s_State.m_HeadphoneStatus = *HEADPHONE_STATUS;
    g_MasterManager.SetIsHeadsetConnected(s_State.m_HeadphoneStatus);
}
} // namespace

// 0x001409E8 | nintendogs:callgraph [confirmed by fefates] [tier A]
nn::Result FlushDataCache(uptr address, size_t size)
{
    return nn::dsp::CTR::FlushDataCache(address, size);
}

// 0x00460794 | nintendogs:callgraph [confirmed by fefates] [tier A]
Voice* AllocVoice(int priority, VoiceDropCallback callback, uptr arg)
{
    return g_VoiceManager.AllocVoice(priority, callback, arg);
}

// 0x00460C44 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
nn::Result Initialize()
{
    if (s_State.m_IsInitialized) {
        return nn::Result(RESULT_ALREADY_INITIALIZED);
    }
    nn::Result result = nn::srv::Initialize();
    if (result.IsFailure()) {
        return result;
    }
    result = g_Dspsnd.Initialize(false);
    if (result.IsFailure()) {
        return result;
    }
    UpdateHeadphoneStatus();
    g_VoiceManager.Initialize();
    g_MasterManager.Initialize();
    DspFxManager::GetInstance()->Initialize();
    nn::dsp::CTR::RegisterSleepWakeUpCallback(Sleep, WakeUp, OrderToWaitForFinalize);
    s_State.m_IsSleeping = false;
    s_State.m_IsSleepRequested = false;
    s_State.m_IsWaitingForFinalize = false;
    s_State.m_SendState = 1;
    s_State.m_IsInitialized = true;
    return result;
}

// 0x00460D00 | fefates:callgraph [tier C]
void ClearEffect(AuxBusId bus)
{
    g_MasterManager.ClearEffect(bus);
}

// 0x004614F0 | fefates:callgraph [tier C]
u32 GetDspCycles()
{
    return *reinterpret_cast<const u32*>(&g_Dspsnd.m_DspStatus[Dspsnd::STATUS_DSP_CYCLES]);
}

// 0x004621BC | nintendogs:callgraph [confirmed by fefates] [tier A]
nn::os::Tick GetSoundThreadTick()
{
    return ThreadManager::GetInstance()->GetSoundThreadTick();
}

// 0x004621E4 | nintendogs:callgraph [confirmed by fefates] [tier A]
void FinalizeSoundThread()
{
    ThreadManager::GetInstance()->FinalizeSoundThread();
}

// 0x0046256C | fefates:callgraph [tier C]
void EnableSoundThreadTickCounter(bool isEnabled)
{
    ThreadManager::GetInstance()->EnableSoundThreadTickCounter(isEnabled);
}

// 0x004625E8 (name is ours)
void GetAuxCallback(AuxBusId bus, AuxCallback* callback, uptr* arg)
{
    g_MasterManager.GetAuxCallback(bus, callback, arg);
}

// 0x00462600 | fefates:bytes [tier B]
void WaitForDspSync(nn::os::Tick* syncTick)
{
    nn::os::detail::AtomicAdd(&g_Dspsnd.m_UseCount, 1);
    if (s_State.m_IsSleepRequested) {
        // a frame of the DSP is 4.888 ms
        if (!g_Dspsnd.WaitPipe(nn::fnd::TimeSpan::FromNanoSeconds(9776000))) {
            s_State.m_IsSleeping = true;
        } else {
            const s64 start = nn::svc::GetSystemTick();
            g_Dspsnd.SyncFrameData();
            s_State.m_SendState = 0;
            *syncTick = nn::os::Tick(nn::svc::GetSystemTick() - start);
            nn::os::detail::AtomicAdd(&g_Dspsnd.m_UseCount, -1);
            return;
        }
    }
    if (s_State.m_IsSleeping) {
        nn::os::detail::AtomicAdd(&g_Dspsnd.m_UseCount, -1);
        s_WakeUpEvent.Wait();
        nn::os::detail::AtomicAdd(&g_Dspsnd.m_UseCount, 1);
        s_WakeUpEvent.ClearSignal();
    }
    if (s_State.m_IsWaitingForFinalize) {
        Wait();
    } else {
        g_Dspsnd.WaitPipe();
        const s64 start = nn::svc::GetSystemTick();
        g_Dspsnd.SyncFrameData();
        s_State.m_SendState = 0;
        *syncTick = nn::os::Tick(nn::svc::GetSystemTick() - start);
    }
    nn::os::detail::AtomicAdd(&g_Dspsnd.m_UseCount, -1);
}

// 0x00462794 | fefates:bytes [tier B]
void WaitForDspSync()
{
    nn::os::detail::AtomicAdd(&g_Dspsnd.m_UseCount, 1);
    if (s_State.m_IsSleepRequested) {
        if (!g_Dspsnd.WaitPipe(nn::fnd::TimeSpan::FromNanoSeconds(9776000))) {
            s_State.m_IsSleeping = true;
        } else {
            g_Dspsnd.SyncFrameData();
            s_State.m_SendState = 0;
            nn::os::detail::AtomicAdd(&g_Dspsnd.m_UseCount, -1);
            return;
        }
    }
    if (s_State.m_IsSleeping) {
        nn::os::detail::AtomicAdd(&g_Dspsnd.m_UseCount, -1);
        s_WakeUpEvent.Wait();
        nn::os::detail::AtomicAdd(&g_Dspsnd.m_UseCount, 1);
        s_WakeUpEvent.ClearSignal();
    }
    if (s_State.m_IsWaitingForFinalize) {
        Wait();
    } else {
        g_Dspsnd.WaitPipe();
        g_Dspsnd.SyncFrameData();
        s_State.m_SendState = 0;
    }
    nn::os::detail::AtomicAdd(&g_Dspsnd.m_UseCount, -1);
}

// 0x004628D8 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
void DecodeAdpcmData(const u8* src, s16* dst, const AdpcmParam& param, AdpcmContext& context, int sampleCount)
{
    // frames of a header (predictor and scale) and 14 samples of 4 bits
    s16 history1 = context.m_History[0];
    s16 history2 = context.m_History[1];
    u8 header = context.m_PredScale;
    while (sampleCount > 0) {
        header = *src++;
        const s32 shift = 17 - (header & 0xF);
        const s16 coefficient1 = param.m_Coefficients[(header >> 4) * 2];
        const s16 coefficient2 = param.m_Coefficients[(header >> 4) * 2 + 1];
        u16 nibbles[14];
        for (s32 i = 0; i < 7; i++) {
            const u8 data = *src++;
            nibbles[i * 2] = data >> 4;
            nibbles[i * 2 + 1] = data & 0xF;
        }
        s32 count = 14;
        if (sampleCount >= 14) {
            sampleCount -= 14;
        } else {
            count = sampleCount;
            sampleCount = 0;
            if (count <= 0) {
                break;
            }
        }
        for (s32 i = 0; i < count; i++) {
            s32 sample = (history1 * coefficient1 + history2 * coefficient2 + (static_cast<s32>(nibbles[i] << 28) >> shift) + 1024) >> 11;
            if (sample < -32768) {
                sample = -32768;
            } else if (sample >= 32768) {
                sample = 32767;
            }
            history2 = history1;
            *dst++ = sample;
            history1 = sample;
        }
    }
    context.m_PredScale = header;
    context.m_History[0] = history1;
    context.m_History[1] = history2;
}

// 0x004629FC | nintendogs:callseq [tier C]
void LockSoundThread()
{
    ThreadManager::GetInstance()->m_Lock.Enter();
}

// 0x00462A10 (name is ours)
void SetMasterVolume(float volume)
{
    g_MasterManager.SetMasterVolume(volume);
}

// 0x00462A1C | fefates:callgraph [tier C]
void ClearAuxCallback(AuxBusId bus)
{
    g_MasterManager.ClearAuxCallback(bus);
}

// 0x00462DFC | nintendogs:bytes [confirmed by fefates] [tier A]
nn::Result StartSoundThread(const ThreadParameter* parameter, void (*callback)(uptr), uptr arg, const ThreadParameter* userParameter,
                            void (*userCallback)(uptr), uptr userArg, int coreNo)
{
    return ThreadManager::GetInstance()->StartSoundThread(parameter, callback, arg, userParameter, userCallback, userArg, coreNo);
}

// 0x0046338C (name is ours)
void SetAuxFrontBypass(AuxBusId bus, bool isBypass)
{
    g_MasterManager.SetAuxFrontBypass(bus, isBypass);
}

// 0x004633A0 | nintendogs:callseq [tier C]
void UnlockSoundThread()
{
    ThreadManager::GetInstance()->m_Lock.Exit();
}

// 0x004633B4 | nintendogs:callgraph [confirmed by fefates] [tier A]
bool GetHeadphoneStatus()
{
    return s_State.m_HeadphoneStatus;
}

// 0x004633C4 (name is ours)
OutputMode GetSoundOutputMode()
{
    return g_MasterManager.GetSoundOutputMode();
}

// 0x004633D0 | fefates:bytes [tier B]
void SendParameterToDsp()
{
    nn::os::detail::AtomicAdd(&g_Dspsnd.m_UseCount, 1);
    if (s_State.m_IsSleeping || s_State.m_IsWaitingForFinalize) {
        s_State.m_SendState = 2;
    } else {
        // the last frame first
        if (s_State.m_SendState != 0) {
            WaitForDspSync();
        }
        UpdateHeadphoneStatus();
        g_Dspsnd.SendParameter();
        s_State.m_SendState = 1;
    }
    nn::os::detail::AtomicAdd(&g_Dspsnd.m_UseCount, -1);
}

// 0x00463470 | nintendogs:callseq-callee [confirmed by fefates, mk7dlp] [tier A]
void SetAuxReturnVolume(AuxBusId bus, float volume)
{
    g_MasterManager.SetAuxReturnVolume(bus, volume);
}

// 0x00463480 (name is ours)
bool SetSoundOutputMode(OutputMode mode)
{
    return g_MasterManager.SetSoundOutputMode(mode);
}

// 0x00463490 | nintendogs:callseq-callee [CONFLICT with mk7dlp:callseq: nn::snd::CTR::UserSoundThreadFunc(unsigned)] [tier X]
void RegisterAuxCallback(AuxBusId bus, AuxCallback callback, uptr arg)
{
    g_MasterManager.RegisterAuxCallback(bus, callback, arg);
}

// 0x004634A8 | fefates:bytes [tier B]
void UserSoundThreadFunc(uptr)
{
    ThreadManager* threadManager = ThreadManager::GetInstance();
    threadManager->m_IsUserThreadRunning = true;
    do {
        threadManager->m_UserThreadEvent.Wait();
        if (threadManager->m_UserCallback != NULL) {
            threadManager->m_UserCallback(threadManager->m_UserCallbackArg);
        }
        nn::dsp::CTR::LockComponent();
        if (nn::dsp::CTR::IsComponentLoaded()) {
            g_MasterManager.AuxUserCallback(AUX_BUS_0, reinterpret_cast<uptr>(g_Dspsnd.m_AuxBuffer[g_Dspsnd.m_StatusIndex][AUX_BUS_0]));
            g_MasterManager.AuxUserCallback(AUX_BUS_1, reinterpret_cast<uptr>(g_Dspsnd.m_AuxBuffer[g_Dspsnd.m_StatusIndex][AUX_BUS_1]));
        }
        nn::dsp::CTR::UnlockComponent();
        nn::os::detail::DataSynchronizationBarrier();
        threadManager->m_DoneEvent.Signal();
    } while (threadManager->m_IsUserThreadRunning);
}

// 0x00463560 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
void InitializeWaveBuffer(WaveBuffer* buffer)
{
    buffer->m_BufferAddress = NULL;
    buffer->m_SampleLength = 0;
    buffer->m_AdpcmContext = NULL;
    buffer->m_UserParam = 0;
    buffer->m_IsLoop = false;
    buffer->m_BufferId = 0;
    buffer->m_Next = NULL;
    buffer->m_Status = WaveBuffer::STATUS_FREE;
}

// 0x004635A0 (name is ours)
void SetOutputBufferCount(int count)
{
    g_MasterManager.SetOutputBufferCount(count);
}

// 0x004635B0 | nintendogs:bytes [confirmed by fefates] [tier A]
void OrderToWaitForFinalize()
{
    if (s_State.m_IsInitialized && s_State.m_IsSleeping) {
        s_State.m_IsWaitingForFinalize = true;
        s_State.m_IsSleeping = false;
        s_State.m_IsSleepRequested = false;
        nn::os::detail::DataSynchronizationBarrier();
        s_WakeUpEvent.Signal();
    }
}

// 0x004635EC | nintendogs:bytes [confirmed by fefates] [tier A]
void Sleep()
{
    if (s_State.m_IsInitialized && !s_State.m_IsSleeping) {
        s_WakeUpEvent.Initialize(true);
        s_State.m_IsSleepRequested = true;
        g_Dspsnd.Finalize(true);
        s_State.m_IsSleeping = true;
    }
}

// 0x0046538C | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
void WakeUp()
{
    if (s_State.m_IsInitialized && s_State.m_IsSleeping) {
        g_Dspsnd.Initialize(true);
        UpdateHeadphoneStatus();
        s_State.m_IsSleeping = false;
        s_State.m_IsSleepRequested = false;
        nn::os::detail::DataSynchronizationBarrier();
        s_WakeUpEvent.Signal();
    }
}

// 0x004658E4 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
nn::Result Finalize()
{
    if (s_State.m_IsInitialized) {
        s_State.m_IsInitialized = false;
        nn::dsp::CTR::ClearSleepWakeUpCallback(Sleep, WakeUp, OrderToWaitForFinalize);
        DspFxManager::GetInstance()->Finalize();
        g_MasterManager.Finalize();
        g_VoiceManager.Finalize();
        if (!s_State.m_IsWaitingForFinalize) {
            g_Dspsnd.Finalize(false);
        }
    }
    return nn::Result();
}

// 0x0046633C | nintendogs:callgraph [confirmed by fefates, mk7dlp] [tier A]
void FreeVoice(Voice* voice)
{
    g_VoiceManager.FreeVoice(voice);
}

// 0x0046634C (name is ours)
bool SetEffect(AuxBusId bus, FxDelay* delay)
{
    return g_MasterManager.SetEffect(bus, delay);
}

// 0x00466360 (name is ours)
bool SetEffect(AuxBusId bus, FxReverb* reverb)
{
    return g_MasterManager.SetEffect(bus, reverb);
}

} // namespace CTR
} // namespace snd
} // namespace nn
