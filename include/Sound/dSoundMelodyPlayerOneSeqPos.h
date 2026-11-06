#pragma once

#include "decomp.h"
#include "Sound/dSoundMelodyPlayerOneSeqBase.h"

class SoundObjBase;

// RTTI 26SoundMelodyPlayerOneSeqPos @ 0x008CD0C8
// vtable 0x008F7ED4 (vptr 0x008F7EDC), offset_to_top 0, 3 entries
// The one-sequence melody player with the volume and the pans of a sound object (the class name is from the RTTI,
// everything else is ours).
class SoundMelodyPlayerOneSeqPos : public ::SoundMelodyPlayerOneSeqBase
{
public:
    SoundMelodyPlayerOneSeqPos(); // 0x00342BD8
    // 0x00342C40
    // 0x00342C18 (deleting dtor)
    virtual ~SoundMelodyPlayerOneSeqPos();

    // the values of the object, then SoundMelodyPlayerOneSeqBase::Update
    void Update(); // 0x00343424
    // Start with the volume and the pans of the object at once
    void StartAtObj(u32 soundId, const u8* notes); // 0x00342B68

    SoundObjBase* m_pObj; // 0x14
    f32 m_ObjVolume;      // 0x18
    f32 m_ObjPan;         // 0x1C
    f32 m_ObjSurroundPan; // 0x20
};
ASSERT_SIZE(SoundMelodyPlayerOneSeqPos, 0x24);
