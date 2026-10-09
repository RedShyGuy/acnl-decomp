#pragma once

// Types of nn::nwm (wireless driver). The type names are from the binary; members are ours.

#include "decomp.h"

namespace nn {
namespace nwm {

struct Mac
{
    u8 address[6];
};
ASSERT_SIZE(Mac, 0x6);

// a network name of up to 32 bytes (the name is ours)
struct Ssid
{
    Ssid(); // 0x003E2480 (name is ours)
    // a copy of up to 32 bytes, the rest cleared
    Ssid(const u8* ssid, size_t length); // 0x003E243C (name is ours)

    u32 length;   // 0x00
    u8 ssid[32];  // 0x04
};
ASSERT_SIZE(Ssid, 0x24);

// a network found by a scan, followed by its information elements (names are ours)
struct BssDescription
{
    u32 size;             // 0x00, of this description with its elements
    u8 unknown04;         // 0x04
    u8 channel;           // 0x05
    s16 signalStrength;   // 0x06
    Mac bssid;            // 0x08
    u8 unknown0E[6];      // 0x0E
    u32 ieSize;           // 0x14, of the beacon data
    u32 ieOffset;         // 0x18, of the beacon data from the start of the description
};
ASSERT_SIZE(BssDescription, 0x1C);

namespace CTR {

// scan parameters as sent to the service (52 bytes); filled by nn::uds::CTR::ScanOnConnection and
// the scan of nn::uds::CTR::StartScan
struct ScanParamIpc
{
    u16 unknown00;          // 0x00 1, or !flag in ScanOnConnection
    u16 scanType;           // 0x02 2 (scan) or 3 (scan on connection)
    u16 channelMask;        // 0x04 bit n-1 = channel n
    u16 scanTime;           // 0x06
    Mac bssid;              // 0x08 FF:FF:FF:FF:FF:FF = any
    u8 unknown0E[0x22];     // 0x0E
    u32 unknown30;          // 0x30 0
};
ASSERT_SIZE(ScanParamIpc, 0x34);

} // namespace CTR
} // namespace nwm
} // namespace nn
