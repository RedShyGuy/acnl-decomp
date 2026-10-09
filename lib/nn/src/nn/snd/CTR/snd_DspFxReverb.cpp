#include "nn/snd/CTR/snd_DspFxReverb.h"
#include <math.h>
#include <string.h>
#include "nn/dsp/CTR/CTR_Api.h"
#include "nn/os/CTR/MPCore/MPCore_Api.h"
#include "nn/snd/CTR/snd_DspFxManager.h"
#include "nn/snd/CTR/snd_Dspsnd.h"

// the DSP takes 32 bit addresses in 16 bit halves (a macro: the conversion is called twice)
#define DSP_ADDRESS(address) (((address) >> 16) | ((address) << 16))

namespace nn {
namespace snd {
namespace CTR {
namespace {
const u32 FRAME_MICROSECONDS = 4888;
const u32 FRAME_SAMPLES = 160;
const u32 FRAME_BYTES = 640;
const f32 SAMPLE_RATE = 32728.0f;
const f32 DAMPING_MAX = 0.95f;

inline u32 GetFrameCount(u32 milliSeconds)
{
    s32 frames = milliSeconds * 1000 / FRAME_MICROSECONDS;
    if (frames < 1) {
        frames = 1;
    }
    return frames;
}
} // namespace

// the filter lengths of DspFxReverb::Param::m_FilterSize == NULL
// 0x0097F060 (name is ours)
ReverbFilterSize s_DefaultDspReverbFilterSize = {{3040, 3680}, 2080};

// 0x00460D18 (name is ours)
bool nn::snd::CTR::DspFxReverb::AssignWorkBuffer(uptr buffer, size_t size)
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

// 0x00460D9C | fefates:callgraph [tier C]
bool nn::snd::CTR::DspFxReverb::IsBufferInUse()
{
    if (m_IsEnabled) {
        return true;
    }
    if (g_Dspsnd.m_IsInitialized && static_cast<s8>(g_Dspsnd.m_WaitCount - m_DisabledFrame) <= 2) {
        return true;
    }
    return false;
}

// 0x00460DF0 (name is ours)
size_t nn::snd::CTR::DspFxReverb::GetRequiredMemSize(const Param& param)
{
    const u32 earlyFrames = GetFrameCount(param.m_EarlyReflectionTime);
    const u32 preDelayFrames = GetFrameCount(param.m_PreDelayTime);
    const ReverbFilterSize* filterSize = param.m_FilterSize;
    if (filterSize == NULL) {
        filterSize = &s_DefaultDspReverbFilterSize;
    }
    // two channels of 32 bit samples
    return (filterSize->m_Comb[0] + filterSize->m_Comb[1] + filterSize->m_AllPass + (preDelayFrames + earlyFrames) * FRAME_SAMPLES) * 8;
}

// 0x00460E64 | fefates:bytes [tier B]
bool nn::snd::CTR::DspFxReverb::Attach(AuxBusId bus)
{
    if (!m_IsInitialized || m_AuxBus != AUX_BUS_NONE) {
        return false;
    }
    if (bus != AUX_BUS_0 && bus != AUX_BUS_1) {
        return false;
    }
    const bool result = DspFxManager::GetInstance()->Attach(DspFxManager::DSP_EFFECT_TYPE_REVERB, bus);
    if (result) {
        m_AuxBus = bus;
    }
    return result;
}

// 0x00460EBC | fefates:bytes [tier B]
bool nn::snd::CTR::DspFxReverb::Enable(bool isEnabled)
{
    if (m_AuxBus != AUX_BUS_0 && m_AuxBus != AUX_BUS_1) {
        return false;
    }
    if (m_Buffer == NULL || m_Size == 0) {
        return false;
    }
    DspFxReverbParams params;
    params.m_Flags = DspFxDelayParams::FLAG_ENABLE;
    params.m_IsEnabled = isEnabled;
    const bool result = DspFxManager::GetInstance()->SetDspReverbEffect(static_cast<AuxBusId>(m_AuxBus), params);
    if (result || !isEnabled) {
        m_IsEnabled = isEnabled;
    }
    if (!m_IsEnabled) {
        m_DisabledFrame = g_Dspsnd.m_WaitCount;
    }
    return result;
}

// 0x00460F48 | fefates:bytes [tier B]
void nn::snd::CTR::DspFxReverb::Finalize()
{
    if (!m_IsInitialized) {
        return;
    }
    if (m_AuxBus == AUX_BUS_0 || m_AuxBus == AUX_BUS_1) {
        Enable(false);
        DspFxManager::GetInstance()->Detach(DspFxManager::DSP_EFFECT_TYPE_REVERB, static_cast<AuxBusId>(m_AuxBus));
        m_AuxBus = AUX_BUS_NONE;
    }
    m_Buffer = NULL;
    m_DeviceAddress = 0;
    m_Size = 0;
    m_IsInitialized = false;
}

// 0x00461014 | fefates:bytes [tier B]
bool nn::snd::CTR::DspFxReverb::SetParam(const Param& param)
{
    if (m_AuxBus != AUX_BUS_0 && m_AuxBus != AUX_BUS_1) {
        return false;
    }
    if (m_Buffer == NULL || m_Size == 0) {
        return false;
    }
    if (param.m_Coloration < 0.0f || param.m_Coloration > 1.0f || param.m_Damping < 0.0f || param.m_Damping > 1.0f ||
        param.m_EarlyGain < 0.0f || param.m_EarlyGain > 1.0f || param.m_FusedGain < 0.0f || param.m_FusedGain > 1.0f) {
        return false;
    }
    DspFxReverbParams params;
    const u32 earlyFrames = GetFrameCount(param.m_EarlyReflectionTime);
    params.m_EarlyReflectionSamples = earlyFrames;
    const u32 preDelayFrames = GetFrameCount(param.m_PreDelayTime);
    params.m_FusedSamples = preDelayFrames;
    params.m_ChannelCount = 2;
    const u32 channelCount = 2;
    // the comb filters decay by 60 dB in the fused time
    const f32 fusedSamples = param.m_FusedTime * 0.001f * SAMPLE_RATE;
    const ReverbFilterSize* filterSize = param.m_FilterSize;
    if (filterSize == NULL) {
        filterSize = &s_DefaultDspReverbFilterSize;
    }
    params.m_FilterSize[0] = filterSize->m_Comb[0] / FRAME_SAMPLES;
    params.m_FilterSize[1] = filterSize->m_Comb[1] / FRAME_SAMPLES;
    for (s32 i = 0; i < 2; i++) {
        const s32 length = static_cast<s16>(params.m_FilterSize[i]) * FRAME_SAMPLES;
        params.m_CombGain[i] = static_cast<s32>(powf(10.0f, length * -3.0f / fusedSamples) * 128.0f);
    }
    params.m_EarlyGain = static_cast<u32>(param.m_EarlyGain * 128.0f);
    params.m_FusedGain = static_cast<u32>(param.m_FusedGain * 128.0f);
    params.m_Coloration = static_cast<u32>(param.m_Coloration * 128.0f);
    params.m_FilterSize[2] = filterSize->m_AllPass / FRAME_SAMPLES;
    f32 damping = param.m_Damping;
    if (damping > DAMPING_MAX) {
        damping = DAMPING_MAX;
    }
    f32 lpfB;
    f32 lpfA;
    if (param.m_IsEnableSurround == 1) {
        lpfA = -damping;
        lpfB = damping - 1.0f;
    } else {
        lpfB = 1.0f - damping;
        lpfA = damping;
    }
    params.m_Flags = DspFxDelayParams::FLAG_PARAM;
    params.m_LpfB = static_cast<s32>(lpfB * 128.0f);
    params.m_LpfA = static_cast<s32>(lpfA * 128.0f);

    // the buffers one after the other as far as the work buffer goes
    const uptr buffer = reinterpret_cast<uptr>(m_Buffer);
    u32 offset = channelCount * earlyFrames * FRAME_BYTES;
    params.m_BufferAddress[0] = DSP_ADDRESS(nn::os::CTR::MPCore::ConvertAddressForDevice(buffer, offset));
    if (m_Size >= offset) {
        const u32 size = channelCount * preDelayFrames * FRAME_BYTES;
        params.m_BufferAddress[1] = DSP_ADDRESS(nn::os::CTR::MPCore::ConvertAddressForDevice(buffer + offset, size));
        offset += size;
        if (m_Size >= offset) {
            bool isEnough = true;
            for (s32 i = 0; i < 2; i++) {
                const u32 size = params.m_FilterSize[i] * (channelCount * FRAME_BYTES);
                params.m_BufferAddress[2 + i] = DSP_ADDRESS(nn::os::CTR::MPCore::ConvertAddressForDevice(buffer + offset, size));
                offset += size;
                isEnough = m_Size >= offset;
            }
            if (isEnough) {
                const u32 size = params.m_FilterSize[2] * (channelCount * FRAME_BYTES);
                params.m_BufferAddress[4] = DSP_ADDRESS(nn::os::CTR::MPCore::ConvertAddressForDevice(offset + buffer, size));
            }
        }
    }
    params.m_Flags |= DspFxDelayParams::FLAG_BUFFER;
    memset(m_Buffer, 0, m_Size);
    nn::dsp::CTR::FlushDataCache(reinterpret_cast<uptr>(m_Buffer), m_Size);
    return DspFxManager::GetInstance()->SetDspReverbEffect(static_cast<AuxBusId>(m_AuxBus), params);
}

// 0x00461414 (name is ours)
nn::snd::CTR::DspFxReverb::DspFxReverb()
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
