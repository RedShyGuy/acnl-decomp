#pragma once

#include "decomp.h"

namespace sead {
class FileDevice;

// RTTI N4sead10HandleBaseE @ 0x008D1390
//
// The handle of an opened file or directory of a FileDevice (member names are ours).
class HandleBase
{
public:
    HandleBase() : mDevice(nullptr), mOriginalDevice(nullptr)
    {
        for (s32 i = 0; i < 8; i++) {
            mHandleBuffer[i] = 0;
        }
    }
    virtual ~HandleBase() {}

    // the device that holds the file (the one that opened it for a device that forwards)
    FileDevice* mDevice; // 0x04
    // the device the handle was opened with
    FileDevice* mOriginalDevice; // 0x08
    // the data of the device (an nn::fs file or directory)
    u32 mHandleBuffer[8]; // 0x0C
};
ASSERT_SIZE(HandleBase, 0x2C);
} // namespace sead
