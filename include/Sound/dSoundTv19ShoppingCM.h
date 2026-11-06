#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 19SoundTv19ShoppingCM @ 0x008CCA88
// vtable 0x008F5370 (vptr 0x008F5378), offset_to_top 0, 5 entries
// A TV program (the class name is from the RTTI, everything else is ours).
class SoundTv19ShoppingCM : public ::SoundTvBase
{
public:
    // (embedded in SoundTvHandle::Create<SoundTv19ShoppingCM> 0x00820610)
    SoundTv19ShoppingCM() : m_Volume(1.0f), m_Scene(-1) {}
    // 0x002FFF64
    // 0x002FFF54 (deleting dtor)
    virtual ~SoundTv19ShoppingCM();
    virtual void Start() override; // 0x002FF22C slot 0x08
    virtual void UpdateProgram(s32 frame) override; // 0x002FFA60 slot 0x10
    DECOMP_NOINLINE void SetScene(s8 scene, s8 variant); // 0x002FFEBC

    f32 m_Volume; // 0xE4 of the music
    s8 m_Scene;   // 0xE8 the scene of the voice (sequence variables 0 and 1)
};
ASSERT_OFFSET(SoundTv19ShoppingCM, m_Volume, 0xE4);
ASSERT_OFFSET(SoundTv19ShoppingCM, m_Scene, 0xE8);
ASSERT_SIZE(SoundTv19ShoppingCM, 0xEC);
