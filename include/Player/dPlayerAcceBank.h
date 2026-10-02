#pragma once

#include "decomp.h"
#include "Sv/dSvFgName.h"

// RTTI 14PlayerAcceBank @ 0x008CBD40
// vtable 0x008F05D4 (vptr 0x008F05DC), offset_to_top 0, 11 entries
// vtable 0x008F0608 (vptr 0x008F0610), offset_to_top -608, 3 entries
class PlayerAcceBank
{
public:
    PlayerAcceBank(); // ctor candidate(s) 0x00271054 (unverified)
    virtual ~PlayerAcceBank(); // 0x00271238 slot 0x00 | slot vf_0x00 of ExchangeBank<14156u, BankVramConfig<300u, 2u, 2000u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, true>
    // 0x002711D0 slot 0x04 | slot vf_0x04 of ExchangeBank<14156u, BankVramConfig<300u, 2u, 2000u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, true> (deleting dtor)
    virtual void vf_0x10(); // 0x00270D30 slot 0x10 | virtual slot, introduced by ExchangeBank<14156u, BankVramConfig<300u, 2u, 2000u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, true>
    virtual void vf_0x14(); // 0x00270F90 slot 0x14 | virtual slot, introduced by ExchangeBank<14156u, BankVramConfig<300u, 2u, 2000u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, true>
    virtual void vf_0x18(); // 0x00270DC4 slot 0x18 | virtual slot, introduced by ExchangeBank<14156u, BankVramConfig<300u, 2u, 2000u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, true>
    virtual void vf_0x1C(); // 0x00270F48 slot 0x1C | virtual slot, introduced by ExchangeBank<14156u, BankVramConfig<300u, 2u, 2000u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, true>
};
