#pragma once

#include "decomp.h"
#include "Search/dSearchCandXZ.h"

// the 16 x 16 units of a block as candidates, tested by a callback (names are ours)
// RTTI 18BkUnitSearchCandCb @ 0x008CC71C
// vtable 0x008F3E98 (vptr 0x008F3EA0), offset_to_top 0, 2 entries
class BkUnitSearchCandCb : public ::SearchCandXZ<16, 16>
{
public:
    // the unit unitX, unitZ of the block blockX, blockZ may be picked
    typedef bool (*Callback)(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, u32 arg);

    // (inline)
    BkUnitSearchCandCb(s32 blockX, s32 blockZ, u32 arg, Callback callback)
        : m_BlockX(blockX), m_BlockZ(blockZ), m_Arg(arg), m_pCallback(callback)
    {
    }
    bool IsCandidateXZ(s32 x, s32 z) override; // 0x002D6BB8 slot 0x04

    s32 m_BlockX;          // 0x3C
    s32 m_BlockZ;          // 0x40
    u32 m_Arg;             // 0x44
    Callback m_pCallback;  // 0x48
};
ASSERT_SIZE(BkUnitSearchCandCb, 0x4C);
