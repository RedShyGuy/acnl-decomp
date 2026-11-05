#pragma once

#include "decomp.h"
#include "Sound/dSoundMelodyPlayerBase.h"

class SoundObjBase;

// RTTI 20SoundMelodyPlayerPos @ 0x008CCC18
// vtable 0x008F5B3C (vptr 0x008F5B44), offset_to_top 0, 3 entries
// The melody player with the volume and the pans of a sound object (the class name is from the RTTI, everything else
// is ours).
class SoundMelodyPlayerPos : public ::SoundMelodyPlayerBase
{
public:
    SoundMelodyPlayerPos(); // 0x00322270
    // 0x0013E56C (falls into ~SoundMelodyPlayerBase)
    // 0x003222B0 (deleting dtor)
    virtual ~SoundMelodyPlayerPos();
    virtual void PlayNote(f32 frames) override; // 0x003221F8 slot 0x08

    // the values of the object, then SoundMelodyPlayerBase::Calc
    void Calc(); // 0x0032C434

    SoundObjBase* m_pObj; // 0x4C
    f32 m_ObjVolume;      // 0x50
    f32 m_ObjPan;         // 0x54
    f32 m_ObjSurroundPan; // 0x58

private:
    void ApplyObj();
};
ASSERT_SIZE(SoundMelodyPlayerPos, 0x5C);
