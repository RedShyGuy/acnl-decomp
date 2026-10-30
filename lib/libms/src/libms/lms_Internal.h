#pragma once

// helpers that ARMCC inlined into the LMS functions (names are ours)

#include "libms/lms_Api.h"

// the label hash table of LBL1 and FEN1: u32 bucketNum, {u32 labelNum, u32 offset}[bucketNum], then per label
// u8 length, the characters (no terminator) and the u32 value
inline s32 LMSi_FindLabel(const LMS_BinaryBlock* block, const char* label, s32 length)
{
    u32 bucket = LMSi_GetHashTableIndex(label, *reinterpret_cast<const u32*>(block->data));
    u32 labelNum = *reinterpret_cast<const u32*>(block->data + 4 + bucket * 8);
    u32 offset = *reinterpret_cast<const u32*>(block->data + 8 + bucket * 8);
    for (u32 i = 0; i < labelNum; i++)
    {
        u8 size = block->data[offset];
        if (size == length && LMSi_MemCmp(label, reinterpret_cast<const char*>(block->data + offset + 1), size))
        {
            return *reinterpret_cast<const s32*>(block->data + offset + size + 1);
        }
        offset += size + 5;
    }
    return LMS_ERROR_NOT_FOUND;
}

inline s32 LMSi_GetLength(const char* text)
{
    s32 length = 0;
    while (text[length] != '\0')
    {
        length++;
    }
    return length;
}
