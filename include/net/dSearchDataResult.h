#pragma once

#include "decomp.h"

#include <cstring>

namespace net {
// RTTI N3net16SearchDataResultE @ 0x008D0FD0
// vtable 0x00904684 (vptr 0x0090468C), offset_to_top 0, 6 entries
//
// An object found by a DataStore search (its id, owner and data type and a name). The game derives
// its own result types from it (all names are ours).
class SearchDataResult
{
public:
    static const u32 NAME_LENGTH = 18;

    SearchDataResult(); // 0x0050D168
    virtual ~SearchDataResult(); // 0x0050D1B0 slot 0x00
    // the buffer for the meta binary of the object (NULL: not wanted) and its size
    virtual void* GetMetaBinaryBuffer(); // 0x0050D108 slot 0x08
    virtual u32 GetMetaBinarySize(); // 0x00747AFC slot 0x0C
    // called after the result was cleared
    virtual void OnClear() {} // 0x0050D104 slot 0x10
    // whether the result takes the found object (before it is set)
    virtual bool OnSet(u32 ownerId, u64 dataId, u16 dataType) { return true; } // 0x00747AF4 slot 0x14

    void Clear()
    {
        m_DataId = 0;
        m_OwnerId = 0;
        m_DataType = 0xFFFF;
        memset(m_Name, 0, sizeof(char16) * NAME_LENGTH);
    }
    void Set(u32 ownerId, u64 dataId, u16 dataType)
    {
        m_DataId = dataId;
        m_OwnerId = ownerId;
        m_DataType = dataType;
    }
    // copies length characters of the name
    void SetName(const char16* name, u32 length); // 0x0050D110

    u32 unk_0x4;
    u64 m_DataId;           // 0x08
    u32 m_OwnerId;          // 0x10
    u16 m_DataType;         // 0x14 (0xFFFF: none)
    char16 m_Name[NAME_LENGTH + 1]; // 0x16 (aligned to 0x40 by the u64)
};
ASSERT_SIZE(SearchDataResult, 0x40);

// The results of a search: up to 100 registered result objects, of which the first m_UsedNum are
// set (names are ours).
class SearchDataResultList
{
public:
    static const u32 MAX_NUM = 100;

    SearchDataResultList(); // 0x0050D2B4
    // clears all used results
    void Reset(); // 0x0050D1B4
    void Register(SearchDataResult* result); // 0x0050D224
    // sets the next unused result; NULL if there is none or it does not take the object
    SearchDataResult* Add(u32 ownerId, u64 dataId, u16 dataType); // 0x0050D244
    DECOMP_EMIT_COPY SearchDataResult* Get(u32 index) const // 0x00747B04
    {
        return index < m_Num ? m_pResults[index] : NULL;
    }

    SearchDataResult* m_pResults[MAX_NUM]; // 0x000
    u32 m_Num;                             // 0x190
    u32 m_UsedNum;                         // 0x194
};
ASSERT_SIZE(SearchDataResultList, 0x198);

// The conditions of a DataStore search that the game fills in (names are ours).
struct SearchDataParam
{
    SearchDataParam(); // 0x0050D0C4
    DECOMP_EMIT_COPY void Clear() // 0x0050D02C
    {
        unk_0x0 = 0;
        unk_0x4 = 0;
        unk_0x8 = 0;
        unk_0xC = 0;
        unk_0x10 = 0;
        unk_0x14 = 0;
        unk_0x18 = 0;
        unk_0x1C = 0;
        unk_0x20 = 0;
        unk_0x26 = 0xFFFF;
        unk_0x28 = 0;
        unk_0x2C = 0;
        unk_0x30 = 0;
    }

    u32 unk_0x0;
    u32 unk_0x4;
    u32 unk_0x8;
    u32 unk_0xC;
    u32 unk_0x10;
    u32 unk_0x14;
    u32 unk_0x18;
    u32 unk_0x1C;
    u32 unk_0x20;
    u16 unk_0x24;
    u16 unk_0x26; // 0xFFFF: none
    u32 unk_0x28;
    u32 unk_0x2C;
    u32 unk_0x30;
};
ASSERT_SIZE(SearchDataParam, 0x34);

// copies length characters of a word into a buffer of SearchDataResult::NAME_LENGTH + 1 characters
// (name is ours)
void CopyName(char16* buffer, const char16* word, u32 length); // 0x0050D06C
} // namespace net
