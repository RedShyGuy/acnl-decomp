#include "nn/cfg/CTR/detail/cfg_IpcUser.h"
#include <string.h>
#include "nn/cfg/CTR/detail/detail_Api.h"
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace cfg {
namespace CTR {
namespace detail {
namespace {
// command headers (3dbrew "Config Services")
const bit32 COMMAND_GET_CONFIG = 0x00010082;
const bit32 COMMAND_GET_REGION = 0x00020000;
const bit32 COMMAND_GET_TRANSFERABLE_ID = 0x00030040;
const bit32 COMMAND_GET_REGION_CANADA_USA = 0x00040000;
const bit32 COMMAND_GET_COUNTRY_CODE_STRING = 0x00090040;

const size_t COUNTRY_CODE_STRING_SIZE = 3;
} // namespace

// 0x00124528 | nintendogs:bytes [tier A]
nn::Result nn::cfg::CTR::detail::IpcUser::GetConfig(void* buffer, size_t size, u32 blockId)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_CONFIG;
    command[1] = size;
    command[2] = blockId;
    // the output buffer, mapped for writing
    command[3] = (size << 4) | 0xC;
    command[4] = reinterpret_cast<uptr>(buffer);
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

// 0x00124574 | nintendogs:bytes [tier A]
nn::Result nn::cfg::CTR::detail::IpcUser::GetRegion(nn::cfg::CTR::CfgRegionCode* region)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_REGION;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *region = *reinterpret_cast<const nn::cfg::CTR::CfgRegionCode*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x00350F4C | nintendogs:bytes [tier A]
nn::Result nn::cfg::CTR::detail::IpcUser::GetTransferableId(u32 unknown, u64* id)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_TRANSFERABLE_ID;
    command[1] = unknown;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *id = *reinterpret_cast<u64*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x00350F94
nn::Result nn::cfg::CTR::detail::IpcUser::GetRegionCanadaUSA(bool* isCanadaOrUsa)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_REGION_CANADA_USA;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *isCanadaOrUsa = *reinterpret_cast<const bool*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x00350FD4
nn::Result nn::cfg::CTR::detail::IpcUser::GetCountryCodeString(char* string, u16 countryCodeId)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_COUNTRY_CODE_STRING;
    // (a half word)
    *reinterpret_cast<u16*>(&command[1]) = countryCodeId;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    memcpy(string, &command[2], COUNTRY_CODE_STRING_SIZE);
    return nn::Result(command[1]);
}

} // namespace detail
} // namespace CTR
} // namespace cfg
} // namespace nn
