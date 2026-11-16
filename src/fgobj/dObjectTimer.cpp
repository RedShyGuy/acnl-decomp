#include "fgobj/dObjectTimer.h"

namespace fgobj {
namespace {
// the shadow appears at this frame and fades in until the second one
const u8 SHADOW_START_FRAME = 10;
const u8 SHADOW_END_FRAME = 22;
} // namespace

// 0x00594C4C slot 0x10
bool fgobj::ObjectTimer::vf_0x10()
{
    return true;
}

// 0x00594C54 slot 0x20 (name is ours)
s32 fgobj::ObjectTimer::Execute()
{
    m_Frame++;
    if (m_HasShadow) {
        m_Frame++;
        if (m_Frame >= SHADOW_START_FRAME) {
            f32 opacity = 1.0f;
            if (m_Frame == SHADOW_START_FRAME) {
                InitializeShadow();
                opacity = 0.0f;
            } else if (m_Frame < SHADOW_END_FRAME) {
                f32 t = (m_Frame - SHADOW_START_FRAME) * (1.0f / 12.0f);
                if (t < 0.0f) {
                    t = 0.0f;
                } else if (t > 1.0f) {
                    t = 1.0f;
                }
                opacity = t;
            }
            m_Shadow.m_Opacity = opacity;
        }
    }
    UpdateMatrix(true, 0.0f);
    if (m_Timer != 0) {
        m_Timer--;
        return EXECUTE_CONTINUE;
    }
    return EXECUTE_FINISHED_KEEP_REQUEST;
}

// 0x00594D5C slot 0x00
// 0x00594D2C (deleting dtor)
fgobj::ObjectTimer::~ObjectTimer()
{
    // (only the base is destroyed)
}

} // namespace fgobj
