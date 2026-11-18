#include "bg/dBgModelMgr.h"
#include "Sv/dSvBgName.h"
#include "sead/seadFixedSafeString.h"
#include "sead/seadStringFormatter.h"
#include "stage/stage_Api.h"

// 0x0018EEFC (name is ours)
BgModelMgr::BgModelMgr()
{
    // (only the members are constructed)
}

// 0x0018EF28 (name is ours)
BgModelMgr::~BgModelMgr()
{
    // (only the members are destroyed)
}

// 0x0018EC74 (name is ours)
g3d::ResourceLoader* BgModelMgr::GetLoader(u32 x, u32 z)
{
    return &m_Loaders[z][x];
}

// 0x0018EC9C (name is ours)
bool BgModelMgr::CreateHeap(sead::Heap* parent, u32 size)
{
    if (parent == NULL) {
        return false;
    }
    return m_Allocator.CreateExpHeap(size, parent, sead::SafeStringBase<char>("BgModelMgr"),
                                     sead::Heap::HEAP_DIRECTION_FORWARD, false);
}

// 0x0018ECF8 (name is ours)
bool BgModelMgr::Load(u32 x, u32 z, u32 bgName)
{
    sead::FixedSafeString<64> path;
    if (stage::GetCurrent() == stage::NAME_CHIP_TUTORIAL) {
        sead::StringFormatter formatter(&path);
        formatter.append("Bg/Indoor/Models/idr_room_ChipTutorial.bcres").flush();
    } else if (stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent())) {
        sead::StringFormatter formatter(&path);
        formatter.append("Bg/Outdoor/Models/%s.bcres").append(GetBgNameString(bgName)).flush();
    } else {
        sead::StringFormatter formatter(&path);
        formatter.append("Bg/Indoor/Models/%s.bcres").append(GetBgNameString(bgName)).flush();
    }
    return m_Loaders[z][x].LoadAsync(path.getCString(), m_Allocator.m_pHeap, 128);
}
