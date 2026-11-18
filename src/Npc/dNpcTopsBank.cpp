#include "Npc/dNpcTopsBank.h"

#include "g3d/g3d_System.h"
#include "nw/gfx/res/gfx_ResModel.h"

// 0x001CDF58 slot 0x10 (name is ours)
bool NpcTopsBank::LoadResource(const Keyword& keyword, u32 slot)
{
    return GetLoader(slot, 0)->Load(&keyword, GetAllocator(slot)->m_pHeap, 0, 0, 0, true, 0);
}

// 0x001CDFBC slot 0x18 (name is ours)
bool NpcTopsBank::Initialize(const Keyword& keyword, u32 slot, bool isReady)
{
    GetLoader(slot, 0)->InitializeCgfx(GetAllocator(slot), NULL, true);
    return true;
}

// 0x001CDFFC (name is ours)
void NpcTopsBank::SetClothTexture(const nw::gfx::res::ResModelData* model)
{
    item::ResLoader* loader = GetLoader(GetCurrentSlot(), 0);
    if (loader == NULL || model == NULL) {
        return;
    }
    const nw::gfx::res::ResTexture texture(loader->GetGraphicsFile().ptr()->textures.GetData(0));
    const nw::gfx::res::ResMaterial material = nw::gfx::res::ResModel(model).FindMaterial("cloth");
    if (material.IsValid() && texture.IsValid()) {
        g3d::SetTexture(material, "cloth", texture);
    }
}

// 0x001CE0EC slot 0x1C (name is ours)
bool NpcTopsBank::Finalize(const Keyword& keyword, u32 slot)
{
    GetLoader(slot, 0)->Release();
    return true;
}

// 0x001CE11C slot 0x14 (name is ours)
s32 NpcTopsBank::LoadResourceSplit(const Keyword& keyword, u32 slot)
{
    return GetLoader(slot, 0)->Func536B74(&keyword, GetAllocator(slot)->m_pHeap, 0, 0, 0, 1, 0);
}

// 0x001CE180 (name is ours)
NpcTopsBank::NpcTopsBank()
{
    // (only the bases and the members are constructed)
}

// 0x001CE2F0 slot 0x00
// 0x001CE2B8 (deleting dtor)
NpcTopsBank::~NpcTopsBank()
{
    // (only the bases and the members are destroyed)
}
