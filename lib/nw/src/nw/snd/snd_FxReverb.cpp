#include "nw/snd/snd_FxReverb.h"

#include <math.h>
#include <cstring>

namespace nw {
namespace snd {
namespace {
const u32 FRAME_SAMPLES = 160;
const u32 FRAME_BYTES = 640;
const f32 FRAME_MILLISECONDS = 4.888f;
const f32 SAMPLE_RATE = 32728.0f;
const f32 DAMPING_MAX = 0.95f;

// the frames of a time (at least one)
inline u32 GetFrameCount(u32 milliSeconds)
{
    f32 time = static_cast<f32>(milliSeconds);
    if (!(time > FRAME_MILLISECONDS)) {
        time = FRAME_MILLISECONDS;
    }
    return static_cast<u32>(time * (1.0f / FRAME_MILLISECONDS));
}

// value * gain (1.7 fixed point), rounded towards zero
inline s32 MultiplyGain(s32 value, s32 gain)
{
    const s32 result = ((value >= 0 ? value : -value) * gain) >> 7;
    return value < 0 ? -result : result;
}
} // namespace

// the filter lengths of the default parameters
// 0x00AF0CA8 (name is ours)
nn::snd::CTR::ReverbFilterSize s_NwDefaultReverbFilterSize = {{3040, 3680}, 2080};

// sets s_NwDefaultReverbFilterSize
// 0x00796868 (static initializer)

// 0x004C3F3C slot 0x08 | nintendogs:bytes
bool nw::snd::FxReverb::Initialize()
{
    // the work buffer fixes the maximum lengths
    m_MaxIsEnableSurround = m_Param.isEnableSurround;
    m_MaxFilterSize = m_FilterSize;
    m_MaxPreDelayTime = m_Param.preDelayTime;
    m_MaxEarlyReflectionTime = m_Param.earlyReflectionTime;
    const u32 earlySize = GetFrameCount(m_Param.earlyReflectionTime) * FRAME_BYTES;
    const u32 preDelaySize = GetFrameCount(m_Param.preDelayTime) * FRAME_BYTES;
    const u32 comb0Size = m_FilterSize.m_Comb[0] * 4;
    const u32 comb1Size = m_FilterSize.m_Comb[1] * 4;
    const u32 allPassSize = m_FilterSize.m_AllPass * 4;
    uptr buffer = (reinterpret_cast<uptr>(m_WorkBuffer) + 31) & ~31;
    for (int i = 0; i < m_ChannelCount; i++) {
        m_EarlyBuffer[i] = reinterpret_cast<s32*>(buffer);
        buffer += earlySize;
        m_PreDelayBuffer[i] = reinterpret_cast<s32*>(buffer);
        buffer += preDelaySize;
        m_CombBuffer[i][0] = reinterpret_cast<s32*>(buffer);
        buffer += comb0Size;
        m_CombBuffer[i][1] = reinterpret_cast<s32*>(buffer);
        buffer += comb1Size;
        m_AllPassBuffer[i] = reinterpret_cast<s32*>(buffer);
        buffer += allPassSize;
    }
    InitializeParam();
    m_IsActive = true;
    return true;
}

// 0x004C4054 slot 0x10
void nw::snd::FxReverb::UpdateBuffer(int /*channelCount*/, nn::snd::CTR::AuxBusData* pData, int sampleLength, SampleFormat /*format*/,
                                     f32 /*sampleRate*/, OutputMode /*outputMode*/)
{
    if (!m_IsActive) {
        return;
    }
    s32* channels[CHANNEL_MAX];
    channels[0] = pData->m_FrontLeft;
    channels[1] = pData->m_FrontRight;
    channels[2] = pData->m_RearLeft;
    channels[3] = pData->m_RearRight;
    u32 earlyPosition = 0;
    u32 preDelayPosition = 0;
    u32 comb0Position = 0;
    u32 comb1Position = 0;
    u32 allPassPosition = 0;
    for (int i = 0; i < m_ChannelCount; i++) {
        s32* early = m_EarlyBuffer[i];
        s32* preDelay = m_PreDelayBuffer[i];
        comb0Position = m_CombPosition[0];
        comb1Position = m_CombPosition[1];
        preDelayPosition = m_PreDelayPosition;
        allPassPosition = m_AllPassPosition;
        earlyPosition = m_EarlyPosition;
        s32 state = m_LpfState[i];
        for (int j = 0; j < sampleLength; j++) {
            s32* samples = channels[i];
            const s32 input = samples[j];
            // the early reflections are the input delayed
            const s32 earlyOut = early[earlyPosition];
            early[earlyPosition] = input;
            const s32 delayed = preDelay[preDelayPosition];
            preDelay[preDelayPosition] = input;
            const s32 earlyScaled = earlyOut * m_EarlyGain;
            // the comb filters
            s32* comb0 = m_CombBuffer[i][0];
            const s32 comb0Out = comb0[comb0Position];
            comb0[comb0Position] = MultiplyGain(comb0Out, m_CombGain[0]) + delayed;
            s32* comb1 = m_CombBuffer[i][1];
            const s32 comb1Out = comb1[comb1Position];
            comb1[comb1Position] = delayed + MultiplyGain(comb1Out, m_CombGain[1]);
            earlyPosition++;
            preDelayPosition++;
            comb0Position++;
            comb1Position++;
            // the all-pass filter
            s32* allPass = m_AllPassBuffer[i];
            const s32 allPassOut = allPass[allPassPosition];
            const s32 allPassIn = MultiplyGain(allPassOut, m_AllPassGain) + (comb0Out - comb1Out);
            allPass[allPassPosition] = allPassIn;
            allPassPosition++;
            const s32 fused = allPassOut - MultiplyGain(allPassIn, m_AllPassGain);
            state = (m_LpfB * fused + state * m_LpfA) >> 7;
            samples[j] = (m_FusedGain * state + earlyScaled) >> 7;
        }
        m_LpfState[i] = state;
    }
    m_EarlyPosition = earlyPosition < m_EarlyLength ? earlyPosition : 0;
    m_PreDelayPosition = preDelayPosition < m_PreDelayLength ? preDelayPosition : 0;
    m_CombPosition[0] = comb0Position < m_CombLength[0] ? comb0Position : 0;
    m_CombPosition[1] = comb1Position < m_CombLength[1] ? comb1Position : 0;
    m_AllPassPosition = allPassPosition < m_AllPassLength ? allPassPosition : 0;
}

// 0x004C42C4 | fefates:bytes [tier B]
void nw::snd::FxReverb::InitializeParam()
{
    m_EarlyLength = GetFrameCount(m_Param.earlyReflectionTime) * FRAME_SAMPLES;
    m_EarlyPosition = 0;
    m_PreDelayLength = GetFrameCount(m_Param.preDelayTime) * FRAME_SAMPLES;
    m_PreDelayPosition = 0;
    m_CombLength[0] = m_FilterSize.m_Comb[0];
    m_CombLength[1] = m_FilterSize.m_Comb[1];
    // the comb filters decay by 60 dB in the fused time
    const f32 fusedSamples = m_Param.fusedTime * 0.001f * SAMPLE_RATE;
    for (int i = 0; i < 2; i++) {
        m_CombPosition[i] = 0;
        m_CombGain[i] = static_cast<s32>(powf(10.0f, static_cast<s32>(m_CombLength[i]) * -3.0f / fusedSamples) * 128.0f);
    }
    m_AllPassLength = m_FilterSize.m_AllPass;
    m_AllPassPosition = 0;
    m_AllPassGain = static_cast<s32>(m_Param.coloration * 128.0f);
    m_EarlyGain = static_cast<s32>(m_Param.earlyGain * 128.0f);
    m_FusedGain = static_cast<s32>(m_Param.fusedGain * 128.0f);
    f32 damping = m_Param.damping;
    if (damping > DAMPING_MAX) {
        damping = DAMPING_MAX;
    }
    if (m_Param.isInvertDamping == true) {
        m_LpfA = static_cast<s32>(damping * -128.0f);
        m_LpfB = static_cast<s32>((damping - 1.0f) * 128.0f);
    } else {
        m_LpfB = static_cast<s32>((1.0f - damping) * 128.0f);
        m_LpfA = static_cast<s32>(damping * 128.0f);
    }
    std::memset(m_WorkBuffer, 0, m_WorkSize);
}

// 0x004C449C | nintendogs:bytes [tier B]
bool nw::snd::FxReverb::AssignWorkBuffer(uptr buffer, u32 size)
{
    if (buffer == 0) {
        return false;
    }
    m_WorkBuffer = reinterpret_cast<void*>(buffer);
    m_WorkSize = size;
    return true;
}

// 0x004C44BC (name after nn::snd::CTR::FxReverb)
void nw::snd::FxReverb::ReleaseWorkBuffer()
{
    m_WorkBuffer = nullptr;
}

// 0x004C44D0 | nintendogs:bytes [tier B]
u32 nw::snd::FxReverb::GetRequiredMemSize()
{
    const u32 size = GetFrameCount(m_Param.earlyReflectionTime) * FRAME_BYTES + GetFrameCount(m_Param.preDelayTime) * FRAME_BYTES +
                     m_FilterSize.m_Comb[0] * 4 + m_FilterSize.m_Comb[1] * 4 + m_FilterSize.m_AllPass * 4;
    // the channels and the alignment
    return size * m_ChannelCount + 32;
}

// 0x004C4570 slot 0x0C | nintendogs:bytes
void nw::snd::FxReverb::Finalize()
{
    if (!m_IsActive) {
        return;
    }
    m_IsActive = false;
    for (int i = 0; i < m_ChannelCount; i++) {
        m_EarlyBuffer[i] = nullptr;
        m_PreDelayBuffer[i] = nullptr;
        m_CombBuffer[i][0] = nullptr;
        m_CombBuffer[i][1] = nullptr;
        m_AllPassBuffer[i] = nullptr;
    }
}

// 0x004C45CC | nintendogs:bytes [tier B]
bool nw::snd::FxReverb::SetParam(const Param& param)
{
    if (param.coloration < 0.0f || param.coloration > 1.0f || param.damping < 0.0f || param.damping > 1.0f ||
        param.earlyGain < 0.0f || param.earlyGain > 1.0f || param.fusedGain < 0.0f || param.fusedGain > 1.0f) {
        return false;
    }
    const nn::snd::CTR::ReverbFilterSize* filterSize = param.filterSize;
    if (filterSize != nullptr && (filterSize->m_Comb[0] == 0 || filterSize->m_Comb[1] == 0 || filterSize->m_AllPass == 0)) {
        return false;
    }
    // while active, within the work buffer
    if (m_IsActive == true) {
        if (param.earlyReflectionTime > m_MaxEarlyReflectionTime || param.preDelayTime > m_MaxPreDelayTime) {
            return false;
        }
        if (filterSize != nullptr &&
            (filterSize->m_Comb[0] > m_MaxFilterSize.m_Comb[0] || filterSize->m_Comb[1] > m_MaxFilterSize.m_Comb[1] ||
             filterSize->m_AllPass > m_MaxFilterSize.m_AllPass)) {
            return false;
        }
        if (!m_MaxIsEnableSurround && param.isEnableSurround == true) {
            return false;
        }
    }
    m_ChannelCount = param.isEnableSurround ? 4 : 2;
    m_Param = param;
    if (m_Param.filterSize != nullptr) {
        m_FilterSize = *m_Param.filterSize;
        m_Param.filterSize = &m_FilterSize;
    }
    if (m_IsActive == true) {
        InitializeParam();
    }
    return true;
}

// 0x004C4758 | nintendogs:bytes [tier B]
nw::snd::FxReverb::FxReverb()
{
    m_Param.earlyReflectionTime = 60;
    m_Param.fusedTime = 4000;
    m_Param.preDelayTime = 100;
    m_Param.coloration = 0.5f;
    m_Param.damping = 0.4f;
    m_Param.filterSize = &s_NwDefaultReverbFilterSize;
    m_Param.earlyGain = 0.6f;
    m_Param.fusedGain = 0.4f;
    m_Param.isEnableSurround = false;
    m_Param.isInvertDamping = false;
    m_WorkBuffer = nullptr;
    m_FilterSize = s_NwDefaultReverbFilterSize;
    m_EarlyGain = 0;
    m_FusedGain = 0;
    m_LpfB = 0;
    m_LpfA = 0;
    m_MaxFilterSize.m_Comb[0] = 3040;
    m_MaxFilterSize.m_Comb[1] = 3680;
    m_MaxFilterSize.m_AllPass = 2080;
    m_ChannelCount = CHANNEL_MAX;
    m_IsActive = false;
    m_EarlyLength = FRAME_SAMPLES;
    m_EarlyPosition = 0;
    m_PreDelayLength = FRAME_SAMPLES;
    m_PreDelayPosition = 0;
    m_CombLength[0] = FRAME_SAMPLES;
    m_CombPosition[0] = 0;
    m_CombGain[0] = 0;
    m_CombLength[1] = FRAME_SAMPLES;
    m_CombPosition[1] = 0;
    m_CombGain[1] = 0;
    m_AllPassLength = FRAME_SAMPLES;
    m_AllPassPosition = 0;
    m_AllPassGain = 0;
    for (int i = 0; i < CHANNEL_MAX; i++) {
        m_EarlyBuffer[i] = nullptr;
        m_PreDelayBuffer[i] = nullptr;
        m_CombBuffer[i][0] = nullptr;
        m_CombBuffer[i][1] = nullptr;
        m_AllPassBuffer[i] = nullptr;
        m_Unknown94[i] = nullptr;
        m_LpfState[i] = 0;
    }
}

// 0x004C4934 slot 0x00
// 0x004C48BC (deleting dtor)
nw::snd::FxReverb::~FxReverb()
{
    Finalize();
    if (m_WorkBuffer != nullptr) {
        m_WorkBuffer = nullptr;
    }
}

} // namespace snd
} // namespace nw
