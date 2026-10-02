#pragma once

#include "decomp.h"

namespace nn {
namespace os {
namespace CTR {
namespace MPCore {
// the offset between device memory addresses and the addresses devices (DMA, GPU) use
void InitializeDeviceAddress(); // 0x0011F6B8 | fefates:bytes [tier B]
// the device address of [address, address + size), or 0 if it is not all in WRAM / device memory
uptr ConvertAddressForWram(uptr address, size_t size); // 0x00143184 | fefates:bytes [tier B]
uptr ConvertAddressForDevice(uptr address, size_t size); // 0x001431D0 | fefates:bytes [tier B]
} // namespace MPCore
} // namespace CTR
} // namespace os
} // namespace nn
