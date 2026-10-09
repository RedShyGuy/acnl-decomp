#include "nn/mic/CTR/detail/mic_Mic.h"
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace mic {
namespace CTR {
namespace detail {
namespace {
// command headers (3dbrew "MIC Services")
const bit32 COMMAND_MAP_SHARED_MEM = 0x00010042;
const bit32 COMMAND_UNMAP_SHARED_MEM = 0x00020000;
const bit32 COMMAND_START_SAMPLING = 0x00030140;
const bit32 COMMAND_ADJUST_SAMPLING = 0x00040040;
const bit32 COMMAND_STOP_SAMPLING = 0x00050000;
const bit32 COMMAND_IS_SAMPLING = 0x00060000;
const bit32 COMMAND_SET_GAIN = 0x00080040;
const bit32 COMMAND_GET_GAIN = 0x00090000;
const bit32 COMMAND_SET_POWER = 0x000A0040;
const bit32 COMMAND_SET_IIR_FILTER = 0x000C0042;
const bit32 COMMAND_SET_CLIENT_VERSION = 0x00100040;

inline nn::Result Send(bit32* command)
{
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

inline void SetByte(bit32* word, u8 value)
{
    *reinterpret_cast<u8*>(word) = value;
}
} // namespace

// 0x0097E8E4
nn::Handle s_Session;

// 0x00131004 | tier B
nn::Result nn::mic::CTR::detail::Mic::MapSharedMem(nn::Handle sharedMemory, size_t size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_MAP_SHARED_MEM;
    command[3] = sharedMemory.GetPrintableBits();
    command[1] = size;
    command[2] = 0;
    return Send(command);
}

// 0x00131048 | tier B
nn::Result nn::mic::CTR::detail::Mic::SetClientVersion(u32 version)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_CLIENT_VERSION;
    command[1] = version;
    return Send(command);
}

// 0x00131084 | nintendogs:bytes [tier B]
nn::Result nn::mic::CTR::detail::Mic::GetPGAB(u8* pGain)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_GAIN;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pGain = static_cast<u8>(command[2]);
    return nn::Result(command[1]);
}

// 0x00140AF0 | nintendogs:bytes [tier A]
nn::Result nn::mic::CTR::detail::Mic::FreeBuffer()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_UNMAP_SHARED_MEM;
    return Send(command);
}

// 0x00140B20 | nintendogs:bytes [tier A]
nn::Result nn::mic::CTR::detail::Mic::IsSampling(bool* pIsSampling)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_IS_SAMPLING;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pIsSampling = *reinterpret_cast<const bool*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x00140B5C | tier B
nn::Result nn::mic::CTR::detail::Mic::SetMicBiasEntry(bool enable)
{
    return SetMicBias(enable);
}

// 0x00140B60 | nintendogs:bytes [tier A]
nn::Result nn::mic::CTR::detail::Mic::SetMicBias(bool enable)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_POWER;
    SetByte(&command[1], enable);
    return Send(command);
}

// 0x00140BA0 | nintendogs:bytes [tier A]
nn::Result nn::mic::CTR::detail::Mic::StopSampling()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_STOP_SAMPLING;
    return Send(command);
}

// 0x003549B4 | tier B
nn::Result nn::mic::CTR::detail::Mic::StartSampling(SamplingType type, SamplingRate rate, s32 offset, size_t size, bool loop)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_START_SAMPLING;
    SetByte(&command[1], type);
    SetByte(&command[2], rate);
    command[3] = offset;
    command[4] = size;
    SetByte(&command[5], loop);
    return Send(command);
}

// 0x00354A08 | tier B
nn::Result nn::mic::CTR::detail::Mic::AdjustSampling(SamplingRate rate)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_ADJUST_SAMPLING;
    SetByte(&command[1], rate);
    return Send(command);
}

// 0x00354A44 | tier B
nn::Result nn::mic::CTR::detail::Mic::SetIirFilterMic(const void* pCoefficients, size_t size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_IIR_FILTER;
    command[1] = size;
    command[3] = reinterpret_cast<uptr>(pCoefficients);
    command[2] = (size << 4) | 0xA;
    return Send(command);
}

// 0x00354A8C | tier B
nn::Result nn::mic::CTR::detail::Mic::SetPGABEntry(u8 gain)
{
    return SetPGAB(gain);
}

// 0x00354A90 | nintendogs:bytes [tier A]
nn::Result nn::mic::CTR::detail::Mic::SetPGAB(u8 gain)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_GAIN;
    SetByte(&command[1], gain);
    return Send(command);
}

} // namespace detail
} // namespace CTR
} // namespace mic
} // namespace nn
