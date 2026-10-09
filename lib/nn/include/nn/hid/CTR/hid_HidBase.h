#pragma once

#include "decomp.h"
#include "nn/os/os_EventBase.h"

namespace nn {
namespace hid {
namespace CTR {
// RTTI N2nn3hid3CTR7HidBaseE @ 0x008CDF0C
// The base of the input devices: the event hid signals after an update.
class HidBase : public ::nn::os::EventBase
{
public:
    HidBase() {}

    // takes the event of GetIPCHandles (inline in HidDevices::Initialize; the name is ours)
    void SetEventHandle(nn::Handle handle) { mHandle = handle; }
};
} // namespace CTR
} // namespace hid
} // namespace nn
