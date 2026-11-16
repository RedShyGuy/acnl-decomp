#include "Item/item_Api.h"
#include "Sv/dSvFgName.h"
#include "fgobj/fgobj_Api.h"
#include "ssys/ma/dFileData.h"

namespace item {
// 0x0076A3B8 (name is ours)
s32 GetFgIndex(const SvFgName& item)
{
    u32 index = item.id & ~0x8000;
    if (index > FG_NUM - 1) {
        return -1;
    }
    return index;
}

// 0x002FCFB4 (name is ours)
const ParamFgEntry* GetParamFgEntry(const SvFgName& item)
{
    if (fgobj::s_pFgParam != NULL) {
        u32 index = GetFgIndex(item);
        const ParamFgEntry* entries = reinterpret_cast<const ParamFgEntry*>(fgobj::s_pFgParam->m_pData);
        if (entries != NULL && index < FG_NUM) {
            return &entries[index];
        }
    }
    return NULL;
}

// 0x006D9BA8 (name after libgarden)
u32 ToInsectFishID(const SvFgName& item)
{
    u32 id = GetIndex(item);
    switch (item.GetKind()) {
    case KIND_INSECT:
    case KIND_FISH:
        return id;
    case KIND_FEATHER:
        return id + 73;
    case KIND_SNOW_CRYSTAL:
    case KIND_DUST:
        return id + 72;
    case KIND_DIVE_FISH:
        return id + 76;
    case KIND_FISH_EGG:
        return id + 75;
    case KIND_DIVE_FISH_EGG:
        return id + 106;
    case KIND_AMULET:
        return id + 81;
    case KIND_SMP:
        return id + 82;
    default:
        return 0;
    }
}
} // namespace item
