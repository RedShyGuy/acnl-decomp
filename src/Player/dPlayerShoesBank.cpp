#include "Player/dPlayerShoesBank.h"

#include "Bank/bank_Extern.h"
#include "g3d/g3d_System.h"
#include "sead/seadSafeStringBase.h"

// 0x002A214C slot 0x10 (name is ours)
bool PlayerShoesBank::LoadResource(const Keyword& keyword, u32 slot)
{
    const char* path = Func179978(&keyword);
    sead::Heap* heap = GetAllocator(slot)->m_pHeap;
    if (path == NULL) {
        return false;
    }
    if (GetLoader(slot, 0)->g3d::ResourceLoader::Load(sead::SafeStringBase<char>(path), heap, 128) != true) {
        return false;
    }
    return GetLoader(slot, 1)->Load(&keyword, heap, 0, 0, 0, true, 0);
}

// 0x002A2220 slot 0x18 (name is ours)
bool PlayerShoesBank::Initialize(const Keyword& keyword, u32 slot, bool isReady)
{
    if (!isReady) {
        return false;
    }
    return SetupModelWithTextures(slot);
}

// 0x002A2410 slot 0x1C (name is ours)
bool PlayerShoesBank::Finalize(const Keyword& keyword, u32 slot)
{
    m_Models[slot].Finalize();
    GetLoader(slot, 1)->Release();
    GetLoader(slot, 0)->Release();
    return true;
}

// 0x002A2470 slot 0x14 (name is ours)
s32 PlayerShoesBank::LoadResourceSplit(const Keyword& keyword, u32 slot)
{
    const char* path = Func179978(&keyword);
    sead::Heap* heap = GetAllocator(slot)->m_pHeap;
    if (path == NULL) {
        return 2;
    }
    item::ResLoader* model = GetLoader(slot, 0);
    const bool isModelLoaded = model->LoadAsync(sead::SafeStringBase<char>(path), heap, 128);
    item::ResLoader* textures = GetLoader(slot, 1);
    const bool isTexturesLoaded = textures->Func536B74(&keyword, heap, 0, 0, 0, 1, 0);
    if (!(isTexturesLoaded & isModelLoaded)) {
        return 0;
    }
    if (model->IsLoadFailed() || textures->IsLoadFailed()) {
        return 2;
    }
    return 1;
}

// 0x002A256C (name is ours)
PlayerShoesBank::PlayerShoesBank()
{
    // (only the bases and the members are constructed)
}

// 0x002A2758 slot 0x00
// 0x002A26F0 (deleting dtor)
PlayerShoesBank::~PlayerShoesBank()
{
    // (only the bases and the members are destroyed)
}
