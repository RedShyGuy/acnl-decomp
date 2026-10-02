#pragma once

#include "decomp.h"
#include "Search/dSearchCandXZCore.h"

// RTTI 14FdBkSearchCand @ 0x008CBCCC
// vtable 0x008F036C (vptr 0x008F0374), offset_to_top 0, 2 entries
class FdBkSearchCand : public ::SearchCandXZCore
{
public:
    FdBkSearchCand(); // ctor candidate(s) 0x00226800, 0x0027D714, 0x00306E58, 0x00698D44 (unverified)
    virtual void vf_0x00(); // 0x002BCEC0 slot 0x00 | virtual slot, introduced by SearchCandCore
    virtual void vf_0x04(); // 0x002BCEEC slot 0x04 | virtual slot, introduced by FdBkSearchCand
};
