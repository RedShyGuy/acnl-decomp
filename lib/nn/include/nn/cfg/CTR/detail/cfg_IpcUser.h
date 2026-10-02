#pragma once

#include "decomp.h"

namespace nn {
namespace cfg {
namespace CTR {
namespace detail {
class IpcUser
{
public:
    void GetConfig(void*, unsigned, unsigned); // 0x00124528 | nintendogs:bytes [tier A]
    void GetRegion(nn::cfg::CTR::CfgRegionCode*); // 0x00124574 | nintendogs:bytes [tier A]
    void GetTransferableId(unsigned, unsigned long long*); // 0x00350F4C | nintendogs:bytes [tier A]
};
} // namespace detail
} // namespace CTR
} // namespace cfg
} // namespace nn
