#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 14SoundTv18UfoCM @ 0x008CBE14
// vtable 0x008F090C (vptr 0x008F0914), offset_to_top 0, 5 entries
// A TV program (the class name is from the RTTI, everything else is ours).
class SoundTv18UfoCM : public ::SoundTvBase
{
public:
    // (embedded in SoundTvHandle::Create<SoundTv18UfoCM> 0x00820180)
    SoundTv18UfoCM() : m_Volume(-1.0f), m_Scene(0), m_Phrase(-1) {}
    // 0x00279C58
    // 0x00279C48 (deleting dtor)
    virtual ~SoundTv18UfoCM();
    virtual void Start() override; // 0x002796F0 slot 0x08
    virtual void OnLoaded() override; // 0x00279B98 slot 0x0C
    virtual void UpdateProgram(s32 frame) override; // 0x00279778 slot 0x10
    void Talk(s8 phrase, s8 variant); // 0x00279B9C

    f32 m_Volume; // 0xE4 of the hum of the UFO
    u8 m_Scene;   // 0xE8 1 comes, 2 the alien appears, 3 talks, 4 disappears, 5 leaves
    s8 m_Phrase;  // 0xE9 the last phrase of the alien
};
ASSERT_OFFSET(SoundTv18UfoCM, m_Volume, 0xE4);
ASSERT_OFFSET(SoundTv18UfoCM, m_Scene, 0xE8);
ASSERT_OFFSET(SoundTv18UfoCM, m_Phrase, 0xE9);
ASSERT_SIZE(SoundTv18UfoCM, 0xEC);
