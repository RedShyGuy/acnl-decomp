#include "fgobj/dObjectBury.h"
#include "fgobj/fgobj_Api.h"

namespace fgobj {
namespace {
// the scales of ANIMATION_SHRINK (read backwards)
// 0x008477A0
const f32 s_BuryScales[9] = {0.36f, 0.48f, 0.6f, 0.72f, 0.82f, 0.9f, 0.95f, 0.98f, 1.0f};

// the frames of the animations
const u8 SHRINK_FRAME_NUM = 9;
const u8 PRESS_FRAME_START = 6;
const u8 PRESS_FRAME_HOLD = 22;
const u8 PRESS_FRAME_END = 34;
const u8 PRESS_DEEP_FRAME_HOLD = 16;
const u8 PRESS_DEEP_FRAME_SHRINK = 21;
const u8 PRESS_DEEP_FRAME_END = 33;
const f32 PRESS_SCALE = 0.8f;
const f32 PRESS_DEEP_SCALE = 0.65f;
} // namespace

// 0x00591DEC slot 0x00
// 0x00591DBC (deleting dtor)
fgobj::ObjectBury::~ObjectBury()
{
    // (only the base is destroyed)
}

// 0x00591BCC slot 0x20 (name is ours)
s32 fgobj::ObjectBury::Execute()
{
    f32 scale = 1.0f;
    u8 frame = m_Frame;
    if (m_Animation == ANIMATION_PRESS) {
        frame++;
        m_Frame = frame;
        if (frame >= PRESS_FRAME_START) {
            if (frame < PRESS_FRAME_HOLD) {
                scale = PRESS_SCALE + (PRESS_FRAME_HOLD - frame) * (1.0f / 16.0f) * 0.2f;
            } else {
                scale = PRESS_SCALE;
                if (frame >= PRESS_FRAME_END) {
                    goto finish;
                }
            }
        }
    } else if (m_Animation == ANIMATION_PRESS_DEEP) {
        frame++;
        m_Frame = frame;
        if (frame >= PRESS_FRAME_START) {
            if (frame <= PRESS_DEEP_FRAME_HOLD) {
                scale -= (frame - PRESS_FRAME_START) * 0.035f;
            } else {
                scale = PRESS_DEEP_SCALE;
                if (frame >= PRESS_DEEP_FRAME_SHRINK) {
                    if (frame >= PRESS_DEEP_FRAME_END) {
                        goto finish;
                    }
                    scale = (PRESS_DEEP_FRAME_END - frame) * scale * (1.0f / 12.0f);
                }
            }
        }
    } else {
        if (frame >= SHRINK_FRAME_NUM) {
            goto finish;
        }
        scale = s_BuryScales[SHRINK_FRAME_NUM - 1 - frame];
        nn::math::VEC3 scales(scale, scale, scale);
        m_Model.SetScale(scales);
        m_Model.Update();
        m_Frame++;
        return EXECUTE_CONTINUE;
    }
    {
        nn::math::VEC3 scales(scale, scale, scale);
        m_Model.SetScale(scales);
        m_Model.Update();
        return EXECUTE_CONTINUE;
    }

finish:
    if (m_FinishMode == FINISH_MODE_KEEP) {
        return EXECUTE_FINISHED_KEEP_REQUEST;
    }
    if (m_FinishMode == FINISH_MODE_TAKE_UP) {
        PlaceBuryResult(m_X, m_Z);
        return EXECUTE_FINISHED_KEEP_REQUEST;
    }
    return EXECUTE_FINISHED;
}

} // namespace fgobj
