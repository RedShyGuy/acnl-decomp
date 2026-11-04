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
