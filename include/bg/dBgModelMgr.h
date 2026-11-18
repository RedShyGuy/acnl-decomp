#pragma once

#include "decomp.h"
#include "g3d/dResourceLoader.h"
#include "sead/seadCheckedArray.h"
#include "ssys/ma/dHeapAllocator.h"

namespace sead {
class Heap;
} // namespace sead

// The models of the grounds of a field (name after its heap "BgModelMgr"; all other names are ours): one loader of
// "Bg/Outdoor/Models/<ground>.bcres" or "Bg/Indoor/Models/<ground>.bcres" per block of a 7x7 field, in its own heap.
// BsCharRoomViewMgr holds one.
class BgModelMgr
{
public:
    static const u32 BLOCK_NUM = 7;

    BgModelMgr(); // 0x0018EEFC
    ~BgModelMgr(); // 0x0018EF28

    // the loader of block x/z (block 0 for indices out of range)
    g3d::ResourceLoader* GetLoader(u32 x, u32 z); // 0x0018EC74
    // the heap of the loaders (false without a parent)
    bool CreateHeap(sead::Heap* parent, u32 size); // 0x0018EC9C
    // starts loading the model of the ground bgName (BgName) for block x/z
    bool Load(u32 x, u32 z, u32 bgName); // 0x0018ECF8
    // all models are unloaded and the heap is destroyed (a copy among the inline functions)
    DECOMP_EMIT_COPY DECOMP_NOINLINE void Finalize() // 0x002F7274
    {
        for (u32 z = 0; z < BLOCK_NUM; z++) {
            for (u32 x = 0; x < BLOCK_NUM; x++) {
                m_Loaders[z][x].Unload();
            }
        }
        m_Allocator.DestroyHeap();
    }

    ssys::ma::HeapAllocator m_Allocator; // 0x00
    sead::CheckedArray<sead::CheckedArray<g3d::ResourceLoader, BLOCK_NUM>, BLOCK_NUM> m_Loaders; // 0x08 [z][x]
};
ASSERT_SIZE(BgModelMgr, 0x3290);
