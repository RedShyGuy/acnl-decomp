#pragma once

#include "decomp.h"
#include "Sound/dSoundObjPlayer.h"

// The sound object of a player for the game (the class name is from the template argument of the RTTI of
// EmoticonActionEx<AudioObjPlayer>; it has no virtual functions; the constructor lies in the code of the game, member names are ours).
class AudioObjPlayer
{
public:
    void Start(const nn::math::VEC3* world); // 0x00250190
    void Walk(f32 speed, u32 walkSound, u32 groundValue); // 0x002501C8
    void SetIsIgnoreMaster(bool isIgnoreMaster); // 0x00250200

    SoundObjPlayer m_Obj;  // 0x00
    f32 m_MaxSpeed; // 0xC0 of the steps
    bool m_IsStarted; // 0xC4
};
ASSERT_OFFSET(AudioObjPlayer, m_MaxSpeed, 0xC0);
ASSERT_SIZE(AudioObjPlayer, 0xC8);
