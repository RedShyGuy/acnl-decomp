#include "Other/dMusicJacketTextureBank.h"

#include "Item/item_Api.h"

// 0x00331130 slot 0x10 (name is ours)
bool MusicJacketTextureBank::LoadResource(const Keyword& keyword, u32 slot)
{
    SvFgName item(keyword.m_Name);
    if (!CanLoad(item)) {
        return false;
    }
    if (!GetLoader(slot, 0)->Load(&item, GetAllocator(slot)->m_pHeap, 0, 0, 0, true, 0)) {
        return false;
    }
    return true;
}

// 0x003311FC slot 0x1C (name is ours)
bool MusicJacketTextureBank::Finalize(const Keyword& keyword, u32 slot)
{
    GetLoader(slot, 0)->Release();
    return true;
}

// 0x0033122C (name is ours)
s32 MusicJacketTextureBank::Request(const SvFgName& item)
{
    Keyword keyword(item);
    return Exchange(keyword);
}

// 0x0033161C slot 0x14 (name is ours)
s32 MusicJacketTextureBank::LoadResourceSplit(const Keyword& keyword, u32 slot)
{
    SvFgName item(keyword.m_Name);
    if (!CanLoad(item)) {
        return 2;
    }
    item::ResLoader* loader = GetLoader(slot, 0);
    if (!loader->Func536B74(&item, GetAllocator(slot)->m_pHeap, 0, 1, 0, 1, 0)) {
        return 0;
    }
    if (loader->IsLoadFailed()) {
        loader->Release();
        return 2;
    }
    return 1;
}

// 0x0033171C (name is ours)
bool MusicJacketTextureBank::ReleaseAll()
{
    for (u32 i = 0; i < 2; i++) {
        if (m_Allocators[i].m_pHeap != NULL) {
            Release(i);
        }
    }
    ClearRequest();
    Bank::FinalizeBank();
    m_Unknown0 = false;
    m_Unknown1 = false;
    return true;
}

// 0x00331874 (name is ours)
MusicJacketTextureBank::MusicJacketTextureBank()
{
    m_Unknown0 = false;
    m_Unknown1 = false;
    m_IsMyDesignAllowed = false;
}

// 0x00331AB0 slot 0x00
// 0x00331A78 (deleting dtor)
MusicJacketTextureBank::~MusicJacketTextureBank()
{
    // (only the base and the members are destroyed)
}
