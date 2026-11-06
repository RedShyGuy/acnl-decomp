#pragma once

#include "decomp.h"
#include "Sound/dSoundFootstep.h"
#include "Sound/dSoundObj.h"

// RTTI 14SoundObjPlayer @ 0x008CBDC0
// vtable 0x008F0800 (vptr 0x008F0808), offset_to_top 0, 14 entries
// The sounds of a player with the footsteps; the object of the own player is the listener (member and method names are ours).
class SoundObjPlayer : public ::SoundObj<4>
{
public:
    SoundObjPlayer(); // 0x002785A0
    // 0x00278700
    // 0x00278608 (deleting dtor)
    virtual ~SoundObjPlayer();
    virtual void Start(const nn::math::VEC3* world, const nn::math::VEC3* relative) override; // 0x00278354 slot 0x08
    virtual void Stop() override; // 0x0027853C slot 0x0C
    virtual void SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative) override; // 0x00278448 slot 0x10

    void Walk(f32 speed, u32 walkSound, u32 groundValue); // 0x00278394
    void Jump(u32 walkSound, u32 groundValue); // 0x002783DC
    void Land(u32 walkSound, u32 groundValue); // 0x00278414

    SoundFootstep m_Footstep; // 0xA4
    bool m_IsListener;        // 0xBC SoundMgr::SetListenerPos follows the object
};
ASSERT_OFFSET(SoundObjPlayer, m_Footstep, 0xA4);
ASSERT_SIZE(SoundObjPlayer, 0xC0);
