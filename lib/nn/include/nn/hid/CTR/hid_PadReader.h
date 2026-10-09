#pragma once

#include "decomp.h"
#include "nn/hid/CTR/hid_AnalogStickClamper.h"
#include "nn/hid/CTR/hid_Types.h"

namespace nn {
namespace hid {
namespace CTR {
class Pad;

// Reads the buttons and the circle pad (member names are ours).
class PadReader
{
public:
    PadReader(Pad& pad); // 0x00354838 | nintendogs:bytes [tier A]
    // the newest state; false without one (or while the extra pad samples)
    bool ReadLatest(PadStatus* pStatus); // 0x003546F0 | nintendogs:bytes [tier A]
    void NormalizeStickWithScale(f32* pX, f32* pY, s16 x, s16 y); // 0x00354820 | nintendogs:bytes [tier B]
    void SetNormalizeStickScaleSettings(f32 scale, s16 threshold); // 0x00353B80 (name is ours)

private:
    Pad* m_pPad;                  // 0x00
    s32 m_LastIndex;              // 0x04
    bit32 m_LastHold;             // 0x08
    AnalogStickClamper m_Clamper; // 0x0C
    bool m_IsFirstRead;           // 0x30
    s64 m_LastTick;               // 0x38
};
ASSERT_SIZE(PadReader, 0x40);
} // namespace CTR
} // namespace hid
} // namespace nn
