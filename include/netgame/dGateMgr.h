#pragma once

#include "decomp.h"

namespace netgame {
// Whether the session is open for new players, with reasons to keep it closed (names after libgarden:
// GateMgr, Flag and its values, Open, IsOpen, IsClosed, Close, m_CmdQueue, m_CurrentCmd, m_CloseFlags).
// settings of the session (name after libgarden)
extern u16 s_GateParam;

class GateMgr
{
public:
    enum Flag
    {
        FLAG_TRAIN_STATION,
        FLAG_BOAT_TRIP,
        FLAG_RANDOM_MATCH_TOUR,
        FLAG_RANDOM_MATCH_PLAYER_JOINING,
        FLAG_RANDOM_MATCH_PLAYER_LEAVING,
        FLAG_UNKNOWN,
    };

    GateMgr(); // 0x00623C5C
    void Process(); // 0x00623948
    void Reset(); // 0x00623C40
    bool Open(Flag flag); // 0x00623A78 | libgarden
    bool Close(Flag flag); // 0x00623ABC | libgarden
    bool IsClosed(Flag flag) const; // 0x0075F774 | libgarden
    bool IsOpen() const { return m_CloseFlags == 0; }

private:
    // the bits of the flags, bit 7 = open
    bool Enqueue(u8 cmd); // 0x00623B04
    void EraseCmd(s32 index);

public:
    u8 m_CmdQueue[5]; // 0x0
    u8 m_CurrentCmd;  // 0x5
    u8 m_CloseFlags;  // 0x6
    u8 padding;       // 0x7
};
ASSERT_SIZE(GateMgr, 8);
} // namespace netgame
