#pragma once

#include "decomp.h"
#include "Sound/dSoundObj.h"
#include "nn/math/math_Vector3.h"

namespace sead {
class SoundHandle;
} // namespace sead

// A sound object of the game around a SoundObj<1>, like the other AudioObj classes but without virtual functions and
// without RTTI (the class and all names are ours; only what the code uses so far). The field objects build it on the
// stack for one sound.
class AudioObj1
{
public:
    // (the constructor and the destructor are the ones of SoundObj<1>)
    // starts at world (relative to the camera)
    void Start(const nn::math::VEC3* world); // 0x00250910
    sead::SoundHandle* StartSound(u32 soundId); // 0x00250A90

    SoundObj<1> m_Obj;  // 0x00
    bool m_IsStarted;   // 0x5C
};
ASSERT_OFFSET(AudioObj1, m_IsStarted, 0x5C);
