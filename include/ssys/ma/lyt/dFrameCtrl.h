#pragma once

#include "decomp.h"
#include "nw/lyt/lyt_AnimTransform.h"

namespace ssys {
namespace ma {
namespace lyt {
// the frame of an animation (no vtable of its own in the binary: only built as a base; names after libgarden except
// the flags and the Set*Parameters)
// RTTI N4ssys2ma3lyt9FrameCtrlE @ 0x008D24C0
class FrameCtrl
{
public:
    // the bits of m_Loop (names are ours)
    enum LoopFlag : u8
    {
        LOOP_FLAG_ONCE = 1 << 0,     // stops at the end (else it wraps)
        LOOP_FLAG_REVERSE = 1 << 1,  // runs backwards
    };

    // (inline)
    FrameCtrl()
        : m_FrameCount(0.0f), m_Frame(0.0f), m_PrevFrame(0.0f), m_FrameIncrease(1.0f), m_Loop(0), m_pAnimTransform(NULL)
    {
    }
    virtual ~FrameCtrl() {}
    // (libgarden marks it final; ARMCC calls it virtually)
    virtual void SetFrame(f32 frame); // 0x0056A2C8 slot 0x08

    void SetFirstFrame() { SetFrame(0.0f); }
    void SetFrameR(f32 r) { SetFrame(m_FrameCount - r); }
    DECOMP_EMIT_COPY void SetLastFrame() // 0x00569FF0
    {
        SetFrame(m_FrameCount - 1.0f);
        if (m_pAnimTransform != NULL) {
            m_pAnimTransform->SetFrame(m_Frame);
        }
    }
    nw::lyt::AnimTransform* GetAnimTransform() const { return m_pAnimTransform; }
    f32 GetFrame() const { return m_Frame; }
    f32 GetFrameCount() const { return m_FrameCount; }
    void SetAnimTransform(nw::lyt::AnimTransform* anim); // 0x0056A25C | libgarden [tier A]
    // (names are ours)
    void SetPlayParameters(f32 frameCount, f32 frameIncrease, f32 frame, u8 loop); // 0x0056A2E0
    void SetPlayParametersNoUpdate(f32 frameCount, f32 frameIncrease, f32 frame, u8 loop); // 0x0056A62C
    DECOMP_EMIT_COPY void SetFrameIncrease(f32 increase) { m_FrameIncrease = increase; } // 0x0056A658

    f32 m_FrameCount;                          // 0x04
    f32 m_Frame;                               // 0x08
    f32 m_PrevFrame;                           // 0x0C
    f32 m_FrameIncrease;                       // 0x10
    u8 m_Loop;                                 // 0x14 LoopFlag
    nw::lyt::AnimTransform* m_pAnimTransform;  // 0x18
};
ASSERT_SIZE(FrameCtrl, 0x1C);
} // namespace lyt
} // namespace ma
} // namespace ssys
