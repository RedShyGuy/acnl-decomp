#pragma once

#include "decomp.h"
#include "Audio/dAudioObjFgBase.h"
#include "Sound/dSoundObjRiver.h"

// RTTI 13AudioObjRiver @ 0x008CB87C
// vtable 0x008EEC2C (vptr 0x008EEC34), offset_to_top 0, 7 entries
// The sound of a river for the game (the class name is from the RTTI, everything else is ours).
class AudioObjRiver : public ::AudioObjFgBase
{
public:
    static AudioObjFgBase* Create(const SoundCreateArg* arg); // 0x00211168
    DECOMP_NOINLINE AudioObjRiver(); // 0x00211184
    // 0x002111CC
    // 0x002111AC (deleting dtor)
    virtual ~AudioObjRiver();
    virtual void Start(const nn::math::VEC3* world) override; // 0x002110C8 slot 0x08
    virtual void SetPosition(const nn::math::VEC3* world) override; // 0x00211110 slot 0x0C
    virtual void Stop() override; // 0x00211140 slot 0x10
    virtual void SetIsIgnoreMaster(bool isIgnoreMaster) override; // 0x00211100 slot 0x14
    virtual s8 IsIgnoreMaster() override; // 0x00211108 slot 0x18

    SoundObjRiver m_Obj; // 0x08
};
ASSERT_SIZE(AudioObjRiver, 0x60);
