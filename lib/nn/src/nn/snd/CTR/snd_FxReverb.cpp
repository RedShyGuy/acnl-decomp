#include "nn/snd/CTR/snd_FxReverb.h"
#include <math.h>
#include <string.h>

namespace nn {
namespace snd {
namespace CTR {
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

// the filter lengths of the default parameters (initialized in a static initializer, 0x0079684C)
// 0x00AE1FD4 (name is ours)
ReverbFilterSize s_DefaultReverbFilterSize = {{3040, 3680}, 2080};

// 0x00465964 | nintendogs:bytes [confirmed by fefates] [tier A]
bool nn::snd::CTR::FxReverb::Initialize()
{
    // the work buffer fixes the maximum lengths
    m_MaxEarlyReflectionTime = m_Param.m_EarlyReflectionTime;
    m_MaxPreDelayTime = m_Param.m_PreDelayTime;
    m_MaxFilterSize = m_FilterSize;
    const u32 earlySize = GetFrameCount(m_Param.m_EarlyReflectionTime) * FRAME_BYTES;
    const u32 preDelaySize = GetFrameCount(m_Param.m_PreDelayTime) * FRAME_BYTES;
    const u32 comb0Size = m_FilterSize.m_Comb[0] * 4;
    const u32 comb1Size = m_FilterSize.m_Comb[1] * 4;
    const u32 allPassSize = m_FilterSize.m_AllPass * 4;
    uptr buffer = (reinterpret_cast<uptr>(m_WorkBuffer) + 31) & ~31;
    for (s32 i = 0; i < 2; i++) {
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

// 0x00465A70 | nintendogs:callgraph [confirmed by fefates] [tier A]
void nn::snd::CTR::FxReverb::UpdateBuffer(uptr data)
{
    if (!m_IsActive) {
        return;
    }
    const AuxBusData* auxBusData = reinterpret_cast<const AuxBusData*>(data);
    s32* channels[CHANNEL_MAX];
    channels[0] = auxBusData->m_FrontLeft;
    channels[1] = auxBusData->m_FrontRight;
    channels[2] = auxBusData->m_RearLeft;
    channels[3] = auxBusData->m_RearRight;
    for (s32 i = 0; i < 2; i++) {
        s32* comb1 = m_CombBuffer[i][1] + m_CombPosition[1];
        s32* allPass = m_AllPassBuffer[i] + m_AllPassPosition;
        s32* early = m_EarlyBuffer[i] + m_EarlyPosition;
        s32* preDelay = m_PreDelayBuffer[i] + m_PreDelayPosition;
        s32* comb0 = m_CombBuffer[i][0] + m_CombPosition[0];
        s32* samples = channels[i];
        s32 state = m_LpfState[i];
        for (u32 j = 0; j < FRAME_SAMPLES; j++) {
            const s32 input = *samples;
            const s32 delayed = *preDelay;
            *preDelay++ = input;
            // the comb filters
            const s32 comb0Out = *comb0;
            *comb0++ = MultiplyGain(comb0Out, m_CombGain[0]) + delayed;
            const s32 comb1Out = *comb1;
            *comb1++ = delayed + MultiplyGain(comb1Out, m_CombGain[1]);
            // the all-pass filter
            const s32 allPassOut = *allPass;
            const s32 allPassIn = MultiplyGain(allPassOut, m_AllPassGain) + (comb0Out - comb1Out);
            *allPass++ = allPassIn;
            const s32 fused = allPassOut - MultiplyGain(allPassIn, m_AllPassGain);
            // the early reflections are the input delayed
            const s32 earlyOut = *early;
            *early++ = input;
            state = (state * m_LpfA + m_LpfB * fused) >> 7;
            *samples++ = (m_FusedGain * state + earlyOut * m_EarlyGain) >> 7;
        }
        m_LpfState[i] = state;
    }
    m_EarlyPosition += FRAME_SAMPLES;
    if (m_EarlyPosition >= m_EarlyLength) {
        m_EarlyPosition = 0;
    }
    m_PreDelayPosition += FRAME_SAMPLES;
    if (m_PreDelayPosition >= m_PreDelayLength) {
        m_PreDelayPosition = 0;
    }
    m_CombPosition[0] += FRAME_SAMPLES;
    if (m_CombPosition[0] >= m_CombLength[0]) {
        m_CombPosition[0] = 0;
    }
    m_CombPosition[1] += FRAME_SAMPLES;
    if (m_CombPosition[1] >= m_CombLength[1]) {
        m_CombPosition[1] = 0;
    }
    m_AllPassPosition += FRAME_SAMPLES;
    if (m_AllPassPosition >= m_AllPassLength) {
        m_AllPassPosition = 0;
    }
}

// 0x00465CA0 | fefates:bytes [tier B]
void nn::snd::CTR::FxReverb::InitializeParam()
{
    m_EarlyLength = GetFrameCount(m_Param.m_EarlyReflectionTime) * FRAME_SAMPLES;
    m_EarlyPosition = 0;
    m_PreDelayLength = GetFrameCount(m_Param.m_PreDelayTime) * FRAME_SAMPLES;
    m_PreDelayPosition = 0;
    m_CombLength[0] = m_FilterSize.m_Comb[0];
    m_CombLength[1] = m_FilterSize.m_Comb[1];
    // the comb filters decay by 60 dB in the fused time
    const f32 fusedSamples = m_Param.m_FusedTime * 0.001f * SAMPLE_RATE;
    for (s32 i = 0; i < 2; i++) {
        m_CombPosition[i] = 0;
        m_CombGain[i] = static_cast<s32>(powf(10.0f, static_cast<s32>(m_CombLength[i]) * -3.0f / fusedSamples) * 128.0f);
    }
    m_AllPassLength = m_FilterSize.m_AllPass;
    m_AllPassPosition = 0;
    m_AllPassGain = static_cast<s32>(m_Param.m_Coloration * 128.0f);
    m_EarlyGain = static_cast<s32>(m_Param.m_EarlyGain * 128.0f);
    m_FusedGain = static_cast<s32>(m_Param.m_FusedGain * 128.0f);
    f32 damping = m_Param.m_Damping;
    if (damping > DAMPING_MAX) {
        damping = DAMPING_MAX;
    }
    if (m_Param.m_IsEnableSurround == true) {
        m_LpfB = static_cast<s32>((damping - 1.0f) * 128.0f);
        m_LpfA = static_cast<s32>(damping * -128.0f);
    } else {
        m_LpfB = static_cast<s32>((1.0f - damping) * 128.0f);
        m_LpfA = static_cast<s32>(damping * 128.0f);
    }
    memset(m_WorkBuffer, 0, m_WorkSize);
}

// 0x00465E9C | nintendogs:bytes [confirmed by fefates] [tier A]
bool nn::snd::CTR::FxReverb::AssignWorkBuffer(uptr buffer, size_t size)
{
    if (buffer == 0) {
        return false;
    }
    m_WorkBuffer = reinterpret_cast<void*>(buffer);
    m_WorkSize = size;
    return true;
}

// 0x00465EBC (name is ours)
void nn::snd::CTR::FxReverb::ReleaseWorkBuffer()
{
    m_WorkBuffer = NULL;
}

// 0x00465ED0 | nintendogs:bytes [confirmed by fefates] [tier A]
size_t nn::snd::CTR::FxReverb::GetRequiredMemSize()
{
    const u32 size = GetFrameCount(m_Param.m_EarlyReflectionTime) * FRAME_BYTES + GetFrameCount(m_Param.m_PreDelayTime) * FRAME_BYTES +
                     m_FilterSize.m_Comb[0] * 4 + m_FilterSize.m_Comb[1] * 4 + m_FilterSize.m_AllPass * 4;
    // two channels and the alignment
    return size * 2 + 32;
}

// 0x00465F7C | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::FxReverb::Finalize()
{
    if (!m_IsActive) {
        return;
    }
    m_IsActive = false;
    for (s32 i = 0; i < 2; i++) {
        m_EarlyBuffer[i] = NULL;
        m_PreDelayBuffer[i] = NULL;
        m_CombBuffer[i][0] = NULL;
        m_CombBuffer[i][1] = NULL;
        m_AllPassBuffer[i] = NULL;
    }
}

// 0x00465FC4 | nintendogs:bytes [confirmed by fefates] [tier A]
bool nn::snd::CTR::FxReverb::SetParam(const Param& param)
{
    if (param.m_Coloration < 0.0f || param.m_Coloration > 1.0f || param.m_Damping < 0.0f || param.m_Damping > 1.0f ||
        param.m_EarlyGain < 0.0f || param.m_EarlyGain > 1.0f || param.m_FusedGain < 0.0f || param.m_FusedGain > 1.0f) {
        return false;
    }
    const ReverbFilterSize* filterSize = param.m_FilterSize;
    if (filterSize != NULL && (filterSize->m_Comb[0] == 0 || filterSize->m_Comb[1] == 0 || filterSize->m_AllPass == 0)) {
        return false;
    }
    // while active, within the work buffer
    if (m_IsActive == true) {
        if (param.m_EarlyReflectionTime > m_MaxEarlyReflectionTime || param.m_PreDelayTime > m_MaxPreDelayTime) {
            return false;
        }
        if (filterSize != NULL &&
            (filterSize->m_Comb[0] > m_MaxFilterSize.m_Comb[0] || filterSize->m_Comb[1] > m_MaxFilterSize.m_Comb[1] ||
             filterSize->m_AllPass > m_MaxFilterSize.m_AllPass)) {
            return false;
        }
    }
    m_Param = param;
    if (filterSize != NULL) {
        m_FilterSize = *filterSize;
        m_Param.m_FilterSize = &m_FilterSize;
    }
    if (m_IsActive == true) {
        InitializeParam();
    }
    return true;
}

// 0x00466138 | stores vtable ptr
nn::snd::CTR::FxReverb::FxReverb()
{
    m_Param.m_EarlyReflectionTime = 60;
    m_Param.m_FusedTime = 4000;
    m_Param.m_PreDelayTime = 100;
    m_Param.m_Coloration = 0.5f;
    m_Param.m_Damping = 0.4f;
    m_Param.m_FilterSize = &s_DefaultReverbFilterSize;
    m_Param.m_EarlyGain = 0.6f;
    m_Param.m_FusedGain = 0.4f;
    m_Param.m_IsEnableSurround = false;
    m_WorkBuffer = NULL;
    m_FilterSize = s_DefaultReverbFilterSize;
    m_EarlyGain = 0;
    m_FusedGain = 0;
    m_LpfB = 0;
    m_LpfA = 0;
    m_MaxFilterSize.m_Comb[0] = 3040;
    m_MaxFilterSize.m_Comb[1] = 3680;
    m_MaxFilterSize.m_AllPass = 2080;
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
    for (s32 i = 0; i < CHANNEL_MAX; i++) {
        m_EarlyBuffer[i] = NULL;
        m_PreDelayBuffer[i] = NULL;
        m_CombBuffer[i][0] = NULL;
        m_CombBuffer[i][1] = NULL;
        m_AllPassBuffer[i] = NULL;
        m_Unknown8C[i] = NULL;
        m_LpfState[i] = 0;
    }
}

// 0x004662E4 | virtual slot, introduced by nn::snd::CTR::FxReverb
// 0x0046628C (deleting dtor)
nn::snd::CTR::FxReverb::~FxReverb()
{
    Finalize();
    if (m_WorkBuffer != NULL) {
        m_WorkBuffer = NULL;
    }
}

} // namespace CTR
} // namespace snd
} // namespace nn
