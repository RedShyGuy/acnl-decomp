#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace hid {
namespace CTR {
class Accelerometer;
class HidDevices;
class Pad;
class TouchPanel;

// connects to hid:USER
nn::Result Initialize(); // 0x003527BC | tier C
// the devices of the library
Pad& GetPad(); // 0x00354518 | tier C
TouchPanel& GetTouchPanel(); // 0x003527D4 | tier C
Accelerometer& GetAccelerometer(); // 0x00353828 | tier C

// the devices (name is ours)
extern HidDevices s_HidDevices;
} // namespace CTR
} // namespace hid
} // namespace nn
