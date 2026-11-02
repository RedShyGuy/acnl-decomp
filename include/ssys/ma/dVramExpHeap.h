#pragma once

#include "decomp.h"
#include "ssys/ma/dVramHeap.h"
#include "ssys/ma/dVramMemBlock.h"
#include "ssys/ma/dVramMemList.h"
#include "sead/seadExpHeap.h"
#include "sead/seadUnitHeap.h"

#include <new>

namespace ssys {
namespace ma {
// a VramHeap on a block of a parent VramHeap: free and used blocks sorted by address, best or first fit; the heap object
// and its block descriptors live in an ExpHeap of a sead heap (function and member names are ours)
// RTTI N4ssys2ma11VramExpHeapE @ 0x008D2404
// vtable 0x0090718C (vptr 0x00907194), offset_to_top 0, 11 entries
class VramExpHeap : public ::ssys::ma::VramHeap
{
public:
    // a heap of size bytes of parent (all it can give for 0) in an ExpHeap of heapSize bytes of heap (all for 0)
    static VramExpHeap* Create(u32 size, const sead::SafeStringBase<char>& name, VramHeap* parent, u32 heapSize,
                               sead::Heap* heap); // 0x00567650

    DECOMP_EMIT_COPY DECOMP_NOINLINE VramExpHeap(const sead::SafeStringBase<char>& name, sead::ExpHeap* heap, VramHeap* parent,
                                 uptr address, u32 size) // 0x00120C6C
        : VramHeap(name), m_pParent(parent), m_Size(size), m_Address(address), m_pHeap(heap), m_IsBestFit(true)
    {
        m_pBlockHeap = sead::UnitHeap::create(m_pHeap->getMaxAllocatableSize(4),
                                              sead::SafeStringBase<char>("VramBlockHeap"), sizeof(VramMemBlock), 4,
                                              m_pHeap, false);
        m_UsedList.Clear();
        m_FreeList.Clear();
    }

    void Destroy() override;                                        // 0x00567928 slot 0x00
    u32 Adjust() override;                                          // 0x00567768 slot 0x04
    uptr Alloc(u32 size, s32 alignment) override;                   // 0x00567474 slot 0x08
    void Free(uptr address) override;                               // 0x005673E8 slot 0x0C
    void FreeAll() override;                                        // 0x00567960 slot 0x10
    u32 GetFreeSize() override;                                     // 0x00750B08 slot 0x14
    u32 GetMaxAllocatableSize(s32 alignment) override;              // 0x00750B4C slot 0x18
    bool Resize(uptr address, u32 oldSize, u32 newSize) override;  // 0x00567830 slot 0x1C
    void Dump() override;                                           // 0x0056782C slot 0x20
    virtual ~VramExpHeap();                                         // 0x00567A24 slot 0x24

    // one free block over all of the heap
    DECOMP_EMIT_COPY DECOMP_NOINLINE void ResetFreeList() // 0x00120BF4
    {
        void* memory = m_pBlockHeap->tryAlloc(sizeof(VramMemBlock), 4);
        if (memory == NULL) {
            return;
        }
        VramMemBlock* block = new (memory) VramMemBlock();
        block->m_Size = m_Size;
        block->m_Address = m_Address;
        m_FreeList.InsertByPriority(block);
    }
    // joins block with the free blocks next to it
    void MergeFreeBlock(VramMemBlock* block); // 0x00567314
    // (inline)
    void AddFreeBlock(VramMemBlock* block)
    {
        block->m_Offset = 0;
        m_FreeList.InsertByPriority(block);
        MergeFreeBlock(block);
    }

    VramHeap* m_pParent;         // 0x28
    u32 m_Size;                  // 0x2C
    uptr m_Address;              // 0x30
    sead::ExpHeap* m_pHeap;      // 0x34 holds this object and the block heap
    sead::UnitHeap* m_pBlockHeap; // 0x38 the VramMemBlocks
    VramMemList m_UsedList;      // 0x3C
    VramMemList m_FreeList;      // 0x48
    bool m_IsBestFit;            // 0x54 false: the first block that fits
};
ASSERT_SIZE(VramExpHeap, 0x58);
} // namespace ma
} // namespace ssys
