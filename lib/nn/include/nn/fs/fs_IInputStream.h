#pragma once

#include "decomp.h"
#include "nn/fs/fs_IPositionable.h"

namespace nn {
namespace fs {
// RTTI N2nn2fs12IInputStreamE @ 0x008CDBE8
// vtable 0x008A35FC (vptr 0x008A3604), offset_to_top 0, 12 entries
// (symbols.json also lists vtable 0x0085FD74 with 13 entries under this RTTI; that is another class)
class IInputStream : public virtual ::nn::fs::IPositionable
{
public:
    virtual ~IInputStream(); // 0x00346018 slot 0x00, 0x00346014 slot 0x04 (deleting)
    // the names are ours
    virtual nn::Result TryRead(s32* readSize, void* buffer, size_t size) = 0; // slot 0x28
    virtual s32 Read(void* buffer, size_t size) = 0; // slot 0x2C
};
} // namespace fs
} // namespace nn
