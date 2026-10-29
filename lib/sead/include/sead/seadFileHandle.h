#pragma once

#include "decomp.h"
#include "sead/seadFileDevice.h"
#include "sead/seadHandleBase.h"

namespace sead {
// RTTI N4sead10FileHandleE @ 0x008D1384
// vtable 0x00904EB4 (vptr 0x00904EBC), offset_to_top 0, 2 entries
//
// The handle of an opened file (closed by the destructor). The functions and member names are ours.
class FileHandle : public ::sead::HandleBase
{
public:
    FileHandle() : HandleBase(), mDivSize(0) {}
    virtual ~FileHandle(); // 0x0053F1B4 slot 0x00
    // 0x0053F184 slot 0x04 (deleting dtor)

    bool close(); // 0x0053F054
    bool read(u32* readSize, u8* buffer, u32 size); // 0x0053F074
    bool seek(s32 offset, FileDevice::SeekOrigin origin); // 0x0053F0A0
    bool write(u32* writtenSize, const u8* buffer, u32 size); // 0x0053F10C

    // the size of the pieces in which the file is read (0: at once)
    u32 mDivSize; // 0x2C
};
ASSERT_SIZE(FileHandle, 0x30);
} // namespace sead
