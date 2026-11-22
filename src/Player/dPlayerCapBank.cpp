#include "Player/dPlayerCapBank.h"
#include "Player/dPlayerModel.h"
#include "Bs/dBsPlayerMgr.h"

// 0x0023D1C8 slot 0x10 (name is ours)
bool PlayerCapBank::LoadResource(const Keyword& keyword, u32 slot)
{
    if (!keyword.m_Name.IsUsual() || IsMiiMask(&keyword.m_Name) != 0) {
        return false;
    }
    sead::Heap* heap = GetAllocator(slot)->m_pHeap;
    if (!GetLoader(slot, 0)->Load(&keyword.m_Name, heap, 0, 0, NULL, true, 0)) {
        return false;
    }
    item::ResLoader* design = GetLoader(slot, 1);
    if (Func76BA5C(&keyword.m_Name)) {
        SvFgName designItem;
        Func76C8E0(&designItem, &keyword.m_Name);
        if (!designItem.IsEmpty()) {
            const void* data = keyword.GetDesignData();
            if (data != NULL && design->Load(&designItem, heap, 10, 1, data, false, 0)) {
                return true;
            }
        }
    }
    return true;
}

// 0x0023D370 slot 0x18 (name is ours)
bool PlayerCapBank::Initialize(const Keyword& keyword, u32 slot, bool isReady)
{
    if (!isReady) {
        return false;
    }
    ExchangeBankKeywordUnknown3 unknownC = keyword.m_UnknownC;
    ssys::ma::HeapAllocator* allocator = GetAllocator(slot);
    item::ResLoader* model = GetLoader(slot, 0);
    if (m_pAppearance != NULL) {
        PlayerModel::SetModelColors(static_cast<const nw::gfx::res::ResModelData*>(model->GetGraphicsFile().GetModels(0)),
                   keyword.m_HairColor, Func713778(m_pAppearance), &unknownC, 2);
    }
    model->Read(0x10000, 0x30000, 0x20000);
    model->SetHeaps(GetVramAHeap(slot), GetVramBHeap(slot), NULL);
    model->InitializeCgfx(allocator, Func536700(), true);
    model->InitializeCgfx(allocator, Func5C22D8(), true);
    if (Func76BA5C(&keyword.m_Name)) {
        model->InitializeCgfx(allocator, GetLoader(slot, 1), true);
    }
    model->InitializeCgfx(allocator, NULL, true);
    g3d::SetLightSetIndex(static_cast<const nw::gfx::res::ResModelData*>(model->GetGraphicsFile().GetModels(0)),
                          Func1E891C());
    return m_Models[slot].Create(nw::gfx::res::ResSceneObject(model->GetGraphicsFile().GetModels(0)), allocator,
                                 allocator, 0, true, 1);
}

// 0x0023D5B0 slot 0x1C (name is ours)
bool PlayerCapBank::Finalize(const Keyword& keyword, u32 slot)
{
    m_Models[slot].Finalize();
    GetLoader(slot, 0)->Release();
    GetLoader(slot, 1)->Release();
    return true;
}

// 0x0023D60C slot 0x14 (name is ours)
s32 PlayerCapBank::LoadResourceSplit(const Keyword& keyword, u32 slot)
{
    if (!keyword.m_Name.IsUsual() || IsMiiMask(&keyword.m_Name) != 0) {
        return 2;
    }
    sead::Heap* heap = GetAllocator(slot)->m_pHeap;
    item::ResLoader* model = GetLoader(slot, 0);
    bool isLoaded = model->Func536B74(&keyword.m_Name, heap, 0, 0, NULL, 1, 0);
    item::ResLoader* design = GetLoader(slot, 1);
    if (Func76BA5C(&keyword.m_Name)) {
        SvFgName designItem;
        Func76C8E0(&designItem, &keyword.m_Name);
        if (!designItem.IsEmpty()) {
            const void* data = keyword.GetDesignData();
            if (data != NULL) {
                isLoaded &= design->Func536B74(&designItem, heap, 10, 1, data, 0, 0);
            }
        }
    }
    if (!isLoaded) {
        return 0;
    }
    if (model->IsLoadFailed() || design->IsLoadFailed()) {
        model->Release();
        design->Release();
        return 2;
    }
    return 1;
}

// 0x0023D7F0 (name is ours)
PlayerCapBank::PlayerCapBank() : m_pAppearance(NULL)
{
}

// 0x0023DB40 slot 0x00
// 0x0023DAD4 (deleting dtor)
PlayerCapBank::~PlayerCapBank()
{
    // (only the bases and the members are destroyed)
}

// 0x0023D0B4 (name is ours)
void PlayerCapBank::RequestDraw(u32 /* sceneIdx */)
{
    g3d::SkeletalModel* model = GetCurrentModel();
    if (model == NULL) {
        return;
    }
    BsPlayerMgr::AddModel(BsPlayerMgr::MODEL_LIST_CAP, model);
}

// 0x0023D104 (name is ours)
bool PlayerCapBank::SetColors(const ExchangeBankKeywordUnknown3* skinColor)
{
    if (m_pAppearance == NULL) {
        return false;
    }
    item::ResLoader* loader = GetLoader(GetCurrentSlot(), 0);
    if (loader->m_SetupResult == g3d::ResourceSet::SETUP_RESULT_NONE || !loader->m_GraphicsFile.IsValid() ||
        loader->m_GraphicsFile.ptr()->models.count == 0) {
        return false;
    }
    const nw::gfx::res::ResModelData* model =
        static_cast<const nw::gfx::res::ResModelData*>(loader->GetGraphicsFile().GetModels(0));
    PlayerModel::SetModelColors(model, m_Keywords[GetCurrentSlot()].m_HairColor, Func713778(m_pAppearance), skinColor,
                                2);
    return true;
}
