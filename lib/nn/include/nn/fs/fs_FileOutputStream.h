#pragma once

#include "decomp.h"
#include "nn/fs/detail/fs_FileBase.h"
#include "nn/fs/fs_IOutputStream.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace nn {
namespace fs {
// RTTI N2nn2fs16FileOutputStreamE @ 0x008CDC48
// vtable 0x008FBDD4 (vptr 0x008FBDDC), offset_to_top 0, 16 entries
class FileOutputStream : public ::nn::fs::IOutputStream, public ::nn::fs::detail::FileBase, public ::nn::util::ADLFireWall::NonCopyable<nn::fs::FileOutputStream>
{
public:
    FileOutputStream(); // ctor address unknown
    virtual void vf_0x00(); // 0x00346510 slot 0x00 | virtual slot, introduced by nn::fs::IOutputStream
    virtual void vf_0x04(); // 0x003464D0 slot 0x04 | virtual slot, introduced by nn::fs::IOutputStream
    virtual void vf_0x08(); // 0x00346498 slot 0x08 | virtual slot, introduced by nn::fs::IOutputStream
    virtual void vf_0x0C(); // 0x003463E8 slot 0x0C | virtual slot, introduced by nn::fs::IOutputStream
    virtual void vf_0x10(); // 0x007268F0 slot 0x10 | virtual slot, introduced by nn::fs::IOutputStream
    virtual void vf_0x14(); // 0x007268E8 slot 0x14 | virtual slot, introduced by nn::fs::IOutputStream
    virtual void vf_0x18(); // 0x003463E0 slot 0x18 | virtual slot, introduced by nn::fs::IOutputStream
    virtual void vf_0x1C(); // 0x003463BC slot 0x1C | virtual slot, introduced by nn::fs::IOutputStream
    virtual void vf_0x20(); // 0x007268E0 slot 0x20 | virtual slot, introduced by nn::fs::IOutputStream
    virtual void vf_0x24(); // 0x00726904 slot 0x24 | virtual slot, introduced by nn::fs::IOutputStream
    virtual void vf_0x28(); // 0x003464B8 slot 0x28 | virtual slot, introduced by nn::fs::IOutputStream
    virtual void vf_0x2C(); // 0x00346438 slot 0x2C | virtual slot, introduced by nn::fs::IOutputStream
    virtual void vf_0x30(); // 0x003463B4 slot 0x30 | virtual slot, introduced by nn::fs::IOutputStream
    virtual void vf_0x34(); // 0x00346474 slot 0x34 | virtual slot, introduced by nn::fs::IOutputStream
    virtual void vf_0x38(); // 0x00346414 slot 0x38 | virtual slot, introduced by nn::fs::FileOutputStream
    virtual void vf_0x3C(); // 0x003464B0 slot 0x3C | virtual slot, introduced by nn::fs::FileOutputStream
};
} // namespace fs
} // namespace nn
