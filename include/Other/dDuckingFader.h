#pragma once

#include "decomp.h"
#include "Sound/dSoundFaderEx.h"

// RTTI 12DuckingFader @ 0x008CB5FC
// vtable 0x008EE418 (vptr 0x008EE420), offset_to_top 0, 5 entries
// A fader for ducking (lowering a volume and bringing it back to 1); keeps the phase it is in (names are ours).
class DuckingFader : public ::SoundFaderEx
{
public:
    enum State
    {
        STATE_NONE = 0,       // at 1 (not ducked)
        STATE_DOWN_WAIT = 1,  // ducking, waiting for the delay
        STATE_DOWN = 2,       // ducking, moving
        STATE_DUCKED = 3,     // ducked, still
        STATE_UP_WAIT = 4,    // back to 1, waiting for the delay
        STATE_UP = 5,         // back to 1, moving or arrived
    };

    DECOMP_EMIT_COPY DuckingFader() : m_State(STATE_NONE) {} // 0x0012F968
    virtual ~DuckingFader();
    virtual f32 Update() override; // 0x001FFFC8 slot 0x0C

    s32 GetState() const { return m_State; }

private:
    s32 m_State; // 0x18
};
ASSERT_SIZE(DuckingFader, 0x1C);
