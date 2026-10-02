#pragma once

#include "decomp.h"
#include "Sv/dSvFgName.h"

// RTTI 14PlayerLegsBank @ 0x008CBD64
// vtable 0x008F0678 (vptr 0x008F0680), offset_to_top 0, 11 entries
// vtable 0x008F06AC (vptr 0x008F06B4), offset_to_top -1200, 3 entries
class PlayerLegsBank
{
public:
    PlayerLegsBank(); // ctor candidate(s) 0x00272ED0 (unverified)
    virtual ~PlayerLegsBank(); // 0x00273184 slot 0x00 | slot vf_0x00 of ExchangeBank<18647u, BankVramConfig<700u, 2u, 3600u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true>
    // 0x00273118 slot 0x04 | slot vf_0x04 of ExchangeBank<18647u, BankVramConfig<700u, 2u, 3600u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true> (deleting dtor)
    virtual void vf_0x10(); // 0x00272980 slot 0x10 | virtual slot, introduced by ExchangeBank<18647u, BankVramConfig<700u, 2u, 3600u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true>
    virtual void vf_0x14(); // 0x00272D98 slot 0x14 | virtual slot, introduced by ExchangeBank<18647u, BankVramConfig<700u, 2u, 3600u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true>
    virtual void vf_0x18(); // 0x00272A7C slot 0x18 | virtual slot, introduced by ExchangeBank<18647u, BankVramConfig<700u, 2u, 3600u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true>
    virtual void vf_0x1C(); // 0x00272D3C slot 0x1C | virtual slot, introduced by ExchangeBank<18647u, BankVramConfig<700u, 2u, 3600u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true>
};
