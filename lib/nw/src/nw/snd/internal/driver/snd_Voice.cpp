#include "nw/snd/internal/driver/snd_Voice.h"

#include <cstring>

#include "nw/snd/internal/Util/Util_Api.h"
#include "nw/snd/internal/driver/snd_HardwareManager.h"
#include "nw/snd/internal/driver/snd_VoiceManager.h"
#include "nw/snd/internal/snd_WaveFileReader.h"
#include "nw/snd/snd_BiquadFilterCallback.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
namespace {
// the highest pitch of the voices of nn::snd
const f32 PITCH_MAX = 255.0f;
// from this cutoff frequency on the low-pass filter is off (Hz)
const u16 LPF_FREQ_OFF = 16000;
// the frames and bytes of a DSP ADPCM frame
const u32 ADPCM_FRAME_SAMPLES = 14;
const u32 ADPCM_FRAME_BYTES = 8;

// (inline) the voice state of what Update decided (names are ours)
enum VoiceState
{
    VOICE_STATE_NONE = 0,
    VOICE_STATE_RUN = 1,
    VOICE_STATE_STOP = 2,
    VOICE_STATE_PAUSE = 3,
};
} // namespace

// 0x008C2A78
const f32 Voice::PAN_CENTER = 0.0f;
// 0x008C2A7C
const f32 Voice::SURROUND_PAN_FRONT = 0.0f;
// 0x008C2A80
const f32 Voice::SURROUND_PAN_REAR = 2.0f;

// 0x004CA1E0 | nintendogs:callseq [tier A]
void nw::snd::internal::driver::Voice::Initialize(const WaveInfo& waveInfo, u32)
{
    m_Format = waveInfo.sampleFormat;
    for (int i = 0; i < m_VoiceCount; ++i) {
        nn::snd::CTR::Voice* pVoice = m_pVoices[i];
        if (pVoice == nullptr) {
            continue;
        }
        pVoice->SetChannelCount(1);
        nn::snd::CTR::SampleFormat format = nn::snd::CTR::SAMPLE_FORMAT_PCM16;
        switch (m_Format) {
        case SAMPLE_FORMAT_PCM_S8:
            format = nn::snd::CTR::SAMPLE_FORMAT_PCM8;
            break;
        case SAMPLE_FORMAT_DSP_ADPCM:
            format = nn::snd::CTR::SAMPLE_FORMAT_ADPCM;
            break;
        }
        pVoice->SetSampleFormat(format);
        pVoice->SetSampleRate(waveInfo.sampleRate);
        int interpolationType = 0;
        switch (HardwareManager::GetInstance().GetInterpolationType()) {
        case 0:
            interpolationType = 2;
            break;
        case 1:
            interpolationType = 1;
            break;
        }
        pVoice->SetInterpolationType(static_cast<nn::snd::CTR::InterpolationType>(interpolationType));
    }
    m_IsPause = false;
    m_IsPausing = false;
    m_IsStart = false;
    m_IsStarted = false;
}

// 0x004CA2B4 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SetLpfFreq(f32 lpfFreq)
{
    if (m_LpfFreq != lpfFreq) {
        m_LpfFreq = lpfFreq;
        m_SyncFlag |= UPDATE_LPF;
    }
}

// 0x004CA2D8 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SetPanMode(PanMode panMode)
{
    if (m_PanMode != panMode) {
        m_PanMode = panMode;
        m_SyncFlag |= UPDATE_MIX;
    }
}

// 0x004CA2F8 | nintendogs:bytes [tier A]
u32 nw::snd::internal::driver::Voice::FrameToByte(u32 frames, SampleFormat format)
{
    switch (format) {
    case SAMPLE_FORMAT_PCM_S8:
        return frames;
    case SAMPLE_FORMAT_PCM_S16:
        return frames * 2;
    case SAMPLE_FORMAT_DSP_ADPCM: {
        // the frames of 14 samples in 8 bytes, the rest with a header byte
        const u32 frameCount = frames / ADPCM_FRAME_SAMPLES;
        const u32 rest = frames % ADPCM_FRAME_SAMPLES;
        u32 nibbles = frameCount * ADPCM_FRAME_BYTES * 2;
        if (rest != 0) {
            nibbles += rest + 2;
        }
        return nibbles / 2;
    }
    default:
        return 0;
    }
}

// 0x004CA364 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SetMainSend(f32 send)
{
    send += 1.0f;
    if (send < 0.0f) {
        send = 0.0f;
    }
    if (m_MainSend != send) {
        m_MainSend = send;
        m_SyncFlag |= UPDATE_MIX;
    }
}

// 0x004CA3A8 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SetPanCurve(PanCurve panCurve)
{
    if (m_PanCurve != panCurve) {
        m_PanCurve = panCurve;
        m_SyncFlag |= UPDATE_MIX;
    }
}

// 0x004CA3C8 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SetPriority(int priority)
{
    m_Priority = priority;
    VoiceManager::GetInstance().ChangeVoicePriority(this);
    if (m_Priority != PRIORITY_RELEASE) {
        return;
    }
    for (int i = 0; i < m_VoiceCount; ++i) {
        if (m_pVoices[i] != nullptr) {
            m_pVoices[i]->SetPriority(PRIORITY_RELEASE);
        }
    }
}

// 0x004CA420 | nintendogs:callgraph [tier A]
void nw::snd::internal::driver::Voice::CalcMixParam(int channelIndex, nn::snd::CTR::MixParam* pMixParam)
{
    f32 mainSend = 0.0f;
    if (m_MainSend > 1.0f) {
        mainSend = 1.0f;
    } else if (m_MainSend >= 0.0f) {
        mainSend = m_MainSend;
    }
    f32 fxSendA = m_FxSend[0];
    if (fxSendA > 1.0f) {
        fxSendA = 1.0f;
    } else if (fxSendA <= 0.0f) {
        fxSendA = 0.0f;
    }
    f32 fxSendB = m_FxSend[1];
    if (fxSendB > 1.0f) {
        fxSendB = 1.0f;
    } else if (fxSendB <= 0.0f) {
        fxSendB = 0.0f;
    }

    f32 left = 0.0f;
    f32 right = 0.0f;
    f32 front = 0.0f;
    f32 rear = 0.0f;

    Util::PanInfo panInfo;
    panInfo.curve = Util::PAN_CURVE_SHAPE_SQRT;
    panInfo.isCenterOne = false;
    panInfo.isClampedToOne = false;
    panInfo.isSurroundIgnored = false;
    switch (m_PanCurve) {
    case PAN_CURVE_SQRT:
        break;
    case PAN_CURVE_SQRT_0DB:
        panInfo.isCenterOne = true;
        break;
    case PAN_CURVE_SQRT_0DB_CLAMP:
        panInfo.isCenterOne = true;
        panInfo.isClampedToOne = true;
        break;
    case PAN_CURVE_SINCOS:
        panInfo.curve = Util::PAN_CURVE_SHAPE_SINCOS;
        break;
    case PAN_CURVE_SINCOS_0DB:
        panInfo.curve = Util::PAN_CURVE_SHAPE_SINCOS;
        panInfo.isCenterOne = true;
        break;
    case PAN_CURVE_SINCOS_0DB_CLAMP:
        panInfo.curve = Util::PAN_CURVE_SHAPE_SINCOS;
        panInfo.isCenterOne = true;
        panInfo.isClampedToOne = true;
        break;
    case PAN_CURVE_LINEAR:
        panInfo.curve = Util::PAN_CURVE_SHAPE_LINEAR;
        break;
    case PAN_CURVE_LINEAR_0DB:
        panInfo.curve = Util::PAN_CURVE_SHAPE_LINEAR;
        panInfo.isCenterOne = true;
        break;
    case PAN_CURVE_LINEAR_0DB_CLAMP:
        panInfo.curve = Util::PAN_CURVE_SHAPE_LINEAR;
        panInfo.isCenterOne = true;
        panInfo.isClampedToOne = true;
        break;
    }
    panInfo.isSurroundIgnored = m_IsFrontBypass;

    const OutputMode outputMode = HardwareManager::GetInstance().GetOutputMode();
    switch (outputMode) {
    case OUTPUT_MODE_MONO:
        left = right = Util::CalcPanRatio(PAN_CENTER, panInfo);
        break;
    case OUTPUT_MODE_STEREO:
    case OUTPUT_MODE_SURROUND:
        if (m_VoiceCount > 1 && m_PanMode == PAN_MODE_BALANCE) {
            // each channel on its side, the pan as the balance
            switch (channelIndex) {
            case 0:
                left = Util::CalcPanRatio(m_Pan, panInfo);
                break;
            case 1:
                left = right;
                right = Util::CalcPanRatio(PAN_CENTER - m_Pan, panInfo);
                break;
            }
        } else {
            f32 pan = m_Pan;
            if (m_VoiceCount == 2) {
                // the channels around their sides
                switch (channelIndex) {
                case 0:
                    pan -= 1.0f;
                    break;
                case 1:
                    pan += 1.0f;
                    break;
                }
            }
            left = Util::CalcPanRatio(pan, panInfo);
            right = Util::CalcPanRatio(PAN_CENTER - pan, panInfo);
        }
        break;
    }

    switch (outputMode) {
    case OUTPUT_MODE_MONO:
    case OUTPUT_MODE_STEREO:
        front = Util::CalcSurroundPanRatio(SURROUND_PAN_FRONT, panInfo);
        rear = Util::CalcSurroundPanRatio(SURROUND_PAN_REAR, panInfo);
        break;
    case OUTPUT_MODE_SURROUND:
        front = Util::CalcSurroundPanRatio(m_SurroundPan, panInfo);
        rear = Util::CalcSurroundPanRatio(SURROUND_PAN_REAR - m_SurroundPan, panInfo);
        break;
    }

    const f32 frontLeft = front * left;
    const f32 frontRight = front * right;
    const f32 rearLeft = rear * left;
    const f32 rearRight = rear * right;
    pMixParam->m_Main[0] = frontLeft * mainSend;
    pMixParam->m_Main[1] = frontRight * mainSend;
    pMixParam->m_Main[2] = rearLeft * mainSend;
    pMixParam->m_Main[3] = rearRight * mainSend;
    pMixParam->m_Aux[0][0] = fxSendA * frontLeft;
    pMixParam->m_Aux[0][1] = fxSendA * frontRight;
    pMixParam->m_Aux[0][2] = fxSendA * rearLeft;
    pMixParam->m_Aux[0][3] = fxSendA * rearRight;
    pMixParam->m_Aux[1][0] = fxSendB * frontLeft;
    pMixParam->m_Aux[1][1] = fxSendB * frontRight;
    pMixParam->m_Aux[1][2] = fxSendB * rearLeft;
    pMixParam->m_Aux[1][3] = fxSendB * rearRight;
}

// 0x004CA730 | nintendogs:callgraph [tier A]
void nw::snd::internal::driver::Voice::StopFinished()
{
    if (!m_IsActive || !m_IsStarted) {
        return;
    }
    if (m_pVoices[0] == nullptr || m_pLastWaveBuffer == nullptr) {
        return;
    }
    if (m_pLastWaveBuffer->m_Status == nn::snd::CTR::WaveBuffer::STATUS_WAIT ||
        m_pLastWaveBuffer->m_Status == nn::snd::CTR::WaveBuffer::STATUS_PLAY) {
        return;
    }
    if (m_pCallback != nullptr) {
        m_pCallback(this, STATUS_FINISH_WAVE, m_pCallbackArg);
    }
    m_IsStarted = false;
    m_IsStart = false;
}

// 0x004CA79C | nintendogs:callseq-callee [tier A]
void nw::snd::internal::driver::Voice::SetAdpcmParam(int channelIndex, const nn::snd::CTR::AdpcmParam& param)
{
    m_pVoices[channelIndex]->SetAdpcmParam(param);
}

// 0x004CA7A8 | fefates:bytes [tier B] (MultiVoice)
void nw::snd::internal::driver::Voice::SetFrontBypass(bool isBypass)
{
    for (int i = 0; i < m_VoiceCount; ++i) {
        if (m_pVoices[i] != nullptr) {
            m_pVoices[i]->SetFrontBypassFlag(isBypass);
        }
    }
    m_IsFrontBypass = isBypass;
}

// 0x004CA7EC | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SetSurroundPan(f32 surroundPan)
{
    if (m_SurroundPan != surroundPan) {
        m_SurroundPan = surroundPan;
        m_SyncFlag |= UPDATE_MIX;
    }
}

// 0x004CA810 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SetBiquadFilter(int type, f32 value)
{
    if (value > 1.0f) {
        value = 1.0f;
    } else if (value < 0.0f) {
        value = 0.0f;
    }
    bool isChanged = false;
    if (m_BiquadType != type) {
        m_BiquadType = type;
        isChanged = true;
    }
    if (m_BiquadValue != value) {
        m_BiquadValue = value;
        isChanged = true;
    }
    if (isChanged) {
        m_SyncFlag |= UPDATE_BIQUAD;
    }
}

// 0x004CA878 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::AppendWaveBuffer(int channelIndex, nn::snd::CTR::WaveBuffer* pBuffer, bool isLast)
{
    m_pVoices[channelIndex]->AppendWaveBuffer(pBuffer);
    if (isLast) {
        m_pLastWaveBuffer = pBuffer;
    }
}

// 0x004CA8A0 | fefates:bytes [tier B] (MultiVoice)
void nw::snd::internal::driver::Voice::UpdateWaveBuffer(int channelIndex, nn::snd::CTR::WaveBuffer* pBuffer)
{
    m_pVoices[channelIndex]->UpdateWaveBuffer(pBuffer);
}

// 0x004CA8AC | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::CalcOffsetAdpcmParam(nn::snd::CTR::AdpcmContext* pContext, const nn::snd::CTR::AdpcmParam& param, u32 offset,
                                                            const void* pData)
{
    s16 samples[ADPCM_FRAME_SAMPLES];
    const u8* pFrame = static_cast<const u8*>(pData);
    for (u32 i = 0; i < offset; i += ADPCM_FRAME_SAMPLES) {
        nn::snd::CTR::DecodeAdpcmData(pFrame, samples, param, *pContext, ADPCM_FRAME_SAMPLES);
        pFrame += ADPCM_FRAME_BYTES;
    }
}

// 0x004CA900 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SetInterpolationType(u8 type)
{
    for (int i = 0; i < m_VoiceCount; ++i) {
        if (m_pVoices[i] == nullptr) {
            continue;
        }
        int interpolationType = 0;
        switch (type) {
        case 1:
            interpolationType = 1;
            break;
        case 2:
            interpolationType = 2;
            break;
        }
        m_pVoices[i]->SetInterpolationType(static_cast<nn::snd::CTR::InterpolationType>(interpolationType));
    }
}

// 0x004CA964 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::UpdateVoicesPriority()
{
    if (m_Priority == PRIORITY_RELEASE) {
        return;
    }
    for (int i = 0; i < m_VoiceCount; ++i) {
        if (m_pVoices[i] != nullptr) {
            m_pVoices[i]->SetPriority(m_Priority);
        }
    }
}

// 0x004CA9AC (name is ours)
void nw::snd::internal::driver::Voice::Set3dSurroundPreprocessed(bool isPreprocessed)
{
    for (int i = 0; i < m_VoiceCount; ++i) {
        if (m_pVoices[i] != nullptr) {
            m_pVoices[i]->Set3dSurroundPreprocessed(isPreprocessed);
        }
    }
}

// 0x004CA9EC | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SdkVoiceDropCallbackFunc(nn::snd::CTR::Voice*, uptr arg)
{
    Voice* pVoice = reinterpret_cast<Voice*>(arg);
    pVoice->m_pVoices[0] = nullptr;
    pVoice->m_IsPause = false;
    pVoice->m_IsStart = false;
    pVoice->m_VoiceCount = 0;
    VoiceManager::GetInstance().FreeVoice(pVoice);
    pVoice->m_IsActive = false;
    if (pVoice->m_pCallback != nullptr) {
        pVoice->m_pCallback(pVoice, STATUS_DROP_DSP, pVoice->m_pCallbackArg);
    }
}

// 0x004CAA3C | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SdkVoiceDropCallbackFuncMulti(nn::snd::CTR::Voice* pDroppedVoice, uptr arg)
{
    Voice* pVoice = reinterpret_cast<Voice*>(arg);
    if (pVoice->m_IsAllocating) {
        // Alloc frees the others
        pVoice->m_IsAllocFailed = true;
        for (int i = 0; i < pVoice->m_VoiceCount; ++i) {
            if (pVoice->m_pVoices[i] == pDroppedVoice) {
                pVoice->m_pVoices[i] = nullptr;
                return;
            }
        }
    } else {
        // the other channels stop as well
        for (int i = 0; i < pVoice->m_VoiceCount; ++i) {
            nn::snd::CTR::Voice* pChannelVoice = pVoice->m_pVoices[i];
            if (pChannelVoice == nullptr) {
                continue;
            }
            if (pChannelVoice != pDroppedVoice) {
                pChannelVoice->SetState(nn::snd::CTR::Voice::STATE_STOP);
                nn::snd::CTR::FreeVoice(pChannelVoice);
            }
            pVoice->m_pVoices[i] = nullptr;
        }
    }
    pVoice->m_IsPause = false;
    pVoice->m_IsStart = false;
    pVoice->m_VoiceCount = 0;
    VoiceManager::GetInstance().FreeVoice(pVoice);
    pVoice->m_IsActive = false;
    if (pVoice->m_pCallback != nullptr) {
        pVoice->m_pCallback(pVoice, STATUS_DROP_DSP, pVoice->m_pCallbackArg);
    }
}

// 0x004CAB24 | nintendogs:callgraph [tier A]
void nw::snd::internal::driver::Voice::Calc()
{
    if (!m_IsStart) {
        return;
    }
    if (m_SyncFlag & UPDATE_PITCH) {
        f32 pitch = m_Pitch;
        if (pitch > PITCH_MAX) {
            pitch = PITCH_MAX;
        }
        for (int i = 0; i < m_VoiceCount; ++i) {
            if (m_pVoices[i] != nullptr) {
                m_pVoices[i]->SetPitch(pitch);
            }
        }
        m_SyncFlag &= ~UPDATE_PITCH;
    }
    if (m_SyncFlag & UPDATE_VOLUME) {
        const f32 volume = HardwareManager::GetInstance().GetOutputVolume() * m_Volume;
        for (int i = 0; i < m_VoiceCount; ++i) {
            if (m_pVoices[i] != nullptr) {
                m_pVoices[i]->SetVolume(volume);
            }
        }
        m_SyncFlag &= ~UPDATE_VOLUME;
    }
    if (m_SyncFlag & UPDATE_MIX) {
        CalcMix();
        m_SyncFlag &= ~UPDATE_MIX;
    }
    if (m_SyncFlag & UPDATE_LPF) {
        const u16 freq = Util::CalcLpfFreq(m_LpfFreq);
        for (int i = 0; i < m_VoiceCount; ++i) {
            nn::snd::CTR::Voice* pVoice = m_pVoices[i];
            if (pVoice == nullptr) {
                continue;
            }
            if (freq >= LPF_FREQ_OFF) {
                pVoice->EnableMonoFilter(false);
            } else {
                pVoice->EnableMonoFilter(true);
                pVoice->SetMonoFilterCoefficients(freq);
            }
        }
        m_SyncFlag &= ~UPDATE_LPF;
    }
    if (m_SyncFlag & UPDATE_BIQUAD) {
        for (int i = 0; i < m_VoiceCount; ++i) {
            nn::snd::CTR::Voice* pVoice = m_pVoices[i];
            if (pVoice == nullptr) {
                continue;
            }
            BiquadFilterCallback* pFilter = HardwareManager::GetInstance().GetBiquadFilterCallback(m_BiquadType);
            if (pFilter != nullptr && m_BiquadValue > 0.0f) {
                pVoice->EnableBiquadFilter(true);
                nn::snd::CTR::BiquadFilterCoefficients coefficients;
                pFilter->GetCoefficients(m_BiquadType, m_BiquadValue, &coefficients);
                pVoice->SetBiquadFilterCoefficients(coefficients);
            } else {
                pVoice->EnableBiquadFilter(false);
            }
        }
        m_SyncFlag &= ~UPDATE_BIQUAD;
    }
}

// 0x004CAD70 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::Free()
{
    if (!m_IsActive) {
        return;
    }
    for (int i = 0; i < m_VoiceCount; ++i) {
        if (m_pVoices[i] != nullptr) {
            nn::snd::CTR::FreeVoice(m_pVoices[i]);
            m_pVoices[i] = nullptr;
        }
    }
    m_VoiceCount = 0;
    VoiceManager::GetInstance().FreeVoice(this);
    m_IsActive = false;
}

// 0x004CADD8 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::Stop()
{
    if (m_IsStarted) {
        for (int i = 0; i < m_VoiceCount; ++i) {
            if (m_pVoices[i] != nullptr) {
                m_pVoices[i]->SetState(nn::snd::CTR::Voice::STATE_STOP);
            }
        }
        m_IsStarted = false;
    }
    m_IsPausing = false;
    m_IsPause = false;
    m_IsStart = false;
}

// 0x004CAE34 | nintendogs:bytes [tier A]
bool nw::snd::internal::driver::Voice::Alloc(int channelCount, int priority, Callback pCallback, void* pCallbackArg)
{
    const int count = channelCount > CHANNEL_MAX ? CHANNEL_MAX : (channelCount < 1 ? 1 : channelCount);
    m_IsAllocating = true;
    const int sdkPriority = priority < 0 ? 0 : (priority > nn::snd::CTR::Voice::PRIORITY_MAX ? nn::snd::CTR::Voice::PRIORITY_MAX : priority);
    m_VoiceCount = 0;
    m_IsAllocFailed = false;
    if (count == 1) {
        nn::snd::CTR::Voice* pVoice = nn::snd::CTR::AllocVoice(sdkPriority, SdkVoiceDropCallbackFunc, reinterpret_cast<uptr>(this));
        if (pVoice == nullptr) {
            return false;
        }
        m_pVoices[0] = pVoice;
        m_VoiceCount = 1;
    } else {
        for (int i = 0; i < count; ++i) {
            nn::snd::CTR::Voice* pVoice;
            for (;;) {
                pVoice = nn::snd::CTR::AllocVoice(sdkPriority, SdkVoiceDropCallbackFuncMulti, reinterpret_cast<uptr>(this));
                if (pVoice != nullptr) {
                    break;
                }
                if (VoiceManager::GetInstance().DropLowestPriorityVoice(sdkPriority) == 0) {
                    m_IsAllocFailed = true;
                    break;
                }
            }
            m_pVoices[i] = pVoice;
            ++m_VoiceCount;
        }
        if (m_IsAllocFailed) {
            for (int i = 0; i < m_VoiceCount; ++i) {
                if (m_pVoices[i] != nullptr) {
                    nn::snd::CTR::FreeVoice(m_pVoices[i]);
                }
            }
            return false;
        }
    }

    m_IsAllocating = false;
    m_SyncFlag = 0;
    m_IsPause = false;
    m_IsPausing = false;
    m_IsStarted = false;
    m_pLastWaveBuffer = nullptr;
    m_Volume = 1.0f;
    m_LpfFreq = 1.0f;
    m_BiquadType = 0;
    m_BiquadValue = 0.0f;
    m_Pan = 0.0f;
    m_SurroundPan = 0.0f;
    m_MainSend = 1.0f;
    m_pCallback = pCallback;
    m_pCallbackArg = pCallbackArg;
    m_FxSend[0] = 0.0f;
    m_FxSend[1] = 0.0f;
    m_Pitch = 1.0f;
    m_PanMode = PAN_MODE_DUAL;
    m_PanCurve = PAN_CURVE_SQRT;
    m_IsActive = true;
    return true;
}

// 0x004CAFCC | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::Pause(bool isPause)
{
    if (m_IsPause != isPause) {
        m_IsPause = isPause;
        m_SyncFlag |= UPDATE_PAUSE;
    }
}

// 0x004CAFEC | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::Start()
{
    m_IsStart = true;
    m_IsPause = false;
    m_SyncFlag |= UPDATE_START;
}

// 0x004CB00C | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SetPan(f32 pan)
{
    if (m_Pan != pan) {
        m_Pan = pan;
        m_SyncFlag |= UPDATE_MIX;
    }
}

// 0x004CB030 | nintendogs:callgraph [tier A]
void nw::snd::internal::driver::Voice::Update()
{
    if (!m_IsActive) {
        return;
    }
    VoiceState state = VOICE_STATE_NONE;
    if ((m_SyncFlag & UPDATE_START) && m_IsStart && !m_IsStarted) {
        // the first frame: all parameters
        f32 pitch = m_Pitch;
        if (pitch > PITCH_MAX) {
            pitch = PITCH_MAX;
        }
        for (int i = 0; i < m_VoiceCount; ++i) {
            if (m_pVoices[i] != nullptr) {
                m_pVoices[i]->SetPitch(pitch);
            }
        }
        CalcMix();
        const f32 volume = HardwareManager::GetInstance().GetOutputVolume() * m_Volume;
        for (int i = 0; i < m_VoiceCount; ++i) {
            if (m_pVoices[i] != nullptr) {
                m_pVoices[i]->SetVolume(volume);
            }
        }
        m_IsStarted = true;
        state = VOICE_STATE_RUN;
        m_SyncFlag &= ~(UPDATE_START | UPDATE_PITCH | UPDATE_MIX);
    }
    if (m_IsStarted && (m_SyncFlag & UPDATE_PAUSE) && m_IsStart) {
        if (m_IsPause) {
            m_IsPausing = true;
            state = VOICE_STATE_PAUSE;
        } else {
            m_IsPausing = false;
            state = VOICE_STATE_RUN;
        }
        m_SyncFlag &= ~UPDATE_PAUSE;
    }

    switch (state) {
    case VOICE_STATE_RUN:
        for (int i = 0; i < m_VoiceCount; ++i) {
            if (m_pVoices[i] != nullptr) {
                m_pVoices[i]->SetState(nn::snd::CTR::Voice::STATE_PLAY);
            }
        }
        break;
    case VOICE_STATE_STOP:
        for (int i = 0; i < m_VoiceCount; ++i) {
            if (m_pVoices[i] != nullptr) {
                m_pVoices[i]->SetState(nn::snd::CTR::Voice::STATE_STOP);
            }
        }
        break;
    case VOICE_STATE_PAUSE:
        for (int i = 0; i < m_VoiceCount; ++i) {
            if (m_pVoices[i] != nullptr) {
                m_pVoices[i]->SetState(nn::snd::CTR::Voice::STATE_PAUSE);
            }
        }
        break;
    default:
        break;
    }
}

// 0x004CB220 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::CalcMix()
{
    nn::snd::CTR::MixParam mixParam;
    std::memset(&mixParam, 0, sizeof(mixParam));
    for (int i = 0; i < m_VoiceCount; ++i) {
        nn::snd::CTR::Voice* pVoice = m_pVoices[i];
        if (pVoice != nullptr) {
            CalcMixParam(i, &mixParam);
            pVoice->SetMixParam(mixParam);
        }
    }
}

// 0x004CB2E8 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SetPitch(f32 pitch)
{
    if (m_Pitch != pitch) {
        m_Pitch = pitch;
        m_SyncFlag |= UPDATE_PITCH;
    }
}

// 0x004CB30C | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SetFxSend(AuxBus bus, f32 send)
{
    if (send < 0.0f) {
        send = 0.0f;
    }
    if (m_FxSend[bus] != send) {
        m_FxSend[bus] = send;
        m_SyncFlag |= UPDATE_MIX;
    }
}

// 0x004CB348 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SetVolume(f32 volume)
{
    if (volume < 0.0f) {
        volume = 0.0f;
    }
    if (m_Volume != volume) {
        m_Volume = volume;
        m_SyncFlag |= UPDATE_VOLUME;
    }
}

// 0x004CB380 | nintendogs:bytes [tier B]
nw::snd::internal::driver::Voice::Voice()
    : m_pCallback(nullptr), m_IsActive(false), m_IsStart(false), m_IsStarted(false), m_IsPause(false), m_SyncFlag(0)
{
    for (int i = 0; i < CHANNEL_MAX; ++i) {
        m_pVoices[i] = nullptr;
    }
}

// 0x004CB3B0 | nintendogs:bytes [tier B]
nw::snd::internal::driver::Voice::~Voice()
{
    for (int i = 0; i < CHANNEL_MAX; ++i) {
        if (m_pVoices[i] != nullptr) {
            nn::snd::CTR::FreeVoice(m_pVoices[i]);
        }
    }
}

// 0x00741424 | nintendogs:bytes [tier A]
bool nw::snd::internal::driver::Voice::IsRun() const
{
    if (m_pVoices[0] == nullptr) {
        return false;
    }
    return m_pVoices[0]->m_State == nn::snd::CTR::Voice::STATE_PLAY;
}

// 0x00741444 | nintendogs:bytes [tier A]
nw::snd::SampleFormat nw::snd::internal::driver::Voice::GetFormat() const
{
    if (m_pVoices[0] == nullptr) {
        return SAMPLE_FORMAT_PCM_S16;
    }
    return m_Format;
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
