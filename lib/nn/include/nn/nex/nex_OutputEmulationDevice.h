#pragma once

#include "decomp.h"
#include "nn/nex/nex_EmulationDevice.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex21OutputEmulationDeviceE @ 0x008CEB24
// vtable 0x008FDED8 (vptr 0x008FDEE0), offset_to_top 0, 3 entries
class OutputEmulationDevice : public ::nn::nex::EmulationDevice
{
public:
    OutputEmulationDevice(); // ctor address unknown
    virtual void vf_0x00(); // 0x0039A6E0 slot 0x00 | virtual slot, introduced by nn::nex::OutputEmulationDevice
    virtual void vf_0x04(); // 0x0039A6C8 slot 0x04 | virtual slot, introduced by nn::nex::OutputEmulationDevice
    virtual void vf_0x08(); // 0x00377650 slot 0x08 | virtual slot, introduced by nn::nex::InputEmulationDevice
};
} // namespace nex
} // namespace nn
