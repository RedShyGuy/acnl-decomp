#include "nn/snd/CTR/snd_VoiceImpl.h"
#include <string.h>
#include "nn/os/os_Atomic.h"
#include "nn/snd/CTR/snd_Dspsnd.h"

namespace nn {
namespace snd {
namespace CTR {
namespace {
// the DSP cycles a voice takes per frame (estimated): a base, the sample format by the rate, the
// interpolation, the filter and each bus [type][channel count - 1]
const u32 BASE_CYCLES = 1400;
const u32 BUS_CYCLES = 2200;
// 0x008C240C
const s32 s_FormatCycles[3][2] = {{1200, 1750}, {1100, 1800}, {3000, 0}};
// 0x008C2424
const s32 s_InterpolationCycles[3][2] = {{800, 1300}, {1800, 3100}, {1800, 2900}};
// 0x008C243C
const s32 s_FilterCycles[4][2] = {{300, 150}, {850, 1200}, {2400, 4000}, {3250, 5200}};
} // namespace

// 0x004664A8 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::VoiceImpl::SetMixParam(const MixParam& mixParam)
{
    m_MixParam = mixParam;
    m_DirtyFlags |= DIRTY_MIX;
}

// 0x00466374 | nintendogs:callgraph [confirmed by fefates] [tier A]
void nn::snd::CTR::VoiceImpl::Initialize()
{
    m_State = Voice::STATE_PAUSE;
    m_IsPlaying = false;
    m_PlayPosition = 0;
    m_HasAdpcmContext = false;
    m_BufferListFlags = 0;
    // mono PCM16
    m_AudioInfo.m_ChannelCount = 1;
    m_AudioInfo.m_SampleFormat = SAMPLE_FORMAT_PCM16;
    m_AudioInfo.m_IsFrontBypass = false;
    m_AudioInfo.m_Interpolation = 0;
    m_AudioInfo.m_Is3dSurroundPreprocessed = false;
    SetVolume(1.0f);
    MixParam mixParam;
    mixParam.m_Main[0] = 1.0f;
    mixParam.m_Main[1] = 1.0f;
    SetMixParam(mixParam);
    SetSampleRate(Voice::DEFAULT_SAMPLE_RATE);
    SetPitch(1.0f);
    SetInterpolationType(static_cast<InterpolationType>(0));
    SetFilterType(static_cast<FilterType>(0));
    memset(&m_MonoFilter, 0, sizeof(m_MonoFilter));
    memset(&m_BiquadFilter, 0, sizeof(m_BiquadFilter));
    m_DspCycles = 0;
    m_WaveBuffers = NULL;
    m_SentBufferCount = 0;
    m_NextSlot = 0;
    m_NextBufferId = 0;
}

// 0x004664E8 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::VoiceImpl::SetMixVolume()
{
    MixParam mixParam = m_MixParam;
    for (s32 i = 0; i < 4; i++) {
        mixParam.m_Main[i] *= m_Volume;
        mixParam.m_Aux[0][i] *= m_Volume;
        mixParam.m_Aux[1][i] *= m_Volume;
    }
    g_Dspsnd.SetChannelMix(m_Id, &mixParam);
}

// 0x004665D0 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::VoiceImpl::SetSyncCount()
{
    if (m_DirtyFlags & DIRTY_SYNC_COUNT) {
        g_Dspsnd.SetChannelSyncCount(m_Id, m_SyncCount);
        m_DirtyFlags &= ~DIRTY_SYNC_COUNT;
    }
}

// 0x0046660C (name is ours)
void nn::snd::CTR::VoiceImpl::ForceUpdateDspParams()
{
    m_DirtyFlags = 0xFFFF;
    UpdateDspParams();
}

// 0x00466618 (name is ours)
void nn::snd::CTR::VoiceImpl::UpdateDspParams()
{
    bool isChanged = false;
    if (m_DirtyFlags & DIRTY_MIX) {
        SetMixVolume();
        isChanged = true;
    }
    if (m_DirtyFlags & DIRTY_RATE) {
        m_TimerRate = m_Pitch * (m_SampleRate * (1.0f / Voice::DEFAULT_SAMPLE_RATE));
        g_Dspsnd.SetChannelTimer(m_Id, m_TimerRate);
        // the automatic interpolation depends on the rate
        if (m_InterpolationType == 0) {
            m_DirtyFlags |= DIRTY_INTERPOLATION;
        }
        isChanged = true;
    }
    if (m_DirtyFlags & DIRTY_FILTER_TYPE) {
        g_Dspsnd.SetChannelIirFilterType(m_Id, static_cast<FilterType>(m_FilterType));
        isChanged = true;
    }
    if (m_DirtyFlags & DIRTY_MONO_FILTER) {
        g_Dspsnd.SetChannelIIRFilter_Mono(m_Id, m_MonoFilter.m_A0, m_MonoFilter.m_B0);
    }
    if (m_DirtyFlags & DIRTY_BIQUAD_FILTER) {
        g_Dspsnd.SetChannelIIRFilter_Biquad(m_Id, m_BiquadFilter.m_Values[0], m_BiquadFilter.m_Values[1], m_BiquadFilter.m_Values[2],
                                             m_BiquadFilter.m_Values[3], m_BiquadFilter.m_Values[4]);
    }
    if (m_DirtyFlags & DIRTY_INTERPOLATION) {
        u16 interpolation;
        u16 interpolation2 = 1;
        if (m_InterpolationType == 0) {
            interpolation = 0;
            interpolation2 = 1;
            if (m_TimerRate > 4.0f / 3.0f) {
                interpolation2 = 0;
            } else if (m_TimerRate <= 1.0f) {
                interpolation2 = 2;
            }
        } else {
            interpolation = 2;
            if (m_InterpolationType == 1) {
                interpolation = 1;
            }
        }
        g_Dspsnd.SetChannelRIM(m_Id, interpolation, interpolation2);
        isChanged = true;
    }
    if (m_DirtyFlags & DIRTY_ADPCM_PARAM) {
        g_Dspsnd.SetChannelAdpcmParam(m_Id, &m_AdpcmParam);
    }
    if (isChanged) {
        const u32 format = m_AudioInfo.m_SampleFormat;
        const u32 channel = m_AudioInfo.m_ChannelCount - 1;
        m_DspCycles = BASE_CYCLES;
        m_DspCycles = static_cast<u32>(BASE_CYCLES + s_FormatCycles[format][channel] * m_TimerRate);
        m_DspCycles += s_InterpolationCycles[m_InterpolationType][channel];
        m_DspCycles += s_FilterCycles[m_FilterType][channel];
        // the main bus and the aux buses that are used
        s32 busCount = 1;
        if (m_MixParam.m_Aux[0][0] != 0.0f || m_MixParam.m_Aux[0][1] != 0.0f || m_MixParam.m_Aux[0][2] != 0.0f ||
            m_MixParam.m_Aux[0][3] != 0.0f) {
            busCount = 2;
        }
        if (m_MixParam.m_Aux[1][0] != 0.0f || m_MixParam.m_Aux[1][1] != 0.0f || m_MixParam.m_Aux[1][2] != 0.0f ||
            m_MixParam.m_Aux[1][3] != 0.0f) {
            busCount++;
        }
        m_DspCycles += busCount * BUS_CYCLES;
    }
    m_DirtyFlags &= DIRTY_SYNC_COUNT;
}

// 0x004668E8 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::VoiceImpl::UpdateStatus(const void* statusData)
{
    const DspChannelStatus* status = static_cast<const DspChannelStatus*>(statusData);
    if (status->m_SyncCount == m_SyncCount) {
        m_PlayPosition = (status->m_Position >> 16) | (status->m_Position << 16);
        // the DSP went on to the next buffer
        if (status->m_Flags & 0xFF00) {
            const u16 currentId = status->m_CurrentBufferId;
            const u16 lastId = status->m_LastBufferId;
            m_Lock.Enter();
            WaveBuffer* buffer = m_WaveBuffers;
            if (buffer == NULL) {
                m_Lock.Exit();
            } else {
                WaveBuffer* doneBuffers[SENT_BUFFER_MAX];
                s32 doneCount = 0;
                if (m_SentBufferCount != 0) {
                    do {
                        if (buffer->m_BufferId == currentId) {
                            buffer->m_Status = WaveBuffer::STATUS_PLAY;
                            break;
                        }
                        m_SentBufferCount--;
                        doneBuffers[doneCount++] = buffer;
                        bool isLast = false;
                        if (currentId == 0 && (buffer->m_BufferId == lastId || lastId == 0)) {
                            isLast = true;
                        }
                        buffer = buffer->m_Next;
                        if (isLast) {
                            break;
                        }
                    } while (m_SentBufferCount != 0);
                }
                if (currentId == 0) {
                    m_SentBufferCount = 0;
                }
                nn::os::detail::DataMemoryBarrier();
                for (s32 i = 0; i < doneCount; i++) {
                    doneBuffers[i]->m_Status = WaveBuffer::STATUS_DONE;
                }
                m_WaveBuffers = buffer;
                m_Lock.Exit();
            }
        }
    }
    m_IsPlaying = (status->m_Flags & 0xFF) == 1;
}

// 0x00466A3C (name is ours)
void nn::snd::CTR::VoiceImpl::SetAdpcmParam(const AdpcmParam& param)
{
    memcpy(&m_AdpcmParam, &param, sizeof(AdpcmParam));
    m_DirtyFlags |= DIRTY_ADPCM_PARAM;
}

// 0x00466A80 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::VoiceImpl::SetFilterType(FilterType type)
{
    m_FilterType = type;
    m_DirtyFlags |= DIRTY_FILTER_TYPE;
}

// 0x00466AAC | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::VoiceImpl::SetSampleRate(int sampleRate)
{
    if (sampleRate < 0) {
        sampleRate = 0;
    }
    m_SampleRate = sampleRate;
    m_DirtyFlags |= DIRTY_RATE;
}

// 0x00466ACC | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
void nn::snd::CTR::VoiceImpl::SendWaveBuffer()
{
    m_Lock.Enter();
    if (m_BufferListFlags != 0) {
        // the list changed: send it again from the playing buffer
        g_Dspsnd.ResetChannelNextBuffer(m_Id);
        if (m_WaveBuffers != NULL && m_WaveBuffers->m_Status == WaveBuffer::STATUS_TO_BE_DELETED) {
            m_SentBufferCount = 0;
        } else if (m_SentBufferCount > 0) {
            g_Dspsnd.UpdateChannelNextBuffer(m_Id, m_WaveBuffers);
            m_SentBufferCount = 1;
        }
        m_NextSlot = 0;
        // drop the deleted buffers
        WaveBuffer* buffer = m_WaveBuffers;
        while (buffer != NULL && buffer->m_Status == WaveBuffer::STATUS_TO_BE_DELETED) {
            WaveBuffer* next = buffer->m_Next;
            nn::os::detail::DataMemoryBarrier();
            buffer->m_Status = WaveBuffer::STATUS_DONE;
            buffer = next;
        }
        m_WaveBuffers = buffer;
        while (buffer != NULL) {
            WaveBuffer* next = buffer->m_Next;
            if (next != NULL && next->m_Status == WaveBuffer::STATUS_TO_BE_DELETED) {
                buffer->m_Next = next->m_Next;
                nn::os::detail::DataMemoryBarrier();
                next->m_Status = WaveBuffer::STATUS_DONE;
            } else {
                buffer = next;
            }
        }
        m_BufferListFlags = 0;
    }
    // the first buffer that is not sent yet
    s32 sentCount = m_SentBufferCount;
    WaveBuffer* buffer = m_WaveBuffers;
    for (s32 i = sentCount; i != 0 && buffer != NULL; i--) {
        buffer = buffer->m_Next;
    }
    for (; sentCount < SENT_BUFFER_MAX; sentCount++) {
        if (buffer == NULL) {
            continue;
        }
        if (m_SentBufferCount == 0) {
            m_NextSlot = 0;
            g_Dspsnd.ResetChannelNextBuffer(m_Id);
            buffer->m_Status = WaveBuffer::STATUS_PLAY;
            if (m_AudioInfo.m_SampleFormat == SAMPLE_FORMAT_ADPCM && (m_HasAdpcmContext || buffer->m_AdpcmContext != NULL)) {
                m_HasAdpcmContext = true;
            }
            g_Dspsnd.AssignPCM(m_Id, buffer, m_AudioInfo);
        } else {
            g_Dspsnd.AppendChannelNextBuffer(m_Id, buffer, m_NextSlot);
            m_NextSlot++;
            if (m_NextSlot >= NEXT_BUFFER_SLOTS) {
                m_NextSlot = 0;
            }
        }
        m_SentBufferCount++;
        buffer = buffer->m_Next;
    }
    m_Lock.Exit();
}

// 0x00466CD4 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
void nn::snd::CTR::VoiceImpl::SetChannelCount(int count)
{
    m_AudioInfo.m_ChannelCount = count;
}

// 0x00466CF4 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::VoiceImpl::SetSampleFormat(SampleFormat format)
{
    m_AudioInfo.m_SampleFormat = format;
}

// 0x00466D18 | fefates:bytes [tier B]
void nn::snd::CTR::VoiceImpl::AppendWaveBuffer(WaveBuffer* buffer)
{
    if (buffer->m_SampleLength == 0) {
        buffer->m_Status = WaveBuffer::STATUS_DONE;
        return;
    }
    buffer->m_Next = NULL;
    buffer->m_Status = WaveBuffer::STATUS_WAIT;
    m_Lock.Enter();
    if (m_WaveBuffers == NULL) {
        m_WaveBuffers = buffer;
    } else {
        WaveBuffer* last = m_WaveBuffers;
        while (last->m_Next != NULL) {
            last = last->m_Next;
        }
        last->m_Next = buffer;
    }
    // 0 is no buffer
    if (m_NextBufferId == 0) {
        m_NextBufferId = 1;
    }
    buffer->m_BufferId = m_NextBufferId;
    m_NextBufferId++;
    m_Lock.Exit();
}

// 0x00466DBC | fefates:bytes [tier B]
void nn::snd::CTR::VoiceImpl::UpdateWaveBuffer(WaveBuffer* buffer)
{
    m_Lock.Enter();
    for (WaveBuffer* b = m_WaveBuffers; b != NULL; b = b->m_Next) {
        if (b == buffer) {
            if (buffer->m_Status != WaveBuffer::STATUS_DONE) {
                m_BufferListFlags |= 2;
            }
            break;
        }
    }
    m_Lock.Exit();
}

// 0x00466E1C | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::VoiceImpl::ReleaseWaveBuffer()
{
    m_Lock.Enter();
    for (WaveBuffer* buffer = m_WaveBuffers; buffer != NULL;) {
        WaveBuffer* next = buffer->m_Next;
        buffer->m_Status = WaveBuffer::STATUS_DONE;
        buffer = next;
    }
    m_WaveBuffers = NULL;
    m_SentBufferCount = 0;
    m_NextSlot = 0;
    m_Lock.Exit();
    // the status of the DSP is old now
    m_DirtyFlags |= DIRTY_SYNC_COUNT;
    m_SyncCount++;
}

// 0x00466E90 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
void nn::snd::CTR::VoiceImpl::SetFrontBypassFlag(bool isBypass)
{
    m_AudioInfo.m_IsFrontBypass = isBypass;
}

// 0x00466EB0 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::VoiceImpl::SetInterpolationType(InterpolationType type)
{
    m_InterpolationType = type;
    m_DirtyFlags |= DIRTY_INTERPOLATION;
}

// 0x00466EC4 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
void nn::snd::CTR::VoiceImpl::UpdateWaveBufferList()
{
    if (m_State == Voice::STATE_PLAY) {
        SendWaveBuffer();
    }
}

// 0x00466EE0 | fefates:bytes [tier B]
void nn::snd::CTR::VoiceImpl::Set3dSurroundPreprocessed(bool isPreprocessed)
{
    m_AudioInfo.m_Is3dSurroundPreprocessed = isPreprocessed;
}

// 0x00466EF4 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::VoiceImpl::SetMonoFilterCoefficients(const MonoFilterCoefficients& coefficients)
{
    m_MonoFilter = coefficients;
    m_DirtyFlags |= DIRTY_MONO_FILTER;
}

// 0x00466F34 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::VoiceImpl::SetBiquadFilterCoefficients(const BiquadFilterCoefficients& coefficients)
{
    m_BiquadFilter = coefficients;
    m_DirtyFlags |= DIRTY_BIQUAD_FILTER;
}

// 0x00466F5C | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::VoiceImpl::Start()
{
    g_Dspsnd.SetChannelPlayStart(m_Id);
    m_IsPlaying = true;
}

// 0x00466F84 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::VoiceImpl::SetPitch(float pitch)
{
    m_Pitch = (pitch < 0.0f) ? 0.0f : pitch;
    m_DirtyFlags |= DIRTY_RATE;
}

// 0x00466FD8 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::VoiceImpl::SetState(Voice::State state)
{
    m_State = state;
    switch (state) {
    case Voice::STATE_PLAY:
        break;
    case Voice::STATE_STOP:
        g_Dspsnd.SetChannelPlayStop(m_Id);
        m_IsPlaying = false;
        g_Dspsnd.InitializeChannelParameters(m_Id);
        break;
    case Voice::STATE_PAUSE:
        g_Dspsnd.SetChannelPlayStop(m_Id);
        break;
    }
}

// 0x00467050 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::VoiceImpl::SetVolume(float volume)
{
    m_Volume = volume;
    m_DirtyFlags |= DIRTY_MIX;
}

// 0x00467064 | nintendogs:bytes [confirmed by fefates] [tier A]
nn::snd::CTR::VoiceImpl::VoiceImpl(int id) : m_Id(id)
{
    m_Lock.Initialize();
    m_SyncCount = 1;
    m_DirtyFlags |= DIRTY_SYNC_COUNT;
}

} // namespace CTR
} // namespace snd
} // namespace nn
