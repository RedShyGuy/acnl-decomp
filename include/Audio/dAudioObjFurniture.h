#pragma once

#include "decomp.h"
#include "Audio/dAudioObjFurnitureBase.h"
#include "Sound/dSoundObjFurniture.h"

// RTTI 17AudioObjFurniture @ 0x008CC450
// vtable 0x008F333C (vptr 0x008F3344), offset_to_top 0, 43 entries
// The sound of a piece of furniture for the game (the class name is from the RTTI, everything else is ours).
class AudioObjFurniture : public ::AudioObjFurnitureBase
{
public:
    static AudioObjFurnitureBase* Create(const SoundCreateArg* arg); // 0x002C554C
    DECOMP_NOINLINE AudioObjFurniture(); // 0x002C556C
    // 0x002C55B4
    // 0x002C5590 (deleting dtor)
    virtual ~AudioObjFurniture();
    virtual void Start(const nn::math::VEC3* world, u32 kind, bool isOn) override; // 0x002C53F8 slot 0x08
    virtual void Update(const nn::math::VEC3* world, f32 frame) override; // 0x002C54E4 slot 0x10
    virtual void Stop() override; // 0x002C5524 slot 0x18
    virtual void SetIsIgnoreMaster(bool isIgnoreMaster) override; // 0x002C54D4 slot 0x1C
    virtual s8 IsIgnoreMaster() override; // 0x002C54DC slot 0x20
    virtual void SetVolumeScale(f32 scale) override; // 0x002C5490 slot 0x24
    virtual bool IsPlaying() override; // 0x002C5498 slot 0x28
    virtual bool IsPlaying(u32 soundId) override; // 0x002C5454 slot 0x2C
    virtual void StartSound(u32 soundId) override; // 0x002D2C48 slot 0x30
    virtual void SetIsCueDisabled(bool isCueDisabled) override; // 0x002C5440 slot 0x34
    virtual bool IsOffSoundPlaying() override; // 0x00720504 slot 0x38
    virtual void SetKind(u32 kind) override; // 0x002D2F64 slot 0x3C
    virtual void TurnOn() override; // 0x002D2FD8 slot 0x40
    virtual void TurnOff() override; // 0x002D25D4 slot 0x44
    virtual void HoldSound(u32 soundId) override; // 0x002D2998 slot 0x48
    virtual void SetNextIsOn() override; // 0x002C5448 slot 0x4C

    SoundObjFurniture m_Obj; // 0x08
};
ASSERT_SIZE(AudioObjFurniture, 0x98);
