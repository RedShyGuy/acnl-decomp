#pragma once

#include "decomp.h"
#include "Bank/dBankTablePrivate.h"

// RTTI 13BankTableBase @ 0x008CB894
// vtable 0x008EEC74 (vptr 0x008EEC7C), offset_to_top 0, 6 entries
class BankTableBase : public ::BankTablePrivate
{
public:
    BankTableBase(); // ctor candidate(s) 0x00213A5C (unverified)
    virtual void vf_0x00(); // 0x00213868 slot 0x00 | virtual slot, introduced by BankTableBase
    virtual void vf_0x04(); // 0x00713F80 slot 0x04 | virtual slot, introduced by BankTableBase
    virtual void vf_0x08(); // 0x00713F78 slot 0x08 | virtual slot, introduced by BankTableBase
    virtual void vf_0x0C(); // 0x00213A58 slot 0x0C | virtual slot, introduced by BankTableBase
    virtual void vf_0x10(); // 0x0021384C slot 0x10 | virtual slot, introduced by BankTableBase
    virtual void vf_0x14(); // 0x00213850 slot 0x14 | virtual slot, introduced by BankTableBase
};
