#pragma once

#include "decomp.h"
#include "nn/nex/nex_EmulationDevice.h"

namespace nn {
namespace nex {
// The emulation of the received packets of a transport.
// RTTI N2nn3nex20InputEmulationDeviceE @ 0x008CE898
// vtable 0x008FD824 (vptr 0x008FD82C), offset_to_top 0, 3 entries
class InputEmulationDevice : public ::nn::nex::EmulationDevice
{
public:
    // (inline in the constructor of RootTransport)
    InputEmulationDevice() {}
    virtual ~InputEmulationDevice(); // 0x003967D4 slot 0x00
    // 0x003967BC slot 0x04 (deleting dtor)
};
} // namespace nex
} // namespace nn
