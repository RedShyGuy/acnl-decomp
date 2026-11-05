#include "Item/item_Api.h"
#include "Sv/dSvFgName.h"

namespace item {
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
