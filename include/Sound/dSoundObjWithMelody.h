#pragma once

#include "decomp.h"
#include "Sound/dSoundMelodyPlayerOneSeqPos.h"
#include "Sound/dSoundMelodyPlayerPos.h"
#include "Sound/dSoundObj.h"

// RTTI 18SoundObjWithMelody @ 0x008CC904
// vtable 0x008F4B8C (vptr 0x008F4B94), offset_to_top 0, 14 entries
// A sound source that plays melodies at its position: a melody of notes and a door chime (member names are ours).
class SoundObjWithMelody : public ::SoundObj<1>
{
public:
    SoundObjWithMelody(); // 0x002E987C
    // 0x002E98F4
    // 0x002E98E0 (deleting dtor)
    virtual ~SoundObjWithMelody();
    virtual void Start(const nn::math::VEC3* world, const nn::math::VEC3* relative) override; // 0x002E96E4 slot 0x08
    virtual void Stop() override; // 0x002E9834 slot 0x0C
    virtual void SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative) override; // 0x002E9760 slot 0x10

    void PlayDoorChime(u32 soundId); // 0x002E9740
    void PlayDoorChime(u32 soundId, const u8* notes); // 0x002E9748

    SoundMelodyPlayerPos m_Melody;          // 0x5C
    SoundMelodyPlayerOneSeqPos m_DoorChime; // 0xB8
};
ASSERT_OFFSET(SoundObjWithMelody, m_DoorChime, 0xB8);
ASSERT_SIZE(SoundObjWithMelody, 0xDC);
