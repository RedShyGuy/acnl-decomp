#pragma once

#include "decomp.h"
#include "nn/nex/nex_EmulationDevice.h"

namespace nn {
namespace nex {
// The emulation of the sent packets of a transport.
// RTTI N2nn3nex21OutputEmulationDeviceE @ 0x008CEB24
// vtable 0x008FDED8 (vptr 0x008FDEE0), offset_to_top 0, 3 entries
class OutputEmulationDevice : public ::nn::nex::EmulationDevice
{
public:
    // (inline in the constructor of RootTransport)
    OutputEmulationDevice() {}
    virtual ~OutputEmulationDevice(); // 0x0039A6E0 slot 0x00
    // 0x0039A6C8 slot 0x04 (deleting dtor)
};
} // namespace nex
} // namespace nn
