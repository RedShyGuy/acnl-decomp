#include "Player/dPlayerLegsBank.h"

#include "sead/seadSafeStringBase.h"

// 0x00954F98
const char* PlayerLegsBank::s_ModelPath = "Player/legs_mdl/Common.bcres";

// 0x00272980 slot 0x10 (name is ours)
bool PlayerLegsBank::LoadResource(const Keyword& keyword, u32 slot)
{
    const bool isSocks = keyword.m_Name.GetKind() == item::KIND_SOCKS;
    sead::Heap* heap = GetAllocator(slot)->m_pHeap;
    if (s_ModelPath == NULL ||
        GetLoader(slot, 0)->g3d::ResourceLoader::Load(sead::SafeStringBase<char>(s_ModelPath), heap, 128) != true) {
        return false;
    }
    item::ResLoader* textures = GetLoader(slot, 1);
    if (isSocks && !textures->Load(&keyword.m_Name, heap, 0, 0, 0, true, 0)) {
        return false;
    }
    return true;
}

// 0x00272A7C slot 0x18 (name is ours)
bool PlayerLegsBank::Initialize(const Keyword& keyword, u32 slot, bool isReady)
{
    if (!isReady) {
        return false;
    }
    ssys::ma::HeapAllocator* allocator = GetAllocator(slot);
    item::ResLoader* model = GetLoader(slot, 0);
    item::ResLoader* textures = GetLoader(slot, 1);
    const bool isSocks = keyword.m_Name.GetKind() == item::KIND_SOCKS;
    if (isSocks) {
        textures->Read(0x10000, 0x30000, 0x20000);
        textures->SetHeaps(GetVramAHeap(slot), GetVramBHeap(slot), NULL);
    }
    model->Read(0x10000, 0x30000, 0x20000);
    model->SetHeaps(GetVramAHeap(slot), GetVramBHeap(slot), NULL);
    if (m_pUnknown4D0 != NULL) {
        Func1CEEF0(static_cast<const nw::gfx::res::ResModelData*>(model->GetGraphicsFile().GetModels(0)),
                   Func713778(m_pUnknown4D0), Func713788(m_pUnknown4D0), &keyword.m_UnknownC, 16);
    }
    if (isSocks) {
        textures->InitializeCgfx(allocator, NULL, true);
    }
    model->InitializeCgfx(allocator, Func5C22D8(), true);
    model->InitializeCgfx(allocator, Func536700(), true);
    if (isSocks) {
        model->InitializeCgfx(allocator, textures, true);
    } else {
        model->InitializeCgfx(allocator, Func5C6794(), true);
    }
    model->InitializeCgfx(allocator, NULL, true);
    g3d::SetLightSetIndex(static_cast<const nw::gfx::res::ResModelData*>(model->GetGraphicsFile().GetModels(0)),
                          Func1E891C());
    return m_Models[slot].Create(nw::gfx::res::ResSceneObject(model->GetGraphicsFile().GetModels(0)), allocator,
                                 allocator, 0, true, 1);
}

// 0x00272D3C slot 0x1C (name is ours)
bool PlayerLegsBank::Finalize(const Keyword& keyword, u32 slot)
{
    m_Models[slot].Finalize();
    GetLoader(slot, 1)->Release();
    GetLoader(slot, 0)->Release();
    return true;
}

// 0x00272D98 slot 0x14 (name is ours)
s32 PlayerLegsBank::LoadResourceSplit(const Keyword& keyword, u32 slot)
{
    const bool isSocks = keyword.m_Name.GetKind() == item::KIND_SOCKS;
    if (s_ModelPath == NULL) {
        return 2;
    }
    sead::Heap* heap = GetAllocator(slot)->m_pHeap;
    item::ResLoader* model = GetLoader(slot, 0);
    bool isLoaded = model->LoadAsync(sead::SafeStringBase<char>(s_ModelPath), heap, 128);
    item::ResLoader* textures = GetLoader(slot, 1);
    if (isSocks) {
        isLoaded &= textures->Func536B74(&keyword.m_Name, heap, 0, 0, 0, 1, 0);
    }
    if (!isLoaded) {
        return 0;
    }
    if (model->IsLoadFailed() || textures->IsLoadFailed()) {
        model->g3d::ResourceLoader::Unload();
        textures->Release();
        return 2;
    }
    return 1;
}

// 0x00272ED0 (name is ours)
PlayerLegsBank::PlayerLegsBank() : m_pUnknown4D0(NULL)
{
}

// 0x00273184 slot 0x00
// 0x00273118 (deleting dtor)
PlayerLegsBank::~PlayerLegsBank()
{
    // (only the bases and the members are destroyed)
}

// 0x00272868 (name is ours)
void PlayerLegsBank::RequestDraw()
{
    g3d::SkeletalModel* model = GetCurrentModel();
    if (model == NULL || g_pUnknown951378 == NULL) {
        return;
    }
    BankModelList* list = reinterpret_cast<BankModelList*>(g_pUnknown951378 + 0x12CA0);
    if (list->m_Count < 7) {
        list->m_pModels[list->m_Count] = model;
        list->m_Count++;
    }
}

// 0x002728BC (name is ours)
bool PlayerLegsBank::SetColors(const ExchangeBankKeywordUnknown3* colors)
{
    if (m_pUnknown4D0 == NULL) {
        return false;
    }
    item::ResLoader* loader = GetLoader(GetCurrentSlot(), 0);
    if (loader->m_SetupResult == g3d::ResourceSet::SETUP_RESULT_NONE || !loader->m_GraphicsFile.IsValid() ||
        loader->m_GraphicsFile.ptr()->models.count == 0) {
        return false;
    }
    const nw::gfx::res::ResModelData* model =
        static_cast<const nw::gfx::res::ResModelData*>(loader->GetGraphicsFile().GetModels(0));
    Func1CEEF0(model, Func713778(m_pUnknown4D0), Func713788(m_pUnknown4D0), colors, 16);
    return true;
}
