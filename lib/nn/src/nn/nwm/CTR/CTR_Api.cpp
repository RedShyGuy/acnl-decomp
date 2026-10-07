#include "nn/nwm/CTR/CTR_Api.h"

namespace nn {
namespace nwm {
namespace CTR {
namespace {
// the wifi link level in the shared page
volatile u8* const WIFI_LINK_LEVEL = reinterpret_cast<volatile u8*>(0x1FF81066);
} // namespace

// 0x003E23C0 (name is ours)
u8 GetWifiLinkLevel()
{
    // (a debug log call was removed by the linker here)
    return *WIFI_LINK_LEVEL;
}

// 0x003E23D4 | fefates:bytes [tier B]
nn::Result GetMacAddress(nn::nwm::Mac& mac)
{
}

} // namespace CTR
} // namespace nwm
} // namespace nn
