#pragma once

#include "decomp.h"
#include "g3d/dResourceLoader.h"

namespace item {
// the loader of the resources of the items (class after the RTTI and libgarden; members after libgarden)
// RTTI N4item9ResLoaderE @ 0x008D1128
// vtable 0x009049A0 (vptr 0x009049A8), offset_to_top 0, 2 entries
class ResLoader : public ::g3d::ResourceLoader
{
public:
    ResLoader(); // 0x0053788C (name after the RTTI)
    virtual ~ResLoader(); // 0x005378C8 slot 0x00 (name after the RTTI)
    // 0x005378B4 (deleting dtor)

    s32 unknown0; // 0x108
    s32 unknown1; // 0x10C
    s32 unknown2; // 0x110
};
ASSERT_SIZE(ResLoader, 0x114);
} // namespace item
