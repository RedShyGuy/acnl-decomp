#include "fgobj/dObjectFall.h"
#include "field/dFdInfo.h"
#include "fgobj/fgobj_Api.h"

namespace fgobj {
namespace {
// the shadow: it appears at size 4 without opacity, then grows from frame 6 to the size of the items
const u8 SHADOW_FRAME_NUM = 15;
const u8 SHADOW_FRAME_GROW = 5;
const f32 SHADOW_START_SIZE = 4.0f;
const f32 SHADOW_FULL_SIZE = 8.0f;
} // namespace

// 0x005920B4 slot 0x18 (name is ours)
u8 fgobj::ObjectFall::GetFallDirection()
{
    return m_Direction;
}

// 0x005920BC slot 0x1C (name is ours)
void fgobj::ObjectFall::StartFall(u8 direction)
{
    s32 x = m_X;
    s32 z = m_Z;
    u8 state;
    if (direction != 255) {
        GetUnitOfDirection(&m_X, &m_Z, x, z, direction);
        FdInfo::ToCenterWorldWithUnitHeight(&m_Target, m_X, m_Z);
        state = STATE_FALL;
    } else {
        switch (m_Direction) {
        case DIRECTION_X_PLUS:
            m_X = x + 1;
            break;
        case DIRECTION_X_MINUS:
            m_X = x - 1;
            break;
        case DIRECTION_Z_PLUS:
            m_Z = z + 1;
            break;
        case DIRECTION_X_MINUS_Z_PLUS:
            m_X = x - 1;
            m_Z = z + 1;
            break;
        case DIRECTION_X_PLUS_Z_PLUS:
            m_X = x + 1;
            m_Z = z + 1;
            break;
        }
        FdInfo::ToCenterWorldWithUnitHeight(&m_Target, m_X, m_Z);
        state = STATE_FALL_TO_NEIGHBOUR;
    }
    m_State = state;
}

// 0x00592174 (name is ours)
void fgobj::ObjectFall::UpdateShadow()
{
    if (m_ShadowFrame >= SHADOW_FRAME_NUM) {
        return;
    }
    m_ShadowFrame++;
    if (m_ShadowFrame == 1) {
        InitializeShadow();
        m_ShadowSize = SHADOW_START_SIZE;
        m_Shadow.m_Opacity = 0.0f;
        return;
    }
    if (m_ShadowFrame <= SHADOW_FRAME_GROW) {
        return;
    }
    f32 t = (m_ShadowFrame - SHADOW_FRAME_GROW) * 0.1f;
    m_ShadowSize = (0.5f + t * 0.5f) * SHADOW_FULL_SIZE;
    f32 opacity;
    if (0.0f > t) {
        opacity = 0.0f;
    } else if (t > 1.0f) {
        opacity = 1.0f;
    } else {
        opacity = t;
    }
    m_Shadow.m_Opacity = opacity;
}

} // namespace fgobj
