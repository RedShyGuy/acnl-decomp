#pragma once

#include "decomp.h"
#include "nw/anim/res/anim_ResAnim.h"
#include "nw/gfx/gfx_BaseAnimEvaluator.h"

namespace nw {
namespace gfx {
class BaseAnimEvaluator;
} // namespace gfx
} // namespace nw

namespace g3d {
// the frame of an animation evaluator: range, speed and play policy (no vtable of its own in the binary: only built
// as a base; names are ours)
// RTTI N3g3d15FrameControllerE @ 0x008D0CF0
class FrameController
{
public:
    // m_PlayMode
    enum PlayMode
    {
        PLAY_MODE_ONETIME = 0,
        PLAY_MODE_LOOP = 1,
        PLAY_MODE_NONE = 2,
    };

    // (inline)
    FrameController() : m_PlayMode(PLAY_MODE_NONE) {}
    virtual ~FrameController() {}
    virtual nw::gfx::BaseAnimEvaluator* GetEvaluator() = 0; // slot 0x08

    void SetFrameRange(f32 startFrame, f32 endFrame); // 0x004EEF64
    // 0: one time, 1: loop (other values only change m_PlayMode)
    void SetPlayMode(s32 playMode); // 0x004EEF8C
    // the play mode of the loop mode of the animation
    void InitPlayMode(nw::anim::res::ResAnim anim); // 0x004EEFE0
    void SetSpeed(f32 speed); // 0x004EF030
    // the frame through the play policy (it becomes the current frame; the cache is dirty)
    void SetFrame(f32 frame); // 0x004EF054

    // (inline; copies for the game) whether a one-time animation is at its end (the start frame for a negative speed)
    DECOMP_EMIT_COPY bool IsFinished(); // 0x00746DA0
    DECOMP_EMIT_COPY f32 GetSpeed(); // 0x00746E54
    DECOMP_EMIT_COPY f32 GetFrame(); // 0x00746E6C

    u8 m_PlayMode; // 0x4 PlayMode
};
ASSERT_SIZE(FrameController, 0x8);

inline bool FrameController::IsFinished()
{
    f32 speed = GetEvaluator()->m_AnimFrameController.m_Speed;
    if (m_PlayMode == PLAY_MODE_LOOP) {
        return false;
    }
    if (0.0f > speed) {
        f32 startFrame = GetEvaluator()->m_AnimFrameController.m_StartFrame;
        return GetEvaluator()->m_AnimFrameController.GetFrame() == startFrame;
    }
    f32 endFrame = GetEvaluator()->m_AnimFrameController.m_EndFrame;
    return GetEvaluator()->m_AnimFrameController.GetFrame() == endFrame;
}

inline f32 FrameController::GetSpeed()
{
    return GetEvaluator()->m_AnimFrameController.m_Speed;
}

inline f32 FrameController::GetFrame()
{
    return GetEvaluator()->m_AnimFrameController.GetFrame();
}
} // namespace g3d
