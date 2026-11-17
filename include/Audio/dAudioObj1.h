#pragma once

#include "decomp.h"
#include "Sound/dSoundObj.h"
#include "nn/math/math_Vector3.h"

namespace sead {
class SoundHandle;
} // namespace sead

// A sound object of the game around a SoundObj<1>, like the other AudioObj classes but without virtual functions and
// without RTTI (the class and all names are ours; only what the code uses so far). The field objects build it on the
// stack for one sound; the mailbox of hobj has two.
class AudioObj1
{
public:
    // (the destructor is the one of SoundObj<1>, a copy at 0x00250B1C)
    DECOMP_NOINLINE AudioObj1(); // 0x00250ACC
    // starts at world (relative to the camera)
    DECOMP_NOINLINE void Start(const nn::math::VEC3* world); // 0x00250910
    DECOMP_NOINLINE void Stop(); // 0x00250A9C
    // whether the current sound is soundId
    DECOMP_NOINLINE bool IsPlaying(u32 soundId) const; // 0x00250948
    DECOMP_NOINLINE void SetIsIgnoreMaster(bool isIgnoreMaster); // 0x00250968
    DECOMP_NOINLINE void SetPosition(const nn::math::VEC3* world); // 0x002509F4
    DECOMP_NOINLINE void BindBgm(); // 0x00250A8C
    DECOMP_NOINLINE sead::SoundHandle* StartSound(u32 soundId); // 0x00250A90

    SoundObj<1> m_Obj;  // 0x00
    bool m_IsStarted;   // 0x5C
};
ASSERT_OFFSET(AudioObj1, m_IsStarted, 0x5C);
