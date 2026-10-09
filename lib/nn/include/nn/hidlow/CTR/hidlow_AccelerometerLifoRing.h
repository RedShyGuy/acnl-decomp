#pragma once

#include "decomp.h"
#include "nn/hid/CTR/hid_Types.h"

namespace nn {
namespace hidlow {
namespace CTR {
// a ring of 8 AccelerometerStatus entries in the shared memory of hid (3dbrew "HID Shared Memory";
// member names are ours)
class AccelerometerLifoRing
{
public:
    static const s32 ENTRY_NUM = 8;

    // copies up to count of the newest states (newest first) that are newer than *pLastTick /
    // *pLastIndex and updates them
    void ReadData(nn::hid::CTR::AccelerometerStatus* pBuffer, int count, int* pReadCount, s64* pLastTick, int* pLastIndex); // 0x004848A4 | nintendogs:bytes [tier B]

private:
    volatile s64 m_Tick;                    // 0x00, the tick of the last update
    volatile s64 m_PrevTick;                // 0x08, the tick of the update before
    volatile s32 m_Index;                   // 0x10, the entry written last
    u8 m_Reserved14[4];                     // 0x14
    nn::hid::CTR::AccelerometerStatus m_Current; // 0x18
    u8 m_Reserved1E[2];                     // 0x1E
    nn::hid::CTR::AccelerometerStatus m_Entries[ENTRY_NUM]; // 0x20

    static void CheckLayout()
    {
        ASSERT_OFFSET(AccelerometerLifoRing, m_Entries, 0x20);
        ASSERT_SIZE(AccelerometerLifoRing, 0x50);
    }
};
} // namespace CTR
} // namespace hidlow
} // namespace nn
