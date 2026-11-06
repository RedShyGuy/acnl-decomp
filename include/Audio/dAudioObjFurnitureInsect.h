#pragma once

#include "decomp.h"
#include "Audio/dAudioObjFurnitureBase.h"
#include "Sound/dSoundObjFurnitureInsect.h"

// RTTI 23AudioObjFurnitureInsect @ 0x008CCE8C
// vtable 0x008F6FDC (vptr 0x008F6FE4), offset_to_top 0, 43 entries
// The sound of an insect in a cage for the game (the class name is from the RTTI, everything else is ours).
class AudioObjFurnitureInsect : public ::AudioObjFurnitureBase
{
public:
    static AudioObjFurnitureBase* Create(const SoundCreateArg* arg); // 0x00333870
    DECOMP_NOINLINE AudioObjFurnitureInsect(); // 0x0033388C
    // 0x003338DC
    // 0x003338B8 (deleting dtor)
    virtual ~AudioObjFurnitureInsect();
    virtual void Start(const nn::math::VEC3* world, u32 kind, bool isOn) override; // 0x00333700 slot 0x08
    virtual void Update(const nn::math::VEC3* world, f32 frame) override; // 0x003337E8 slot 0x10
    virtual void Stop() override; // 0x00333838 slot 0x18
    virtual void SetIsIgnoreMaster(bool isIgnoreMaster) override; // 0x003337D0 slot 0x1C
    virtual s8 IsIgnoreMaster() override; // 0x003337D8 slot 0x20
    virtual void SetVolumeScale(f32 scale) override; // 0x0033378C slot 0x24
    virtual bool IsPlaying() override; // 0x00333794 slot 0x28
    virtual bool IsPlaying(u32 soundId) override; // 0x00333750 slot 0x2C
    virtual void StartSound(u32 soundId) override; // 0x00333830 slot 0x30
    virtual void SetKind(u32 kind) override; // 0x00333860 slot 0x3C
    virtual void TurnOn() override; // 0x00333868 slot 0x40
    virtual void TurnOff() override; // 0x00333748 slot 0x44
    virtual void HoldSound(u32 soundId) override; // 0x003337E0 slot 0x48
    virtual bool IsSinging() override; // 0x00333828 slot 0xA8

    SoundObjFurnitureInsect m_Obj; // 0x08
};
ASSERT_SIZE(AudioObjFurnitureInsect, 0x9C);
