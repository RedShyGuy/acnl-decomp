#pragma once

// The input devices: the event and the ring in the shared memory of hid. Pad, TouchPanel and
// Accelerometer are names from the symbols (GetPad, GetTouchPanel, the reader constructors),
// DebugPad is named after 3dbrew "HID Shared Memory"; the member names are ours.

#include "decomp.h"
#include "nn/hid/CTR/hid_HidBase.h"

namespace nn {
namespace hidlow {
namespace CTR {
class AccelerometerLifoRing;
class PadLifoRing;
class TouchPanelLifoRing;
} // namespace CTR
} // namespace hidlow

namespace hid {
namespace CTR {
class Pad : public HidBase
{
public:
    nn::hidlow::CTR::PadLifoRing* m_pRing; // 0x4
};
ASSERT_SIZE(Pad, 0x8);

class TouchPanel : public HidBase
{
public:
    nn::hidlow::CTR::TouchPanelLifoRing* m_pRing; // 0x4
};
ASSERT_SIZE(TouchPanel, 0x8);

class Accelerometer : public HidBase
{
public:
    nn::hidlow::CTR::AccelerometerLifoRing* m_pRing; // 0x4
};
ASSERT_SIZE(Accelerometer, 0x8);

// (no reader in this program)
class DebugPad : public HidBase
{
public:
    void* m_pRing; // 0x4
};
ASSERT_SIZE(DebugPad, 0x8);
} // namespace CTR
} // namespace hid
} // namespace nn
