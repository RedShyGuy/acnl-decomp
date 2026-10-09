#include "nn/fnd/detail/detail_Api.h"
#include <stddef.h>
#include <string.h>

namespace nn {
namespace fnd {
namespace detail {
namespace {
// the signatures ('EXPH', 'FR', 'UD')
const u32 HEAP_SIGNATURE_EXP = 0x45585048;
const u16 MBLOCK_SIGNATURE_FREE = 0x4652;
const u16 MBLOCK_SIGNATURE_USED = 0x5544;

// ExpHeapImpl::attribute: clear allocated memory
const u32 HEAP_OPTION_ZERO_CLEAR = 1 << 0;

// NNSiFndExpHeapHead::feature bit 0: take the first block that fits, or the smallest one
const u16 ALLOC_MODE_FIRST_FIT = 0;
const u16 ALLOC_MODE_BEST_FIT = 1;

// where a block is taken from in the free block
const u16 ALLOC_DIRECTION_FRONT = 0;
const u16 ALLOC_DIRECTION_BACK = 1;

// a free block needs room for its header and at least this many bytes
const u32 MIN_FREE_BLOCK_SIZE = 4;

// 0x00982EBC
bool s_IsRootListInitialized = false;

// the heaps that are not inside another heap
// 0x00AF7FDC
NNSFndList s_RootList;

// a range of memory [start, end)
struct MemoryRegion
{
    u8* start;
    u8* end;
};

inline ListLink* GetLink(const NNSFndList* list, const void* object)
{
    return reinterpret_cast<ListLink*>(reinterpret_cast<uptr>(object) + list->offset);
}

inline u8* RoundUp(u8* p, u32 alignment)
{
    return reinterpret_cast<u8*>((reinterpret_cast<uptr>(p) + alignment - 1) & ~(alignment - 1));
}

inline u8* RoundDown(u8* p, u32 alignment)
{
    return reinterpret_cast<u8*>(reinterpret_cast<uptr>(p) & ~(alignment - 1));
}

inline ExpHeapImpl* GetHeap(NNSiFndExpHeapHead* expHeapHead)
{
    return reinterpret_cast<ExpHeapImpl*>(reinterpret_cast<u8*>(expHeapHead) - offsetof(ExpHeapImpl, expHeapHead));
}

// the parts of NNSiFndExpHeapMBlockHead::attribute
inline u16 GetAlignment(const NNSiFndExpHeapMBlockHead* block)
{
    return (block->attribute >> 8) & 0x7F;
}

inline void SetAlignment(NNSiFndExpHeapMBlockHead* block, u16 alignment)
{
    block->attribute = (block->attribute & ~0x7F00) | ((alignment & 0x7F) << 8);
}

inline void SetAllocDirection(NNSiFndExpHeapMBlockHead* block, u16 direction)
{
    block->attribute = (block->attribute & ~0x8000) | (direction << 15);
}

inline void SetGroupId(NNSiFndExpHeapMBlockHead* block, u16 groupId)
{
    block->attribute = (block->attribute & ~0xFF) | (groupId & 0xFF);
}

inline u8* GetMemory(NNSiFndExpHeapMBlockHead* block)
{
    return reinterpret_cast<u8*>(block) + sizeof(NNSiFndExpHeapMBlockHead);
}

inline u8* GetBlockEnd(NNSiFndExpHeapMBlockHead* block)
{
    return GetMemory(block) + block->blockSize;
}

// the memory of a block with its header and the alignment bytes before it
inline void GetRegion(MemoryRegion* region, NNSiFndExpHeapMBlockHead* block)
{
    region->start = reinterpret_cast<u8*>(block) - GetAlignment(block);
    region->end = GetBlockEnd(block);
}

// makes a block of a region
inline NNSiFndExpHeapMBlockHead* InitMBlock(const MemoryRegion* region, u16 signature)
{
    NNSiFndExpHeapMBlockHead* block = reinterpret_cast<NNSiFndExpHeapMBlockHead*>(region->start);
    block->signature = signature;
    block->attribute = 0;
    block->blockSize = region->end - GetMemory(block);
    block->prev = 0;
    block->next = 0;
    return block;
}

// inserts block after prev (at the head for 0)
inline NNSiFndExpHeapMBlockHead* InsertMBlock(MBlockList* list, NNSiFndExpHeapMBlockHead* block, NNSiFndExpHeapMBlockHead* prev)
{
    block->prev = prev;
    NNSiFndExpHeapMBlockHead* next;
    if (prev != 0) {
        next = prev->next;
        prev->next = block;
    } else {
        next = list->head;
        list->head = block;
    }
    block->next = next;
    if (next != 0) {
        next->prev = block;
    } else {
        list->tail = block;
    }
    return block;
}

// returns the block before it
inline NNSiFndExpHeapMBlockHead* RemoveMBlock(MBlockList* list, NNSiFndExpHeapMBlockHead* block)
{
    NNSiFndExpHeapMBlockHead* prev = block->prev;
    NNSiFndExpHeapMBlockHead* next = block->next;
    if (prev != 0) {
        prev->next = next;
    } else {
        list->head = next;
    }
    if (next != 0) {
        next->prev = prev;
    } else {
        list->tail = prev;
    }
    return prev;
}

// the list a heap belongs into
inline NNSFndList* FindListContainHeap(ExpHeapImpl* heap)
{
    NNSFndList* list = &s_RootList;
    ExpHeapImpl* parent = FindContainHeap(&s_RootList, heap);
    if (parent != 0) {
        list = &parent->childList;
    }
    return list;
}

// puts a region back into the free list, merged with the free blocks next to it
inline bool RecycleRegion(NNSiFndExpHeapHead* expHeapHead, const MemoryRegion* region)
{
    NNSiFndExpHeapMBlockHead* prev = 0;
    MemoryRegion freeRegion = *region;
    for (NNSiFndExpHeapMBlockHead* block = expHeapHead->freeList.head; block != 0; block = block->next) {
        if (reinterpret_cast<u8*>(block) < region->start) {
            prev = block;
            continue;
        }
        if (reinterpret_cast<u8*>(block) == region->end) {
            freeRegion.end = GetBlockEnd(block);
            RemoveMBlock(&expHeapHead->freeList, block);
        }
        break;
    }
    if (prev != 0 && GetBlockEnd(prev) == region->start) {
        freeRegion.start = reinterpret_cast<u8*>(prev);
        prev = RemoveMBlock(&expHeapHead->freeList, prev);
    }
    if (static_cast<u32>(freeRegion.end - freeRegion.start) < sizeof(NNSiFndExpHeapMBlockHead)) {
        return false;
    }
    InsertMBlock(&expHeapHead->freeList, InitMBlock(&freeRegion, MBLOCK_SIGNATURE_FREE), prev);
    return true;
}
} // namespace

// 0x001245F8 | nintendogs:bytes [tier A]
nn::fnd::detail::ExpHeapImpl* CreateHeap(nn::fnd::detail::ExpHeapImpl* heap, void* startAddress, unsigned size, unsigned short option)
{
    u8* end = RoundDown(static_cast<u8*>(startAddress) + size, 4);
    u8* start = RoundUp(static_cast<u8*>(startAddress), 4);
    if (start > end || static_cast<u32>(end - start) < sizeof(NNSiFndExpHeapMBlockHead) + MIN_FREE_BLOCK_SIZE) {
        return 0;
    }
    NNSi_FndInitHeapHead(heap, HEAP_SIGNATURE_EXP, start, end, option);
    NNSiFndExpHeapHead* expHeapHead = &heap->expHeapHead;
    expHeapHead->groupId = 0;
    expHeapHead->feature = 0;
    MemoryRegion region;
    region.start = static_cast<u8*>(heap->heapStart);
    region.end = static_cast<u8*>(heap->heapEnd);
    NNSiFndExpHeapMBlockHead* block = InitMBlock(&region, MBLOCK_SIGNATURE_FREE);
    expHeapHead->freeList.head = block;
    expHeapHead->freeList.tail = block;
    expHeapHead->usedList.head = 0;
    expHeapHead->usedList.tail = 0;
    return heap;
}

// 0x00129FD0 | nintendogs:bytes [tier A]
void NNSi_FndInitHeapHead(nn::fnd::detail::ExpHeapImpl* heap, unsigned signature, void* heapStart, void* heapEnd, unsigned short option)
{
    heap->signature = signature;
    heap->heapStart = heapStart;
    heap->heapEnd = heapEnd;
    heap->attribute = static_cast<u8>(option);
    InitList(&heap->childList, offsetof(ExpHeapImpl, link));
    if (!s_IsRootListInitialized) {
        InitList(&s_RootList, offsetof(ExpHeapImpl, link));
        s_IsRootListInitialized = true;
    }
    AppendListObject(FindListContainHeap(heap), heap);
}

// 0x00130CBC | nintendogs:bytes [tier A]
void* AllocFromHeap(nn::fnd::detail::ExpHeapImpl* heap, unsigned size, int alignment)
{
    if (size == 0) {
        size = 1;
    }
    size = (size + 3) & ~3;
    void* memory = 0;
    bool isFirstFit = (heap->expHeapHead.feature & 1) == ALLOC_MODE_FIRST_FIT;
    if (alignment >= 0) {
        // the lowest fitting address in the free blocks from the front
        NNSiFndExpHeapHead* expHeapHead = &heap->expHeapHead;
        NNSiFndExpHeapMBlockHead* found = 0;
        u32 foundSize = 0xFFFFFFFF;
        u8* foundMemory = 0;
        for (NNSiFndExpHeapMBlockHead* block = expHeapHead->freeList.head; block != 0; block = block->next) {
            u8* blockMemory = GetMemory(block);
            u8* aligned = RoundUp(blockMemory, alignment);
            u32 neededSize = (aligned - blockMemory) + size;
            if (block->blockSize >= neededSize && foundSize > block->blockSize) {
                found = block;
                foundSize = block->blockSize;
                foundMemory = aligned;
                if (isFirstFit || foundSize == size) {
                    break;
                }
            }
        }
        if (found != 0) {
            memory = AllocUsedBlockFromFreeBlock(expHeapHead, found, foundMemory, size, ALLOC_DIRECTION_FRONT);
        }
    } else {
        // the highest fitting address in the free blocks from the back
        NNSiFndExpHeapHead* expHeapHead = &heap->expHeapHead;
        u32 negativeAlignment = -alignment;
        NNSiFndExpHeapMBlockHead* found = 0;
        u32 foundSize = 0xFFFFFFFF;
        u8* foundMemory = 0;
        for (NNSiFndExpHeapMBlockHead* block = expHeapHead->freeList.tail; block != 0; block = block->prev) {
            u32 blockSize = block->blockSize;
            u8* blockMemory = GetMemory(block);
            u8* aligned = RoundDown(blockMemory + blockSize - size, negativeAlignment);
            if (static_cast<s32>(aligned - blockMemory) >= 0 && foundSize > blockSize) {
                found = block;
                foundSize = blockSize;
                foundMemory = aligned;
                if (isFirstFit || foundSize == size) {
                    break;
                }
            }
        }
        if (found != 0) {
            memory = AllocUsedBlockFromFreeBlock(expHeapHead, found, foundMemory, size, ALLOC_DIRECTION_BACK);
        }
    }
    return memory;
}

// 0x00130E04 | nintendogs:bytes [tier A]
void AppendListObject(nn::fnd::detail::NNSFndList* list, void* object)
{
    if (list->headObject == 0) {
        ListLink* link = GetLink(list, object);
        link->nextObject = 0;
        link->prevObject = 0;
        list->headObject = object;
        list->tailObject = object;
        list->numObjects++;
    } else {
        ListLink* link = GetLink(list, object);
        link->nextObject = 0;
        link->prevObject = list->tailObject;
        GetLink(list, list->tailObject)->nextObject = object;
        list->tailObject = object;
        list->numObjects++;
    }
}

// 0x00130E6C | nintendogs:callgraph [tier A]
u16 SetGroupIDForHeap(nn::fnd::detail::ExpHeapImpl* heap, unsigned short groupId)
{
    NNSiFndExpHeapHead* expHeapHead = &heap->expHeapHead;
    u16 old = expHeapHead->groupId;
    expHeapHead->groupId = groupId;
    return old;
}

// 0x00130E7C | nintendogs:bytes [tier A]
u16 SetAllocModeForHeap(nn::fnd::detail::ExpHeapImpl* heap, unsigned short mode)
{
    NNSiFndExpHeapHead* expHeapHead = &heap->expHeapHead;
    u16 old = expHeapHead->feature & 1;
    expHeapHead->feature = (expHeapHead->feature & ~1) | (mode & 1);
    return old;
}

// 0x00130E9C | nintendogs:callgraph [tier A]
bool UseMarginOfAlignmentForHeap(nn::fnd::detail::ExpHeapImpl* heap, bool reuse)
{
    NNSiFndExpHeapHead* expHeapHead = &heap->expHeapHead;
    bool old = expHeapHead->useMarginOfAlignment;
    expHeapHead->useMarginOfAlignment = reuse;
    return old;
}

// 0x00130EAC | nintendogs:bytes [tier A]
void InitList(nn::fnd::detail::NNSFndList* list, unsigned short offset)
{
    list->offset = offset;
    list->headObject = 0;
    list->tailObject = 0;
    list->numObjects = 0;
}

// 0x00136A44 | fefates:bytes [tier B]
void* AllocUsedBlockFromFreeBlock(nn::fnd::detail::NNSiFndExpHeapHead* expHeapHead, nn::fnd::detail::NNSiFndExpHeapMBlockHead* freeBlock,
                                  void* memory, unsigned int size, unsigned short direction)
{
    MemoryRegion freeRegion;
    GetRegion(&freeRegion, freeBlock);
    NNSiFndExpHeapMBlockHead* prev = RemoveMBlock(&expHeapHead->freeList, freeBlock);

    // the memory before the block becomes a free block if it is large enough (and wanted)
    MemoryRegion headRegion;
    headRegion.start = freeRegion.start;
    headRegion.end = static_cast<u8*>(memory) - sizeof(NNSiFndExpHeapMBlockHead);
    if (static_cast<u32>(headRegion.end - headRegion.start) < sizeof(NNSiFndExpHeapMBlockHead) + MIN_FREE_BLOCK_SIZE ||
        (direction == ALLOC_DIRECTION_FRONT && !expHeapHead->useMarginOfAlignment)) {
        headRegion.end = headRegion.start;
    } else {
        prev = InsertMBlock(&expHeapHead->freeList, InitMBlock(&headRegion, MBLOCK_SIGNATURE_FREE), prev);
    }

    // the same for the memory after it
    MemoryRegion tailRegion;
    tailRegion.start = static_cast<u8*>(memory) + size;
    tailRegion.end = freeRegion.end;
    if (static_cast<u32>(tailRegion.end - tailRegion.start) < sizeof(NNSiFndExpHeapMBlockHead) + MIN_FREE_BLOCK_SIZE ||
        (direction == ALLOC_DIRECTION_BACK && !expHeapHead->useMarginOfAlignment)) {
        tailRegion.start = tailRegion.end;
    } else {
        InsertMBlock(&expHeapHead->freeList, InitMBlock(&tailRegion, MBLOCK_SIGNATURE_FREE), prev);
    }

    if (GetHeap(expHeapHead)->attribute & HEAP_OPTION_ZERO_CLEAR) {
        u32* p = reinterpret_cast<u32*>(headRegion.end);
        for (u32 count = static_cast<u32>(tailRegion.start - headRegion.end) / sizeof(u32); count > 0; count--) {
            *p++ = 0;
        }
    }

    MemoryRegion usedRegion;
    usedRegion.start = static_cast<u8*>(memory) - sizeof(NNSiFndExpHeapMBlockHead);
    usedRegion.end = tailRegion.start;
    NNSiFndExpHeapMBlockHead* block = InitMBlock(&usedRegion, MBLOCK_SIGNATURE_USED);
    SetAllocDirection(block, direction);
    SetAlignment(block, reinterpret_cast<u8*>(block) - headRegion.end);
    SetGroupId(block, expHeapHead->groupId);
    InsertMBlock(&expHeapHead->usedList, block, expHeapHead->usedList.tail);
    return memory;
}

// 0x0013B454 | fefates:bytes [tier B]
nn::fnd::detail::ExpHeapImpl* FindContainHeap(nn::fnd::detail::NNSFndList* list, const void* memory)
{
    ExpHeapImpl* heap = 0;
    while ((heap = static_cast<ExpHeapImpl*>(GetNextListObject(list, heap))) != 0) {
        if (heap->heapStart <= memory && memory < heap->heapEnd) {
            ExpHeapImpl* child = FindContainHeap(&heap->childList, memory);
            if (child == 0) {
                return heap;
            }
            return child;
        }
    }
    return 0;
}

// 0x0013B4AC | nintendogs:bytes [tier A]
void* GetNextListObject(const nn::fnd::detail::NNSFndList* list, const void* object)
{
    if (object == 0) {
        return list->headObject;
    }
    return GetLink(list, object)->nextObject;
}

// 0x0013ECA4 | nintendogs:bytes-fuzzy [tier A]
void FreeToHeap(nn::fnd::detail::ExpHeapImpl* heap, void* memory)
{
    NNSiFndExpHeapHead* expHeapHead = &heap->expHeapHead;
    NNSiFndExpHeapMBlockHead* block = reinterpret_cast<NNSiFndExpHeapMBlockHead*>(static_cast<u8*>(memory) - sizeof(NNSiFndExpHeapMBlockHead));
    MemoryRegion region;
    GetRegion(&region, block);
    RemoveMBlock(&expHeapHead->usedList, block);
    RecycleRegion(expHeapHead, &region);
}

// 0x0013EDE0 | nintendogs:callgraph [tier A]
void DestroyHeap(nn::fnd::detail::ExpHeapImpl* heap)
{
    NNSi_FndFinalizeHeap(heap);
}

// 0x0013EDE4 | nintendogs:bytes [tier A]
void NNSi_FndFinalizeHeap(nn::fnd::detail::ExpHeapImpl* heap)
{
    RemoveListObject(FindListContainHeap(heap), heap);
}

// 0x00140A28 | nintendogs:bytes [tier A]
void RemoveListObject(nn::fnd::detail::NNSFndList* list, void* object)
{
    ListLink* link = GetLink(list, object);
    if (link->prevObject == 0) {
        list->headObject = link->nextObject;
    } else {
        GetLink(list, link->prevObject)->nextObject = link->nextObject;
    }
    if (link->nextObject == 0) {
        list->tailObject = link->prevObject;
    } else {
        GetLink(list, link->nextObject)->prevObject = link->prevObject;
    }
    link->prevObject = 0;
    link->nextObject = 0;
    list->numObjects--;
}

} // namespace detail
} // namespace fnd
} // namespace nn
