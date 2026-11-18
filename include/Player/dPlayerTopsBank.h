#pragma once

#include "decomp.h"
#include "Exchange/dExchangeModelBank.h"
#include "Item/dResLoader.h"

// The bank of the tops of the player (class name from the RTTI; the member names are ours): per slot the model file
// (loader 0) and the textures (loader 1): the item file, or for items that carry a My Design the design (with the data of
// the keyword); items of another type than 3 are replaced by 0x2493/0x2494, a failed design by 0x2682.
// RTTI 14PlayerTopsBank @ 0x008CBD68
// vtable 0x008F06C0 (vptr 0x008F06C8), offset_to_top 0, 11 entries
// vtable 0x008F06F4 (vptr 0x008F06FC), offset_to_top -1200, 3 entries
class PlayerTopsBank
    : public ::ExchangeModelBank<32891u, BankVramConfig<2800u, 2u, 8800u, 2u, false>,
                                 ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)0x7FFD>, item::ResLoader, 2u, true>
{
public:
    typedef ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)0x7FFD> Keyword;

    PlayerTopsBank(); // 0x00273B58
    virtual ~PlayerTopsBank(); // 0x00273E0C slot 0x00

    virtual bool LoadResource(const Keyword& keyword, u32 slot) override; // 0x00273304 slot 0x10
    virtual s32 LoadResourceSplit(const Keyword& keyword, u32 slot) override; // 0x00273894 slot 0x14
    virtual bool Initialize(const Keyword& keyword, u32 slot, bool isReady) override; // 0x002735D0 slot 0x18
    virtual bool Finalize(const Keyword& keyword, u32 slot) override; // 0x00273838 slot 0x1C
    // the shown model is drawn this frame (BsPlayerMgr::MODEL_LIST_TOPS)
    void RequestDraw(); // 0x002731EC

private:
    // (inline) the item that is loaded for keyword
    void GetItem(SvFgName* item) const
    {
        if (Func76BB44(item) != 3) {
            if (m_pUnknown4D0 == NULL || static_cast<const u8*>(m_pUnknown4D0)[38] != 0) {
                *item = SvFgName(static_cast<SvFgName::ID>(0x2493));
            } else {
                *item = SvFgName(static_cast<SvFgName::ID>(0x2494));
            }
        }
    }

public:
    const void* m_pUnknown4D0; // 0x4D0 (also read by Initialize with Func713778/Func713788)
};
ASSERT_SIZE(PlayerTopsBank, 0x4D4);
