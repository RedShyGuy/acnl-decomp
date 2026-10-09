#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/cfg/CTR/cfg_Types.h"

namespace nn {
namespace cfg {
namespace CTR {
namespace detail {
// the commands of cfg:u (3dbrew "Config Services"); they use detail::s_Session (the class name is
// from the reference symbols)
class IpcUser
{
public:
    // 0x0001 GetConfigInfoBlk2
    static nn::Result GetConfig(void* buffer, size_t size, u32 blockId); // 0x00124528 | nintendogs:bytes [tier A]
    // 0x0002 SecureInfoGetRegion
    static nn::Result GetRegion(nn::cfg::CTR::CfgRegionCode* region); // 0x00124574 | nintendogs:bytes [tier A]
    // 0x0003 GenHashConsoleUnique
    static nn::Result GetTransferableId(u32 unknown, u64* id); // 0x00350F4C | nintendogs:bytes [tier A]
    // 0x0004 (names after 3dbrew)
    static nn::Result GetRegionCanadaUSA(bool* isCanadaOrUsa); // 0x00350F94
    // 0x0009 (names after 3dbrew): two characters and the terminator
    static nn::Result GetCountryCodeString(char* string, u16 countryCodeId); // 0x00350FD4
};
} // namespace detail
} // namespace CTR
} // namespace cfg
} // namespace nn
