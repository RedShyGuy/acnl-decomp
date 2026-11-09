#pragma once

#include "decomp.h"

// RTTI 14SearchCandCore @ 0x008CBD94
// vtable 0x008F0730 (vptr 0x008F0738), offset_to_top 0, 1 entries
//
// A set of candidates 0..num-1 as a bit field: candidates are removed and one of the rest is picked at random. The
// derived classes hold the bits (SearchCand<N>). Member and function names are ours.
class SearchCandCore
{
public:
    SearchCandCore(); // (placeholder for the derived classes that are not decompiled yet)
    SearchCandCore(s32 num, u32* bits); // 0x00306FF8
    // whether the candidate may be picked (the derived classes filter)
    virtual bool IsCandidate(s32 index); // 0x00276058 slot 0x00

    // all candidates again
    void Reset(); // 0x00306EA8
    // a candidate is removed
    void Remove(s32 index); // 0x003071A8
    // a random one of the remaining candidates (none: -1)
    s32 Pick(); // 0x00306F8C

    s32 m_Num;        // 0x04
    s32 m_WordNum;    // 0x08 the words of the bits
    u32* m_pBits;     // 0x0C
    s32 m_RestNum;    // 0x10 the remaining candidates
};
ASSERT_SIZE(SearchCandCore, 0x14);
