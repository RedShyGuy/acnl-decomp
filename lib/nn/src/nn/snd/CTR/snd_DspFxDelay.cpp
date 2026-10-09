#include "nn/snd/CTR/snd_DspFxDelay.h"
#include <string.h>
#include "nn/dsp/CTR/CTR_Api.h"
#include "nn/os/CTR/MPCore/MPCore_Api.h"
#include "nn/snd/CTR/snd_DspFxManager.h"
#include "nn/snd/CTR/snd_Dspsnd.h"

namespace nn {
namespace snd {
namespace CTR {
namespace {
// the length of a frame of the DSP in microseconds
const u32 FRAME_MICROSECONDS = 4888;
// the bytes of a frame of samples of one channel
const u32 FRAME_BYTES = 640;
const f32 DAMPING_MAX = 0.95f;
} // namespace

// 0x004607B4 (name is ours)
bool nn::snd::CTR::DspFxDelay::AssignWorkBuffer(uptr buffer, size_t size)
{
    if (m_IsInitialized || buffer == 0 || size == 0) {
        return false;
    }
    bool result = false;
    if (m_Buffer == NULL) {
        const uptr deviceAddress = nn::os::CTR::MPCore::ConvertAddressForDevice(buffer, size);
        if (deviceAddress != 0) {
            m_Buffer = reinterpret_cast<void*>(buffer);
            m_DeviceAddress = deviceAddress;
            m_Size = size;
            result = true;
            m_IsInitialized = true;
        }
    }
    return result;
}

// 0x00460838 | fefates:callgraph [tier C]
bool nn::snd::CTR::DspFxDelay::IsBufferInUse()
{
    if (m_IsEnabled) {
        return true;
    }
    // the DSP may still be in the frames after the effect was disabled
    if (g_Dspsnd.m_IsInitialized && static_cast<s8>(g_Dspsnd.m_WaitCount - m_DisabledFrame) <= 2) {
        return true;
    }
    return false;
}

// 0x0046088C (name is ours)
size_t nn::snd::CTR::DspFxDelay::GetRequiredMemSize(const Param& param)
{
    u32 frames = param.m_DelayTime * 1000 / FRAME_MICROSECONDS;
    if (frames == 0) {
        frames = 1;
    }
    const u32 channelCount = param.m_IsEnableSurround ? 4 : 2;
    return channelCount * frames * FRAME_BYTES;
}

// 0x004608CC | fefates:bytes [tier B]
bool nn::snd::CTR::DspFxDelay::Attach(AuxBusId bus)
{
    if (!m_IsInitialized || m_AuxBus != AUX_BUS_NONE) {
        return false;
    }
    if (bus != AUX_BUS_0 && bus != AUX_BUS_1) {
        return false;
    }
    if (!DspFxManager::GetInstance()->Attach(DspFxManager::DSP_EFFECT_TYPE_DELAY, bus)) {
        return false;
    }
    DspFxDelayParams params;
    params.m_Flags = DspFxDelayParams::FLAG_BUFFER;
    params.m_BufferAddress = (m_DeviceAddress >> 16) | (m_DeviceAddress << 16);
    const bool result = DspFxManager::GetInstance()->SetDspDelayEffect(bus, params);
    if (result) {
        m_AuxBus = bus;
    }
    return result;
}

// 0x0046095C | fefates:bytes [tier B]
bool nn::snd::CTR::DspFxDelay::Enable(bool isEnabled)
{
    if (m_AuxBus != AUX_BUS_0 && m_AuxBus != AUX_BUS_1) {
        return false;
    }
    if (m_Buffer == NULL || m_Size == 0) {
        return false;
    }
    DspFxDelayParams params;
    params.m_Flags = DspFxDelayParams::FLAG_ENABLE;
    params.m_IsEnabled = isEnabled;
    const bool result = DspFxManager::GetInstance()->SetDspDelayEffect(static_cast<AuxBusId>(m_AuxBus), params);
    if (result || !isEnabled) {
        m_IsEnabled = isEnabled;
    }
    if (!m_IsEnabled) {
        m_DisabledFrame = g_Dspsnd.m_WaitCount;
    }
    return result;
}

// 0x004609E8 | fefates:bytes [tier B]
void nn::snd::CTR::DspFxDelay::Finalize()
{
    if (!m_IsInitialized) {
        return;
    }
    if (m_AuxBus == AUX_BUS_0 || m_AuxBus == AUX_BUS_1) {
        Enable(false);
        DspFxManager::GetInstance()->Detach(DspFxManager::DSP_EFFECT_TYPE_DELAY, static_cast<AuxBusId>(m_AuxBus));
        m_AuxBus = AUX_BUS_NONE;
    }
    m_Buffer = NULL;
    m_DeviceAddress = 0;
    m_Size = 0;
    m_IsInitialized = false;
}

// 0x00460AB4 | fefates:bytes [tier B]
bool nn::snd::CTR::DspFxDelay::SetParam(const Param& param)
{
    if (m_AuxBus != AUX_BUS_0 && m_AuxBus != AUX_BUS_1) {
        return false;
    }
    if (m_Buffer == NULL || m_Size == 0) {
        return false;
    }
    if (param.m_Damping < 0.0f || param.m_Damping > 1.0f) {
        return false;
    }
    if (param.m_FeedbackGain < 0.0f || param.m_FeedbackGain > 1.0f) {
        return false;
    }
    u32 frames = param.m_DelayTime * 1000 / FRAME_MICROSECONDS;
    if (frames == 0) {
        frames = 1;
    }
    const u32 channelCount = param.m_IsEnableSurround ? 4 : 2;
    if (m_Size < channelCount * frames * FRAME_BYTES) {
        return false;
    }
    DspFxDelayParams params;
    params.m_DelaySamples = frames;
    params.m_ChannelCount = channelCount;
    params.m_Feedback = static_cast<u32>(param.m_FeedbackGain * 128.0f);
    params.m_Flags = DspFxDelayParams::FLAG_PARAM;
    f32 damping = param.m_Damping;
    if (damping > DAMPING_MAX) {
        damping = DAMPING_MAX;
    }
    params.m_LpfB = static_cast<s32>((1.0f - damping) * 128.0f);
    params.m_LpfA = static_cast<s32>(damping * 128.0f);
    memset(m_Buffer, 0, m_Size);
    nn::dsp::CTR::FlushDataCache(reinterpret_cast<uptr>(m_Buffer), m_Size);
    return DspFxManager::GetInstance()->SetDspDelayEffect(static_cast<AuxBusId>(m_AuxBus), params);
}

// 0x00460C1C (name is ours)
nn::snd::CTR::DspFxDelay::DspFxDelay()
{
    m_Buffer = NULL;
    m_DeviceAddress = 0;
    m_Size = 0;
    m_IsInitialized = false;
    m_AuxBus = AUX_BUS_NONE;
    m_IsEnabled = false;
    m_DisabledFrame = 0;
}

} // namespace CTR
} // namespace snd
} // namespace nn
