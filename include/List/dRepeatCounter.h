#pragma once

#include "decomp.h"

// The repeat of a held key: fires after 11 frames, then every 3 frames (the same timing as the buttons). A small class
// without RTTI in the code of MsgWindow (0x6E6xxx); all names are ours.
class RepeatCounter
{
public:
    DECOMP_NOINLINE RepeatCounter(); // 0x006E63B0
    // a frame with the key held (or not); m_IsFirst is cleared when the repeat fires
    void Update(bool isHeld); // 0x006E636C

    s32 m_Counter;  // 0x0
    s32 m_Limit;    // 0x4 11 before the first repeat, 3 after it
    bool m_IsFirst; // 0x8 the repeat has not fired yet
};
ASSERT_SIZE(RepeatCounter, 0xC);
