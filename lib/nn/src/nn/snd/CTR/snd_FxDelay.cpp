#include "nn/snd/CTR/snd_FxDelay.h"
#include <string.h>

namespace nn {
namespace snd {
namespace CTR {
namespace {
const u32 FRAME_MICROSECONDS = 4888;
const u32 FRAME_SAMPLES = 160;
const u32 FRAME_BYTES = 640;
const f32 DAMPING_MAX = 0.95f;

// value * gain (1.7 fixed point), rounded towards zero
inline s32 MultiplyGain(s32 value, s32 gain)
{
    const s32 result = ((value >= 0 ? value : -value) * gain) >> 7;
    return value < 0 ? -result : result;
}
} // namespace

// 0x004653F4 | nintendogs:bytes [confirmed by fefates] [tier A]
bool nn::snd::CTR::FxDelay::Initialize()
{
    if (m_IsActive) {
        return false;
    }
    // the work buffer fixes the maximum delay
    m_MaxDelayTime = m_Param.m_DelayTime;
    m_MaxIsEnableSurround = m_Param.m_IsEnableSurround;
    const u32 lineSize = m_DelayFrames * FRAME_BYTES;
    const uptr buffer = (reinterpret_cast<uptr>(m_WorkBuffer) + 31) & ~31;
    for (s32 i = 0; i < m_ChannelCount; i++) {
        m_DelayLine[i] = reinterpret_cast<s32*>(buffer + lineSize * i);
    }
    m_Position = 0;
    memset(m_WorkBuffer, 0, m_WorkSize);
    for (s32 i = 0; i < m_ChannelCount; i++) {
        m_LpfState[i] = 0;
    }
    m_IsActive = true;
    return true;
}

// 0x0046549C | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::FxDelay::UpdateBuffer(uptr data)
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
    const u32 offset = m_Position * FRAME_SAMPLES;
    for (s32 i = 0; i < m_ChannelCount; i++) {
        s32* line = m_DelayLine[i] + offset;
        s32* samples = channels[i];
        s32 state = m_LpfState[i];
        for (u32 j = 0; j < FRAME_SAMPLES; j++) {
            const s32 delayed = *line;
            const s32 input = *samples;
            state = (m_LpfB * (input - MultiplyGain(delayed, m_Feedback)) + m_LpfA * state) >> 7;
            *line++ = state;
            *samples++ = delayed;
        }
        m_LpfState[i] = state;
    }
    m_Position++;
    if (m_DelayFrames <= m_Position) {
        m_Position = 0;
    }
}

// 0x00465594 | nintendogs:bytes [confirmed by fefates] [tier A]
bool nn::snd::CTR::FxDelay::AssignWorkBuffer(uptr buffer, size_t size)
{
    if (buffer == 0) {
        return false;
    }
    m_WorkBuffer = reinterpret_cast<void*>(buffer);
    m_WorkSize = size;
    return true;
}

// 0x004655B4 (name is ours)
void nn::snd::CTR::FxDelay::ReleaseWorkBuffer()
{
    m_WorkBuffer = NULL;
}

// 0x004655C8 | nintendogs:bytes [confirmed by fefates] [tier A]
size_t nn::snd::CTR::FxDelay::GetRequiredMemSize()
{
    // (with room for the alignment)
    return m_DelayFrames * m_ChannelCount * FRAME_BYTES + 32;
}

// 0x004655E4 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::FxDelay::Finalize()
{
    if (!m_IsActive) {
        return;
    }
    m_IsActive = false;
    memset(&m_Param, 0, sizeof(m_Param));
    for (s32 i = 0; i < m_ChannelCount; i++) {
        m_DelayLine[i] = NULL;
    }
}

// 0x00465638 | nintendogs:bytes [confirmed by fefates] [tier A]
bool nn::snd::CTR::FxDelay::SetParam(const Param& param)
{
    if (param.m_Damping < 0.0f || param.m_Damping > 1.0f) {
        return false;
    }
    if (param.m_FeedbackGain < 0.0f || param.m_FeedbackGain > 1.0f) {
        return false;
    }
    // while active, within the work buffer
    if (m_IsActive == true) {
        if (param.m_DelayTime > m_MaxDelayTime) {
            return false;
        }
        if (!m_MaxIsEnableSurround && param.m_IsEnableSurround == true) {
            return false;
        }
    }
    m_DelayFrames = param.m_DelayTime * 1000 / FRAME_MICROSECONDS;
    if (m_DelayFrames == 0) {
        m_DelayFrames = 1;
    }
    m_ChannelCount = param.m_IsEnableSurround ? 4 : 2;
    m_Feedback = static_cast<s32>(param.m_FeedbackGain * 128.0f);
    f32 damping = param.m_Damping;
    if (damping > DAMPING_MAX) {
        damping = DAMPING_MAX;
    }
    m_LpfB = static_cast<s32>((1.0f - damping) * 128.0f);
    m_LpfA = static_cast<s32>(damping * 128.0f);
    m_Param = param;
    return true;
}

// 0x00465768 | nintendogs:bytes [confirmed by fefates] [tier A]
nn::snd::CTR::FxDelay::FxDelay()
{
    m_Param.m_DelayTime = 250;
    m_Param.m_FeedbackGain = 0.4f;
    m_Param.m_Damping = 0.5f;
    m_Param.m_IsEnableSurround = false;
    m_WorkBuffer = NULL;
    m_WorkSize = 0;
    m_Position = 0;
    m_Feedback = 0;
    m_LpfB = 0x10000;
    m_LpfA = 0;
    m_ChannelCount = CHANNEL_MAX;
    m_IsActive = false;
    for (s32 i = 0; i < CHANNEL_MAX; i++) {
        m_DelayLine[i] = NULL;
    }
    for (s32 i = 0; i < m_ChannelCount; i++) {
        m_LpfState[i] = 0;
    }
}

// 0x00465874 | fefates:bytes [tier B]
// 0x00465804 (deleting dtor)
nn::snd::CTR::FxDelay::~FxDelay()
{
    Finalize();
    if (m_WorkBuffer != NULL) {
        m_WorkBuffer = NULL;
    }
}

} // namespace CTR
} // namespace snd
} // namespace nn
