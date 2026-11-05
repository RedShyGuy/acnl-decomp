#pragma once

#include "decomp.h"

// RTTI 10SoundFader @ 0x008CB1E8
// vtable 0x008EC49C (vptr 0x008EC4A4), offset_to_top 0, 5 entries
// A value that moves linearly to a target over a number of frames (member and method names are ours).
class SoundFader
{
public:
    DECOMP_EMIT_COPY SoundFader() : m_Value(0.0f), m_Target(0.0f), m_Step(0.0f), m_Frames(-1) {} // 0x0012F8B8
    virtual ~SoundFader();
    // the value and the target, no movement
    virtual void Reset(f32 value); // 0x0012F8A4 slot 0x08
    // one frame; returns the value
    virtual f32 Update(); // 0x001B0AC0 slot 0x0C
    // the frames until the target (-1: none)
    virtual s32 GetRemainingFrames() const { return m_Frames; } // 0x0070BDF4 slot 0x10

    // move to the target in the frames (0: at once)
    void SetTarget(f32 target, s32 frames); // 0x001B0B00

    f32 GetValue() const { return m_Value; }
    f32 GetTarget() const { return m_Target; }

protected:
    f32 m_Value;  // 0x04
    f32 m_Target; // 0x08
    f32 m_Step;   // 0x0C per frame
    s32 m_Frames; // 0x10 left until the target, -1: none
};
ASSERT_SIZE(SoundFader, 0x14);
