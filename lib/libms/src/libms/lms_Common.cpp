#include "libms/lms_Api.h"

namespace {

struct MemFuncs
{
    LMS_AllocFunc alloc;  // +0x00
    LMS_FreeFunc free;    // +0x04
};

// 0x0095F0E0
MemFuncs s_MemFuncs;

} // namespace

extern "C" {

// 0x00135F50 (name is ours)
void LMS_SetMemFuncs(LMS_AllocFunc alloc, LMS_FreeFunc free)
{
    s_MemFuncs.alloc = alloc;
    s_MemFuncs.free = free;
}

// 0x0015A3B4 (name is ours)
DECOMP_NOINLINE void* LMSi_Allocate(u32 size)
{
    return s_MemFuncs.alloc(size);
}

// 0x0013DE04 (name is ours)
void LMSi_Free(void* ptr)
{
    s_MemFuncs.free(ptr);
}

// the file header (0x20 bytes) and the block headers (0x10 bytes each, blocks aligned to 16 bytes)
// 0x0015A204 (name is ours)
void LMSi_AnalyzeMessageHeader(LMS_Binary* binary, const char* magic)
{
    s32 pos;
    // (the magic is not checked: the loop over it has no body)
    for (pos = 0; pos < 8; pos++)
    {
    }
    pos += 4; // byte order mark and two bytes
    binary->encoding = binary->data[pos];
    pos += 2; // and the version
    binary->blockNum = *reinterpret_cast<const u16*>(binary->data + pos);
    pos += 2;
    binary->blocks = binary->blockNum != 0
                         ? static_cast<LMS_BinaryBlock*>(LMSi_Allocate(binary->blockNum * sizeof(LMS_BinaryBlock)))
                         : NULL;
    pos += 2;
    binary->fileSize = *reinterpret_cast<const u32*>(binary->data + pos);

    pos = 0x20;
    for (s32 i = 0; i < binary->blockNum; i++)
    {
        s32 data = pos + 0x10;
        LMS_BinaryBlock* block = &binary->blocks[i];
        block->data = binary->data + data;
        block->tag[0] = binary->data[pos++];
        block->tag[1] = binary->data[pos++];
        block->tag[2] = binary->data[pos++];
        block->tag[3] = binary->data[pos++];
        u32 size = *reinterpret_cast<const u32*>(binary->data + pos);
        block->size = size;
        pos += 4;
        block->_0C = *reinterpret_cast<const u16*>(binary->data + pos);
        pos = (size + data + 15) & ~15;
    }
}

// (armlink replaced the tail call by a nop: the function falls into LMSi_GetBlock)
// 0x0015A308 (name is ours)
LMS_BinaryBlock* LMS_GetBlock(LMS_Binary* binary, const char* tag)
{
    return LMSi_GetBlock(binary, tag);
}

// 0x0015A30C (name is ours)
DECOMP_NOINLINE LMS_BinaryBlock* LMSi_GetBlock(LMS_Binary* binary, const char* tag)
{
    for (s32 i = 0; i < binary->blockNum; i++)
    {
        if (LMSi_MemCmp(binary->blocks[i].tag, tag, 4))
        {
            return &binary->blocks[i];
        }
    }
    return NULL;
}

// 0x0015A498 (name is ours)
s32 LMSi_SearchBlockByName(LMS_Binary* binary, const char* tag)
{
    for (u16 i = 0; i < binary->blockNum; i++)
    {
        if (LMSi_MemCmp(binary->blocks[i].tag, tag, 4))
        {
            return i;
        }
    }
    return -1;
}

// the bucket of a label in the hash tables LBL1 and FEN1
// 0x0015A374 (name is ours)
u32 LMSi_GetHashTableIndex(const char* label, u32 bucketNum)
{
    u32 hash = 0;
    for (s32 i = 0; static_cast<s8>(label[i]) != '\0'; i++)
    {
        hash = hash * 0x492 + static_cast<s8>(label[i]);
    }
    return hash % bucketNum;
}

// 1 if the first size bytes are equal
// 0x0015A3C4 (name is ours)
DECOMP_NOINLINE s32 LMSi_MemCmp(const char* a, const char* b, s32 size)
{
    for (s32 i = 0; i < size; i++)
    {
        if (static_cast<s8>(a[i]) != static_cast<s8>(b[i]))
        {
            return 0;
        }
    }
    return 1;
}

// 0x0015A454 (name is ours)
void LMSi_MemCopy(char* dst, const char* src, s32 size)
{
    for (s32 i = 0; i < size; i++)
    {
        dst[i] = src[i];
    }
}

} // extern "C"
