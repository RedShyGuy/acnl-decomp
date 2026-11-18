#pragma once

#include "decomp.h"
#include "Exchange/dExchangeResBank.h"
#include "Item/dResLoader.h"
#include "Item/item_Api.h"

// The bank of the textures of the record jackets (class name from the RTTI; the function and member names are ours): the
// item file of the song per slot; My Design textures only when m_IsMyDesignAllowed is set.
// RTTI 22MusicJacketTextureBank @ 0x008CCE18
// vtable 0x008F6BBC (vptr 0x008F6BC4), offset_to_top 0, 8 entries
class MusicJacketTextureBank
    : public ::ExchangeResBank<16768u, BankVramConfig<0u, 0u, 0u, 0u, false>,
                               ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)0x7FFD>, item::ResLoader, 1u, false>
{
public:
    typedef ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)0x7FFD> Keyword;

    MusicJacketTextureBank(); // 0x00331874
    virtual ~MusicJacketTextureBank(); // 0x00331AB0 slot 0x00

    virtual bool LoadResource(const Keyword& keyword, u32 slot) override; // 0x00331130 slot 0x10
    virtual s32 LoadResourceSplit(const Keyword& keyword, u32 slot) override; // 0x0033161C slot 0x14
    virtual bool Finalize(const Keyword& keyword, u32 slot) override; // 0x003311FC slot 0x1C

    // the texture of item is shown (ExchangeBank::Exchange: 0 loading, 1 nothing new, 2 shown)
    s32 Request(const SvFgName& item); // 0x0033122C
    // the slots are released, the request is cleared and the heaps are destroyed (always true)
    bool ReleaseAll(); // 0x0033171C

private:
    // (inline) whether the bank loads item
    bool CanLoad(const SvFgName& item) const
    {
        return item.GetKind() == item::KIND_MUSIC || (m_IsMyDesignAllowed && Func76BBF0(&item));
    }

public:
    u8 m_Unknown0 : 1;           // 0x288 (cleared by ReleaseAll)
    u8 m_Unknown1 : 1;           // (cleared by ReleaseAll)
    u8 m_IsMyDesignAllowed : 1;
};
ASSERT_SIZE(MusicJacketTextureBank, 0x28C);
