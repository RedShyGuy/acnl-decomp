#include "netgame/dMachineBitTable.h"

namespace netgame {
// (in the file of MachineBitTable: the original inlines it there)
// 0x00625750 | libgarden
u8 GetMask(PlayerNo player)
{
    if (player < PLAYER_NO_MAX) {
        return 1 << player;
    }
    return 0;
}

// 0x0062557C | libgarden
void netgame::MachineBitTable::Add(PlayerNo no)
{
    u8 bits = m_Bit | GetMask(no);
    if (m_Bit != bits && m_Count < PLAYER_NO_MAX) {
        m_Bit = bits;
        m_Count++;
    }
}

// 0x006255FC | libgarden
void netgame::MachineBitTable::Remove(PlayerNo no)
{
    u8 bits = m_Bit & ~GetMask(no);
    if (m_Bit != bits && m_Count != 0) {
        m_Bit = bits;
        m_Count--;
    }
}

// 0x00625638 (name after libgarden)
void netgame::MachineBitTable::Add(u8 bits)
{
    m_Bit = 0;
    m_Count = 0;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (GetMask(no) & bits) {
            Add(no);
        }
    }
}

// 0x00625740 | libgarden
void netgame::MachineBitTable::Reset()
{
    m_Bit = 0;
    m_Count = 0;
}
} // namespace netgame
