#pragma once

#include "decomp.h"
#include "Exchange/dExchangeModelBank.h"
#include "Item/dResLoader.h"

struct SvPlayerAppearance;
#include "Item/item_Api.h"

// The bank of the legs of the player (class name from the RTTI; the member names are ours): the common legs model
// (s_ModelPath, loader 0) and, for a socks item, its item file (loader 1).
// RTTI 14PlayerLegsBank @ 0x008CBD5C
// vtable 0x008F0678 (vptr 0x008F0680), offset_to_top 0, 11 entries
// vtable 0x008F06AC (vptr 0x008F06B4), offset_to_top -1200, 3 entries
class PlayerLegsBank
    : public ::ExchangeModelBank<17879u, BankVramConfig<700u, 2u, 3600u, 2u, false>,
                                 ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)0x7FFD>, item::ResLoader, 2u, true>
{
public:
    typedef ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)0x7FFD> Keyword;

    PlayerLegsBank(); // 0x00272ED0
    virtual ~PlayerLegsBank(); // 0x00273184 slot 0x00

    virtual bool LoadResource(const Keyword& keyword, u32 slot) override; // 0x00272980 slot 0x10
    virtual s32 LoadResourceSplit(const Keyword& keyword, u32 slot) override; // 0x00272D98 slot 0x14
    virtual bool Initialize(const Keyword& keyword, u32 slot, bool isReady) override; // 0x00272A7C slot 0x18
    virtual bool Finalize(const Keyword& keyword, u32 slot) override; // 0x00272D3C slot 0x1C

    // the shown model goes into the model list of the object 0x00951378 (at +0x12CA0)
    void RequestDraw(u32 sceneIdx); // (sceneIdx: not read, PlayerModel passes 0) 0x00272868
    // the colors of the shown model: the two bytes of hair and eye color of m_pAppearance and colors (false: no appearance or no model)
    bool SetColors(const ExchangeBankKeywordUnknown3* colors); // 0x002728BC

    static const char* s_ModelPath;

    const SvPlayerAppearance* m_pAppearance; // 0x4D0 of the player (PlayerModel::Setup)
};
ASSERT_SIZE(PlayerLegsBank, 0x4D4);
