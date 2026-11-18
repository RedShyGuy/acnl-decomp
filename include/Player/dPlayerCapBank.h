#pragma once

#include "decomp.h"
#include "Exchange/dExchangeModelBank.h"
#include "Item/dResLoader.h"

// The bank of the caps of the player (class name from the RTTI; the member name is ours): per slot the item file (loader
// 0) and, for items that carry a My Design, the design (loader 1, with the data of the keyword).
// RTTI 13PlayerCapBank @ 0x008CBA78
// vtable 0x008EF6CC (vptr 0x008EF6D4), offset_to_top 0, 11 entries
// vtable 0x008EF700 (vptr 0x008EF708), offset_to_top -1212, 3 entries
class PlayerCapBank : public ::ExchangeModelBank<31423u, BankVramConfig<2500u, 3u, 12400u, 3u, false>, PlayerCapBankKeyword,
                                                 item::ResLoader, 2u, true>
{
public:
    typedef PlayerCapBankKeyword Keyword;

    PlayerCapBank(); // 0x0023D7F0
    virtual ~PlayerCapBank(); // 0x0023DB40 slot 0x00

    virtual bool LoadResource(const Keyword& keyword, u32 slot) override; // 0x0023D1C8 slot 0x10
    virtual s32 LoadResourceSplit(const Keyword& keyword, u32 slot) override; // 0x0023D60C slot 0x14
    virtual bool Initialize(const Keyword& keyword, u32 slot, bool isReady) override; // 0x0023D370 slot 0x18
    virtual bool Finalize(const Keyword& keyword, u32 slot) override; // 0x0023D5B0 slot 0x1C
    // the shown model is drawn this frame (BsPlayerMgr::MODEL_LIST_CAP)
    void RequestDraw(); // 0x0023D0B4

    const void* m_pUnknown4DC; // 0x4DC (read by Initialize with Func713778)
};
ASSERT_SIZE(PlayerCapBank, 0x4E0);
