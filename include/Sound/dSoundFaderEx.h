#pragma once

#include "decomp.h"
#include "Sound/dSoundFader.h"

// RTTI 12SoundFaderEx @ 0x008CB80C
// vtable 0x008EE81C (vptr 0x008EE824), offset_to_top 0, 5 entries
// A fader that waits some frames before it moves (names are ours).
class SoundFaderEx : public ::SoundFader
{
public:
    DECOMP_EMIT_COPY SoundFaderEx() : m_Delay(0) {} // 0x00135F64
    virtual ~SoundFaderEx();
    virtual void Reset(f32 value) override; // 0x0012F988 slot 0x08
    virtual f32 Update() override; // 0x0020A9D4 slot 0x0C
    virtual s32 GetRemainingFrames() const override; // 0x007132C8 slot 0x10 (a copy in the shared area)

    // move to the target in the frames after the delay
    void SetTarget(f32 target, s32 frames, s32 delay); // 0x0020AA30

protected:
    s32 m_Delay; // 0x14 frames before the movement starts
};
ASSERT_SIZE(SoundFaderEx, 0x18);
