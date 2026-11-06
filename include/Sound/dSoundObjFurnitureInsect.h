#pragma once

#include "decomp.h"
#include "Sound/dSoundObjFurniture.h"

// RTTI 23SoundObjFurnitureInsect @ 0x008CCF40
// vtable 0x008F72E4 (vptr 0x008F72EC), offset_to_top 0, 16 entries
// An insect kept as furniture: the sequence of its loop tells with variable 13 whether it sings (member names are ours).
class SoundObjFurnitureInsect : public ::SoundObjFurniture
{
public:
    SoundObjFurnitureInsect(); // 0x00335B68
    // 0x002D3158
    // 0x00335B88 (deleting dtor)
    virtual ~SoundObjFurnitureInsect();
    virtual void Update(const nn::math::VEC3* world, const nn::math::VEC3* relative, f32 frame) override; // 0x00335AB4 slot 0x3C

    bool m_IsSinging; // 0x90
};
ASSERT_SIZE(SoundObjFurnitureInsect, 0x94);
