#pragma once

#include "decomp.h"
#include "Sound/dSoundInstrument.h"
#include "Sound/dSoundObjFurniture.h"

// RTTI 21SoundObjFurnitureInst @ 0x008CCD40
// vtable 0x008F658C (vptr 0x008F6594), offset_to_top 0, 16 entries
// An instrument as furniture: its sound plays notes of the chord of the room music (method names are ours).
class SoundObjFurnitureInst : public ::SoundObjFurniture
{
public:
    SoundObjFurnitureInst(); // 0x0032CD78
    // 0x0032CDF0
    // 0x0032CD98 (deleting dtor)
    virtual ~SoundObjFurnitureInst();
    virtual void Stop() override; // 0x0032CD18 slot 0x0C
    virtual void Update(const nn::math::VEC3* world, const nn::math::VEC3* relative, f32 frame) override; // 0x002E9DAC slot 0x3C

    void InitializeInstrument(); // 0x0032CCC0
    void PlayNoteAt(s16 note); // 0x0032CCF4
    u8 PlayNote(); // 0x0032CD54

    SoundInstrument m_Instrument; // 0x90
};
ASSERT_SIZE(SoundObjFurnitureInst, 0xB4);
