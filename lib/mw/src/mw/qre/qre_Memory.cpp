#include <cstring>

#include "mw/qre/qre_Internal.h"

namespace mw {
namespace qre {
namespace detail {

// 0x0095F5C4
WorkState s_WorkState;
// 0x00ADA900
WorkBlock s_WorkBlocks[WORK_BLOCK_NUM];

namespace {

// 0x0097F9A8
u8 s_Error;

} // namespace

// 0x001596E8
void ResetMemory()
{
    s_WorkState.workUsed = 0;
    s_WorkState.blockNum = 0;
    std::memset(s_WorkBlocks, 0, sizeof(s_WorkBlocks));
}

// 0x0015970C
void ResetError()
{
    s_Error = ERROR_NONE;
}

// 0x0015FC30
u8 GetError()
{
    return s_Error;
}

// keeps the first error unless forced
// 0x001603E4
void SetError(u8 error, bool force)
{
    if (force || s_Error == ERROR_NONE)
    {
        s_Error = error;
    }
}

// 0x001611C8
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

// 0x00161220
void* Allocate(u32 size)
{
    if (s_WorkState.work == NULL)
    {
        return NULL;
    }

    size = (size + 31) & ~31;
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

// (copies size bytes from the old block)
// 0x001612F4
void* Reallocate(void* ptr, u32 size)
{
    if (ptr == NULL)
    {
        return Allocate(size);
    }
    void* result = Allocate(size);
    std::memcpy(result, ptr, size);
    Free(ptr);
    return result;
}

// 0x00164B18
bool GetTotalCodewordNum(s32 version, s32* num)
{
    if (version > 40)
    {
        return false;
    }
    *num = s_VersionInfos[version - 1].totalCodewordNum;
    return true;
}

// 0x00164B44
bool GetAlignmentPositions(s32 version, s32* num, s32* x, s32* y)
{
    if (version > 40)
    {
        return false;
    }
    const VersionInfo& info = s_VersionInfos[version - 1];
    *num = info.alignmentNum;
    for (s32 i = 0; i < 46; i++)
    {
        x[i] = info.alignmentX[i];
        y[i] = info.alignmentY[i];
    }
    return true;
}

// 0x00164B9C
bool GetErrorCorrectionInfo(s32 version, u8 level, ErrorCorrectionInfo* info)
{
    if (version > 40)
    {
        return false;
    }
    *info = s_VersionInfos[version - 1].levels[level];
    return true;
}

// 0x00164BDC
bool GetSymbolSize(s32 version, s32* size)
{
    if (version > 40)
    {
        return false;
    }
    *size = s_VersionInfos[version - 1].size;
    return true;
}

} // namespace detail
} // namespace qre
} // namespace mw
