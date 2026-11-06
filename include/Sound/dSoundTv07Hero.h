#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 13SoundTv07Hero @ 0x008CBAF8
// vtable 0x008EF838 (vptr 0x008EF840), offset_to_top 0, 5 entries
// A TV program (the class name is from the RTTI, everything else is ours).
class SoundTv07Hero : public ::SoundTvBase
{
public:
    // (embedded in SoundTvHandle::Create<SoundTv07Hero> 0x0081FEDC)
    SoundTv07Hero() : m_IsTrackMuted(0), m_ChangeCount(0), m_LastScene(3), m_Scene(3) {}
    // 0x00248744
    // 0x00248734 (deleting dtor)
    virtual ~SoundTv07Hero();
    virtual void Start() override; // 0x0024837C slot 0x08
    virtual void OnLoaded() override; // 0x00248724 slot 0x0C
    virtual void UpdateProgram(s32 frame) override; // 0x00248400 slot 0x10

    u16 m_IsTrackMuted; // 0xE2 the track bits 63 of the music are muted (changes every second scene change)
    u16 m_ChangeCount;  // 0xE4
    u16 m_LastScene;    // 0xE6
    u16 m_Scene;        // 0xE8 0 none, 1 laugh, 2 power up, 3 run with bombs, 4 run with wind, 5 BGM3 with bombs
};
ASSERT_OFFSET(SoundTv07Hero, m_IsTrackMuted, 0xE2);
ASSERT_OFFSET(SoundTv07Hero, m_ChangeCount, 0xE4);
ASSERT_OFFSET(SoundTv07Hero, m_LastScene, 0xE6);
ASSERT_OFFSET(SoundTv07Hero, m_Scene, 0xE8);
ASSERT_SIZE(SoundTv07Hero, 0xEC);
