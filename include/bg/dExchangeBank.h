#pragma once

#include "decomp.h"
#include "Exchange/dExchangeResBank.h"
#include "Item/dResLoader.h"
#include "Item/item_Api.h"

namespace bg {
// The bank of the textures of the room backgrounds (class name from the RTTI; the member name is ours): the item file of
// a wallpaper (KIND_DUMMY_WALL), a carpet (KIND_CARPET) or a My Design texture per slot; m_Alignment is passed to the
// item loader (set by the users).
// Instantiations found in the binary:
//   bg::ExchangeBank<BankVramConfig<66000u, 1u, 0u, 0u, false>, 66000u>  typeinfo 0x008CDB98  vtable 0x008FBBB4
//   bg::ExchangeBank<BankVramConfig<8320u, 1u, 0u, 0u, false>, 8600u>  typeinfo 0x008CDBA4  vtable 0x008FBBDC
template <typename TVramConfig, u32 FILE_SIZE>
class ExchangeBank : public ::ExchangeResBank<FILE_SIZE, TVramConfig, ExchangeBankKeywordMyDesignFgNameEx<SvFgName::NAME_EMPTY>,
                                              item::ResLoader, 1u, false>
{
public:
    typedef ExchangeBankKeywordMyDesignFgNameEx<SvFgName::NAME_EMPTY> Keyword;

    ExchangeBank() {}
    virtual ~ExchangeBank();

    virtual s32 LoadResourceSplit(const Keyword& keyword, u32 slot) override; // slot 0x14
    virtual bool Initialize(const Keyword& keyword, u32 slot, bool isReady) override; // slot 0x18
    virtual bool Finalize(const Keyword& keyword, u32 slot) override; // slot 0x1C

    u8 m_Alignment; // 0x288
};

template <typename TVramConfig, u32 FILE_SIZE>
inline ExchangeBank<TVramConfig, FILE_SIZE>::~ExchangeBank()
{
    // (only the base is destroyed)
}

template <typename TVramConfig, u32 FILE_SIZE>
inline s32 ExchangeBank<TVramConfig, FILE_SIZE>::LoadResourceSplit(const Keyword& keyword, u32 slot)
{
    const item::Kind kind = keyword.m_Name.GetKind();
    u32 isDesign;
    if (kind == item::KIND_DUMMY_WALL || kind == item::KIND_CARPET) {
        isDesign = 0;
    } else if (kind == item::KIND_MY_DESIGN_TEXTURE) {
        isDesign = 1;
    } else {
        return 2;
    }
    return this->GetLoader(slot, 0)->Func536B74(&keyword.m_Name, this->GetAllocator(slot)->m_pHeap, m_Alignment, isDesign,
                                                 NULL, 1, 0);
}

template <typename TVramConfig, u32 FILE_SIZE>
inline bool ExchangeBank<TVramConfig, FILE_SIZE>::Initialize(const Keyword& keyword, u32 slot, bool isReady)
{
    if (!isReady) {
        return false;
    }
    ssys::ma::HeapAllocator* allocator = this->GetAllocator(slot);
    item::ResLoader* loader = this->GetLoader(slot, 0);
    if (keyword.m_Name.GetKind() == item::KIND_MY_DESIGN_TEXTURE) {
        loader->Read(0x10000, 0x10000, 0x10000);
    } else {
        loader->Read(0x20000, 0x10000, 0x10000);
    }
    loader->SetHeaps(this->GetVramAHeap(slot), this->GetVramBHeap(slot), NULL);
    loader->InitializeCgfx(allocator, NULL, true);
    return true;
}

template <typename TVramConfig, u32 FILE_SIZE>
inline bool ExchangeBank<TVramConfig, FILE_SIZE>::Finalize(const Keyword& keyword, u32 slot)
{
    this->GetLoader(slot, 0)->Release();
    return true;
}
} // namespace bg
