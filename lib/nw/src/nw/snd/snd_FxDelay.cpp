#include "nw/snd/snd_FxDelay.h"

#include <cstring>

namespace nw {
namespace snd {
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

// 0x004C3A38 slot 0x08 | nintendogs:bytes
bool nw::snd::FxDelay::Initialize()
{
    if (m_IsActive) {
        return false;
    }
    // the work buffer fixes the maximum delay
    m_MaxDelayTime = m_Param.delayTime;
    m_MaxIsEnableSurround = m_Param.isEnableSurround;
    const u32 lineSize = m_DelayFrames * FRAME_BYTES;
    const uptr buffer = (reinterpret_cast<uptr>(m_WorkBuffer) + 31) & ~31;
    for (int i = 0; i < m_ChannelCount; i++) {
        m_DelayLine[i] = reinterpret_cast<s32*>(buffer + lineSize * i);
    }
    m_Position = 0;
    std::memset(m_WorkBuffer, 0, m_WorkSize);
    for (int i = 0; i < m_ChannelCount; i++) {
        m_LpfState[i] = 0;
    }
    m_IsActive = true;
    return true;
}

// 0x004C3AE0 slot 0x10 | nintendogs:bytes
void nw::snd::FxDelay::UpdateBuffer(int /*channelCount*/, nn::snd::CTR::AuxBusData* pData, int sampleLength, SampleFormat /*format*/,
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
    const u32 offset = m_Position * FRAME_SAMPLES;
    for (u32 i = 0; i < m_ChannelCount; i++) {
        s32* samples = channels[i];
        s32* line = m_DelayLine[i];
        s32* pState = &m_LpfState[i];
        u32 position = offset;
        for (u32 j = 0; j < static_cast<u32>(sampleLength); j++) {
            const s32 delayed = line[position];
            const s32 input = samples[j];
            const s32 state = (m_LpfB * (input - MultiplyGain(delayed, m_Feedback)) + m_LpfA * *pState) >> 7;
            *pState = state;
            line[position] = state;
            samples[j] = delayed;
            position++;
        }
    }
    m_Position++;
    if (m_DelayFrames <= m_Position) {
        m_Position = 0;
    }
}

// 0x004C3BE8 | nintendogs:bytes [tier B]
bool nw::snd::FxDelay::AssignWorkBuffer(uptr buffer, u32 size)
{
    if (buffer == 0) {
        return false;
    }
    m_WorkBuffer = reinterpret_cast<void*>(buffer);
    m_WorkSize = size;
    return true;
}

// 0x004C3C08 (name after nn::snd::CTR::FxDelay)
void nw::snd::FxDelay::ReleaseWorkBuffer()
{
    m_WorkBuffer = nullptr;
}

// 0x004C3C1C | nintendogs:bytes [tier B]
u32 nw::snd::FxDelay::GetRequiredMemSize()
{
    // (with room for the alignment)
    return m_DelayFrames * m_ChannelCount * FRAME_BYTES + 32;
}

// 0x004C3C38 slot 0x0C | nintendogs:bytes
void nw::snd::FxDelay::Finalize()
{
    if (!m_IsActive) {
        return;
    }
    m_IsActive = false;
    std::memset(&m_Param, 0, sizeof(m_Param));
    for (int i = 0; i < m_ChannelCount; i++) {
        m_DelayLine[i] = nullptr;
    }
}

// 0x004C3C8C | nintendogs:bytes [tier B]
bool nw::snd::FxDelay::SetParam(const Param& param)
{
    if (param.damping < 0.0f || param.damping > 1.0f) {
        return false;
    }
    if (param.feedbackGain < 0.0f || param.feedbackGain > 1.0f) {
        return false;
    }
    // while active, within the work buffer
    if (m_IsActive == true) {
        if (param.delayTime > m_MaxDelayTime) {
            return false;
        }
        if (!m_MaxIsEnableSurround && param.isEnableSurround == true) {
            return false;
        }
    }
    m_DelayFrames = param.delayTime * 1000 / FRAME_MICROSECONDS;
    if (m_DelayFrames == 0) {
        m_DelayFrames = 1;
    }
    m_ChannelCount = param.isEnableSurround ? 4 : 2;
    m_Feedback = static_cast<s32>(param.feedbackGain * 128.0f);
    f32 damping = param.damping;
    if (damping > DAMPING_MAX) {
        damping = DAMPING_MAX;
    }
    m_LpfB = static_cast<s32>((1.0f - damping) * 128.0f);
    m_LpfA = static_cast<s32>(damping * 128.0f);
    m_Param = param;
    return true;
}

// 0x004C3DB4 | nintendogs:bytes [tier B]
nw::snd::FxDelay::FxDelay()
{
    m_Param.delayTime = 250;
    m_Param.feedbackGain = 0.4f;
    m_Param.damping = 0.5f;
    m_Param.isEnableSurround = false;
    m_WorkBuffer = nullptr;
    m_WorkSize = 0;
    m_Position = 0;
    m_Feedback = 0;
    m_LpfB = 0x10000;
    m_LpfA = 0;
    m_ChannelCount = CHANNEL_MAX;
    m_IsActive = false;
    for (int i = 0; i < m_ChannelCount; i++) {
        m_DelayLine[i] = nullptr;
    }
    for (int i = 0; i < m_ChannelCount; i++) {
        m_LpfState[i] = 0;
    }
}

// 0x004C3ECC slot 0x00
// 0x004C3E5C (deleting dtor)
nw::snd::FxDelay::~FxDelay()
{
    Finalize();
    if (m_WorkBuffer != nullptr) {
        m_WorkBuffer = nullptr;
    }
}

} // namespace snd
} // namespace nw
