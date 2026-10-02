#pragma once

#include "decomp.h"
#include "Bank/dBank.h"

// RTTI 23HumanResTextureAnimBank @ 0x008CCF14
// vtable 0x008F723C (vptr 0x008F7244), offset_to_top 0, 3 entries
class HumanResTextureAnimBank : public ::Bank<3072u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >
{
public:
    HumanResTextureAnimBank(); // ctor candidate(s) 0x00335400 (unverified)
    virtual void vf_0x00(); // 0x003354C4 slot 0x00 | virtual slot, introduced by Bank<3072u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >
    virtual void vf_0x04(); // 0x0033543C slot 0x04 | virtual slot, introduced by Bank<3072u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >
};
