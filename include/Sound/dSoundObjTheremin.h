#pragma once

#include "decomp.h"
#include "Sound/dSoundObjFurniture.h"
#include "nw/snd/snd_SoundHandle.h"

// The sound of the theremin among the furniture (BsFtrMgr +0x7B4; code at 0x00247520..0x002478A4; no RTTI, names are
// ours): SE_FUR_THEREMIN plays through the furniture object while the player is closer than 200 to the nearest
// theremin, its volume, pitch and sequence variables follow the distance.
class SoundObjTheremin
{
public:
    SoundObjTheremin(); // 0x00247850
    ~SoundObjTheremin(); // 0x0024786C

    // the furniture object starts (off)
    void Start(); // 0x00247520
    void Update(const nn::math::VEC3* position, f32 distance); // 0x00247554
    // the sound stops (after a frame with sound the playing flag is cleared first)
    void StopSound(); // 0x002477FC
    void Stop(); // 0x00247838

    nw::snd::SoundHandle m_Handle;   // 0x00
    SoundObjFurniture m_Obj;         // 0x04
    bool m_IsPlaying;                // 0x94
    u8 _95[3];

    // (the statics of the file; initializer 0x00796D48)
    static nn::math::VEC3 s_Position;
    struct SoundParam
    {
        u32 soundId; // 0x0 SE_FUR_THEREMIN
        f32 unk4;    // 0x4
    };
    static SoundParam s_SoundParam;
};
ASSERT_SIZE(SoundObjTheremin, 0x98);
