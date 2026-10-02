#pragma once

#include "decomp.h"
#include "nn/nex/nex_EmulationDevice.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex20InputEmulationDeviceE @ 0x008CE898
// vtable 0x008FD824 (vptr 0x008FD82C), offset_to_top 0, 3 entries
class InputEmulationDevice : public ::nn::nex::EmulationDevice
{
public:
    InputEmulationDevice(); // ctor address unknown
    virtual void vf_0x00(); // 0x003967D4 slot 0x00 | virtual slot, introduced by nn::nex::InputEmulationDevice
    virtual void vf_0x04(); // 0x003967BC slot 0x04 | virtual slot, introduced by nn::nex::InputEmulationDevice
    virtual void vf_0x08(); // 0x00377650 slot 0x08 | virtual slot, introduced by nn::nex::InputEmulationDevice
};
} // namespace nex
} // namespace nn
