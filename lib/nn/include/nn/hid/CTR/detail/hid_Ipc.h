#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"

namespace nn {
namespace hid {
namespace CTR {
namespace detail {
// The commands of hid:USER (3dbrew "HID Services"); all static.
class Ipc
{
public:
    // the shared memory and the events of Pad, TouchPanel (3dbrew: pad 1), Accelerometer,
    // Gyroscope and DebugPad
    static nn::Result GetIPCHandles(nn::Handle* pSharedMemory, nn::Handle* pPadEvent, nn::Handle* pTouchPanelEvent, nn::Handle* pAccelerometerEvent, nn::Handle* pGyroscopeEvent, nn::Handle* pDebugPadEvent); // 0x00354578 | nintendogs:bytes [tier A]
    static nn::Result DisableGyroscopeLow(); // 0x003545F0 | tier C
    static nn::Result EnableAccelerometer(); // 0x00354620 | nintendogs:bytes [tier B]
    static nn::Result DisableAccelerometer(); // 0x00354650 (name after 3dbrew)
};

// the session of hid:USER (set by HidDevices::Initialize)
extern nn::Handle s_Session;
} // namespace detail
} // namespace CTR
} // namespace hid
} // namespace nn
