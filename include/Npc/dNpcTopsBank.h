#pragma once

#include "decomp.h"
#include "Sv/dSvFgName.h"

// RTTI 11NpcTopsBank @ 0x008CB2FC
// vtable 0x008ECC44 (vptr 0x008ECC4C), offset_to_top 0, 8 entries
class NpcTopsBank
{
public:
    NpcTopsBank(); // ctor candidate(s) 0x001CE180 (unverified)
    virtual ~NpcTopsBank(); // 0x001CE2F0 slot 0x00 | slot vf_0x00 of ExchangeBank<8984u, BankVramConfig<0u, 0u, 0u, 0u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, false>
    // 0x001CE2B8 slot 0x04 | slot vf_0x04 of ExchangeBank<8984u, BankVramConfig<0u, 0u, 0u, 0u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, false> (deleting dtor)
    virtual void vf_0x10(); // 0x001CDF58 slot 0x10 | virtual slot, introduced by ExchangeBank<8984u, BankVramConfig<0u, 0u, 0u, 0u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, false>
    virtual void vf_0x14(); // 0x001CE11C slot 0x14 | virtual slot, introduced by ExchangeBank<8984u, BankVramConfig<0u, 0u, 0u, 0u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, false>
    virtual void vf_0x18(); // 0x001CDFBC slot 0x18 | virtual slot, introduced by ExchangeBank<8984u, BankVramConfig<0u, 0u, 0u, 0u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, false>
    virtual void vf_0x1C(); // 0x001CE0EC slot 0x1C | virtual slot, introduced by ExchangeBank<8984u, BankVramConfig<0u, 0u, 0u, 0u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, false>
};
