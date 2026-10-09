#include "nn/snd/CTR/snd_Dspsnd.h"
#include <string.h>
#include "nn/dsp/CTR/CTR_Api.h"
#include "nn/os/CTR/MPCore/MPCore_Api.h"
#include "nn/os/os_Atomic.h"
#include "nn/os/os_Thread.h"
#include "nn/snd/CTR/snd_DspFxManager.h"
#include "nn/snd/CTR/snd_DspFxManagerImpl.h"
#include "nn/snd/CTR/snd_MasterManager.h"
#include "nn/snd/CTR/snd_MasterManagerImpl.h"
#include "nn/snd/CTR/snd_OutputCapture.h"
#include "nn/snd/CTR/snd_VoiceManager.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace snd {
namespace CTR {
namespace {
// Result 0xC8A0A801: the DSP component is not loaded
const bit32 RESULT_COMPONENT_NOT_LOADED = 0xC8A0A801;

// the pipe of the sound component and its commands
const s32 PIPE_CHANNEL = 2;
const u16 COMMAND_INITIALIZE = 0;
const u16 COMMAND_FINALIZE = 1;
const u16 COMMAND_WAKE_UP = 2;
const u16 COMMAND_SLEEP = 3;
// the DSP semaphores: the frame is ready / the interrupt
const u16 SEMAPHORE_FRAME = 0x4000;
const u16 SEMAPHORE_MASK = 0x2000;
const u32 DESCRIPTION_TIMEOUT = 0x3FE;
const s32 DEFAULT_DSP_CYCLE_LIMIT = 0x97FC7;

// DspMasterParam::m_DirtyFlags
const u32 MASTER_DIRTY_FRONT_BYPASS_0 = 0x40;
const u32 MASTER_DIRTY_FRONT_BYPASS_1 = 0x80;
const u32 MASTER_DIRTY_AUX_BUS_0 = 0x100;
const u32 MASTER_DIRTY_AUX_BUS_1 = 0x200;
const u32 MASTER_DIRTY_DELAY_0 = 0x400;
const u32 MASTER_DIRTY_DELAY_1 = 0x800;
const u32 MASTER_DIRTY_REVERB_0 = 0x1000;
const u32 MASTER_DIRTY_REVERB_1 = 0x2000;
const u32 MASTER_DIRTY_OUTPUT_BUFFER_COUNT = 0x8000;
const u32 MASTER_DIRTY_MASTER_VOLUME = 0x10000;
const u32 MASTER_DIRTY_AUX_RETURN_VOLUME_0 = 0x1000000;
const u32 MASTER_DIRTY_AUX_RETURN_VOLUME_1 = 0x2000000;
const u32 MASTER_DIRTY_OUTPUT_MODE = 0x4000000;
const u32 MASTER_DIRTY_CLIPPING_MODE = 0x8000000;
const u32 MASTER_DIRTY_HEADSET = 0x10000000;
const u32 MASTER_DIRTY_SURROUND_DEPTH = 0x20000000;
const u32 MASTER_DIRTY_SPEAKER_POSITION = 0x40000000;
const u32 MASTER_DIRTY_REAR_RATIO = 0x80000000;
const u32 MASTER_DIRTY2_SYNC_MODE = 0x10000;

// DspChannelParam::m_DirtyFlags
const u32 CHANNEL_DIRTY_ADPCM_PARAM = 0x4;
const u32 CHANNEL_DIRTY_UPDATE_BUFFER = 0x8;
const u32 CHANNEL_DIRTY_RESET_NEXT_BUFFER = 0x10;
const u32 CHANNEL_DIRTY_PLAY_STATE = 0x10000;
const u32 CHANNEL_DIRTY_INTERPOLATION = 0x20000;
const u32 CHANNEL_DIRTY_TIMER = 0x40000;
const u32 CHANNEL_DIRTY_NEXT_BUFFER = 0x80000;
const u32 CHANNEL_DIRTY_BUFFER = 0x200000;
const u32 CHANNEL_DIRTY_FILTER_TYPE = 0x400000;
const u32 CHANNEL_DIRTY_MONO_FILTER = 0x800000;
const u32 CHANNEL_DIRTY_BIQUAD_FILTER = 0x1000000;
const u32 CHANNEL_DIRTY_MIX = 0xE000000;
const u32 CHANNEL_DIRTY_SYNC_COUNT = 0x10000000;
const u32 CHANNEL_DIRTY_INITIALIZE = 0x20000000;
const u32 CHANNEL_DIRTY_ASSIGN = 0x40000000;

// the DSP takes 32 bit values in 16 bit halves
inline u32 SwapHalves(u32 value)
{
    return (value >> 16) | (value << 16);
}

// the size in bytes of the samples of a buffer
inline u32 GetSampleDataSize(DspsndAudioInfo audioInfo, s32 sampleLength)
{
    const u32 channelCount = audioInfo.m_ChannelCount;
    const u32 format = audioInfo.m_SampleFormat;
    if (format == SAMPLE_FORMAT_PCM8) {
        if (channelCount == 1) {
            return sampleLength;
        }
        if (channelCount == 2) {
            return sampleLength * 2;
        }
    }
    if (format == SAMPLE_FORMAT_PCM16) {
        if (channelCount == 1) {
            return sampleLength * 2;
        }
        if (channelCount == 2) {
            return sampleLength * 4;
        }
    }
    if (format == SAMPLE_FORMAT_ADPCM && channelCount == 1) {
        // frames of 14 samples in 8 bytes
        return (sampleLength + 13) / 14 * 8;
    }
    return sampleLength;
}

inline void Sleep()
{
    nn::os::Thread::SleepImpl(nn::fnd::TimeSpan::FromNanoSeconds(4888000));
}
} // namespace

// 0x00AEB884 (name is ours)
Dspsnd g_Dspsnd;

// (the constructor is in __sti___14_snd_System_cpp)
nn::snd::CTR::Dspsnd::Dspsnd() : m_SemaphoreEvent(), m_UseCount(0)
{
    m_IsInitialized = false;
    m_Lock.Initialize();
    m_DspCycleLimit = DEFAULT_DSP_CYCLE_LIMIT;
}

// 0x00463850 | nintendogs:callgraph [confirmed by mk7dlp] [tier A]
nn::Result nn::snd::CTR::Dspsnd::Initialize(bool isWakeUp)
{
    nn::dsp::CTR::LockComponent();
    m_Lock.Enter();
    if (!nn::dsp::CTR::IsComponentLoaded()) {
        m_Lock.Exit();
        nn::dsp::CTR::UnlockComponent();
        return nn::Result(RESULT_COMPONENT_NOT_LOADED);
    }
    if (!m_IsInitialized) {
        m_Event.Initialize(nn::os::RESET_TYPE_STICKY);
        nn::Result result = nn::dsp::CTR::RegisterInterruptEvents(m_Event.GetHandle(), 2, 2);
        if (result.IsFailure()) {
            m_Event.Close();
            m_Lock.Exit();
            nn::dsp::CTR::UnlockComponent();
            return result;
        }
        nn::Handle semaphoreEvent;
        result = nn::dsp::CTR::GetSemaphoreEventHandle(&semaphoreEvent);
        if (result.IsFailure() || !semaphoreEvent.IsValid()) {
            m_Lock.Exit();
            nn::dsp::CTR::UnlockComponent();
            return result;
        }
        m_SemaphoreEvent = semaphoreEvent;
        nn::dsp::CTR::SetSemaphoreMask(SEMAPHORE_MASK);
        InitializeVariables(isWakeUp);
        if (isWakeUp) {
            // send all settings again
            g_VoiceManager.ForceUpdateDspParams();
            g_MasterManagerImpl.ForceUpdateParams();
            DspFxManager::GetInstance();
            DspFxManagerImpl::GetInstance()->ForceUpdateParams();
        } else {
            m_IsAuxUserCallbackEnabled = true;
            m_OutputCapture = NULL;
        }
    }
    m_Lock.Exit();
    nn::dsp::CTR::UnlockComponent();
    return nn::Result();
}

// 0x00463A44 | fefates:bytes [tier B]
void nn::snd::CTR::Dspsnd::SetSyncMode(SyncMode mode)
{
    nn::dsp::CTR::LockComponent();
    if (nn::dsp::CTR::IsComponentLoaded()) {
        DspMasterParam* param = GetMasterParam();
        param->m_SyncMode = mode;
        param->m_DirtyFlags2 |= MASTER_DIRTY2_SYNC_MODE;
    }
    nn::dsp::CTR::UnlockComponent();
}

// 0x00463A98 | nintendogs:callgraph [confirmed by fefates] [tier A]
bool nn::snd::CTR::Dspsnd::EnableAuxBus(AuxBusId bus, bool isEnabled)
{
    nn::dsp::CTR::LockComponent();
    if (!nn::dsp::CTR::IsComponentLoaded()) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    DspMasterParam* param = GetMasterParam();
    param->m_AuxBusEnable[bus] = isEnabled;
    if (bus == AUX_BUS_0) {
        param->m_DirtyFlags |= MASTER_DIRTY_AUX_BUS_0;
    } else if (bus == AUX_BUS_1) {
        param->m_DirtyFlags |= MASTER_DIRTY_AUX_BUS_1;
    }
    nn::dsp::CTR::UnlockComponent();
    return true;
}

// 0x00463B24 | fefates:bytes [tier B]
bool nn::snd::CTR::Dspsnd::SetRearRatio(u16 ratio)
{
    nn::dsp::CTR::LockComponent();
    if (!nn::dsp::CTR::IsComponentLoaded()) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    if (ratio > 0x8000) {
        ratio = 0x8000;
    }
    DspMasterParam* param = GetMasterParam();
    param->m_RearRatio = ratio;
    param->m_DirtyFlags |= MASTER_DIRTY_REAR_RATIO;
    nn::dsp::CTR::UnlockComponent();
    return true;
}

// 0x00463B8C | fefates:callgraph [tier C]
void nn::snd::CTR::Dspsnd::SendParameter()
{
    nn::dsp::CTR::LockComponent();
    m_Lock.Enter();
    if (nn::dsp::CTR::IsComponentLoaded() && m_IsInitialized) {
        MasterManager* master = &g_MasterManager;
        VoiceManager* voices = &g_VoiceManager;
        DspFxManager* fx = DspFxManager::GetInstance();
        const uptr aux0 = reinterpret_cast<uptr>(m_AuxBuffer[m_StatusIndex][AUX_BUS_0]);
        const uptr aux1 = reinterpret_cast<uptr>(m_AuxBuffer[m_StatusIndex][AUX_BUS_1]);
        if (m_IsAuxUserCallbackEnabled) {
            master->AuxUserCallback(AUX_BUS_0, aux0);
            master->AuxUserCallback(AUX_BUS_1, aux1);
        }
        master->ExecuteEffect(AUX_BUS_0, aux0);
        master->ExecuteEffect(AUX_BUS_1, aux1);
        voices->UpdateDspParams();
        voices->UpdateWaveBufferLists();
        // the voices get what the master output and the effects leave
        const s32 cycles = m_DspCycleLimit - master->GetDspCycles() - fx->GetDspCycles();
        voices->AdjustVoicePlayState(cycles, *reinterpret_cast<s32*>(m_DspStatus));
        *m_FrameCounter[m_ParamIndex] = m_FrameCount;
        m_FrameCount++;
        nn::Result result = nn::svc::SignalEvent(m_SemaphoreEvent);
        if (result.IsFailure()) {
            nn::os::CTR::detail::HandleInternalError(result);
        }
        m_ParamIndex = m_FrameCount & 1;
        m_WaitCount++;
    }
    m_Lock.Exit();
    nn::dsp::CTR::UnlockComponent();
}

// 0x00463CF0 | nintendogs:callgraph [confirmed by fefates] [tier A]
bool nn::snd::CTR::Dspsnd::SetChannelMix(u8 channel, const MixParam* mixParam)
{
    nn::dsp::CTR::LockComponent();
    if (!nn::dsp::CTR::IsComponentLoaded()) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    DspChannelParam* param = GetChannelParam(channel);
    const u16* main = reinterpret_cast<const u16*>(mixParam->m_Main);
    for (s32 i = 0; i < 8; i++) {
        param->m_Mix[0][i] = main[i];
    }
    const u16* aux0 = reinterpret_cast<const u16*>(mixParam->m_Aux[0]);
    for (s32 i = 0; i < 8; i++) {
        param->m_Mix[1][i] = aux0[i];
    }
    const u16* aux1 = reinterpret_cast<const u16*>(mixParam->m_Aux[1]);
    for (s32 i = 0; i < 8; i++) {
        param->m_Mix[2][i] = aux1[i];
    }
    param->m_DirtyFlags |= CHANNEL_DIRTY_MIX;
    nn::dsp::CTR::UnlockComponent();
    return true;
}

// 0x00463DCC | fefates:bytes [tier B]
bool nn::snd::CTR::Dspsnd::SetChannelRIM(u8 channel, u16 interpolation, u16 interpolation2)
{
    nn::dsp::CTR::LockComponent();
    if (!nn::dsp::CTR::IsComponentLoaded()) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    DspChannelParam* param = GetChannelParam(channel);
    param->m_Interpolation[0] = interpolation;
    if (interpolation == 0) {
        param->m_Interpolation[1] = interpolation2;
    }
    param->m_DirtyFlags |= CHANNEL_DIRTY_INTERPOLATION;
    nn::dsp::CTR::UnlockComponent();
    return true;
}

// 0x00463E4C | fefates:callgraph [tier C]
void nn::snd::CTR::Dspsnd::SyncFrameData()
{
    nn::dsp::CTR::LockComponent();
    m_Lock.Enter();
    if (nn::dsp::CTR::IsComponentLoaded() && m_IsInitialized) {
        // the frame the DSP finished
        const u16 frame = *m_FrameCounter[~m_FrameCount & 1];
        if (frame != 0) {
            m_FrameCount = frame + 1;
            if (m_FrameCount == 0) {
                m_FrameCount = 2;
            }
            m_StatusIndex = m_FrameCount & 1;
            for (s32 channel = 0; channel < CHANNEL_COUNT; channel++) {
                g_VoiceManager.UpdateStatus(channel, m_ChannelStatus[m_StatusIndex][static_cast<u8>(channel)]);
            }
            memcpy(m_DspStatus, m_DspStatusBuffer[m_StatusIndex], STATUS_SIZE);
            if (m_OutputCapture != NULL && m_OutputCapture->m_IsEnabled) {
                m_OutputCapture->Write(m_OutputBuffer[m_StatusIndex], FRAME_SAMPLES);
            }
            g_MasterManager.UpdateDroppedSoundFrameCount();
        }
    }
    m_Lock.Exit();
    nn::dsp::CTR::UnlockComponent();
}

// 0x00463F70 | fefates:bytes [tier B]
bool nn::snd::CTR::Dspsnd::SetChannelTimer(u8 channel, f32 rate)
{
    nn::dsp::CTR::LockComponent();
    if (!nn::dsp::CTR::IsComponentLoaded()) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    DspChannelParam* param = GetChannelParam(channel);
    param->m_TimerRate = rate;
    param->m_DirtyFlags |= CHANNEL_DIRTY_TIMER;
    nn::dsp::CTR::UnlockComponent();
    return true;
}

// 0x00463FEC | fefates:bytes [tier B]
bool nn::snd::CTR::Dspsnd::SetClippingMode(ClippingMode mode)
{
    nn::dsp::CTR::LockComponent();
    if (!nn::dsp::CTR::IsComponentLoaded()) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    DspMasterParam* param = GetMasterParam();
    param->m_ClippingMode = mode;
    param->m_DirtyFlags |= MASTER_DIRTY_CLIPPING_MODE;
    nn::dsp::CTR::UnlockComponent();
    return true;
}

// 0x0046404C | nintendogs:callgraph [confirmed by fefates] [tier A]
void nn::snd::CTR::Dspsnd::SetMasterVolume(f32 volume)
{
    nn::dsp::CTR::LockComponent();
    if (nn::dsp::CTR::IsComponentLoaded()) {
        DspMasterParam* param = GetMasterParam();
        param->m_MasterVolume = volume;
        param->m_DirtyFlags |= MASTER_DIRTY_MASTER_VOLUME;
    }
    nn::dsp::CTR::UnlockComponent();
}

// 0x004640AC | fefates:bytes [tier B]
bool nn::snd::CTR::Dspsnd::SetSurroundDepth(u16 depth)
{
    nn::dsp::CTR::LockComponent();
    if (!nn::dsp::CTR::IsComponentLoaded()) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    if (depth >= 0x8000) {
        depth = 0x7FFF;
    }
    DspMasterParam* param = GetMasterParam();
    param->m_SurroundDepth = depth;
    param->m_DirtyFlags |= MASTER_DIRTY_SURROUND_DEPTH;
    nn::dsp::CTR::UnlockComponent();
    return true;
}

// 0x00464118 | nintendogs:callgraph [confirmed by fefates] [tier A]
bool nn::snd::CTR::Dspsnd::SetAuxFrontBypass(AuxBusId bus, bool isBypass)
{
    nn::dsp::CTR::LockComponent();
    if (!nn::dsp::CTR::IsComponentLoaded()) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    DspMasterParam* param = GetMasterParam();
    param->m_AuxFrontBypass[bus] = isBypass;
    if (bus == AUX_BUS_0) {
        param->m_DirtyFlags |= MASTER_DIRTY_FRONT_BYPASS_0;
    } else if (bus == AUX_BUS_1) {
        param->m_DirtyFlags |= MASTER_DIRTY_FRONT_BYPASS_1;
    }
    nn::dsp::CTR::UnlockComponent();
    return true;
}

// 0x004641A4 | nintendogs:callgraph [confirmed by fefates] [tier A]
bool nn::snd::CTR::Dspsnd::SetDspDelayEffect(AuxBusId bus, DspFxDelayParams& params)
{
    nn::dsp::CTR::LockComponent();
    if (!nn::dsp::CTR::IsComponentLoaded()) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    DspMasterParam* param = GetMasterParam();
    DspFxDelayParams* delay = &param->m_Delay[bus];
    if (params.m_Flags & DspFxDelayParams::FLAG_ENABLE) {
        delay->m_IsEnabled = params.m_IsEnabled;
    }
    if (params.m_Flags & DspFxDelayParams::FLAG_PARAM) {
        delay->m_ChannelCount = params.m_ChannelCount;
        delay->m_DelaySamples = params.m_DelaySamples;
        delay->m_Feedback = params.m_Feedback;
        delay->m_LpfB = params.m_LpfB;
        delay->m_LpfA = params.m_LpfA;
    }
    if (params.m_Flags & DspFxDelayParams::FLAG_BUFFER) {
        delay->m_BufferAddress = params.m_BufferAddress;
    }
    delay->m_Flags |= params.m_Flags;
    if (bus == AUX_BUS_0) {
        param->m_DirtyFlags |= MASTER_DIRTY_DELAY_0;
    } else if (bus == AUX_BUS_1) {
        param->m_DirtyFlags |= MASTER_DIRTY_DELAY_1;
    }
    nn::dsp::CTR::UnlockComponent();
    return true;
}

// 0x004642A8 | nintendogs:callgraph [confirmed by fefates] [tier A]
void nn::snd::CTR::Dspsnd::SetAuxReturnVolume(AuxBusId bus, f32 volume)
{
    nn::dsp::CTR::LockComponent();
    if (nn::dsp::CTR::IsComponentLoaded()) {
        DspMasterParam* param = GetMasterParam();
        param->m_AuxReturnVolume[bus] = volume;
        if (bus == AUX_BUS_0) {
            param->m_DirtyFlags |= MASTER_DIRTY_AUX_RETURN_VOLUME_0;
        } else if (bus == AUX_BUS_1) {
            param->m_DirtyFlags |= MASTER_DIRTY_AUX_RETURN_VOLUME_1;
        }
    }
    nn::dsp::CTR::UnlockComponent();
}

// 0x00464320 | fefates:bytes [tier B]
bool nn::snd::CTR::Dspsnd::SetChannelPlayStop(u8 channel)
{
    nn::dsp::CTR::LockComponent();
    if (!nn::dsp::CTR::IsComponentLoaded()) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    DspChannelParam* param = GetChannelParam(channel);
    param->m_PlayState &= ~0xFF;
    param->m_DirtyFlags |= CHANNEL_DIRTY_PLAY_STATE;
    nn::dsp::CTR::UnlockComponent();
    return true;
}

// 0x00464394 | nintendogs:callgraph [confirmed by fefates] [tier A]
bool nn::snd::CTR::Dspsnd::SetDspReverbEffect(AuxBusId bus, DspFxReverbParams& params)
{
    nn::dsp::CTR::LockComponent();
    if (!nn::dsp::CTR::IsComponentLoaded()) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    DspMasterParam* param = GetMasterParam();
    DspFxReverbParams* reverb = &param->m_Reverb[bus];
    if (params.m_Flags & DspFxDelayParams::FLAG_ENABLE) {
        reverb->m_IsEnabled = params.m_IsEnabled;
    }
    if (params.m_Flags & DspFxDelayParams::FLAG_PARAM) {
        reverb->m_ChannelCount = params.m_ChannelCount;
        reverb->m_EarlyReflectionSamples = params.m_EarlyReflectionSamples;
        reverb->m_FusedSamples = params.m_FusedSamples;
        reverb->m_FilterSize[0] = params.m_FilterSize[0];
        reverb->m_FilterSize[1] = params.m_FilterSize[1];
        reverb->m_FilterSize[2] = params.m_FilterSize[2];
        reverb->m_EarlyGain = params.m_EarlyGain;
        reverb->m_FusedGain = params.m_FusedGain;
        reverb->m_Coloration = params.m_Coloration;
        reverb->m_CombGain[0] = params.m_CombGain[0];
        reverb->m_CombGain[1] = params.m_CombGain[1];
        reverb->m_LpfB = params.m_LpfB;
        reverb->m_LpfA = params.m_LpfA;
    }
    if (params.m_Flags & DspFxDelayParams::FLAG_BUFFER) {
        for (s32 i = 0; i < 5; i++) {
            reverb->m_BufferAddress[i] = params.m_BufferAddress[i];
        }
    }
    reverb->m_Flags |= params.m_Flags;
    if (bus == AUX_BUS_0) {
        param->m_DirtyFlags |= MASTER_DIRTY_REVERB_0;
    } else if (bus == AUX_BUS_1) {
        param->m_DirtyFlags |= MASTER_DIRTY_REVERB_1;
    }
    nn::dsp::CTR::UnlockComponent();
    return true;
}

// 0x00464500 | fefates:bytes [tier B]
bool nn::snd::CTR::Dspsnd::SetSoundOutputMode(OutputMode mode)
{
    nn::dsp::CTR::LockComponent();
    if (!nn::dsp::CTR::IsComponentLoaded()) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    DspMasterParam* param = GetMasterParam();
    param->m_OutputMode = mode;
    param->m_DirtyFlags |= MASTER_DIRTY_OUTPUT_MODE;
    nn::dsp::CTR::UnlockComponent();
    return true;
}

// 0x00464560 | fefates:bytes [tier B]
void nn::snd::CTR::Dspsnd::InitializeVariables(bool isWakeUp)
{
    if (m_IsInitialized) {
        return;
    }
    m_IsInitialized = true;
    u16 command;
    if (!isWakeUp) {
        command = COMMAND_INITIALIZE;
    } else {
        command = COMMAND_WAKE_UP;
        memcpy(m_MasterStatus[0], m_SavedDspMemory, SAVED_MEMORY_SIZE);
        nn::os::detail::DataSynchronizationBarrier();
    }
    nn::dsp::CTR::WriteProcessPipe(PIPE_CHANNEL, &command, 4);
    nn::dsp::CTR::SetSemaphore(SEMAPHORE_FRAME);
    WaitPipe();

    // the component answers with the addresses of its structures (two of each)
    u16 count;
    u16 readSize;
    u16 addresses[15];
    DspChannelParam* channelParam[2];
    DspChannelStatus* channelStatus[2];
    AdpcmParam* adpcmParam[2];
    s32* auxBuffer[2];
    void* pointers[15] = {
        m_FrameCounter,     channelParam,     channelStatus,     adpcmParam,        m_MasterParam,
        m_MasterStatus,     m_OutputBuffer,   auxBuffer,         m_Unknown130C,     m_DspStatusBuffer,
        m_Unknown1588[0],   m_Unknown1588[1], m_Unknown1588[2],  m_Unknown1588[3],  m_Unknown1588[4],
    };
    nn::dsp::CTR::ReadPipeIfPossible(PIPE_CHANNEL, &count, sizeof(count), &readSize);
    nn::dsp::CTR::ReadPipeIfPossible(PIPE_CHANNEL, addresses, count * 2, &readSize);
    for (s32 i = 0; i < count; i++) {
        u32* pointer = static_cast<u32*>(pointers[i]);
        nn::dsp::CTR::ConvertProcessAddressFromDspDram(addresses[i], pointer);
        nn::dsp::CTR::ConvertProcessAddressFromDspDram(addresses[i] | 0x10000, pointer + 1);
    }
    // two channels per aux bus buffer
    m_AuxBuffer[0][AUX_BUS_0] = auxBuffer[0];
    m_AuxBuffer[0][AUX_BUS_1] = auxBuffer[0] + 640;
    m_AuxBuffer[1][AUX_BUS_0] = auxBuffer[1];
    m_AuxBuffer[1][AUX_BUS_1] = auxBuffer[1] + 640;
    for (s32 i = 0; i < CHANNEL_COUNT; i++) {
        m_ChannelParam[0][i] = channelParam[0] + i;
        m_ChannelStatus[0][i] = channelStatus[0] + i;
        m_AdpcmParam[0][i] = adpcmParam[0] + i;
        m_ChannelParam[1][i] = channelParam[1] + i;
        m_ChannelStatus[1][i] = channelStatus[1] + i;
        m_AdpcmParam[1][i] = adpcmParam[1] + i;
    }
    nn::dsp::CTR::SetSemaphore(SEMAPHORE_FRAME);
    m_WaitCount = 0;
    m_FrameCount = 4;
    *m_FrameCounter[0] = m_FrameCount;
    m_FrameCount++;
    nn::Result result = nn::svc::SignalEvent(m_SemaphoreEvent);
    if (result.IsFailure()) {
        nn::os::CTR::detail::HandleInternalError(result);
    }
    m_ParamIndex = m_FrameCount & 1;
    m_StatusIndex = m_ParamIndex;
}

// 0x00464810 | fefates:bytes [tier B]
bool nn::snd::CTR::Dspsnd::SetChannelPlayStart(u8 channel)
{
    nn::dsp::CTR::LockComponent();
    if (!nn::dsp::CTR::IsComponentLoaded()) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    DspChannelParam* param = GetChannelParam(channel);
    *reinterpret_cast<u8*>(&param->m_PlayState) = 1;
    param->m_DirtyFlags |= CHANNEL_DIRTY_PLAY_STATE;
    nn::dsp::CTR::UnlockComponent();
    return true;
}

// 0x00464880 | nintendogs:callgraph [confirmed by fefates] [tier A]
bool nn::snd::CTR::Dspsnd::SetChannelSyncCount(u8 channel, s16 syncCount)
{
    nn::dsp::CTR::LockComponent();
    if (!nn::dsp::CTR::IsComponentLoaded()) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    DspChannelParam* param = GetChannelParam(channel);
    param->m_SyncCount = syncCount;
    param->m_DirtyFlags |= CHANNEL_DIRTY_SYNC_COUNT;
    nn::dsp::CTR::UnlockComponent();
    return true;
}

// 0x004648F0 | fefates:bytes [tier B]
s32 nn::snd::CTR::Dspsnd::GetDroppedFrameCount()
{
    nn::dsp::CTR::LockComponent();
    if (!nn::dsp::CTR::IsComponentLoaded()) {
        nn::dsp::CTR::UnlockComponent();
        return -1;
    }
    const s32 count = m_MasterStatus[m_StatusIndex]->m_DroppedFrameCount;
    nn::dsp::CTR::UnlockComponent();
    return count;
}

// 0x00464940 | nintendogs:callseq [confirmed by fefates] [tier A]
bool nn::snd::CTR::Dspsnd::SetChannelAdpcmParam(u8 channel, const AdpcmParam* adpcmParam)
{
    nn::dsp::CTR::LockComponent();
    if (!nn::dsp::CTR::IsComponentLoaded()) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    DspChannelParam* param = GetChannelParam(channel);
    memcpy(m_AdpcmParam[m_ParamIndex][channel], adpcmParam, sizeof(AdpcmParam));
    param->m_DirtyFlags |= CHANNEL_DIRTY_ADPCM_PARAM;
    nn::dsp::CTR::UnlockComponent();
    return true;
}

// 0x004649CC | fefates:bytes [tier B]
void nn::snd::CTR::Dspsnd::SetOutputBufferCount(int count)
{
    nn::dsp::CTR::LockComponent();
    if (nn::dsp::CTR::IsComponentLoaded()) {
        DspMasterParam* param = GetMasterParam();
        param->m_OutputBufferCount = count;
        param->m_DirtyFlags |= MASTER_DIRTY_OUTPUT_BUFFER_COUNT;
    }
    nn::dsp::CTR::UnlockComponent();
}

// 0x00464A20 | fefates:bytes [tier B]
bool nn::snd::CTR::Dspsnd::SetIsHeadsetConnected(bool isConnected)
{
    nn::dsp::CTR::LockComponent();
    if (!nn::dsp::CTR::IsComponentLoaded()) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    DspMasterParam* param = GetMasterParam();
    param->m_IsHeadsetConnected = isConnected;
    param->m_DirtyFlags |= MASTER_DIRTY_HEADSET;
    nn::dsp::CTR::UnlockComponent();
    return true;
}

// 0x00464A80 | fefates:bytes [tier B]
bool nn::snd::CTR::Dspsnd::ResetChannelNextBuffer(u8 channel)
{
    nn::dsp::CTR::LockComponent();
    if (!nn::dsp::CTR::IsComponentLoaded()) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    DspChannelParam* param = GetChannelParam(channel);
    param->m_DirtyFlags |= CHANNEL_DIRTY_RESET_NEXT_BUFFER;
    nn::dsp::CTR::UnlockComponent();
    return true;
}

// 0x00464AE8 | nintendogs:callgraph [confirmed by fefates] [tier A]
bool nn::snd::CTR::Dspsnd::AppendChannelNextBuffer(u8 channel, const WaveBuffer* buffer, int slot)
{
    nn::dsp::CTR::LockComponent();
    if (!nn::dsp::CTR::IsComponentLoaded()) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    const u32 sampleLength = buffer->m_SampleLength;
    const u16 bufferId = buffer->m_BufferId;
    DspChannelParam* param = GetChannelParam(channel);
    const uptr address = nn::os::CTR::MPCore::ConvertAddressForDevice(reinterpret_cast<uptr>(buffer->m_BufferAddress),
                                                                       GetSampleDataSize(param->m_AudioInfo, sampleLength));
    if (address == 0) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    DspNextBuffer* next = &param->m_NextBuffers[slot];
    next->m_BufferId = bufferId;
    next->m_Address = SwapHalves(address);
    next->m_SampleLength = SwapHalves(sampleLength);
    if (buffer->m_AdpcmContext != NULL) {
        memcpy(&next->m_AdpcmContext, buffer->m_AdpcmContext, sizeof(AdpcmContext));
        next->m_HasAdpcmContext = true;
    } else {
        next->m_HasAdpcmContext = false;
    }
    param->m_NextBufferMask |= 1 << slot;
    next->m_IsLoop = buffer->m_IsLoop;
    param->m_DirtyFlags |= CHANNEL_DIRTY_NEXT_BUFFER;
    nn::dsp::CTR::UnlockComponent();
    return true;
}

// 0x00464C64 | fefates:bytes [tier B]
bool nn::snd::CTR::Dspsnd::SetChannelIirFilterType(u8 channel, FilterType type)
{
    nn::dsp::CTR::LockComponent();
    if (!nn::dsp::CTR::IsComponentLoaded()) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    DspChannelParam* param = GetChannelParam(channel);
    param->m_FilterType = type;
    param->m_DirtyFlags |= CHANNEL_DIRTY_FILTER_TYPE;
    nn::dsp::CTR::UnlockComponent();
    return true;
}

// 0x00464CD4 | fefates:bytes [tier B]
bool nn::snd::CTR::Dspsnd::UpdateChannelNextBuffer(u8 channel, const WaveBuffer* buffer)
{
    nn::dsp::CTR::LockComponent();
    if (!nn::dsp::CTR::IsComponentLoaded()) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    const u16 bufferId = buffer->m_BufferId;
    const DspChannelStatus* status = m_ChannelStatus[m_StatusIndex][channel];
    DspChannelParam* param = GetChannelParam(channel);
    // only the buffer that is playing
    if (status->m_CurrentBufferId == bufferId) {
        param->m_BufferId = bufferId;
        param->m_BufferFlags = (param->m_BufferFlags & ~2) | ((buffer->m_IsLoop << 1) & 2);
        param->m_SampleLength = SwapHalves(buffer->m_SampleLength);
        param->m_DirtyFlags |= CHANNEL_DIRTY_UPDATE_BUFFER;
    }
    nn::dsp::CTR::UnlockComponent();
    return true;
}

// 0x00464D94 | fefates:bytes [tier B]
bool nn::snd::CTR::Dspsnd::SetChannelIIRFilter_Mono(u8 channel, s16 a0, s16 b0)
{
    nn::dsp::CTR::LockComponent();
    if (!nn::dsp::CTR::IsComponentLoaded()) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    DspChannelParam* param = GetChannelParam(channel);
    param->m_MonoFilter[0] = a0;
    param->m_MonoFilter[1] = b0;
    param->m_DirtyFlags |= CHANNEL_DIRTY_MONO_FILTER;
    nn::dsp::CTR::UnlockComponent();
    return true;
}

// 0x00464E0C | fefates:bytes [tier B]
bool nn::snd::CTR::Dspsnd::SetChannelIIRFilter_Biquad(u8 channel, s16 b2, s16 b1, s16 b0, s16 a2, s16 a1)
{
    nn::dsp::CTR::LockComponent();
    if (!nn::dsp::CTR::IsComponentLoaded()) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    // in the reverse order
    DspChannelParam* param = GetChannelParam(channel);
    param->m_BiquadFilter[0] = a1;
    param->m_BiquadFilter[1] = a2;
    param->m_BiquadFilter[2] = b0;
    param->m_BiquadFilter[3] = b1;
    param->m_BiquadFilter[4] = b2;
    param->m_DirtyFlags |= CHANNEL_DIRTY_BIQUAD_FILTER;
    nn::dsp::CTR::UnlockComponent();
    return true;
}

// 0x00464E98 | fefates:bytes [tier B]
bool nn::snd::CTR::Dspsnd::SetSurroundSpeakerPosition(SurroundSpeakerPosition position)
{
    nn::dsp::CTR::LockComponent();
    if (!nn::dsp::CTR::IsComponentLoaded() || (position != 0 && position != 1)) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    DspMasterParam* param = GetMasterParam();
    param->m_SurroundSpeakerPosition = position;
    param->m_DirtyFlags |= MASTER_DIRTY_SPEAKER_POSITION;
    nn::dsp::CTR::UnlockComponent();
    return true;
}

// 0x00464F04 | fefates:bytes [tier B]
bool nn::snd::CTR::Dspsnd::InitializeChannelParameters(u8 channel)
{
    nn::dsp::CTR::LockComponent();
    if (!nn::dsp::CTR::IsComponentLoaded()) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    DspChannelParam* param = GetChannelParam(channel);
    param->m_DirtyFlags |= CHANNEL_DIRTY_INITIALIZE;
    nn::dsp::CTR::UnlockComponent();
    return true;
}

// 0x00464F6C | nintendogs:callgraph [confirmed by fefates, mk7dlp] [tier A]
void nn::snd::CTR::Dspsnd::Finalize(bool isSleep)
{
    if (!m_IsInitialized) {
        return;
    }
    nn::dsp::CTR::LockComponent();
    m_Lock.Enter();
    if (nn::dsp::CTR::IsComponentLoaded()) {
        u16 command = isSleep ? COMMAND_SLEEP : COMMAND_FINALIZE;
        nn::dsp::CTR::WriteProcessPipe(PIPE_CHANNEL, &command, 4);
        // until the component acknowledges
        for (;;) {
            u16 value = 0;
            bool isReady;
            nn::dsp::CTR::RecvDataIsReady(0, &isReady);
            if (isReady) {
                nn::dsp::CTR::RecvData(0, &value);
            }
            if (value == 1) {
                break;
            }
            Sleep();
        }
    }
    m_IsInitialized = false;
    m_Lock.Exit();
    nn::dsp::CTR::UnlockComponent();
    while (m_UseCount > 0) {
        Sleep();
    }
    if (isSleep) {
        memcpy(m_SavedDspMemory, m_MasterStatus[0], SAVED_MEMORY_SIZE);
    }
    m_Event.Close();
    nn::dsp::CTR::RegisterInterruptEvents(m_Event.GetHandle(), 2, 2);
    nn::svc::CloseHandle(m_SemaphoreEvent);
    m_SemaphoreEvent = nn::Handle();
}

// 0x004650B0 | fefates:bytes [tier B]
bool nn::snd::CTR::Dspsnd::WaitPipe(nn::fnd::TimeSpan timeout)
{
    bool isSignaled = false;
    if (m_IsInitialized) {
        nn::Result result = nn::svc::WaitSynchronization1(m_Event.GetHandle(), timeout.GetNanoSeconds());
        if (result.IsFailure()) {
            nn::os::CTR::detail::HandleInternalError(result);
        }
        if (result.GetDescription() != DESCRIPTION_TIMEOUT) {
            isSignaled = true;
            result = nn::svc::ClearEvent(m_Event.GetHandle());
            if (result.IsFailure()) {
                nn::os::CTR::detail::HandleInternalError(result);
            }
            m_WaitCount++;
        }
    }
    return isSignaled;
}

// 0x00465120 | fefates:bytes [tier B]
void nn::snd::CTR::Dspsnd::WaitPipe()
{
    if (m_IsInitialized) {
        nn::Result result = nn::svc::WaitSynchronization1(m_Event.GetHandle(), -1);
        if (result.IsFailure()) {
            nn::os::CTR::detail::HandleInternalError(result);
        }
        result = nn::svc::ClearEvent(m_Event.GetHandle());
        if (result.IsFailure()) {
            nn::os::CTR::detail::HandleInternalError(result);
        }
        m_WaitCount++;
    }
}

// 0x00465178 | nintendogs:callgraph [confirmed by fefates] [tier A]
bool nn::snd::CTR::Dspsnd::AssignPCM(u8 channel, const WaveBuffer* buffer, DspsndAudioInfo audioInfo)
{
    nn::dsp::CTR::LockComponent();
    if (!nn::dsp::CTR::IsComponentLoaded()) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    const u32 sampleLength = buffer->m_SampleLength;
    const u16 bufferId = buffer->m_BufferId;
    const uptr address = nn::os::CTR::MPCore::ConvertAddressForDevice(reinterpret_cast<uptr>(buffer->m_BufferAddress),
                                                                       GetSampleDataSize(audioInfo, sampleLength));
    if (address == 0) {
        nn::dsp::CTR::UnlockComponent();
        return false;
    }
    DspChannelParam* param = GetChannelParam(channel);
    param->m_BufferId = bufferId;
    param->m_Address = SwapHalves(address);
    param->m_SampleLength = SwapHalves(sampleLength);
    param->m_AudioInfo = audioInfo;
    param->m_BufferFlags = (param->m_BufferFlags & ~2) | ((buffer->m_IsLoop << 1) & 2);
    if (audioInfo.m_SampleFormat == SAMPLE_FORMAT_ADPCM) {
        const AdpcmContext* context = buffer->m_AdpcmContext;
        if (context != NULL) {
            const u16* source = reinterpret_cast<const u16*>(context);
            for (s32 i = 0; i < 3; i++) {
                param->m_AdpcmContext[i] = source[i];
            }
            param->m_BufferFlags |= 1;
        } else {
            param->m_BufferFlags &= ~1;
        }
    }
    param->m_DirtyFlags |= CHANNEL_DIRTY_ASSIGN | CHANNEL_DIRTY_BUFFER;
    param->m_PlayPosition = 0;
    nn::dsp::CTR::UnlockComponent();
    return true;
}

// 0x00465344 | fefates:bytes [tier B]
nn::snd::CTR::Dspsnd::~Dspsnd()
{
    m_Lock.Finalize();
    if (m_SemaphoreEvent.IsValid()) {
        nn::svc::CloseHandle(m_SemaphoreEvent);
        m_SemaphoreEvent = nn::Handle();
    }
}

} // namespace CTR
} // namespace snd
} // namespace nn
