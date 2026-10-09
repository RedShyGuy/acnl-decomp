#pragma once

// The states of the input devices. The type names are from the symbols (mangled signatures of
// nn::hid::CTR); the layouts follow 3dbrew "HID Shared Memory" and the code, the member names
// are ours.

#include "decomp.h"

namespace nn {
namespace hid {
namespace CTR {

// the buttons and the circle pad
struct PadStatus
{
    bit32 hold;    // 0x0
    bit32 trigger; // 0x4, pressed since the last state
    bit32 release; // 0x8, released since the last state
    s16 stickX;    // 0xC
    s16 stickY;    // 0xE
};
ASSERT_SIZE(PadStatus, 0x10);

struct TouchPanelStatus
{
    u16 x;    // 0x0
    u16 y;    // 0x2
    u8 touch; // 0x4
    u8 padding[3];
};
ASSERT_SIZE(TouchPanelStatus, 0x8);

// the raw values of the accelerometer
struct AccelerometerStatus
{
    s16 x; // 0x0
    s16 y; // 0x2
    s16 z; // 0x4
};
ASSERT_SIZE(AccelerometerStatus, 0x6);

// the acceleration in G
struct AccelerationFloat
{
    f32 x; // 0x0
    f32 y; // 0x4
    f32 z; // 0x8
};
ASSERT_SIZE(AccelerationFloat, 0xC);

// the raw values of the gyroscope
struct GyroscopeLowStatus
{
    s16 x; // 0x0
    s16 y; // 0x2
    s16 z; // 0x4
};
ASSERT_SIZE(GyroscopeLowStatus, 0x6);

// the computed state of the gyroscope
struct GyroscopeStatus
{
    f32 speed[3];        // 0x00, the angular speed of each axis
    f32 angle[3];        // 0x0C, the integrated angles
    f32 direction[3][3]; // 0x18, the orientation as rotation matrix
};
ASSERT_SIZE(GyroscopeStatus, 0x3C);

} // namespace CTR
} // namespace hid
} // namespace nn
