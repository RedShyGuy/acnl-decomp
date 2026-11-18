#pragma once

#include "decomp.h"
#include "Exchange/dExchangeModelBankBase.h"
#include "Exchange/dExchangeResBank.h"
#include "Bank/bank_Extern.h"
#include "g3d/dSkeletalModel.h"
#include "g3d/g3d_System.h"

// An exchange bank with a skeletal model per slot (class name from the RTTI, slot names after libgarden). It implements
// ExchangeModelBankBase; ARMCC appends the three overrides to the primary vtable (slots 0x20/0x24/0x28) and calls them
// from the secondary vtable through this-adjusting thunks.
// Instantiations found in the binary:
//   ExchangeModelBank<13772u, BankVramConfig<300u, 2u, 2000u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, item::ResLoader, 1u, true>  typeinfo 0x008CC548  vtable 0x008F3830
//   ExchangeModelBank<17703u, BankVramConfig<600u, 2u, 3600u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, item::ResLoader, 2u, true>  typeinfo 0x008CC568  vtable 0x008F3878
//   ExchangeModelBank<17829u, BankVramConfig<500u, 2u, 2800u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32765>, item::ResLoader, 2u, true>  typeinfo 0x008CC588  vtable 0x008F38C0
//   ExchangeModelBank<17879u, BankVramConfig<700u, 2u, 3600u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, item::ResLoader, 2u, true>  typeinfo 0x008CC5A8  vtable 0x008F3908
//   ExchangeModelBank<24448u, BankVramConfig<736u, 4u, 3824u, 4u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, item::ResLoader, 2u, false>  typeinfo 0x008CC5C8  vtable 0x008F3950
//   ExchangeModelBank<25891u, BankVramConfig<1800u, 4u, 6600u, 4u, false>, PlayerHeadBankKeyword, g3d::ResourceLoader, 1u, false>  typeinfo 0x008CC5E8  vtable 0x008F3998
//   ExchangeModelBank<31423u, BankVramConfig<2500u, 3u, 12400u, 3u, false>, PlayerCapBankKeyword, item::ResLoader, 2u, true>  typeinfo 0x008CC608  vtable 0x008F39E0
//   ExchangeModelBank<32891u, BankVramConfig<2800u, 2u, 8800u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, item::ResLoader, 2u, true>  typeinfo 0x008CC628  vtable 0x008F3A28
template <u32 FILE_SIZE, typename TVramConfig, typename TKeyword, typename TLoader, u32 MODEL_NUM, bool FLAG>
class ExchangeModelBank : public ExchangeResBank<FILE_SIZE, TVramConfig, TKeyword, TLoader, MODEL_NUM, FLAG>,
                          public ExchangeModelBankBase
{
public:
    ExchangeModelBank() {}
    virtual ~ExchangeModelBank();

    virtual g3d::SkeletalModel* GetCurrentModel() override; // slot 0x20
    // (the original calls its copy directly where a derived class qualifies it, e.g. ToolBank)
    DECOMP_NOINLINE virtual const g3d::SkeletalModel* GetCurrentModel() const override; // slot 0x24
    virtual const void* GetCurrentResource(u32 index) const override; // slot 0x28

    // (inline) the model of the slot from loader 0 (the model file) with the textures of loader 1 (the item file)
    DECOMP_ALWAYS_INLINE bool SetupModelWithTextures(u32 slot)
    {
        ssys::ma::HeapAllocator* allocator = this->GetAllocator(slot);
        TLoader* model = this->GetLoader(slot, 0);
        TLoader* textures = this->GetLoader(slot, 1);
        textures->Read(0x10000, 0x30000, 0x20000);
        textures->SetHeaps(this->GetVramAHeap(slot), this->GetVramBHeap(slot), NULL);
        model->Read(0x10000, 0x30000, 0x20000);
        model->SetHeaps(this->GetVramAHeap(slot), this->GetVramBHeap(slot), NULL);
        textures->InitializeCgfx(allocator, NULL, true);
        model->InitializeCgfx(allocator, Func5C22D8(), true);
        model->InitializeCgfx(allocator, Func536700(), true);
        model->InitializeCgfx(allocator, textures, true);
        model->InitializeCgfx(allocator, NULL, true);
        g3d::SetLightSetIndex(static_cast<const nw::gfx::res::ResModelData*>(model->GetGraphicsFile().GetModels(0)),
                              Func1E891C());
        return m_Models[slot].Create(nw::gfx::res::ResSceneObject(model->GetGraphicsFile().GetModels(0)), allocator,
                                     allocator, 0, true, 1);
    }

    g3d::SkeletalModel m_Models[2];
};

template <u32 FILE_SIZE, typename TVramConfig, typename TKeyword, typename TLoader, u32 MODEL_NUM, bool FLAG>
inline ExchangeModelBank<FILE_SIZE, TVramConfig, TKeyword, TLoader, MODEL_NUM, FLAG>::~ExchangeModelBank()
{
    // (only the members and the bases are destroyed)
}

template <u32 FILE_SIZE, typename TVramConfig, typename TKeyword, typename TLoader, u32 MODEL_NUM, bool FLAG>
inline g3d::SkeletalModel* ExchangeModelBank<FILE_SIZE, TVramConfig, TKeyword, TLoader, MODEL_NUM, FLAG>::GetCurrentModel()
{
    // (the original calls the const slot virtually)
    return const_cast<g3d::SkeletalModel*>(static_cast<const ExchangeModelBank*>(this)->GetCurrentModel());
}

template <u32 FILE_SIZE, typename TVramConfig, typename TKeyword, typename TLoader, u32 MODEL_NUM, bool FLAG>
inline const g3d::SkeletalModel*
ExchangeModelBank<FILE_SIZE, TVramConfig, TKeyword, TLoader, MODEL_NUM, FLAG>::GetCurrentModel() const
{
    const u32 slot = this->GetCurrentSlot();
    if (slot < 2 && m_Models[slot].GetNode() != NULL) {
        return &m_Models[slot];
    }
    return NULL;
}

template <u32 FILE_SIZE, typename TVramConfig, typename TKeyword, typename TLoader, u32 MODEL_NUM, bool FLAG>
inline const void* ExchangeModelBank<FILE_SIZE, TVramConfig, TKeyword, TLoader, MODEL_NUM, FLAG>::GetCurrentResource(u32 index) const
{
    if (index >= MODEL_NUM) {
        return NULL;
    }
    const TLoader* loader = this->GetLoader(this->GetCurrentSlot(), index);
    if (loader == NULL || !loader->GetGraphicsFile().IsValid() ||
        loader->GetGraphicsFile().ptr()->models.count <= 0) {
        return NULL;
    }
    return loader->GetGraphicsFile().GetModels(0);
}
