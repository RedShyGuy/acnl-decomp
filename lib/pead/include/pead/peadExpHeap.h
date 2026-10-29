#pragma once

#include "decomp.h"
#include "pead/RuntimeTypeInfo/peadDerive.h"
#include "pead/peadHeap.h"
#include "pead/peadSafeStringBase.h"

#include <new>

namespace pead {
// The header of a block of an ExpHeap, free or used (the class name is from the reference symbols of
// nintendogs, pushToFreeList_(pead::MemBlock*); unlike sead's ExpHeap::MemBlock it has no heap id, the size
// comes first and the alignment offset is a word; the member names are ours).
class MemBlock
{
public:
    MemBlock() : mListNode(), mSize(0), mAlignOffset(0) {}

    // the memory of the block (behind the header and the alignment offset)
    u8* getMemory() const { return reinterpret_cast<u8*>(const_cast<MemBlock*>(this)) + sizeof(MemBlock) + mAlignOffset; }
    // the block of the memory: with an alignment offset the word before the memory holds the block address + 1
    static MemBlock* fromMemory(void* memory)
    {
        uptr before = static_cast<uptr*>(memory)[-1];
        if ((before & 1) != 0) {
            return reinterpret_cast<MemBlock*>(before - 1);
        }
        return reinterpret_cast<MemBlock*>(static_cast<u8*>(memory) - sizeof(MemBlock));
    }
    void setAlignOffset(u32 offset)
    {
        mAlignOffset = offset;
        if (offset != 0) {
            reinterpret_cast<uptr*>(getMemory())[-1] = reinterpret_cast<uptr>(this) + 1;
        }
    }

    ListNode mListNode; // 0x0, in the free or the used list
    u32 mSize;          // 0x8, of the memory behind the header
    u32 mAlignOffset;   // 0xC
};
ASSERT_SIZE(MemBlock, 0x10);

// RTTI N4pead7ExpHeapE @ 0x008D12EC
// vtable 0x00904D34 (vptr 0x00904D3C), offset_to_top 0, 24 entries
//
// pia's copy of sead::ExpHeap (an older one): a heap of blocks of any size with a list of the free blocks
// (sorted by address, merged by mergeFreeList_) and a list of the used blocks (always appended). The names
// with a reference comment are those of the reference symbols; the member names and the other names are ours
// or those of our sead.
class ExpHeap : public ::pead::Heap
{
public:
    // how a free block is chosen
    enum FindMode : u8
    {
        FIND_MODE_FIRST_FIT = 0,
        FIND_MODE_SMALLEST_FIT = 1,
        FIND_MODE_LARGEST_FIT = 2,
    };

    ExpHeap(const SafeStringBase<char>& name, Heap* parent, void* start, u32 size, HeapDirection direction,
            bool enableLock); // 0x0053D59C | nintendogs:bytes [tier A]
    virtual ~ExpHeap(); // 0x0053D66C slot 0x00 | slot vf_0x00 of pead::IDisposer
    // 0x0053D640 slot 0x04 | slot vf_0x04 of pead::IDisposer (deleting dtor)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x00749B70 (name after sead)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return Heap::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074974C (name after sead)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name after sead)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x00983394 (guard 0x00983390)
        static const RuntimeTypeInfo::Derive<Heap> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual void destroy(); // 0x0053D0CC slot 0x10 (name after sead)
    virtual u32 adjust(); // 0x0053CEF8 slot 0x14 | nintendogs:bytes-fuzzy
    virtual void* tryAlloc(u32 size, s32 alignment); // 0x0053D1B8 slot 0x18 | nintendogs:bytes-fuzzy
    virtual void free(void* ptr); // 0x0053883C slot 0x1C | nintendogs:callseq
    virtual void* resizeFront(void* ptr, u32 newSize); // 0x0053C81C slot 0x20 (name after sead)
    virtual void* resizeBack(void* ptr, u32 newSize); // 0x0053C6F0 slot 0x24 (name after sead)
    virtual void freeAll(); // 0x0053D13C slot 0x28 | nintendogs:callseq
    virtual const void* getStartAddress() const; // 0x00749744 slot 0x2C (name after sead)
    virtual const void* getEndAddress() const; // 0x00749734 slot 0x30 (name after sead)
    virtual u32 getSize() const; // 0x00749C80 slot 0x34 (name after sead)
    virtual u32 getFreeSize() const; // 0x00749638 slot 0x38 | nintendogs:bytes
    // the size of the free block that an allocation with the alignment would take (largest fit)
    virtual u32 getMaxAllocatableSize(s32 alignment) const; // 0x00749798 slot 0x3C | nintendogs:bytes
    virtual bool isInclude(const void* ptr) const; // 0x00749C88 slot 0x40 | nintendogs:bytes
    virtual bool isFreeable() const; // 0x007495C4 slot 0x44 (name after sead)
    virtual bool isResizable() const; // 0x007496B0 slot 0x48 (name after sead)
    virtual bool isAdjustable() const; // 0x0074972C slot 0x4C (name after sead)
    virtual void dump() const; // 0x00749C24 slot 0x50 (name after sead)
    virtual void vf_0x54(); // 0x0053B438 slot 0x54
    virtual void setAllocMode(FindMode mode); // 0x0053C9A4 slot 0x58 (name after sead)
    virtual FindMode getAllocMode() const; // 0x00749724 slot 0x5C (name after sead)

    // a root heap in [start, start + size) (the heap object at the start)
    static ExpHeap* tryCreate(void* start, u32 size, const SafeStringBase<char>& name, bool enableLock); // 0x0053D400 | nintendogs:callseq [tier A]
    // a heap of size bytes of parent (the current heap for NULL; all that parent can give for size 0)
    static ExpHeap* tryCreate(u32 size, const SafeStringBase<char>& name, Heap* parent, HeapDirection direction,
                              bool enableLock); // 0x0053D490 | nintendogs:callseq (parameter names are ours)
    // the first free block (all of the memory behind the heap object)
    static DECOMP_NOINLINE void createMaxSizeFreeMemBlock_(ExpHeap* heap); // 0x0053CE64 | nintendogs:bytes [tier A]

    // number of allocated blocks (inline in pia's common::HeapManager::Cleanup; name is ours)
    s32 getUseCount() const { return mUseList.mCount; }

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
        mUseList.pushBack(block);
        splitRest_(block, memory + size, rest, size);
        return block;
    }
    DECOMP_NOINLINE MemBlock* allocFromHead_(u32 size, s32 alignment); // 0x0053C9AC | nintendogs:bytes [tier A]
    DECOMP_NOINLINE MemBlock* allocFromTail_(u32 size); // 0x0053CAAC | nintendogs:bytes [tier A]
    DECOMP_NOINLINE MemBlock* allocFromTail_(u32 size, s32 alignment); // 0x0053CC14 | nintendogs:bytes [tier B]
    // links the block into the free list (sorted by address) and merges neighbours
    DECOMP_NOINLINE void pushToFreeList_(MemBlock* block); // 0x0053CD1C | nintendogs:bytes [tier A]
    DECOMP_NOINLINE void mergeFreeList_(); // 0x0053CDA4 | nintendogs:bytes [tier A]
    MemBlock* findFreeMemBlockFromHead_(u32 size, FindMode mode) const; // 0x00749928 | nintendogs:bytes [tier A]
    MemBlock* findFreeMemBlockFromHead_(u32 size, s32 alignment, FindMode mode) const; // 0x007499C4 | nintendogs:bytes [tier A]
    MemBlock* findFreeMemBlockFromTail_(u32 size, s32 alignment, FindMode mode) const; // 0x00749A90 | nintendogs:bytes [tier A]
    // (inline everywhere)
    MemBlock* findFreeMemBlockFromTail_(u32 size, FindMode mode) const
    {
        MemBlock* found = nullptr;
        for (MemBlock* it = mFreeList.backOrNull(); it != nullptr && it != mFreeList.endObj(); it = mFreeList.prev(it)) {
            if (size > it->mSize) {
                continue;
            }
            if (mode == FIND_MODE_FIRST_FIT) {
                return it;
            }
            if (found == nullptr) {
                found = it;
            } else if (mode == FIND_MODE_SMALLEST_FIT) {
                if (found->mSize > it->mSize) {
                    found = it;
                }
            } else if (mode == FIND_MODE_LARGEST_FIT) {
                if (found->mSize < it->mSize) {
                    found = it;
                }
            }
        }
        return found;
    }
    DECOMP_NOINLINE void dumpUseList() const; // 0x007495CC | nintendogs:bytes [tier A]
    DECOMP_NOINLINE void dumpFreeList() const; // 0x007496B8 | nintendogs:bytes [tier A]
    // orders blocks by their address (for ListImpl::sort; name after sead)
    static s32 compareMemBlockAddress_(const void* a, const void* b); // 0x0053CE50

    // the rest behind a used block: a free block of its own if it has room for a header, else part of the used
    // block
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

    u8 mAllocMode;                  // 0x70, the FindMode of the allocations
    OffsetList<MemBlock> mFreeList; // 0x74
    OffsetList<MemBlock> mUseList;  // 0x84
};
ASSERT_SIZE(ExpHeap, 0x94);
} // namespace pead
