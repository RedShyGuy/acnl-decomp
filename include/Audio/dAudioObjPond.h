#pragma once

#include "decomp.h"
#include "Audio/dAudioObjFgBase.h"
#include "Sound/dSoundObjPond.h"

// RTTI 12AudioObjPond @ 0x008CB4F0
// vtable 0x008EDF78 (vptr 0x008EDF80), offset_to_top 0, 7 entries
// The sound of a pond for the game (the class name is from the RTTI, everything else is ours).
class AudioObjPond : public ::AudioObjFgBase
{
public:
    static AudioObjFgBase* Create(const SoundCreateArg* arg); // 0x001F8368
    DECOMP_NOINLINE AudioObjPond(u8 kind); // 0x001F8388
    // 0x001F841C
    // 0x001F83B8 (deleting dtor)
    virtual ~AudioObjPond();
    virtual void Start(const nn::math::VEC3* world) override; // 0x001F82BC slot 0x08
    virtual void SetPosition(const nn::math::VEC3* world) override; // 0x001F8304 slot 0x0C
    virtual void Stop() override; // 0x001F8334 slot 0x10
    virtual void SetIsIgnoreMaster(bool isIgnoreMaster) override; // 0x001F82F4 slot 0x14
    virtual s8 IsIgnoreMaster() override; // 0x001F82FC slot 0x18

    SoundObjPond m_Obj; // 0x08
};
ASSERT_SIZE(AudioObjPond, 0x78);
