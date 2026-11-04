#include "Sv/dSvApi.h"

// The checks of the save numbers (the file of the original is unknown; they are not in the file of the access
// functions, which calls them).

// 0x0027B0E0 (name is ours)
bool SvIsNetPlayerSaveNo(PlayerSaveNumber save)
{
    return static_cast<u32>(save - PLAYER_SAVE_NUMBER_NET_PLAYER_SLOT_0) < 4;
}

// 0x0027AB98 (name is ours)
s32 SvGetNetPlayerSlot(PlayerSaveNumber save)
{
    u32 slot = save - PLAYER_SAVE_NUMBER_NET_PLAYER_SLOT_0;
    if (slot >= 4) {
        return -1;
    }
    return slot;
}

// 0x0027B0FC (name after libgarden's sv.hpp)
bool SvIsValidPlayerSaveNo(PlayerSaveNumber save)
{
    return save < 4;
}

// 0x002FDE1C (name is ours)
bool SvIsAnyPlayerSaveNo(PlayerSaveNumber save)
{
    return save < PLAYER_SAVE_NUMBER_INVALID;
}
