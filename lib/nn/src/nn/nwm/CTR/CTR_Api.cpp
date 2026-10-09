#include "nn/nwm/CTR/CTR_Api.h"
#include <string.h>
#include "nn/nwm/nwm_Types.h"

namespace nn {
namespace nwm {
namespace CTR {
namespace {
// the wifi fields of the shared page (3dbrew "Configuration Memory")
const uptr SHARED_PAGE = 0x1FF81000;
volatile u8* const WIFI_LINK_LEVEL = reinterpret_cast<volatile u8*>(SHARED_PAGE + 0x66);
const u8* const WIFI_MAC_ADDRESS = reinterpret_cast<const u8*>(SHARED_PAGE + 0x60);
volatile u8* const WIFI_STATE = reinterpret_cast<volatile u8*>(SHARED_PAGE + 0x67);

// status, not found, module 27 nwm, 1007 no data (name is ours)
const bit32 RESULT_NO_MAC_ADDRESS = 0xC9606FEF;
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
    memcpy(&mac, WIFI_MAC_ADDRESS, sizeof(nn::nwm::Mac));
    nn::nwm::Mac none = { { 0, 0, 0, 0, 0, 0 } };
    if (memcmp(&mac, &none, sizeof(nn::nwm::Mac)) == 0) {
        return nn::Result(RESULT_NO_MAC_ADDRESS);
    }
    return nn::Result();
}

// 0x003E242C (name is ours)
u8 GetWifiState()
{
    return *WIFI_STATE;
}

} // namespace CTR

// 0x003E243C (name is ours)
nn::nwm::Ssid::Ssid(const u8* ssid, size_t length)
{
    if (length > sizeof(this->ssid)) {
        length = sizeof(this->ssid);
    }
    this->length = length;
    memcpy(this->ssid, ssid, length);
    if (length < sizeof(this->ssid)) {
        memset(this->ssid + length, 0, sizeof(this->ssid) - length);
    }
}

// 0x003E2480 (name is ours)
nn::nwm::Ssid::Ssid()
{
    memset(ssid, 0, sizeof(ssid));
    length = 0;
}

} // namespace nwm
} // namespace nn
