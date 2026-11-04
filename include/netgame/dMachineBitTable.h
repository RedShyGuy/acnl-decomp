#pragma once

#include "decomp.h"
#include "netgame/netgame_Api.h"

namespace netgame {
// A set of players as a bit mask with their number (names after libgarden: MachineBitTable, Add, Remove,
// Reset, m_Bit, m_Count; the others are ours)
// RTTI N7netgame15MachineBitTableE @ 0x008D3F68
class MachineBitTable
{
public:
    DECOMP_NOINLINE MachineBitTable() : m_Bit(0), m_Count(0) {} // 0x00625768
    // (the set must not be full)
    void Add(PlayerNo no); // 0x0062557C | libgarden
    // the players of the mask
    void Add(u8 bits); // 0x00625638 (name after libgarden)
    void Remove(PlayerNo no); // 0x006255FC | libgarden
    void Reset(); // 0x00625740 | libgarden

    u8 GetBit() const { return m_Bit; }
    u8 GetCount() const { return m_Count; }
    bool IsSet(PlayerNo no) const { return (GetMask(no) & m_Bit) != 0; }

    u8 m_Bit;   // 0x0
    u8 m_Count; // 0x1
};
ASSERT_SIZE(MachineBitTable, 2);
} // namespace netgame
