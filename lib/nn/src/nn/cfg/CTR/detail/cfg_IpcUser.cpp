#include "nn/cfg/CTR/detail/cfg_IpcUser.h"

namespace nn {
namespace cfg {
namespace CTR {
namespace detail {
// 0x00124528 | nintendogs:bytes [tier A]
void nn::cfg::CTR::detail::IpcUser::GetConfig(void*, unsigned, unsigned)
{
}

// 0x00124574 | nintendogs:bytes [tier A]
void nn::cfg::CTR::detail::IpcUser::GetRegion(nn::cfg::CTR::CfgRegionCode*)
{
}

// 0x00350F4C | nintendogs:bytes [tier A]
void nn::cfg::CTR::detail::IpcUser::GetTransferableId(unsigned, unsigned long long*)
{
}

} // namespace detail
} // namespace CTR
} // namespace cfg
} // namespace nn
