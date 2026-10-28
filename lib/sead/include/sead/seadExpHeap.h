#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadHeap.h"

#include <new>

namespace sead {
// RTTI N4sead7ExpHeapE @ 0x008D2138
// vtable 0x00906C00 (vptr 0x00906C08), offset_to_top 0, 27 entries
// vtable 0x00906C74 (vptr 0x00906C7C), offset_to_top -24, 1 entries
//
// A heap of blocks of any size: a list of the free blocks (sorted by address, merged with their
// neighbours) and a list of the used blocks, each with a header in front of its memory. The names
// of the functions with the pead comment are the reference names of the same functions of pead;
// the member names and the other names are ours.
class ExpHeap : public ::sead::Heap
{
public:
    // how a free block is chosen
    enum FindMode : u8
    {
        FIND_MODE_FIRST_FIT = 0,
        FIND_MODE_SMALLEST_FIT = 1,
        FIND_MODE_LARGEST_FIT = 2,
    };

    // the header of a block of the heap (free or used)
    struct MemBlock
    {
        MemBlock() : mListNode(), mHeapCheckTag(0), mAlignOffset(0), mSize(0) {}

        // the memory of the block (behind the header and the alignment offset)
        u8* getMemory() const { return reinterpret_cast<u8*>(const_cast<MemBlock*>(this)) + sizeof(MemBlock) + mAlignOffset; }
        u8* getMemoryEnd() const { return getMemory() + mSize; }
        // the block of the memory: with an alignment offset the word before the memory holds the block
        // address + 1
        static MemBlock* fromMemory(void* memory)
        {
            uptr before = static_cast<uptr*>(memory)[-1];
            if ((before & 1) != 0) {
                return reinterpret_cast<MemBlock*>(before - 1);
            }
            return reinterpret_cast<MemBlock*>(static_cast<u8*>(memory) - sizeof(MemBlock));
        }
        void setAlignOffset(u16 offset)
        {
            mAlignOffset = offset;
            if (offset != 0) {
                reinterpret_cast<uptr*>(getMemory())[-1] = reinterpret_cast<uptr>(this) + 1;
            }
        }

        ListNode mListNode; // 0x0, in the free or the used list
        u16 mHeapCheckTag;  // 0x8, the id of the heap (used blocks)
        u16 mAlignOffset;   // 0xA
        u32 mSize;          // 0xC, of the memory behind the header
    };

    ExpHeap(const SafeStringBase<char>& name, Heap* parent, void* start, u32 size, HeapDirection direction, bool enableLock); // 0x00133578 (name is ours)
    virtual ~ExpHeap(); // 0x0055F184 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x0055F154 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074F558 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return Heap::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074F0C4 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009835BC (guard 0x009835B8)
        static const RuntimeTypeInfo::Derive<Heap> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual void destroy(); // 0x0055EDF4 slot 0x10
    virtual u32 adjust(); // 0x0055EBF0 slot 0x14
    virtual void* tryAlloc(u32 size, s32 alignment); // 0x0055EEE0 slot 0x18
    virtual void free(void* ptr); // 0x0055EAEC slot 0x1C
    virtual void* resizeFront(void* ptr, u32 newSize); // 0x0055E588 slot 0x20
    virtual void* resizeBack(void* ptr, u32 newSize); // 0x0055E19C slot 0x24
    virtual void* tryRealloc(void* ptr, u32 newSize, s32 alignment); // 0x0055E2D0 slot 0x28
    virtual void freeAll(); // 0x0055EE64 slot 0x2C
    virtual const void* getStartAddress() const; // 0x0074F0BC slot 0x30
    virtual const void* getEndAddress() const; // 0x0074F0AC slot 0x34
    virtual u32 getSize() const; // 0x0074F668 slot 0x38
    virtual u32 getFreeSize() const; // 0x0074EFA4 slot 0x3C
    virtual u32 getMaxAllocatableSize(s32 alignment) const; // 0x0074F110 slot 0x40
    virtual bool isInclude(const void* ptr) const; // 0x0074F904 slot 0x44
    virtual bool isFreeable() const; // 0x0074EF28 slot 0x48
    virtual bool isResizable() const; // 0x0074F020 slot 0x4C
    virtual bool isAdjustable() const; // 0x0074F0A4 slot 0x50
    virtual void dump() const; // 0x0074F60C slot 0x54
    virtual void vf_0x58(); // 0x0074F670 slot 0x58 | virtual slot, introduced by sead::Heap
    virtual void vf_0x5C(); // 0x0054E1E0 slot 0x5C
    virtual void setAllocMode(FindMode mode); // 0x0055E710 slot 0x64
    virtual FindMode getAllocMode() const; // 0x0074F09C slot 0x68

    // a root heap in [start, start + size) (the heap object at the start)
    static ExpHeap* CreateRoot(void* start, u32 size, const SafeStringBase<char>* name, bool enableLock); // 0x0013351C | libgarden [tier A]
    // a heap of size bytes of parent (the current heap for NULL; all that parent can give for size 0)
    static ExpHeap* tryCreate(u32 size, const SafeStringBase<char>& name, Heap* parent, HeapDirection direction,
                              bool enableLock); // 0x0012CC60 | manual:ref-verified
    static ExpHeap* create(u32 size, const SafeStringBase<char>& name, Heap* parent, HeapDirection direction,
                           bool enableLock); // 0x002F6D8C | manual:inferred
    // the first free block (all of the memory behind the heap object; name is ours)
    void initializeFreeList_(); // 0x00133484

    // a used block of size bytes from the free block found from the head (the rest stays free)
    MemBlock* allocFromHead_(u32 size)
    {
        MemBlock* block = findFreeMemBlockFromHead_(size, static_cast<FindMode>(mAllocMode));
        if (block == nullptr) {
            return nullptr;
        }
        u8* memory = block->getMemory();
        u32 rest = block->mSize - size;
        block->mSize = size;
        mFreeList.erase(block);
        pushToUseList_(block);
        splitRest_(block, memory + size, rest, size);
        return block;
    }
    DECOMP_NOINLINE MemBlock* allocFromHead_(u32 size, s32 alignment); // 0x0055E718 (name after pead::ExpHeap)
    DECOMP_NOINLINE MemBlock* allocFromTail_(u32 size); // 0x0055E85C (name after pead::ExpHeap)
    DECOMP_NOINLINE MemBlock* allocFromTail_(u32 size, s32 alignment); // 0x0055E9A4 (name after pead::ExpHeap)
    DECOMP_NOINLINE void pushToUseList_(MemBlock* block); // 0x0055EA8C (name is ours)
    DECOMP_NOINLINE void pushToFreeList_(MemBlock* block); // 0x00139208 (name after pead::ExpHeap)
    MemBlock* findFreeMemBlockFromHead_(u32 size, FindMode mode) const; // 0x0074F304 (name after pead::ExpHeap)
    MemBlock* findFreeMemBlockFromHead_(u32 size, s32 alignment, FindMode mode) const; // 0x0074F3A8 (name after pead::ExpHeap)
    MemBlock* findFreeMemBlockFromTail_(u32 size, s32 alignment, FindMode mode) const; // 0x0074F478 (name after pead::ExpHeap)
    // (inline everywhere)
    DECOMP_ALWAYS_INLINE MemBlock* findFreeMemBlockFromTail_(u32 size, FindMode mode) const;
    DECOMP_NOINLINE void dumpUseList() const; // 0x0074EF30 (name after pead::ExpHeap)
    DECOMP_NOINLINE void dumpFreeList() const; // 0x0074F028 (name after pead::ExpHeap)
    // orders blocks by their address (for ListImpl::sort; name is ours)
    static s32 compareMemBlockAddress_(const void* a, const void* b); // 0x0055EAD8

    // the rest behind a used block: a free block of its own if it has room for a header, else part of
    // the used block
    void splitRest_(MemBlock* block, u8* restStart, u32 rest, u32 size)
    {
        if (rest > sizeof(MemBlock)) {
            MemBlock* free = new (restStart) MemBlock();
            free->mSize = rest - sizeof(MemBlock);
            pushToFreeList_(free);
        } else if (rest != 0) {
            block->mSize = size + rest;
        }
    }

    u8 mAllocMode;                  // 0x74, the FindMode of the allocations
    OffsetList<MemBlock> mFreeList; // 0x78
    OffsetList<MemBlock> mUseList;  // 0x88
};
ASSERT_SIZE(ExpHeap, 0x98);
} // namespace sead
