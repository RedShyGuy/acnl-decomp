#pragma once

#include "decomp.h"
#include "Sound/dSoundCreateArg.h"
#include "nn/math/math_Vector3.h"

// RTTI 21AudioObjFurnitureBase @ 0x008CCC3C
// A sound source of the furniture for the game (furniture, gyroid, stereo, instrument, music box, insect cage): it holds a
// SoundObj and gives it the position relative to the camera; the slots of all kinds are here, each kind overrides its own
// (the class name is from the RTTI, everything else is ours).
class AudioObjFurnitureBase
{
public:
    enum Kind
    {
        KIND_FURNITURE = 0,
        KIND_FURNITURE_AUDIO = 1,
        KIND_FURNITURE_INST = 2,
        KIND_HANIWA = 3,
        KIND_FURNITURE_ORGEL = 4,
        KIND_FURNITURE_INSECT = 5,
        KIND_NUM = 6,
    };

    explicit AudioObjFurnitureBase(u8 kind) : m_Kind(kind), m_IsStarted(false) {}
    // 0x00324FFC
    virtual ~AudioObjFurnitureBase();
    virtual void Start(const nn::math::VEC3* world, u32 kind, bool isOn); // 0x00324F04 slot 0x08
    virtual void StartWithKind(u32 kind, const nn::math::VEC3* world); // 0x00324F08 slot 0x0C
    virtual void Update(const nn::math::VEC3* world, f32 frame); // 0x00324FD4 slot 0x10
    virtual f32 UpdateDance(const nn::math::VEC3* world); // 0x00324FC8 slot 0x14
    virtual void Stop() = 0; // slot 0x18
    virtual void SetIsIgnoreMaster(bool isIgnoreMaster) = 0; // slot 0x1C
    virtual s8 IsIgnoreMaster() = 0; // slot 0x20
    virtual void SetVolumeScale(f32 scale) = 0; // slot 0x24
    virtual bool IsPlaying() = 0; // slot 0x28
    virtual bool IsPlaying(u32 soundId) = 0; // slot 0x2C
    virtual void StartSound(u32 soundId) = 0; // slot 0x30
    virtual void SetIsCueDisabled(bool isCueDisabled); // 0x00324F0C slot 0x34
    virtual bool IsOffSoundPlaying(); // 0x00725824 slot 0x38
    virtual void SetKind(u32 kind); // 0x00324FF0 slot 0x3C
    virtual void TurnOn(); // 0x00324FF4 slot 0x40
    virtual void TurnOff(); // 0x00324F14 slot 0x44
    virtual void HoldSound(u32 soundId); // 0x00324FC4 slot 0x48
    virtual void SetNextIsOn(); // 0x00324F1C slot 0x4C
    virtual void LoadMusic(u32 song); // 0x00324F80 slot 0x50
    virtual bool IsMusicLoaded(); // 0x00324FB4 slot 0x54
    virtual void PlayMusic(s8 isFromStart, u32 milliSeconds); // 0x00324F7C slot 0x58
    virtual void MusicSlot5C(); // 0x00324F6C slot 0x5C
    virtual void StopMusic(s32 fadeFrames); // 0x00324F68 slot 0x60
    virtual bool IsMusicStopped(); // 0x00324FA4 slot 0x64
    virtual bool IsMusicPlaying(); // 0x00324F8C slot 0x68
    virtual u32 GetMusicSong(); // 0x00324FBC slot 0x6C
    virtual bool IsMusicFinished(); // 0x00324FAC slot 0x70
    virtual void Prepare(); // 0x00324F58 slot 0x74
    virtual u8 PlayNote(); // 0x00324FE4 slot 0x78
    virtual void PlayNoteAt(s16 note); // 0x00324F78 slot 0x7C
    virtual void StartDance(bool isFree); // 0x00324FE0 slot 0x80
    virtual void StopDanceSound(); // 0x00324FEC slot 0x84
    virtual s32 GetFramesToStart(); // 0x00324F84 slot 0x88
    virtual void PlaySong(u32 song, s32 wait); // 0x00324F18 slot 0x8C
    virtual void StopSong(); // 0x00324FF8 slot 0x90
    virtual void PauseSong(bool isPause); // 0x00324F10 slot 0x94
    virtual bool IsSongPlaying(); // 0x00324F70 slot 0x98
    virtual bool IsSongPaused(); // 0x00324F50 slot 0x9C
    virtual u32 GetSongFrame(); // 0x00324F48 slot 0xA0
    virtual f32 GetFadeVolume(u32 song, u32 frame); // 0x00324F5C slot 0xA4
    virtual bool IsSinging(); // 0x00324FD8 slot 0xA8

    // by the kind (the table of the factories 0x00846304)
    static AudioObjFurnitureBase* Create(const SoundCreateArg* arg, u32 kind); // 0x00324F20
    static void Destroy(AudioObjFurnitureBase* obj); // 0x00324F30
    // BsAudioLoadSignal is on
    static bool IsLoadSignalOn(); // 0x00324F94

    static bool s_IsLoadSignalOn;

    u8 m_Kind;        // 0x04
    bool m_IsStarted; // 0x05
};
