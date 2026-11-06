#pragma once

#include "decomp.h"
#include "Audio/dAudioObjFurnitureBase.h"
#include "Sound/dSoundObjFurnitureAudio.h"

// RTTI 22AudioObjFurnitureAudio @ 0x008CCD64
// vtable 0x008F6750 (vptr 0x008F6758), offset_to_top 0, 43 entries
// The sound of a stereo for the game (the class name is from the RTTI, everything else is ours).
class AudioObjFurnitureAudio : public ::AudioObjFurnitureBase
{
public:
    static AudioObjFurnitureBase* Create(const SoundCreateArg* arg); // 0x0032DD98
    DECOMP_NOINLINE AudioObjFurnitureAudio(); // 0x0032DDB4
    // 0x0032DE04
    // 0x0032DDE0 (deleting dtor)
    virtual ~AudioObjFurnitureAudio();
    virtual void Start(const nn::math::VEC3* world, u32 kind, bool isOn) override; // 0x0032DC24 slot 0x08
    virtual void Update(const nn::math::VEC3* world, f32 frame) override; // 0x0032DD18 slot 0x10
    virtual void Stop() override; // 0x0032DD60 slot 0x18
    virtual void SetIsIgnoreMaster(bool isIgnoreMaster) override; // 0x0032DD00 slot 0x1C
    virtual s8 IsIgnoreMaster() override; // 0x0032DD08 slot 0x20
    virtual void SetVolumeScale(f32 scale) override; // 0x0032DCB0 slot 0x24
    virtual bool IsPlaying() override; // 0x0032DCB8 slot 0x28
    virtual bool IsPlaying(u32 soundId) override; // 0x0032DC74 slot 0x2C
    virtual void StartSound(u32 soundId) override; // 0x0032DD58 slot 0x30
    virtual void SetKind(u32 kind) override; // 0x0032DD88 slot 0x3C
    virtual void TurnOn() override; // 0x0032DD90 slot 0x40
    virtual void TurnOff() override; // 0x0032DC6C slot 0x44
    virtual void HoldSound(u32 soundId) override; // 0x0032DD10 slot 0x48
    virtual void LoadMusic(u32 song) override; // 0x003320C0 slot 0x50
    virtual bool IsMusicLoaded() override; // 0x0072648C slot 0x54
    virtual void PlayMusic(s8 isFromStart, u32 milliSeconds) override; // 0x0033209C slot 0x58
    virtual void StopMusic(s32 fadeFrames) override; // 0x00332054 slot 0x60
    virtual bool IsMusicStopped() override; // 0x0072642C slot 0x64
    virtual bool IsMusicPlaying() override; // 0x00726408 slot 0x68
    virtual u32 GetMusicSong() override; // 0x007264BC slot 0x6C
    virtual bool IsMusicFinished() override; // 0x00726458 slot 0x70

    SoundObjFurnitureAudio m_Obj; // 0x08
};
ASSERT_SIZE(AudioObjFurnitureAudio, 0x98);
