#pragma once

#include "decomp.h"
#include "nn/fs/fs_IPositionable.h"

namespace nn {
namespace fs {
// RTTI N2nn2fs13IOutputStreamE @ 0x008CDC00
// vtable 0x008A365C (vptr 0x008A3664), offset_to_top 0, 14 entries
// vtable 0x008A37F0 (vptr 0x008A37F8), offset_to_top 0, 14 entries
class IOutputStream : public virtual ::nn::fs::IPositionable
{
public:
    virtual ~IOutputStream(); // 0x003460C8 slot 0x00, 0x003460C4 slot 0x04 (deleting)
    // the names are ours
    virtual nn::Result TryWrite(s32* writtenSize, const void* buffer, size_t size, bool flush) = 0; // slot 0x28
    virtual s32 Write(const void* buffer, size_t size, bool flush) = 0; // slot 0x2C
    virtual nn::Result TrySetSize(s64 size) = 0; // slot 0x30
    virtual void SetSize(s64 size) = 0; // slot 0x34
};
} // namespace fs
} // namespace nn
