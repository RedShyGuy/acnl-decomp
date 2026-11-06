#pragma once

#include "decomp.h"
#include "Audio/dAudioObjFurnitureBase.h"
#include "Sound/dSoundObjFurnitureOrgel.h"

// RTTI 22AudioObjFurnitureOrgel @ 0x008CCD70
// vtable 0x008F6804 (vptr 0x008F680C), offset_to_top 0, 43 entries
// The sound of a music box for the game (the class name is from the RTTI, everything else is ours).
class AudioObjFurnitureOrgel : public ::AudioObjFurnitureBase
{
public:
    static AudioObjFurnitureBase* Create(const SoundCreateArg* arg); // 0x0032DFC4
    DECOMP_NOINLINE AudioObjFurnitureOrgel(); // 0x0032DFE0
    // 0x0032E030
    // 0x0032E00C (deleting dtor)
    virtual ~AudioObjFurnitureOrgel();
    virtual void Start(const nn::math::VEC3* world, u32 kind, bool isOn) override; // 0x0032DE24 slot 0x08
    virtual void Update(const nn::math::VEC3* world, f32 frame) override; // 0x0032DF44 slot 0x10
    virtual void Stop() override; // 0x0032DF8C slot 0x18
    virtual void SetIsIgnoreMaster(bool isIgnoreMaster) override; // 0x0032DF2C slot 0x1C
    virtual s8 IsIgnoreMaster() override; // 0x0032DF34 slot 0x20
    virtual void SetVolumeScale(f32 scale) override; // 0x0032DEE8 slot 0x24
    virtual bool IsPlaying() override; // 0x0032DEF0 slot 0x28
    virtual bool IsPlaying(u32 soundId) override; // 0x0032DEAC slot 0x2C
    virtual void StartSound(u32 soundId) override; // 0x0032DF84 slot 0x30
    virtual void SetKind(u32 kind) override; // 0x0032DFB4 slot 0x3C
    virtual void TurnOn() override; // 0x0032DFBC slot 0x40
    virtual void TurnOff() override; // 0x0032DE6C slot 0x44
    virtual void HoldSound(u32 soundId) override; // 0x0032DF3C slot 0x48
    virtual void PlaySong(u32 song, s32 wait) override; // 0x0033248C slot 0x8C
    virtual void StopSong() override; // 0x00332AEC slot 0x90
    virtual void PauseSong(bool isPause) override; // 0x00332368 slot 0x94
    virtual bool IsSongPlaying() override; // 0x0032DE94 slot 0x98
    virtual bool IsSongPaused() override; // 0x0032DE7C slot 0x9C
    virtual u32 GetSongFrame() override; // 0x0032DE74 slot 0xA0
    virtual f32 GetFadeVolume(u32 song, u32 frame) override; // 0x007264DC slot 0xA4

    SoundObjFurnitureOrgel m_Obj; // 0x08
    bool m_IsActive; // 0xD0 the started flag of the music box (m_IsStarted stays false)
};
ASSERT_SIZE(AudioObjFurnitureOrgel, 0xD4);
