#pragma once

#include "decomp.h"
#include "Sv/dSvFgName.h"

// RTTI 14PlayerTopsBank @ 0x008CBD70
// vtable 0x008F06C0 (vptr 0x008F06C8), offset_to_top 0, 11 entries
// vtable 0x008F06F4 (vptr 0x008F06FC), offset_to_top -1200, 3 entries
class PlayerTopsBank
{
public:
    PlayerTopsBank(); // ctor candidate(s) 0x00273B58 (unverified)
    virtual ~PlayerTopsBank(); // 0x00273E0C slot 0x00 | slot vf_0x00 of ExchangeBank<33659u, BankVramConfig<2800u, 2u, 8800u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true>
    // 0x00273DA0 slot 0x04 | slot vf_0x04 of ExchangeBank<33659u, BankVramConfig<2800u, 2u, 8800u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true> (deleting dtor)
    virtual void vf_0x10(); // 0x00273304 slot 0x10 | virtual slot, introduced by ExchangeBank<33659u, BankVramConfig<2800u, 2u, 8800u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true>
    virtual void vf_0x14(); // 0x00273894 slot 0x14 | virtual slot, introduced by ExchangeBank<33659u, BankVramConfig<2800u, 2u, 8800u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true>
    virtual void vf_0x18(); // 0x002735D0 slot 0x18 | virtual slot, introduced by ExchangeBank<33659u, BankVramConfig<2800u, 2u, 8800u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true>
    virtual void vf_0x1C(); // 0x00273838 slot 0x1C | virtual slot, introduced by ExchangeBank<33659u, BankVramConfig<2800u, 2u, 8800u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true>
};
