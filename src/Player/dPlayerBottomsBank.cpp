#include "Player/dPlayerBottomsBank.h"
#include "Bs/dBsPlayerMgr.h"

#include "sead/seadSafeStringBase.h"

// 0x002D0728 slot 0x10 (name is ours)
bool PlayerBottomsBank::LoadResource(const Keyword& keyword, u32 slot)
{
    SvFgName item(keyword);
    GetItem(&item);
    const char* path = Func17C3D0(&item);
    if (path == NULL) {
        return false;
    }
    sead::Heap* heap = GetAllocator(slot)->m_pHeap;
    if (GetLoader(slot, 0)->g3d::ResourceLoader::Load(sead::SafeStringBase<char>(path), heap, 128) != true) {
        return false;
    }
    if (!GetLoader(slot, 1)->Load(&item, heap, 0, 0, 0, true, 0)) {
        return false;
    }
    return true;
}

// 0x002D0888 slot 0x18 (name is ours)
bool PlayerBottomsBank::Initialize(const Keyword& keyword, u32 slot, bool isReady)
{
    if (!isReady) {
        return false;
    }
    return SetupModelWithTextures(slot);
}

// 0x002D0A78 slot 0x1C (name is ours)
bool PlayerBottomsBank::Finalize(const Keyword& keyword, u32 slot)
{
    m_Models[slot].Finalize();
    GetLoader(slot, 1)->Release();
    GetLoader(slot, 0)->Release();
    return true;
}

// 0x002D0AD8 slot 0x14 (name is ours)
s32 PlayerBottomsBank::LoadResourceSplit(const Keyword& keyword, u32 slot)
{
    SvFgName item(keyword);
    GetItem(&item);
    const char* path = Func17C3D0(&item);
    if (path == NULL) {
        return 2;
    }
    sead::Heap* heap = GetAllocator(slot)->m_pHeap;
    item::ResLoader* model = GetLoader(slot, 0);
    const bool isModelLoaded = model->LoadAsync(sead::SafeStringBase<char>(path), heap, 128);
    item::ResLoader* textures = GetLoader(slot, 1);
    if (!(textures->Func536B74(&item, heap, 0, 0, 0, 1, 0) & isModelLoaded)) {
        return 0;
    }
    if (model->IsLoadFailed() || textures->IsLoadFailed()) {
        model->g3d::ResourceLoader::Unload();
        textures->Release();
        return 2;
    }
    return 1;
}

// 0x002D0C98 (name is ours)
PlayerBottomsBank::PlayerBottomsBank() : m_IsMale(false)
{
}

// 0x002D0E88 slot 0x00
// 0x002D0E20 (deleting dtor)
PlayerBottomsBank::~PlayerBottomsBank()
{
    // (only the bases and the members are destroyed)
}

// 0x002D06D4 (name is ours)
void PlayerBottomsBank::RequestDraw(u32 /* sceneIdx */)
{
    g3d::SkeletalModel* model = GetCurrentModel();
    if (model == NULL) {
        return;
    }
    BsPlayerMgr::AddModel(BsPlayerMgr::MODEL_LIST_BOTTOMS, model);
}
