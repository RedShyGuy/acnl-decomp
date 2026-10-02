#pragma once

#include "decomp.h"
#include "nn/fs/detail/fs_FileBase.h"
#include "nn/fs/fs_IInputStream.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace nn {
namespace fs {
// RTTI N2nn2fs15FileInputStreamE @ 0x008CDC20
// vtable 0x008FBD64 (vptr 0x008FBD6C), offset_to_top 0, 12 entries
class FileInputStream : public ::nn::fs::IInputStream, public ::nn::fs::detail::FileBase, public ::nn::util::ADLFireWall::NonCopyable<nn::fs::FileInputStream>
{
public:
    FileInputStream(); // ctor address unknown
    virtual void vf_0x00(); // 0x0013E5F8 slot 0x00 | virtual slot, introduced by nn::fs::IInputStream
    virtual void vf_0x04(); // 0x00346398 slot 0x04 | virtual slot, introduced by nn::fs::IInputStream
    virtual void vf_0x08(); // 0x00346380 slot 0x08 | virtual slot, introduced by nn::fs::IInputStream
    virtual void vf_0x0C(); // 0x0034634C slot 0x0C | virtual slot, introduced by nn::fs::IInputStream
    virtual void vf_0x10(); // 0x007268A0 slot 0x10 | virtual slot, introduced by nn::fs::IInputStream
    virtual void vf_0x14(); // 0x00726898 slot 0x14 | virtual slot, introduced by nn::fs::IInputStream
    virtual void vf_0x18(); // 0x00346318 slot 0x18 | virtual slot, introduced by nn::fs::IInputStream
    virtual void vf_0x1C(); // 0x003462F4 slot 0x1C | virtual slot, introduced by nn::fs::IInputStream
    virtual void vf_0x20(); // 0x00726890 slot 0x20 | virtual slot, introduced by nn::fs::IInputStream
    virtual void vf_0x24(); // 0x007268B4 slot 0x24 | virtual slot, introduced by nn::fs::IInputStream
    virtual void vf_0x28(); // 0x00346378 slot 0x28 | virtual slot, introduced by nn::fs::IInputStream
    virtual void Read(void*, unsigned int); // 0x00346320 slot 0x2C | fefates:bytes
    ~FileInputStream(); // 0x0013E600 | fefates:bytes [tier B]
};
} // namespace fs
} // namespace nn
