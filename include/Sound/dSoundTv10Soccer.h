#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 15SoundTv10Soccer @ 0x008CC154
// vtable 0x008F1F58 (vptr 0x008F1F60), offset_to_top 0, 5 entries
// A TV program (the class name is from the RTTI, everything else is ours).
class SoundTv10Soccer : public ::SoundTvBase
{
public:
    enum
    {
        KIND_NUM = 5,
        STATE_GOAL = 15,
        STATE_MISS = 16,
    };

    // (embedded in SoundTvHandle::Create<SoundTv10Soccer> 0x00820250)
    SoundTv10Soccer() : m_IsChanged(0), m_State(0), m_LastState(0), m_StateFrames(0) {}
    // 0x002A6450
    // 0x002A6440 (deleting dtor)
    virtual ~SoundTv10Soccer();
    virtual void Start() override; // 0x002A5CDC slot 0x08
    virtual void OnLoaded() override; // 0x002A6430 slot 0x0C
    virtual void UpdateProgram(s32 frame) override; // 0x002A5D5C slot 0x10

    u8 m_IsChanged;    // 0xE1 the play changes in this frame
    u16 m_State;       // 0xE2 group * 5 + kind of play (kind 0 normal, 1 call, 2 song, 3 fine play, 4 rough play; group 0 run, 1 talk, 2 run near the goal), 15 goal, 16 miss
    u16 m_LastState;   // 0xE4
    u16 m_StateFrames; // 0xE6
};
ASSERT_OFFSET(SoundTv10Soccer, m_IsChanged, 0xE1);
ASSERT_OFFSET(SoundTv10Soccer, m_State, 0xE2);
ASSERT_OFFSET(SoundTv10Soccer, m_LastState, 0xE4);
ASSERT_OFFSET(SoundTv10Soccer, m_StateFrames, 0xE6);
ASSERT_SIZE(SoundTv10Soccer, 0xE8);
