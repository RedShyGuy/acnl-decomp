#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/hid/CTR/hid_Devices.h"
#include "nn/hid/CTR/hid_Gyroscope.h"
#include "nn/os/os_SharedMemoryBlock.h"

namespace nn {
namespace hid {
namespace CTR {
// The devices of hid and its shared memory (one object, made by the static initializer of
// hid_Api.cpp; the member names are ours).
class HidDevices
{
public:
    HidDevices() {}
    // connects to the service and maps the shared memory
    nn::Result Initialize(const char* serviceName); // 0x003525EC | nintendogs:bytes [tier A]
    ~HidDevices(); // 0x00352714 | nintendogs:bytes [tier A]

    Pad m_Pad;                              // 0x00
    TouchPanel m_TouchPanel;                // 0x08
    Accelerometer m_Accelerometer;          // 0x10
    Gyroscope m_Gyroscope;                  // 0x18
    DebugPad m_DebugPad;                    // 0x24
    nn::os::SharedMemoryBlock m_SharedMemory; // 0x2C
};
ASSERT_SIZE(HidDevices, 0x48);
} // namespace CTR
} // namespace hid
} // namespace nn
