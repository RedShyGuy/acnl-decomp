#pragma once

#include "decomp.h"
#include "Audio/dAudioObjFgBase.h"
#include "Sound/dSoundObjFall.h"

// RTTI 12AudioObjFall @ 0x008CB4E4
// vtable 0x008EDF54 (vptr 0x008EDF5C), offset_to_top 0, 7 entries
// The sound of a water fall for the game (the class name is from the RTTI, everything else is ours).
class AudioObjFall : public ::AudioObjFgBase
{
public:
    static AudioObjFgBase* Create(const SoundCreateArg* arg); // 0x001F81A8
    DECOMP_NOINLINE AudioObjFall(u8 kind); // 0x001F81C8
    // 0x001F825C
    // 0x001F81F8 (deleting dtor)
    virtual ~AudioObjFall();
    virtual void Start(const nn::math::VEC3* world) override; // 0x001F80FC slot 0x08
    virtual void SetPosition(const nn::math::VEC3* world) override; // 0x001F8144 slot 0x0C
    virtual void Stop() override; // 0x001F8174 slot 0x10
    virtual void SetIsIgnoreMaster(bool isIgnoreMaster) override; // 0x001F8134 slot 0x14
    virtual s8 IsIgnoreMaster() override; // 0x001F813C slot 0x18

    SoundObjFall m_Obj; // 0x08
};
ASSERT_SIZE(AudioObjFall, 0x78);
