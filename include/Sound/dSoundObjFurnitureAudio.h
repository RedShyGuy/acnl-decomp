#pragma once

#include "decomp.h"
#include "Sound/dSoundObjFurniture.h"
#include "Sound/dSoundRoomMusicPlayer.h"

// RTTI 22SoundObjFurnitureAudio @ 0x008CCE50
// vtable 0x008F6C58 (vptr 0x008F6C60), offset_to_top 0, 16 entries
// A stereo: it plays the room music (SoundRoomMusicPlayer) and is on while the music plays (method names are ours).
class SoundObjFurnitureAudio : public ::SoundObjFurniture
{
public:
    SoundObjFurnitureAudio(); // 0x00332268
    // 0x003322F8
    // 0x00332280 (deleting dtor)
    virtual ~SoundObjFurnitureAudio();
    virtual void Stop() override; // 0x00332208 slot 0x0C
    virtual void Start(const nn::math::VEC3* world, const nn::math::VEC3* relative, u32 kind, bool isOn) override; // 0x0033202C slot 0x38
    virtual void Update(const nn::math::VEC3* world, const nn::math::VEC3* relative, f32 frame) override; // 0x003320EC slot 0x3C

    void StopMusic(s32 fadeFrames); // 0x0033205C
    void PlayMusic(s8 isFromStart, u32 milliSeconds); // 0x003320A4
    void LoadMusic(u32 song); // 0x003320C8
    // the filter of the kind of stereo (0: none)
    s32 GetBiquadType() const; // 0x00726368
    // the state of the room music (copies in the shared area)
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsMusicPlaying() // 0x00726410
    {
        SoundRoomMusicPlayer* player = SoundRoomMusicPlayer::s_pInstance;
        return player != NULL && player->IsPlaying();
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsMusicStopped() // 0x00726434
    {
        SoundRoomMusicPlayer* player = SoundRoomMusicPlayer::s_pInstance;
        return player == NULL || player->m_State == SoundRoomMusicPlayer::STATE_NONE;
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsMusicFinished() // 0x00726460
    {
        SoundRoomMusicPlayer* player = SoundRoomMusicPlayer::s_pInstance;
        return player != NULL && player->m_State == SoundRoomMusicPlayer::STATE_NONE && player->m_IsFinished == true;
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsMusicLoaded() // 0x00726494
    {
        SoundRoomMusicPlayer* player = SoundRoomMusicPlayer::s_pInstance;
        return player != NULL && player->m_State == SoundRoomMusicPlayer::STATE_LOADED;
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE u32 GetMusicSong() // 0x007264C4
    {
        SoundRoomMusicPlayer* player = SoundRoomMusicPlayer::s_pInstance;
        return player != NULL ? player->m_Song : 0;
    }
};
