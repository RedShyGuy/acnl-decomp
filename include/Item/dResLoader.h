#pragma once

#include "decomp.h"
#include "g3d/dResourceLoader.h"
#include "nw/ut/internal/ut_LinkListImpl.h"

struct SvMyDesign;

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
    bool Load(const SvFgName* item, sead::Heap* heap, u32 arg3, u32 arg4, const void* arg5, bool arg6, u32 arg7); // 0x00537704
    // the image / the format of the texture of the index
    // (game code, not decompiled; name after the address) the resources of a pattern item (fgobj passes 4, 1, 0, 1, 0)
    bool Func536B74(const SvFgName* item, sead::Heap* heap, u32 alignment, u32 arg4, const void* arg5, u32 arg6, u32 arg7); // 0x00536B74
    void* GetTextureImage(s32 index); // 0x005370D4
    u8 GetTextureFormat(s32 index); // 0x0053707C
    // (the extra part freed first, then ResourceLoader::Unload)
    void Release(); // 0x005376D4

    nw::ut::LinkListNode m_DesignNode; // 0x108 in g_DesignLoaders while m_pDesign is shown (names are ours; libgarden: unknown0/1)
    SvMyDesign* m_pDesign;             // 0x110 the design of the textures (libgarden: unknown2)
};
ASSERT_SIZE(ResLoader, 0x114);
} // namespace item
