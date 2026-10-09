#include "nn/snd/CTR/snd_Voice.h"
#include <math.h>
#include <string.h>
#include "nn/math/math_Api.h"
#include "nn/snd/CTR/snd_VoiceImpl.h"
#include "nn/snd/CTR/snd_VoiceManager.h"

namespace nn {
namespace snd {
namespace CTR {
// 0x00463634 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::Voice::Initialize()
{
    m_State = STATE_PAUSE;
    m_SampleRate = DEFAULT_SAMPLE_RATE;
    m_Pitch = 1.0f;
    m_InterpolationType = 0;
    m_FilterType = 0;
    memset(&m_MonoFilter, 0, sizeof(m_MonoFilter));
    memset(&m_BiquadFilter, 0, sizeof(m_BiquadFilter));
    m_Volume = 1.0f;
    m_Priority = 0;
    m_Prev = NULL;
    m_Next = NULL;
    m_DropCallback = NULL;
    // the main bus, front left and right
    MixParam mixParam;
    mixParam.m_Main[0] = 1.0f;
    mixParam.m_Main[1] = 1.0f;
    m_MixParam = mixParam;
    m_DropCallbackArg = 0;
    m_Impl->Initialize();
}

// 0x00463710 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
void nn::snd::CTR::Voice::SetPriority(int priority)
{
    if (priority > PRIORITY_MAX) {
        priority = PRIORITY_MAX;
    }
    if (priority < 0) {
        priority = 0;
    }
    m_Priority = priority;
    g_VoiceManager.SetPriority(this, priority);
}

// 0x00463744 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::Voice::EnableBiquadFilter(bool isEnabled)
{
    if (isEnabled) {
        m_FilterType |= FILTER_TYPE_BIQUAD;
    } else {
        m_FilterType &= ~FILTER_TYPE_BIQUAD;
    }
    m_Impl->SetFilterType(static_cast<FilterType>(m_FilterType));
}

// 0x00463764 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::Voice::SetMonoFilterCoefficients(u16 cutoffFrequency)
{
    // a one pole low pass filter (1.15 fixed point coefficients)
    if (cutoffFrequency > 16000) {
        cutoffFrequency = 16000;
    }
    const f32 c = 2.0f - nn::math::CosFIdx(cutoffFrequency * 0.008f);
    const f32 b = sqrtf(c * c - 1.0f) - c;
    m_MonoFilter.m_A0 = static_cast<s32>((b + 1.0f) * 32768.0f);
    m_MonoFilter.m_B0 = -static_cast<s32>(b * 32768.0f);
    m_Impl->SetMonoFilterCoefficients(m_MonoFilter);
}

// 0x004637F0 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::Voice::SetPitch(float pitch)
{
    m_Pitch = (pitch >= 0.0f) ? pitch : 0.0f;
    m_Impl->SetPitch(m_Pitch);
}

// 0x00463810 | nintendogs:bytes [confirmed by fefates] [tier A]
nn::snd::CTR::Voice::Voice(int id) : m_Id(id)
{
}

// 0x0046647C | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
void nn::snd::CTR::Voice::SetMixParam(const MixParam& mixParam)
{
    m_MixParam = mixParam;
    m_Impl->SetMixParam(mixParam);
}

// 0x00466A34 (name is ours)
void nn::snd::CTR::Voice::SetAdpcmParam(const AdpcmParam& param)
{
    m_Impl->SetAdpcmParam(param);
}

// 0x00466A60 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::Voice::EnableMonoFilter(bool isEnabled)
{
    if (isEnabled) {
        m_FilterType |= FILTER_TYPE_MONO;
    } else {
        m_FilterType &= ~FILTER_TYPE_MONO;
    }
    m_Impl->SetFilterType(static_cast<FilterType>(m_FilterType));
}

// 0x00466A94 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::Voice::SetSampleRate(int sampleRate)
{
    m_SampleRate = (sampleRate < 0) ? 0 : sampleRate;
    m_Impl->SetSampleRate(sampleRate);
}

// 0x00466CCC (name is ours)
void nn::snd::CTR::Voice::SetChannelCount(int count)
{
    m_Impl->SetChannelCount(count);
}

// 0x00466CEC (name is ours)
void nn::snd::CTR::Voice::SetSampleFormat(SampleFormat format)
{
    m_Impl->SetSampleFormat(format);
}

// 0x00466D10 | nintendogs:callgraph [confirmed by fefates, mk7dlp] [tier A]
void nn::snd::CTR::Voice::AppendWaveBuffer(WaveBuffer* buffer)
{
    m_Impl->AppendWaveBuffer(buffer);
}

// 0x00466DB4 (name is ours)
void nn::snd::CTR::Voice::UpdateWaveBuffer(WaveBuffer* buffer)
{
    m_Impl->UpdateWaveBuffer(buffer);
}

// 0x00466E88 | fefates:callgraph [tier C]
void nn::snd::CTR::Voice::SetFrontBypassFlag(bool isBypass)
{
    m_Impl->SetFrontBypassFlag(isBypass);
}

// 0x00466EA4 | nintendogs:callgraph [confirmed by fefates, mk7dlp] [tier A]
void nn::snd::CTR::Voice::SetInterpolationType(InterpolationType type)
{
    m_InterpolationType = type;
    m_Impl->SetInterpolationType(type);
}

// 0x00466ED8 | fefates:callgraph [tier C]
void nn::snd::CTR::Voice::Set3dSurroundPreprocessed(bool isPreprocessed)
{
    m_Impl->Set3dSurroundPreprocessed(isPreprocessed);
}

// 0x00466F0C | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::Voice::SetBiquadFilterCoefficients(const BiquadFilterCoefficients& coefficients)
{
    m_BiquadFilter = coefficients;
    m_Impl->SetBiquadFilterCoefficients(m_BiquadFilter);
}

// 0x00466FAC | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
void nn::snd::CTR::Voice::SetState(State state)
{
    m_State = state;
    if (state == STATE_STOP) {
        m_Impl->ReleaseWaveBuffer();
    }
    m_Impl->SetState(state);
}

// 0x00467044 | fefates:callgraph [tier C]
void nn::snd::CTR::Voice::SetVolume(float volume)
{
    m_Volume = volume;
    m_Impl->SetVolume(volume);
}

} // namespace CTR
} // namespace snd
} // namespace nn
