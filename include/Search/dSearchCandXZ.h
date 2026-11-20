#pragma once

#include "decomp.h"
#include "Search/dSearchCandXZCore.h"

// a rectangle W x H of candidates with its bits (every place is one; names are ours)
// Instantiations found in the binary:
//   SearchCandXZ<16, 16>  typeinfo 0x008CB7F4  vtable 0x008EE7F8
template <s32 W, s32 H>
class SearchCandXZ : public SearchCandXZCore
{
public:
    // (inline)
    SearchCandXZ() : SearchCandXZCore(W, H, m_Bits) {}
    bool IsCandidateXZ(s32 x, s32 z) override;

    u32 m_Bits[(W * H + 31) / 32]; // 0x1C
};

template <s32 W, s32 H>
bool SearchCandXZ<W, H>::IsCandidateXZ(s32 /* x */, s32 /* z */)
{
    return true;
}
