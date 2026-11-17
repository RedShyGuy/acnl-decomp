#pragma once

#include "decomp.h"
#include "Sound/dSoundMelodyPlayerBase.h"
#include "Sound/dSoundObjWithMelody.h"
#include "nn/math/math_Vector3.h"

namespace sead {
class SoundHandle;
} // namespace sead

// The sound of a house door (with the door chime; all functions are called out of line in the original), like the other AudioObj classes but without virtual functions and
// without RTTI (the class and all names are ours). The doors of hobj and the door sequence 0x002D6198 use it.
class AudioObjDoor
{
public:
    AudioObjDoor(); // 0x002D655C
    // starts at world (relative to the camera)
    DECOMP_NOINLINE void Start(const nn::math::VEC3* world); // 0x002D610C
    DECOMP_NOINLINE void Stop(); // 0x002D6534
    DECOMP_NOINLINE void SetPosition(const nn::math::VEC3* world); // 0x002D6314
    // whether the current sound is soundId
    DECOMP_NOINLINE bool IsPlaying(u32 soundId) const; // 0x002D6144
    // (a copy of SoundObj<1>::StartSound with its helpers inline)
    DECOMP_NOINLINE sead::SoundHandle* StartSound(u32 soundId); // 0x002D6344
    // the door chime with the notes of a packed melody (unpacked into s_DoorChimeMelody)
    DECOMP_NOINLINE void PlayDoorChime(u32 soundId, const u8* packedMelody); // 0x002D6164
    DECOMP_NOINLINE void SetIsIgnoreMaster(bool isIgnoreMaster); // 0x002D6194

    // the notes of the last door chime
    static SoundMelody s_DoorChimeMelody;

    SoundObjWithMelody m_Obj; // 0x00
    bool m_IsStarted;         // 0xDC
};
ASSERT_SIZE(AudioObjDoor, 0xE0);
