#include <cstring>

#include "mw/qrd/qrd_Internal.h"

namespace mw {
namespace qrd {
namespace detail {

// 0x0095F324
WorkState s_WorkState;
// 0x00AD7670
WorkBlock s_WorkBlocks[WORK_BLOCK_NUM];

// 0x0015896C
void FreeTop(void* ptr)
{
    if (s_WorkBlocks[0].ptr == ptr)
    {
        s_WorkBlocks[0].ptr = NULL;
        s_WorkBlocks[0].size = 0;
        s_WorkBlocks[0].isUsed = 0;
    }
}

// 0x00158994
void* AllocateTop(u32 size)
{
    if (s_WorkState.workSize - s_WorkState.workUsed < size)
    {
        return NULL;
    }

    s_WorkBlocks[0].isUsed = 1;
    s_WorkBlocks[0].size = size;
    s_WorkBlocks[0].ptr = s_WorkState.work + s_WorkState.workSize - size;
    if (size > 28)
    {
        std::memmove(s_WorkBlocks[0].ptr, static_cast<u8*>(s_WorkBlocks[0].ptr) + 28, size - 28);
    }
    return s_WorkBlocks[0].ptr;
}

// 0x00161380
void Free(void* ptr)
{
    for (u32 i = 1; i < s_WorkState.blockNum; i++)
    {
        if (s_WorkBlocks[i].ptr == ptr)
        {
            s_WorkBlocks[i].isUsed = 0;
            return;
        }
    }
}

// 0x001613D8
void* Allocate(u32 size)
{
    if (s_WorkState.work == NULL)
    {
        return NULL;
    }

    size = (size + 31) & ~31;
    // a free block that is large enough (block 0 belongs to AllocateTop)
    for (u32 i = 1; i < s_WorkState.blockNum; i++)
    {
        if (!s_WorkBlocks[i].isUsed && s_WorkBlocks[i].size >= size)
        {
            s_WorkBlocks[i].isUsed = 1;
            return s_WorkBlocks[i].ptr;
        }
    }

    if (size > s_WorkState.workSize - s_WorkState.workUsed || s_WorkState.blockNum >= WORK_BLOCK_NUM)
    {
        return NULL;
    }
    WorkBlock* block = &s_WorkBlocks[s_WorkState.blockNum];
    block->isUsed = 1;
    block->size = size;
    block->ptr = s_WorkState.work + s_WorkState.workUsed;
    s_WorkState.workUsed += size;
    s_WorkState.blockNum++;
    return block->ptr;
}

} // namespace detail
} // namespace qrd
} // namespace mw
