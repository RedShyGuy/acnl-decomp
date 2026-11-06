#pragma once

#include "decomp.h"
#include "Sound/dSoundFader.h"
#include "Sound/dSoundObjFurniture.h"
#include "nn/math/math_Vector2.h"

// RTTI 22SoundObjFurnitureOrgel @ 0x008CCE5C
// vtable 0x008F6CA0 (vptr 0x008F6CA8), offset_to_top 0, 16 entries
// A music box: it plays one of the songs of K.K. (SoundMuInfo) as its own sequence; after a while it winds down and the
// tempo falls to 40 (member and method names are ours).
class SoundObjFurnitureOrgel : public ::SoundObjFurniture
{
public:
    // m_State
    enum State
    {
        STATE_NONE = 0,
        STATE_WAITING = 1,
        STATE_PLAYING = 2,
        STATE_PAUSED = 3,
        STATE_WAITING_PAUSED = 4,
    };
    struct Loop
    {
        u32 start; // 0x0
        u32 end;   // 0x4
    };

    SoundObjFurnitureOrgel(); // 0x00332B38
    // 0x00332C14
    // 0x00332BA8 (deleting dtor)
    virtual ~SoundObjFurnitureOrgel();
    virtual void Stop() override; // 0x00332AA8 slot 0x0C
    virtual void Update(const nn::math::VEC3* world, const nn::math::VEC3* relative, f32 frame) override; // 0x003328F4 slot 0x3C

    void Pause(bool isPause); // 0x00332370
    // wait < 0: the frames until it starts, else the frame it starts at
    void Play(u32 song, s32 wait); // 0x00332494
    // the frames a song plays
    static u32 GetSongFrames(u32 song); // 0x003324CC
    DECOMP_NOINLINE void Start(u32 song, u32 frame); // 0x003324E0
    void StopSong(); // 0x00332AF4
    // the volume of the song while it winds down (a copy in the shared area)
    DECOMP_EMIT_COPY DECOMP_NOINLINE f32 GetFadeVolume(u32 song, u32 frame) // 0x007264E4
    {
        if (frame < 1770) {
            return 1.0f;
        }
        if (frame >= 2760) {
            return 0.0f;
        }
        return (2760 - frame) * (1.0f / 990.0f);
    }

    static Loop s_Loops[91];
    static nn::math::VEC2 s_WindDown;

    s32 m_State;                  // 0x90 State
    nw::snd::SoundHandle m_Handle; // 0x94
    u32 m_Song;                   // 0x98
    u32 m_SoundId;                // 0x9C
    u32 m_Frame;                  // 0xA0
    s32 m_Wait;                   // 0xA4
    u32 m_Tempo;                  // 0xA8
    SoundFader m_Fader;           // 0xAC
    f32 m_TickRate;               // 0xC0
    f32 m_WindDownTicks;          // 0xC4
};
ASSERT_OFFSET(SoundObjFurnitureOrgel, m_Fader, 0xAC);
ASSERT_SIZE(SoundObjFurnitureOrgel, 0xC8);
