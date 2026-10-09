#pragma once

#include "decomp.h"

namespace nn {
namespace hid {
namespace CTR {
// The dead zone and limits of the circle pad, and the scaling of NormalizeStickWithScale.
// Member names are ours.
class AnalogStickClamper
{
public:
    // the clamping of the stick (ClampStickCircle / Cross / Minimum of hidlow)
    enum ClampMode : u8 {
        CLAMP_MODE_CIRCLE = 0,
        CLAMP_MODE_CROSS = 1,
        CLAMP_MODE_MINIMUM = 2,
    };

    AnalogStickClamper(); // 0x00353C30 | nintendogs:bytes [tier A]
    // keeps the limits in the allowed range
    void ClampValueOfClamp(); // 0x003538B8 | nintendogs:bytes [tier A]
    // the stick as -1..1 with an adaptive scale
    void NormalizeStickWithScale(f32* pX, f32* pY, s16 x, s16 y); // 0x00353904 | nintendogs:bytes [tier B]
    void SetNormalizeStickScaleSettings(f32 scale, s16 threshold); // 0x00353B88 (name is ours)
    void ClampCore(s16* pX, s16* pY, int x, int y); // 0x00353B9C | nintendogs:bytes [tier A]

    s16 m_Min[3];        // 0x00, per mode
    s16 m_Max[3];        // 0x06, per mode
    ClampMode m_Mode;    // 0x0C
    s16 m_Threshold;     // 0x0E, the stick value the scale starts at
    f32 m_ScaleMax;      // 0x10
    f32 m_Range;         // 0x14, the current range of the stick
    f32 m_RangeChange;   // 0x18
    f32 m_LastLength;    // 0x1C
    f32 m_LastChange;    // 0x20
};
ASSERT_SIZE(AnalogStickClamper, 0x24);
} // namespace CTR
} // namespace hid
} // namespace nn
