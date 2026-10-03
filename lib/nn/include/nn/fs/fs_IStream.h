#pragma once

#include "decomp.h"
#include "nn/fs/fs_IInputStream.h"
#include "nn/fs/fs_IOutputStream.h"

namespace nn {
namespace fs {
// RTTI N2nn2fs7IStreamE @ 0x008CDD60
// vtable 0x008A359C (vptr 0x008A35A4), offset_to_top 0, 12 entries
// vtable 0x008A3718 (vptr 0x008A3720), offset_to_top -4, 14 entries
class IStream : public ::nn::fs::IInputStream, public ::nn::fs::IOutputStream
{
public:
    virtual ~IStream(); // 0x003498DC slot 0x00, 0x003498D8 slot 0x04 (deleting)
};
} // namespace fs
} // namespace nn
