#pragma once

#include "decomp.h"
#include "Audio/dAudioObjFurnitureBase.h"
#include "Sound/dSoundObjFurnitureInst.h"

// RTTI 21AudioObjFurnitureInst @ 0x008CCC44
// vtable 0x008F5E28 (vptr 0x008F5E30), offset_to_top 0, 43 entries
// The sound of an instrument for the game (the class name is from the RTTI, everything else is ours).
class AudioObjFurnitureInst : public ::AudioObjFurnitureBase
{
public:
    static AudioObjFurnitureBase* Create(const SoundCreateArg* arg); // 0x00325160
    DECOMP_NOINLINE AudioObjFurnitureInst(); // 0x0032517C
    // 0x003251CC
    // 0x003251A8 (deleting dtor)
    virtual ~AudioObjFurnitureInst();
    virtual void Start(const nn::math::VEC3* world, u32 kind, bool isOn) override; // 0x00325000 slot 0x08
    virtual void Update(const nn::math::VEC3* world, f32 frame) override; // 0x003250E8 slot 0x10
    virtual void Stop() override; // 0x00325130 slot 0x18
    virtual void SetIsIgnoreMaster(bool isIgnoreMaster) override; // 0x003250D0 slot 0x1C
    virtual s8 IsIgnoreMaster() override; // 0x003250D8 slot 0x20
    virtual void SetVolumeScale(f32 scale) override; // 0x0032508C slot 0x24
    virtual bool IsPlaying() override; // 0x00325094 slot 0x28
    virtual bool IsPlaying(u32 soundId) override; // 0x00325050 slot 0x2C
    virtual void StartSound(u32 soundId) override; // 0x00325128 slot 0x30
    virtual void TurnOn() override; // 0x00325158 slot 0x40
    virtual void TurnOff() override; // 0x00325048 slot 0x44
    virtual void HoldSound(u32 soundId) override; // 0x003250E0 slot 0x48
    virtual void Prepare() override; // 0x0032CCB8 slot 0x74
    virtual u8 PlayNote() override; // 0x0032CD4C slot 0x78
    virtual void PlayNoteAt(s16 note) override; // 0x0032CCEC slot 0x7C

    SoundObjFurnitureInst m_Obj; // 0x08
};
ASSERT_SIZE(AudioObjFurnitureInst, 0xBC);
