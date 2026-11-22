#include "Player/dPlayerAcceBank.h"
#include "Bs/dBsPlayerMgr.h"

#include "Bank/bank_Extern.h"
#include "g3d/g3d_System.h"
#include "script/dScriptExtern.h"

// 0x00270D30 slot 0x10 (name is ours)
bool PlayerAcceBank::LoadResource(const Keyword& keyword, u32 slot)
{
    if (!keyword.IsUsual() || Func311840(&keyword) != 1) {
        return false;
    }
    return GetLoader(slot, 0)->Load(&keyword, GetAllocator(slot)->m_pHeap, 0, 0, 0, true, 0);
}

// 0x00270DC4 slot 0x18 (name is ours)
bool PlayerAcceBank::Initialize(const Keyword& keyword, u32 slot, bool isReady)
{
    if (!isReady) {
        return false;
    }
    ssys::ma::HeapAllocator* allocator = GetAllocator(slot);
    item::ResLoader* loader = GetLoader(slot, 0);
    loader->Read(0x10000, 0x30000, 0x20000);
    loader->SetHeaps(GetVramAHeap(slot), GetVramBHeap(slot), NULL);
    loader->InitializeCgfx(allocator, Func5C22D8(), true);
    loader->InitializeCgfx(allocator, Func536700(), true);
    loader->InitializeCgfx(allocator, NULL, true);
    g3d::SetLightSetIndex(static_cast<const nw::gfx::res::ResModelData*>(loader->GetGraphicsFile().GetModels(0)),
                          Func1E891C());
    return m_Models[slot].Create(nw::gfx::res::ResSceneObject(loader->GetGraphicsFile().GetModels(0)), allocator,
                                 allocator, 0, true, 1);
}

// 0x00270F48 slot 0x1C (name is ours)
bool PlayerAcceBank::Finalize(const Keyword& keyword, u32 slot)
{
    m_Models[slot].Finalize();
    GetLoader(slot, 0)->Release();
    return true;
}

// 0x00270F90 slot 0x14 (name is ours)
s32 PlayerAcceBank::LoadResourceSplit(const Keyword& keyword, u32 slot)
{
    if (!keyword.IsUsual() || Func311840(&keyword) != 1) {
        return 2;
    }
    item::ResLoader* loader = GetLoader(slot, 0);
    if (!loader->Func536B74(&keyword, GetAllocator(slot)->m_pHeap, 0, 0, 0, 1, 0)) {
        return 0;
    }
    if (!loader->IsLoadFailed()) {
        return 1;
    }
    loader->Release();
    return 2;
}

// 0x00271054 (name is ours)
PlayerAcceBank::PlayerAcceBank()
{
    // (only the bases and the members are constructed)
}

// 0x00271238 slot 0x00
// 0x002711D0 (deleting dtor)
PlayerAcceBank::~PlayerAcceBank()
{
    // (only the bases and the members are destroyed)
}

// 0x00270CDC (name is ours)
void PlayerAcceBank::RequestDraw(u32 /* sceneIdx */)
{
    g3d::SkeletalModel* model = GetCurrentModel();
    if (model == NULL) {
        return;
    }
    BsPlayerMgr::AddModel(BsPlayerMgr::MODEL_LIST_ACCE, model);
}
