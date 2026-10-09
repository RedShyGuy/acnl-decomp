#pragma once

// The rings in the shared memory of the hid module (3dbrew "HID Shared Memory"): the module
// writes the newest state to entries[index] and stores the tick of the update. The readers copy
// the newest entries and check afterwards that the module did not write meanwhile. The four
// ring classes have the same code for their entry types (in the original as copies; here as
// the template ReadLifoRing). All names are ours.

#include "decomp.h"
#include "nn/hid/CTR/hid_Types.h"

namespace nn {
namespace hidlow {
namespace CTR {
namespace detail {
// the entries are copied field by field
inline void CopyStatus(nn::hid::CTR::PadStatus& dst, const nn::hid::CTR::PadStatus& src)
{
    dst.hold = src.hold;
    dst.trigger = src.trigger;
    dst.release = src.release;
    dst.stickX = src.stickX;
    dst.stickY = src.stickY;
}

inline void CopyStatus(nn::hid::CTR::TouchPanelStatus& dst, const nn::hid::CTR::TouchPanelStatus& src)
{
    dst.x = src.x;
    dst.y = src.y;
    dst.touch = src.touch;
}

inline void CopyStatus(nn::hid::CTR::AccelerometerStatus& dst, const nn::hid::CTR::AccelerometerStatus& src)
{
    dst.x = src.x;
    dst.y = src.y;
    dst.z = src.z;
}

inline void CopyStatus(nn::hid::CTR::GyroscopeLowStatus& dst, const nn::hid::CTR::GyroscopeLowStatus& src)
{
    dst.x = src.x;
    dst.y = src.y;
    dst.z = src.z;
}

// copies up to count of the newest entries (newest first) that are newer than *pLastTick /
// *pLastIndex and updates them
template <typename Status, s32 EntryNum>
inline void ReadLifoRing(const volatile s64& headerTick, const volatile s64& headerPrevTick, const volatile s32& headerIndex, const Status* entries, Status* pBuffer, int count,
                         int* pReadCount, s64* pLastTick, int* pLastIndex)
{
    *pReadCount = 0;
    if (count <= 0) {
        return;
    }
    if (count > EntryNum - 1) {
        count = EntryNum - 1;
    }
    for (;;) {
        s32 index = headerIndex;
        s64 tick = headerTick;
        s64 prevTick = headerPrevTick;
        s32 latest = index;
        if (index <= 0) {
            if (index != 0) {
                return;
            }
            if (tick == prevTick) {
                // only one update so far
                if (tick < 0) {
                    return;
                }
                latest = EntryNum - 1;
            }
        }
        s32 num;
        if (*pLastTick >= tick) {
            num = latest - *pLastIndex;
        } else if (prevTick < 0) {
            num = latest + 1;
        } else if (*pLastTick < prevTick) {
            num = count;
        } else {
            num = latest - *pLastIndex + EntryNum;
        }
        if (num > count) {
            num = count;
        }
        for (s32 i = 0; i < num; i++) {
            CopyStatus(pBuffer[i], entries[(latest - i + EntryNum) % EntryNum]);
        }
        if (headerIndex == index && headerTick == tick && headerPrevTick == prevTick) {
            *pReadCount = num;
            *pLastTick = tick;
            *pLastIndex = latest;
            return;
        }
    }
}
} // namespace detail
} // namespace CTR
} // namespace hidlow
} // namespace nn
