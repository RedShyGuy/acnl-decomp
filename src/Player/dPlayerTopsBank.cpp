#include "Player/dPlayerTopsBank.h"
#include "Bs/dBsPlayerMgr.h"

#include "sead/seadSafeStringBase.h"

// 0x00273304 slot 0x10 (name is ours)
bool PlayerTopsBank::LoadResource(const Keyword& keyword, u32 slot)
{
    SvFgName item(keyword.m_Name);
    GetItem(&item);
    const char* path = Func1768C8(&item);
    sead::Heap* heap = GetAllocator(slot)->m_pHeap;
    if (path == NULL ||
        GetLoader(slot, 0)->g3d::ResourceLoader::Load(sead::SafeStringBase<char>(path), heap, 128) != true) {
        return false;
    }
    item::ResLoader* textures = GetLoader(slot, 1);
    if (!Func76BA5C(&item)) {
        if (textures->Load(&item, heap, 0, 0, NULL, true, 0)) {
            return true;
        }
    } else {
        SvFgName design;
        Func76C8E0(&design, &item);
        if (!design.IsEmpty()) {
            const void* data = keyword.GetDesignData();
            if (data != NULL && textures->Load(&design, heap, 0, 0, data, false, 0)) {
                return true;
            }
        }
    }
    SvFgName fallback(static_cast<SvFgName::ID>(0x2682));
    return textures->Load(&fallback, heap, 0, 0, NULL, true, 0);
}

// 0x002735D0 slot 0x18 (name is ours)
bool PlayerTopsBank::Initialize(const Keyword& keyword, u32 slot, bool isReady)
{
    if (!isReady) {
        return false;
    }
    ssys::ma::HeapAllocator* allocator = GetAllocator(slot);
    item::ResLoader* model = GetLoader(slot, 0);
    item::ResLoader* textures = GetLoader(slot, 1);
    textures->Read(0x10000, 0x30000, 0x20000);
    textures->SetHeaps(GetVramAHeap(slot), GetVramBHeap(slot), NULL);
    model->Read(0x10000, 0x30000, 0x20000);
    model->SetHeaps(GetVramAHeap(slot), GetVramBHeap(slot), NULL);
    if (m_pUnknown4D0 != NULL) {
        Func1CEEF0(static_cast<const nw::gfx::res::ResModelData*>(model->GetGraphicsFile().GetModels(0)),
                   Func713778(m_pUnknown4D0), Func713788(m_pUnknown4D0), &keyword.m_UnknownC, 8);
    }
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

// 0x00273838 slot 0x1C (name is ours)
bool PlayerTopsBank::Finalize(const Keyword& keyword, u32 slot)
{
    m_Models[slot].Finalize();
    GetLoader(slot, 1)->Release();
    GetLoader(slot, 0)->Release();
    return true;
}

// 0x00273894 slot 0x14 (name is ours)
s32 PlayerTopsBank::LoadResourceSplit(const Keyword& keyword, u32 slot)
{
    SvFgName item(keyword.m_Name);
    GetItem(&item);
    const char* path = Func1768C8(&item);
    if (path == NULL) {
        return 2;
    }
    sead::Heap* heap = GetAllocator(slot)->m_pHeap;
    item::ResLoader* model = GetLoader(slot, 0);
    bool isLoaded = model->LoadAsync(sead::SafeStringBase<char>(path), heap, 128);
    item::ResLoader* textures = GetLoader(slot, 1);
    if (!Func76BA5C(&item)) {
        isLoaded &= textures->Func536B74(&item, heap, 0, 0, NULL, 1, 0);
    } else {
        SvFgName design;
        Func76C8E0(&design, &item);
        if (!design.IsEmpty()) {
            const void* data = keyword.GetDesignData();
            if (data != NULL) {
                isLoaded &= textures->Func536B74(&design, heap, 0, 0, data, 0, 0);
            }
        }
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

// 0x00273B58 (name is ours)
PlayerTopsBank::PlayerTopsBank() : m_pUnknown4D0(NULL)
{
}

// 0x00273E0C slot 0x00
// 0x00273DA0 (deleting dtor)
PlayerTopsBank::~PlayerTopsBank()
{
    // (only the bases and the members are destroyed)
}

// 0x002731EC (name is ours)
void PlayerTopsBank::RequestDraw()
{
    g3d::SkeletalModel* model = GetCurrentModel();
    if (model == NULL) {
        return;
    }
    BsPlayerMgr::AddModel(BsPlayerMgr::MODEL_LIST_TOPS, model);
}
