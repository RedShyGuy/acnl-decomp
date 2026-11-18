#pragma once

#include "decomp.h"
#include "Exchange/dExchangeModelBank.h"
#include "Item/dResLoader.h"

// The bank of the accessories of the player (class name from the RTTI; nothing beyond the model bank): the item file of
// the keyword per slot, one model.
// RTTI 14PlayerAcceBank @ 0x008CBD40
// vtable 0x008F05D4 (vptr 0x008F05DC), offset_to_top 0, 11 entries
// vtable 0x008F0608 (vptr 0x008F0610), offset_to_top -608, 3 entries
class PlayerAcceBank : public ::ExchangeModelBank<13772u, BankVramConfig<300u, 2u, 2000u, 2u, false>,
                                                  ExchangeBankKeywordFgNameEx<SvFgName::NAME_EMPTY>, item::ResLoader, 1u, true>
{
public:
    typedef ExchangeBankKeywordFgNameEx<SvFgName::NAME_EMPTY> Keyword;

    PlayerAcceBank(); // 0x00271054
    virtual ~PlayerAcceBank(); // 0x00271238 slot 0x00

    virtual bool LoadResource(const Keyword& keyword, u32 slot) override; // 0x00270D30 slot 0x10
    virtual s32 LoadResourceSplit(const Keyword& keyword, u32 slot) override; // 0x00270F90 slot 0x14
    virtual bool Initialize(const Keyword& keyword, u32 slot, bool isReady) override; // 0x00270DC4 slot 0x18
    virtual bool Finalize(const Keyword& keyword, u32 slot) override; // 0x00270F48 slot 0x1C
    // the shown model is drawn this frame (BsPlayerMgr::MODEL_LIST_ACCE)
    void RequestDraw(); // 0x00270CDC
};
ASSERT_SIZE(PlayerAcceBank, 0x280);
