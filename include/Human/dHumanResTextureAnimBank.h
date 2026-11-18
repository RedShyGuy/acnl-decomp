#pragma once

#include "decomp.h"
#include "Bank/dBank.h"

// A bank for the texture animations of the human models (class name from the RTTI; nothing beyond the bank).
// RTTI 23HumanResTextureAnimBank @ 0x008CCF14
// vtable 0x008F723C (vptr 0x008F7244), offset_to_top 0, 3 entries
class HumanResTextureAnimBank : public ::Bank<3072u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >
{
public:
    HumanResTextureAnimBank(); // 0x003353FC
    virtual ~HumanResTextureAnimBank(); // 0x003354C4 slot 0x00
};
ASSERT_SIZE(HumanResTextureAnimBank, 0x14);
