#pragma once

#include "decomp.h"
#include "Sound/dSoundObjNpc.h"

// The sound object of a npc for the game (the class name is from the template argument of the RTTI of
// EmoticonActionEx<AudioObjNpc>; it has no virtual functions; the constructor lies in the code of the game, member names are ours).
class AudioObjNpc
{
public:
    void Start(const nn::math::VEC3* world); // 0x001C0DCC
    void SetHumAlwaysMadeUp(bool isAlwaysMadeUp); // 0x001C0E04
    void Walk(f32 speed, u32 walkSound, u32 groundValue); // 0x001C0E0C
    void SetFootstepVariable(s32 value); // 0x001C0E44

    SoundObjNpc m_Obj;  // 0x00
    f32 m_MaxSpeed; // 0xE4 of the steps
    bool m_IsStarted; // 0xE8
};
ASSERT_OFFSET(AudioObjNpc, m_MaxSpeed, 0xE4);
ASSERT_SIZE(AudioObjNpc, 0xEC);
