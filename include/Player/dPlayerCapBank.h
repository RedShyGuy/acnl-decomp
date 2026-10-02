#pragma once

#include "decomp.h"
#include "Exchange/dExchangeModelBank.h"
#include "item/dResLoader.h"

// RTTI 13PlayerCapBank @ 0x008CBA78
// vtable 0x008EF6CC (vptr 0x008EF6D4), offset_to_top 0, 11 entries
// vtable 0x008EF700 (vptr 0x008EF708), offset_to_top -1212, 3 entries
class PlayerCapBank : public ::ExchangeModelBank<31423u, BankVramConfig<2500u, 3u, 12400u, 3u, false>, PlayerCapBankKeyword, item::ResLoader, 2u, true>
{
public:
    PlayerCapBank(); // ctor candidate(s) 0x0023D7F0 (unverified)
    virtual ~PlayerCapBank(); // 0x0023DB40 slot 0x00 | slot vf_0x00 of ExchangeBank<32191u, BankVramConfig<2500u, 3u, 12400u, 3u, false>, PlayerCapBankKeyword, true>
    // 0x0023DAD4 slot 0x04 | slot vf_0x04 of ExchangeBank<32191u, BankVramConfig<2500u, 3u, 12400u, 3u, false>, PlayerCapBankKeyword, true> (deleting dtor)
    virtual void vf_0x10(); // 0x0023D1C8 slot 0x10 | virtual slot, introduced by ExchangeBank<32191u, BankVramConfig<2500u, 3u, 12400u, 3u, false>, PlayerCapBankKeyword, true>
    virtual void vf_0x14(); // 0x0023D60C slot 0x14 | virtual slot, introduced by ExchangeBank<32191u, BankVramConfig<2500u, 3u, 12400u, 3u, false>, PlayerCapBankKeyword, true>
    virtual void vf_0x18(); // 0x0023D370 slot 0x18 | virtual slot, introduced by ExchangeBank<32191u, BankVramConfig<2500u, 3u, 12400u, 3u, false>, PlayerCapBankKeyword, true>
    virtual void vf_0x1C(); // 0x0023D5B0 slot 0x1C | virtual slot, introduced by ExchangeBank<32191u, BankVramConfig<2500u, 3u, 12400u, 3u, false>, PlayerCapBankKeyword, true>
};
