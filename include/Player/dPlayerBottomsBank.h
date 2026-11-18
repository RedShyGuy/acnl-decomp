#pragma once

#include "decomp.h"
#include "Exchange/dExchangeModelBank.h"
#include "Item/dResLoader.h"

// The bank of the bottoms of the player (class name from the RTTI; the member name is ours): per slot the model file
// (loader 0) and the item file (loader 1). Items of another type than 4 are replaced by 0x26F6 or 0x26F7 (m_Unknown4A8;
// both IDs are not in the item list).
// RTTI 17PlayerBottomsBank @ 0x008CC528
// vtable 0x008F3BB4 (vptr 0x008F3BBC), offset_to_top 0, 11 entries
// vtable 0x008F3BE8 (vptr 0x008F3BF0), offset_to_top -1160, 3 entries
class PlayerBottomsBank
    : public ::ExchangeModelBank<17829u, BankVramConfig<500u, 2u, 2800u, 2u, false>,
                                 ExchangeBankKeywordFgNameEx<(SvFgName::Name)0x7FFD>, item::ResLoader, 2u, true>
{
public:
    typedef ExchangeBankKeywordFgNameEx<(SvFgName::Name)0x7FFD> Keyword;

    PlayerBottomsBank(); // 0x002D0C98
    virtual ~PlayerBottomsBank(); // 0x002D0E88 slot 0x00

    virtual bool LoadResource(const Keyword& keyword, u32 slot) override; // 0x002D0728 slot 0x10
    virtual s32 LoadResourceSplit(const Keyword& keyword, u32 slot) override; // 0x002D0AD8 slot 0x14
    virtual bool Initialize(const Keyword& keyword, u32 slot, bool isReady) override; // 0x002D0888 slot 0x18
    virtual bool Finalize(const Keyword& keyword, u32 slot) override; // 0x002D0A78 slot 0x1C

private:
    // (inline) the item that is loaded for keyword
    void GetItem(SvFgName* item) const
    {
        if (Func76BB44(item) != 4) {
            if (m_Unknown4A8) {
                *item = SvFgName(static_cast<SvFgName::ID>(0x26F6));
            } else {
                *item = SvFgName(static_cast<SvFgName::ID>(0x26F7));
            }
        }
    }

public:
    bool m_Unknown4A8; // 0x4A8
};
ASSERT_SIZE(PlayerBottomsBank, 0x4AC);
