#pragma once

#include "decomp.h"
#include "Sound/dSoundCreateArg.h"
#include "nn/math/math_Vector3.h"

// RTTI 14AudioObjFgBase @ 0x008CBB34
// A sound source of the field for the game (a river, a water fall, a pond): it holds a SoundObj and gives it the position
// relative to the camera (the class name is from the RTTI, everything else is ours).
class AudioObjFgBase
{
public:
    enum Kind
    {
        KIND_RIVER = 0,
        KIND_FALL = 1,
        KIND_POND = 2,
        KIND_NUM = 3,
    };

    explicit AudioObjFgBase(u8 kind) : m_Kind(kind), m_IsStarted(false) {}
    virtual ~AudioObjFgBase() {}
    virtual void Start(const nn::math::VEC3* world) = 0; // slot 0x08
    virtual void SetPosition(const nn::math::VEC3* world) = 0; // slot 0x0C
    virtual void Stop() = 0; // slot 0x10
    virtual void SetIsIgnoreMaster(bool isIgnoreMaster) = 0; // slot 0x14
    virtual s8 IsIgnoreMaster() = 0; // slot 0x18

    // by the kind (the table of the factories 0x008462F8)
    static AudioObjFgBase* Create(const SoundCreateArg* arg, u32 kind); // 0x0024FCC8
    static void Destroy(AudioObjFgBase* obj); // 0x0024FCD8

    u8 m_Kind;        // 0x04 Kind
    bool m_IsStarted; // 0x05
};
