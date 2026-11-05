#pragma once

#include "decomp.h"
#include "nw/snd/snd_SoundHandle.h"

// RTTI 27SoundMelodyPlayerOneSeqBase @ 0x008CD0F8
// vtable 0x008F80C4 (vptr 0x008F80CC), offset_to_top 0, 3 entries
// Plays a melody with one sequence: the 16 notes go into the sequence variables 0..15 as pitches (500: hold, 1000: rest)
// (the class name is from the RTTI, everything else is ours).
class SoundMelodyPlayerOneSeqBase
{
public:
    // m_State
    enum State
    {
        STATE_NONE = 0,
        STATE_PLAY = 1,
        STATE_STOP = 2,
    };

    SoundMelodyPlayerOneSeqBase(); // 0x00136128
    virtual ~SoundMelodyPlayerOneSeqBase(); // 0x0034350C slot 0x00
    // starts the sequence with the notes
    virtual void Setup(); // 0x0034337C slot 0x08

    // the melody (NULL: the melody of SoundMelodyMgr) with the sequence soundId, if its data is loaded
    void Start(u32 soundId, const u8* notes); // 0x003432D0
    void Stop(int fadeFrames); // 0x003432A4
    void Update(); // 0x00343498
    // stops the sound of the door chime of the convenience store
    void StopDoorChime(); // 0x003434BC

    DECOMP_EMIT_COPY DECOMP_NOINLINE void StopSound() // 0x0013F63C
    {
        m_Handle.Stop(0);
    }

    const u8* m_pNotes;            // 0x04
    nw::snd::SoundHandle m_Handle; // 0x08
    u32 m_SoundId;                 // 0x0C
    u8 m_State;                    // 0x10 State
    bool m_CanRestart;             // 0x11
};
ASSERT_SIZE(SoundMelodyPlayerOneSeqBase, 0x14);
