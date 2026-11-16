#include "fgobj/dObjectMove.h"

namespace fgobj {
// 0x00592568 (name is ours)
void fgobj::ObjectMove::SetVelocityToTarget(s32 frames, f32 ratio)
{
    nn::math::VEC3 way = m_Target - m_Position;
    m_Velocity = way * (ratio / frames);
    f32 y = m_Position.y;
    f32 speed = 0.0f;
    for (s32 i = 0; i < frames; i++) {
        speed -= m_Gravity;
        y += speed;
    }
    m_Velocity.y = (m_Target.y - y) / frames;
}

} // namespace fgobj
