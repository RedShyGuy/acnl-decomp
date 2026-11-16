#pragma once

#include "decomp.h"
#include "fgobj/dObjectMove.h"

namespace fgobj {
// A field object that falls onto a unit, bounces once onto the neighbour unit of its direction and fades its shadow in
// (no vtable of its own in the binary: only built as a base; the class name is from the RTTI, the rest is ours).
// RTTI N5fgobj10ObjectFallE @ 0x008D2934
class ObjectFall : public ::fgobj::ObjectMove
{
public:
    // m_State (StartFall)
    enum State : u8
    {
        STATE_FALL = 0,         // to the unit
        STATE_BOUNCE = 1,       // after the first bounce
        STATE_FALL_TO_NEIGHBOUR = 2,    // to the neighbour unit of m_Direction
        STATE_BOUNCE_TO_NEIGHBOUR = 3,  // after the bounce there (then the object has no unit)
    };
    // the directions of m_Direction (the neighbour unit)
    enum Direction : u8
    {
        DIRECTION_X_PLUS = 0,
        DIRECTION_X_MINUS = 1,
        DIRECTION_Z_PLUS = 2,
        DIRECTION_X_MINUS_Z_PLUS = 3,
        DIRECTION_X_PLUS_Z_PLUS = 4,
    };

    u8 GetFallDirection() override; // 0x005920B4 slot 0x18
    void StartFall(u8 direction) override; // 0x005920BC slot 0x1C

    // the shadow appears and grows in the first 15 frames
    void UpdateShadow(); // 0x00592174

    u8 m_State;         // 0x140 State
    u8 m_Direction;     // 0x141 Direction
    u8 m_ShadowFrame;   // 0x142
};
ASSERT_OFFSET(ObjectFall, m_State, 0x140);
} // namespace fgobj
