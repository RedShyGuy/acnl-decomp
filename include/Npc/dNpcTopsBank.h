#pragma once

#include "decomp.h"
#include "Exchange/dExchangeResBank.h"
#include "Item/dResLoader.h"

namespace nw {
namespace gfx {
namespace res {
struct ResModelData;
} // namespace res
} // namespace gfx
} // namespace nw

// The bank of the tops of the villagers (class name from the RTTI; the function names are ours): the item file of the
// keyword per slot, its texture is put into the material "cloth" of the villager models.
// RTTI 11NpcTopsBank @ 0x008CB2FC
// vtable 0x008ECC44 (vptr 0x008ECC4C), offset_to_top 0, 8 entries
class NpcTopsBank : public ::ExchangeResBank<8600u, BankVramConfig<0u, 0u, 0u, 0u, false>,
                                             ExchangeBankKeywordFgNameEx<SvFgName::NAME_EMPTY>, item::ResLoader, 1u, false>
{
public:
    typedef ExchangeBankKeywordFgNameEx<SvFgName::NAME_EMPTY> Keyword;

    NpcTopsBank(); // 0x001CE180
    virtual ~NpcTopsBank(); // 0x001CE2F0 slot 0x00

    virtual bool LoadResource(const Keyword& keyword, u32 slot) override; // 0x001CDF58 slot 0x10
    virtual s32 LoadResourceSplit(const Keyword& keyword, u32 slot) override; // 0x001CE11C slot 0x14
    virtual bool Initialize(const Keyword& keyword, u32 slot, bool isReady) override; // 0x001CDFBC slot 0x18
    virtual bool Finalize(const Keyword& keyword, u32 slot) override; // 0x001CE0EC slot 0x1C

    // the texture of the shown slot in the material "cloth" of model
    void SetClothTexture(const nw::gfx::res::ResModelData* model); // 0x001CDFFC
};
