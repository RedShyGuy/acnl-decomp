#pragma once

#include "decomp.h"
#include "nw/anim/anim_AnimFrame.h"
#include "nw/anim/anim_Api.h"

namespace nw {
namespace anim {
// RTTI N2nw4anim19AnimFrameControllerE @ 0x008D0BD0
// vtable 0x0090364C (vptr 0x00903654), offset_to_top 0, 2 entries
// The frame of an animation with its range, speed and PlayPolicyFunc, which maps the advanced frame
// into the range (member names are ours).
class AnimFrameController
{
public:
    // (inline)
    AnimFrameController()
        : m_Speed(1.0f), m_StartFrame(0.0f), m_EndFrame(1.0f), m_PlayPolicy(PlayPolicy_Onetime), m_pUserData(nullptr)
    {
    }
    virtual ~AnimFrameController();

    // (inline)
    f32 GetFrame() const { return m_Frame.GetFrame(); }
    f32 GetSpeed() const { return m_Speed; }
    // advances the frame by the speed
    void UpdateFrame()
    {
        m_Frame.UpdateFrame(m_PlayPolicy(m_StartFrame, m_EndFrame, m_Frame.GetFrame() + m_Speed, m_pUserData));
    }

    AnimFrame m_Frame;           // 0x4
    f32 m_Speed;                 // 0x10
    f32 m_StartFrame;            // 0x14
    f32 m_EndFrame;              // 0x18
    PlayPolicyFunc m_PlayPolicy; // 0x1C
    void* m_pUserData;           // 0x20
};
ASSERT_SIZE(AnimFrameController, 0x24);
} // namespace anim
} // namespace nw
