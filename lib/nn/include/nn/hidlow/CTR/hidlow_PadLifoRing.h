#pragma once

#include "decomp.h"
#include "nn/hid/CTR/hid_Types.h"

namespace nn {
namespace hidlow {
namespace CTR {
// a ring of 8 PadStatus entries in the shared memory of hid (3dbrew "HID Shared Memory";
// member names are ours)
class PadLifoRing
{
public:
    static const s32 ENTRY_NUM = 8;

    // copies up to count of the newest states (newest first) that are newer than *pLastTick /
    // *pLastIndex and updates them
    void ReadData(nn::hid::CTR::PadStatus* pBuffer, int count, int* pReadCount, s64* pLastTick, int* pLastIndex); // 0x00484414 | nintendogs:bytes [tier A]

private:
    volatile s64 m_Tick;                    // 0x00, the tick of the last update
    volatile s64 m_PrevTick;                // 0x08, the tick of the update before
    volatile s32 m_Index;                   // 0x10, the entry written last
    f32 m_Slider;                           // 0x14, the 3D slider
    bit32 m_CurrentHold;                    // 0x18
    s16 m_RawStickX;                        // 0x1C
    s16 m_RawStickY;                        // 0x1E
    u8 m_Reserved20[8];                     // 0x20
    nn::hid::CTR::PadStatus m_Entries[ENTRY_NUM]; // 0x28

    static void CheckLayout()
    {
        ASSERT_OFFSET(PadLifoRing, m_Entries, 0x28);
        ASSERT_SIZE(PadLifoRing, 0xA8);
    }
};
} // namespace CTR
} // namespace hidlow
} // namespace nn
