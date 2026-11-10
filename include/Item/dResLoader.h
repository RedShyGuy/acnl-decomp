#pragma once

#include "decomp.h"
#include "g3d/dResourceLoader.h"

class SvFgName;
namespace sead {
class Heap;
} // namespace sead

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

    // (game code, not decompiled; names are ours) the resources of an item (arguments as the letter paper passes them)
    bool Load(const SvFgName* item, sead::Heap* heap, u32 arg3, u32 arg4, u32 arg5, bool arg6, u32 arg7); // 0x00537704
    // the image / the format of the texture of the index
    void* GetTextureImage(s32 index); // 0x005370D4
    u8 GetTextureFormat(s32 index); // 0x0053707C
    // (the extra part freed first, then ResourceLoader::Unload)
    void Release(); // 0x005376D4

    s32 unknown0; // 0x108
    s32 unknown1; // 0x10C
    s32 unknown2; // 0x110
};
ASSERT_SIZE(ResLoader, 0x114);
} // namespace item
