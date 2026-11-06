#pragma once

#include "decomp.h"
#include "Sound/dSoundFootstep.h"
#include "Sound/dSoundHum.h"
#include "Sound/dSoundObj.h"

// RTTI 11SoundObjNpc @ 0x008CB354
// vtable 0x008ECDC4 (vptr 0x008ECDCC), offset_to_top 0, 14 entries
// The sounds of a character with its footsteps and its humming; like SoundObjHold the position stays frozen while the fade of
// SoundMgr is down (member and method names are ours).
class SoundObjNpc : public ::SoundObj<2>
{
public:
    enum
    {
        NPC_NUM = 399, // the entries of the table of the characters (g_UnknownA84AEC)
    };

    SoundObjNpc(); // 0x001DA5D8
    // 0x001DA718
    // 0x001DA668 (deleting dtor)
    virtual ~SoundObjNpc();
    virtual void Start(const nn::math::VEC3* world, const nn::math::VEC3* relative) override; // 0x001DA258 slot 0x08
    virtual void Stop() override; // 0x001DA56C slot 0x0C
    virtual void SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative) override; // 0x001DA400 slot 0x10

    void SetFootstepSize(u32 size); // 0x001DA29C
    void Jump(u32 walkSound, u32 groundValue); // 0x001DA2AC
    void Land(u32 walkSound, u32 groundValue); // 0x001DA2E4
    void Walk(f32 speed, u32 walkSound, u32 groundValue); // 0x002A67B4
    // the size of the footsteps by the kind of the character (byte 2 of its entry)
    void SetNpc(u16 npcId); // 0x001DA31C
    // for this frame (SoundBgmMgr::m_IsFadeInRequested)
    DECOMP_NOINLINE void RequestBgmFadeIn(); // 0x001DA3E8

    SoundFootstep m_Footstep;    // 0x74
    SoundHum m_Hum;              // 0x8C
    bool m_IsFrozen;             // 0xD0
    nn::math::VEC3 m_FrozenPos;  // 0xD4
    u16 m_NpcId;                 // 0xE0
};
ASSERT_OFFSET(SoundObjNpc, m_Footstep, 0x74);
ASSERT_OFFSET(SoundObjNpc, m_Hum, 0x8C);
ASSERT_OFFSET(SoundObjNpc, m_NpcId, 0xE0);
ASSERT_SIZE(SoundObjNpc, 0xE4);
