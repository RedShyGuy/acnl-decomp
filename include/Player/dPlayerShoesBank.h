#pragma once

#include "decomp.h"
#include "Exchange/dExchangeModelBank.h"
#include "Item/dResLoader.h"
#include "Sv/dSvFgName.h"

// The bank of the shoes of the player (class name from the RTTI): per slot the model file of the item (loader 0) and
// the item file (loader 1).
// RTTI 15PlayerShoesBank @ 0x008CC11C
// vtable 0x008F1ECC (vptr 0x008F1ED4), offset_to_top 0, 11 entries
// vtable 0x008F1F00 (vptr 0x008F1F08), offset_to_top -1160, 3 entries
class PlayerShoesBank : public ::ExchangeModelBank<17703u, BankVramConfig<600u, 2u, 3600u, 2u, false>, ExchangeBankKeywordFgNameEx<SvFgName::NAME_EMPTY>, item::ResLoader, 2u, true>
{
public:
    typedef ExchangeBankKeywordFgNameEx<SvFgName::NAME_EMPTY> Keyword;

    PlayerShoesBank(); // 0x002A256C
    virtual ~PlayerShoesBank(); // 0x002A2758 slot 0x00
    virtual bool LoadResource(const Keyword& keyword, u32 slot) override; // 0x002A214C slot 0x10
    virtual s32 LoadResourceSplit(const Keyword& keyword, u32 slot) override; // 0x002A2470 slot 0x14
    virtual bool Initialize(const Keyword& keyword, u32 slot, bool isReady) override; // 0x002A2220 slot 0x18
    virtual bool Finalize(const Keyword& keyword, u32 slot) override; // 0x002A2410 slot 0x1C
    // the shown model is drawn this frame (BsPlayerMgr::MODEL_LIST_SHOES)
    void RequestDraw(); // 0x002A20F8
};
ASSERT_SIZE(PlayerShoesBank, 0x4A8);
