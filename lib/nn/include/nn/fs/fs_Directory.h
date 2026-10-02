#pragma once

#include "decomp.h"
#include "nn/fs/detail/fs_DirectoryBase.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace nn {
namespace fs {
// RTTI N2nn2fs9DirectoryE @ 0x008CDD80
// vtable 0x008FBFC8 (vptr 0x008FBFD0), offset_to_top 0, 2 entries
class Directory : public ::nn::util::ADLFireWall::NonCopyable<nn::fs::Directory>, public ::nn::fs::detail::DirectoryBase
{
public:
    Directory(); // ctor candidate(s) 0x005AC08C, 0x005B0AC0, 0x005B0B54 (unverified)
    virtual void vf_0x00(); // 0x00349908 slot 0x00 | virtual slot, introduced by nn::fs::Directory
    virtual ~Directory(); // 0x003498E0 slot 0x04 | fefates:bytes
};
} // namespace fs
} // namespace nn
