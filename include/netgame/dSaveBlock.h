#pragma once

#include "decomp.h"
#include "netgame/netgame_Api.h"

namespace netgame {
// A block of saved data of a player that the host sends along (game code that is not decompiled yet; name is ours)
class SaveBlock
{
public:
    SaveBlock(); // 0x001AE79C
    ~SaveBlock(); // 0x001AE7C0
    void Reset(); // 0x001AE310
    // fills the block for the player
    void Build(PlayerNo player); // 0x001ADCE8

    u8 unk_0x0[0x251C];
    bool m_IsValid; // 0x251C
    u8 unk_0x251D[3];
};
ASSERT_SIZE(SaveBlock, 0x2520);

void ClearSaveBlock(void* block); // 0x00308CC8 (Thumb)

// a block about the villagers (game code that is not decompiled yet; name is ours)
class NpcInfo
{
public:
    NpcInfo(); // 0x0029BE2C
    ~NpcInfo(); // 0x0029BE44

    u8 data[50];
};
ASSERT_SIZE(NpcInfo, 50);
} // namespace netgame
