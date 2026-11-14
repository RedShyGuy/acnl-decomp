#include "Sv/dSvHouse.h"

// 0x0072108C (name is ours)
u8 SvPlayerHouse::GetRoomSize(u32 room) const
{
    return rooms[room].flags.roomSize;
}

// 0x0030F770 (name is ours)
SvPlayerHouse* SvPlayerHouseList::Get(u32 index)
{
    if (index >= 4) {
        return &houses[0];
    }
    return &houses[index];
}

namespace {
// (ARMCC inlines SvRoomActiveFlags::SetAllActive into the functions of both layers)
DECOMP_ALWAYS_INLINE void SetAllActiveInline(SvRoomActiveFlags* flags)
{
    for (u16* state = flags->activeStates; state < flags->activeStates + 0x10; state++) {
        *state = 0xFFFF;
    }
}

DECOMP_ALWAYS_INLINE void SetAllLayersActiveInline(SvRoomActiveLayers* states)
{
    for (SvRoomActiveFlags* layer = states->layers; layer < states->layers + 2; layer++) {
        SetAllActiveInline(layer);
    }
}
} // namespace

// 0x005835CC (name is ours)
void SvRoomActiveFlags::SetAllActive()
{
    SetAllActiveInline(this);
}

// 0x005835F0 (name is ours)
SvRoomActiveFlags::SvRoomActiveFlags()
{
    SetAllActive();
}

// 0x0058358C (name is ours)
void SvRoomActiveLayers::SetAllActive()
{
    SetAllLayersActiveInline(this);
}

// 0x00583648 (name is ours)
SvRoomActiveLayers::SvRoomActiveLayers()
{
    SetAllLayersActiveInline(this);
}
