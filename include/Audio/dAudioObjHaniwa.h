#pragma once

#include "decomp.h"
#include "Audio/dAudioObjFurnitureBase.h"
#include "Sound/dSoundObjHaniwa.h"

// RTTI 14AudioObjHaniwa @ 0x008CBB3C
// vtable 0x008EFA50 (vptr 0x008EFA58), offset_to_top 0, 43 entries
// The sound of a gyroid for the game (the class name is from the RTTI, everything else is ours).
class AudioObjHaniwa : public ::AudioObjFurnitureBase
{
public:
    static AudioObjFurnitureBase* Create(const SoundCreateArg* arg); // 0x00250104
    DECOMP_NOINLINE AudioObjHaniwa(); // 0x00250124
    // 0x00250170
    // 0x0025014C (deleting dtor)
    virtual ~AudioObjHaniwa();
    virtual void StartWithKind(u32 kind, const nn::math::VEC3* world) override; // 0x0024FCF0 slot 0x0C
    virtual f32 UpdateDance(const nn::math::VEC3* world) override; // 0x0024FDC4 slot 0x14
    virtual void Stop() override; // 0x002500DC slot 0x18
    virtual void SetIsIgnoreMaster(bool isIgnoreMaster) override; // 0x0024FDB4 slot 0x1C
    virtual s8 IsIgnoreMaster() override; // 0x0024FDBC slot 0x20
    virtual void SetVolumeScale(f32 scale) override; // 0x0024FD70 slot 0x24
    virtual bool IsPlaying() override; // 0x0024FD78 slot 0x28
    virtual bool IsPlaying(u32 soundId) override; // 0x0024FD34 slot 0x2C
    virtual void StartSound(u32 soundId) override; // 0x0024FDF4 slot 0x30
    virtual void Prepare() override; // 0x002777F0 slot 0x74
    virtual void StartDance(bool isFree) override; // 0x00277EF4 slot 0x80
    virtual void StopDanceSound() override; // 0x0027808C slot 0x84
    virtual s32 GetFramesToStart() override; // 0x00277CDC slot 0x88

    SoundObjHaniwa m_Obj; // 0x08
};
ASSERT_SIZE(AudioObjHaniwa, 0xB4);
