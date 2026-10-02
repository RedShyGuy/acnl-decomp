#pragma once

#include "decomp.h"
#include "Exchange/dExchangeModelBank.h"
#include "g3d/dResourceLoader.h"

// RTTI 14PlayerHeadBank @ 0x008CBD58
// vtable 0x008F0630 (vptr 0x008F0638), offset_to_top 0, 11 entries
// vtable 0x008F0664 (vptr 0x008F066C), offset_to_top -636, 3 entries
class PlayerHeadBank : public ::ExchangeModelBank<25891u, BankVramConfig<1800u, 4u, 6600u, 4u, false>, PlayerHeadBankKeyword, g3d::ResourceLoader, 1u, false>
{
public:
    PlayerHeadBank(); // ctor candidate(s) 0x00272404 (unverified)
    virtual ~PlayerHeadBank(); // 0x00272798 slot 0x00 | slot vf_0x00 of ExchangeBank<26275u, BankVramConfig<1800u, 4u, 6600u, 4u, false>, PlayerHeadBankKeyword, false>
    // 0x002726C4 slot 0x04 | slot vf_0x04 of ExchangeBank<26275u, BankVramConfig<1800u, 4u, 6600u, 4u, false>, PlayerHeadBankKeyword, false> (deleting dtor)
    virtual void vf_0x10(); // 0x002718F8 slot 0x10 | virtual slot, introduced by ExchangeBank<26275u, BankVramConfig<1800u, 4u, 6600u, 4u, false>, PlayerHeadBankKeyword, false>
    virtual void vf_0x14(); // 0x00271D84 slot 0x14 | virtual slot, introduced by ExchangeBank<26275u, BankVramConfig<1800u, 4u, 6600u, 4u, false>, PlayerHeadBankKeyword, false>
    virtual void vf_0x18(); // 0x002719F4 slot 0x18 | virtual slot, introduced by ExchangeBank<26275u, BankVramConfig<1800u, 4u, 6600u, 4u, false>, PlayerHeadBankKeyword, false>
    virtual void vf_0x1C(); // 0x00271D04 slot 0x1C | virtual slot, introduced by ExchangeBank<26275u, BankVramConfig<1800u, 4u, 6600u, 4u, false>, PlayerHeadBankKeyword, false>
};
