#include "g3d/dFrameController.h"
#include "nw/anim/anim_Api.h"
#include "nw/gfx/gfx_BaseAnimEvaluator.h"

namespace g3d {
// 0x004EEF64 (name is ours)
void g3d::FrameController::SetFrameRange(f32 startFrame, f32 endFrame)
{
    nw::anim::AnimFrameController& frameController = GetEvaluator()->m_AnimFrameController;
    frameController.m_StartFrame = startFrame;
    frameController.m_EndFrame = endFrame;
}

// 0x004EEF8C (name is ours)
void g3d::FrameController::SetPlayMode(s32 playMode)
{
    switch (playMode) {
    case PLAY_MODE_ONETIME:
        GetEvaluator()->m_AnimFrameController.m_PlayPolicy = nw::anim::PlayPolicy_Onetime;
        break;
    case PLAY_MODE_LOOP:
        GetEvaluator()->m_AnimFrameController.m_PlayPolicy = nw::anim::PlayPolicy_Loop;
        break;
    default:
        break;
    }
    m_PlayMode = playMode;
}

// 0x004EEFE0 (name is ours)
void g3d::FrameController::InitPlayMode(nw::anim::res::ResAnim anim)
{
    s32 playMode;
    if (anim.GetLoopMode() != nw::anim::res::ResAnimData::LOOP_MODE_ONETIME) {
        playMode = PLAY_MODE_LOOP;
        GetEvaluator()->m_AnimFrameController.m_PlayPolicy = nw::anim::PlayPolicy_Loop;
    } else {
        playMode = PLAY_MODE_ONETIME;
        GetEvaluator()->m_AnimFrameController.m_PlayPolicy = nw::anim::PlayPolicy_Onetime;
    }
    m_PlayMode = playMode;
}

// 0x004EF030 (name is ours)
void g3d::FrameController::SetSpeed(f32 speed)
{
    GetEvaluator()->m_AnimFrameController.m_Speed = speed;
}

// 0x004EF054 (name is ours)
void g3d::FrameController::SetFrame(f32 frame)
{
    nw::gfx::BaseAnimEvaluator* evaluator = GetEvaluator();
    nw::anim::AnimFrameController& frameController = evaluator->m_AnimFrameController;
    frameController.m_Frame.UpdateFrame(frameController.m_PlayPolicy(frameController.m_StartFrame, frameController.m_EndFrame,
                                                                     frame, frameController.m_pUserData));
    evaluator->m_IsCacheDirty = true;
}

} // namespace g3d
