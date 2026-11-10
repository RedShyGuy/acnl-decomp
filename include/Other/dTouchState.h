#pragma once

#include "decomp.h"
#include "nn/math/math_Vector2.h"

// The touch screen as the menus of the game see it: updated once per frame by the system task (0x1B7918, reset by
// 0x1B78E8; both game code that is not decompiled yet). All names are ours.
struct TouchState
{
    u8 isTouch;            // 0x00
    u8 isChanged;          // 0x01 touched or released in this frame
    s16 x;                 // 0x02 the current position (-1: none)
    s16 y;                 // 0x04
    s16 startX;            // 0x06 where the touch began
    s16 startY;            // 0x08
    s16 lastX;             // 0x0A the last touched position
    s16 lastY;             // 0x0C
    u8 holdFrames;         // 0x0E the frames since the touch began (at most 200)
    u8 releaseFrames;      // 0x0F the frames since the release (at most 200)
    // the values of the previous frame
    s16 prevStartX;        // 0x10
    s16 prevStartY;        // 0x12
    s16 prevLastX;         // 0x14
    s16 prevLastY;         // 0x16
    u8 prevHoldFrames;     // 0x18
    u8 prevReleaseFrames;  // 0x19
    u8 prevIsTouch;        // 0x1A
    u8 prevIsChanged;      // 0x1B
    s16 prevY;             // 0x1C

    // held for at least 3 frames (game code that is not decompiled yet; name is ours)
    bool IsHeld() const; // 0x001B7ABC
};
ASSERT_SIZE(TouchState, 0x1E);

extern TouchState g_TouchState;
// the touch position in the coordinates of the lower screen layouts (set by the game code of g_TouchState)
extern nn::math::VEC2 g_TouchPosition;
