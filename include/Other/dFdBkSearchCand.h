#pragma once

#include "decomp.h"
#include "Search/dSearchCandXZCore.h"

// the blocks of a field as candidates (every block is one; names are ours)
// RTTI 14FdBkSearchCand @ 0x008CBCCC
// vtable 0x008F036C (vptr 0x008F0374), offset_to_top 0, 2 entries
class FdBkSearchCand : public ::SearchCandXZCore
{
public:
    // (inline)
    FdBkSearchCand(s32 width, s32 height) : SearchCandXZCore(width, height, m_Bits) {}
    bool IsCandidateXZ(s32 x, s32 z) override; // 0x002BCEEC slot 0x04

    u32 m_Bits[4]; // 0x1C
};
ASSERT_SIZE(FdBkSearchCand, 0x2C);
