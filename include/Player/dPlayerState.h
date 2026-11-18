#pragma once

#include "decomp.h"

// The state of a player as the other machines get it (layout and names after libgarden; only what the decompiled code
// reads is typed, the action data is raw).
#pragma pack(push, 1)
struct PlayerState
{
    u8 stage;        // 0x00 stage::Name
    u8 actionName;   // 0x01 PlayerAction::Name
    s16 positionX;   // 0x02 the position in quarters of a world unit (libgarden: PlayerAction::CompactXZ)
    s16 positionZ;   // 0x04
    u16 rotation;    // 0x06 the rotation around y
    u8 counter;      // 0x08
    u8 flags[3];     // 0x09
    u8 chairDist;    // 0x0C
    u8 pad;          // 0x0D
    u8 action[0x18]; // 0x0E (libgarden: PlayerAction)
};
#pragma pack(pop)
ASSERT_SIZE(PlayerState, 0x26);
