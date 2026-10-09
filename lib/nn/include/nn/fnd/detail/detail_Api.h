#pragma once

#include "decomp.h"

// The heap of nn::fnd::ExpHeapBase: blocks of any size taken from a list of free blocks. The type
// and function names are from the symbols (nintendogs, fefates); the members, the inline helpers
// and the constants are ours, after what the code does.

namespace nn {
namespace fnd {
namespace detail {
// the links of an object in an NNSFndList (at the offset the list was made with; name is ours)
struct ListLink
{
    void* prevObject; // 0x0
    void* nextObject; // 0x4
};
ASSERT_SIZE(ListLink, 0x8);

// a doubly linked list of objects that contain a ListLink
struct NNSFndList
{
    void* headObject; // 0x0
    void* tailObject; // 0x4
    u16 numObjects;   // 0x8
    u16 offset;       // 0xA, of the ListLink in the objects
};
ASSERT_SIZE(NNSFndList, 0xC);

// the header in front of every block of an ExpHeapImpl
struct NNSiFndExpHeapMBlockHead
{
    u16 signature; // 0x0, MBLOCK_SIGNATURE_FREE or MBLOCK_SIGNATURE_USED
    // bits 0-7 group ID, 8-14 the bytes before the header that belong to the block (alignment),
    // 15 the direction it was allocated from (see the accessors in detail_Api.cpp)
    u16 attribute;                   // 0x2
    u32 blockSize;                   // 0x4, without the header
    NNSiFndExpHeapMBlockHead* prev; // 0x8
    NNSiFndExpHeapMBlockHead* next; // 0xC
};
ASSERT_SIZE(NNSiFndExpHeapMBlockHead, 0x10);

// a list of blocks (name is ours)
struct MBlockList
{
    NNSiFndExpHeapMBlockHead* head; // 0x0
    NNSiFndExpHeapMBlockHead* tail; // 0x4
};

struct NNSiFndExpHeapHead
{
    MBlockList freeList;       // 0x00, by address
    MBlockList usedList;       // 0x08, in the order of allocation
    u16 groupId;               // 0x10, for the next allocation
    u16 feature;               // 0x12, bit 0: the allocation mode (ExpHeapBase::AllocationMode)
    bool useMarginOfAlignment; // 0x14
};
ASSERT_SIZE(NNSiFndExpHeapHead, 0x18);

// a heap; the heaps form a tree by address (a heap inside the memory of another one is its child)
struct ExpHeapImpl
{
    u32 signature;                  // 0x00, HEAP_SIGNATURE_EXP; 0 while there is no heap
    ListLink link;                  // 0x04, in the list of the parent (or the root list)
    NNSFndList childList;           // 0x0C
    void* heapStart;                // 0x18
    void* heapEnd;                  // 0x1C
    u32 attribute;                  // 0x20, the options of CreateHeap (bit 0: clear allocated memory)
    NNSiFndExpHeapHead expHeapHead; // 0x24
};
ASSERT_SIZE(ExpHeapImpl, 0x3C);

ExpHeapImpl* CreateHeap(nn::fnd::detail::ExpHeapImpl* heap, void* startAddress, unsigned size, unsigned short option); // 0x001245F8 | nintendogs:bytes [tier A]
DECOMP_NOINLINE void NNSi_FndInitHeapHead(nn::fnd::detail::ExpHeapImpl* heap, unsigned signature, void* heapStart, void* heapEnd, unsigned short option); // 0x00129FD0 | nintendogs:bytes [tier A]
// alignment > 0: from the start of the heap, < 0: from the end
void* AllocFromHeap(nn::fnd::detail::ExpHeapImpl* heap, unsigned size, int alignment); // 0x00130CBC | nintendogs:bytes [tier A]
DECOMP_NOINLINE void AppendListObject(nn::fnd::detail::NNSFndList* list, void* object); // 0x00130E04 | nintendogs:bytes [tier A]
// the Set functions return the old value
u16 SetGroupIDForHeap(nn::fnd::detail::ExpHeapImpl* heap, unsigned short groupId); // 0x00130E6C | nintendogs:callgraph [tier A]
u16 SetAllocModeForHeap(nn::fnd::detail::ExpHeapImpl* heap, unsigned short mode); // 0x00130E7C | nintendogs:bytes [tier A]
bool UseMarginOfAlignmentForHeap(nn::fnd::detail::ExpHeapImpl* heap, bool reuse); // 0x00130E9C | nintendogs:callgraph [tier A]
DECOMP_NOINLINE void InitList(nn::fnd::detail::NNSFndList* list, unsigned short offset); // 0x00130EAC | nintendogs:bytes [tier A]
DECOMP_NOINLINE void* AllocUsedBlockFromFreeBlock(nn::fnd::detail::NNSiFndExpHeapHead* expHeapHead, nn::fnd::detail::NNSiFndExpHeapMBlockHead* freeBlock, void* memory,
                                  unsigned int size, unsigned short direction); // 0x00136A44 | fefates:bytes [tier B]
// the innermost heap of the list (recursively its children) that contains memory, 0 for none
DECOMP_NOINLINE ExpHeapImpl* FindContainHeap(nn::fnd::detail::NNSFndList* list, const void* memory); // 0x0013B454 | fefates:bytes [tier B]
// the object after object (the first one for 0)
DECOMP_NOINLINE void* GetNextListObject(const nn::fnd::detail::NNSFndList* list, const void* object); // 0x0013B4AC | nintendogs:bytes [tier A]
void FreeToHeap(nn::fnd::detail::ExpHeapImpl* heap, void* memory); // 0x0013ECA4 | nintendogs:bytes-fuzzy [tier A]
void DestroyHeap(nn::fnd::detail::ExpHeapImpl* heap); // 0x0013EDE0 | nintendogs:callgraph [tier A]
DECOMP_NOINLINE void NNSi_FndFinalizeHeap(nn::fnd::detail::ExpHeapImpl* heap); // 0x0013EDE4 | nintendogs:bytes [tier A]
DECOMP_NOINLINE void RemoveListObject(nn::fnd::detail::NNSFndList* list, void* object); // 0x00140A28 | nintendogs:bytes [tier A]
} // namespace detail
} // namespace fnd
} // namespace nn
