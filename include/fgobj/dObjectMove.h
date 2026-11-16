#pragma once

#include "decomp.h"
#include "fgobj/dObjectExe.h"
#include "nn/math/math_Vector3.h"

namespace fgobj {
// A field object that moves to a target under gravity (no vtable of its own in the binary: only built as a base; the
// class name is from the RTTI, the rest is ours).
// RTTI N5fgobj10ObjectMoveE @ 0x008D2958
class ObjectMove : public ::fgobj::ObjectExe
{
public:
    // the velocity that brings the object to the target in frames: horizontally ratio of the way per frame, vertically
    // the speed that reaches the height of the target after the frames under m_Gravity
    void SetVelocityToTarget(s32 frames, f32 ratio); // 0x00592568

    nn::math::VEC3 m_Target;    // 0x124
    nn::math::VEC3 m_Velocity;  // 0x130
    f32 m_Gravity;              // 0x13C taken from the y velocity every frame
};
ASSERT_OFFSET(ObjectMove, m_Target, 0x124);
ASSERT_SIZE(ObjectMove, 0x140);
} // namespace fgobj
