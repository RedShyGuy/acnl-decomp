#pragma once

#include "decomp.h"

namespace nw {
namespace anim {
// RTTI N2nw4anim9AnimFrameE @ 0x008D0BE4
// vtable 0x00903680 (vptr 0x00903688), offset_to_top 0, 2 entries
// The frame of an animation and the one before the last update (a member of AnimFrameController;
// member names are ours).
class AnimFrame
{
public:
    // (inline)
    AnimFrame() : m_Frame(0.0f), m_PrevFrame(0.0f) {}
    virtual ~AnimFrame();

    // (inline) a new frame; the current one becomes the previous one
    void UpdateFrame(f32 frame)
    {
        m_PrevFrame = m_Frame;
        m_Frame = frame;
    }
    // (inline) jumps to a frame (also the previous one)
    void ResetFrame(f32 frame)
    {
        m_PrevFrame = frame;
        m_Frame = frame;
    }
    f32 GetFrame() const { return m_Frame; }

    f32 m_Frame;     // 0x4
    f32 m_PrevFrame; // 0x8
};
ASSERT_SIZE(AnimFrame, 0xC);
} // namespace anim
} // namespace nw
