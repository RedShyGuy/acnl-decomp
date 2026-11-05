#pragma once

#include "decomp.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundFader.h"
#include "Sound/dSoundVolumeMgr.h"
#include "Sound/dSoundFxMgr.h"
#include "Sound/dSoundEnvMgr.h"
#include "nw/snd/snd_SoundStartable.h"
#include "nw/snd/snd_SoundArchive.h"
#include "Sound/dSoundBgmMgr.h"
#include "Sound/dSoundMelodyMgr.h"
#include "sead/seadCheckedArray.h"
#include "nw/snd/snd_SoundHandle.h"
#include "sead/seadAudioSettingParameter.h"
#include "sead/seadAudioResourceLoaderCtr.h"

namespace nw { namespace snd { class SoundArchive; class SoundPlayer; } }
namespace sead { class AudioPlayer; class AudioPlayerCtr; class Heap; }

// The sound manager of the game: the sound players of the archive, the managers of the kinds of sounds (volume,
// animal speech, melodies, BGM, effects, environment, shared objects) and pausing (the class name and PlaySound are
// from libgarden, everything else is ours). (being written: the members of the managers are still raw memory)
class SoundMgr
{
public:
    class SingletonDisposer_;

    // the pause reasons of m_PauseFlags (a player is paused while one of its reasons is set)
    enum PauseFlag
    {
        PAUSE_FLAG_1 = 1 << 0,
        PAUSE_FLAG_2 = 1 << 1,
        PAUSE_FLAG_10 = 1 << 4,
        PAUSE_FLAG_20 = 1 << 5,
    };

    static void PlaySound(SeID id, float param = 0.0f); // 0x0058DE9C | libgarden [tier A]

    // a pause reason other than 0x20 (and other than 0x10 alone) is set
    DECOMP_EMIT_COPY bool IsPaused() const // 0x007680A0
    {
        u32 flags = m_PauseFlags;
        if (flags & PAUSE_FLAG_20) {
            flags ^= PAUSE_FLAG_20;
        }
        if (flags & PAUSE_FLAG_10) {
            return flags != PAUSE_FLAG_10;
        }
        return flags != 0;
    }

    DECOMP_EMIT_COPY DECOMP_NOINLINE nw::snd::SoundPlayer* GetSoundPlayer(u32 index) // 0x00767F20
    {
        return m_pSoundPlayers[index];
    }

    // the sound in the handle (only with the audio player)
    nw::snd::SoundStartable::StartResult StartSound(nw::snd::SoundHandle* handle, u32 soundId); // 0x006B2884
    nw::snd::SoundStartable::StartResult StartSound(nw::snd::SoundHandle* handle, u32 soundId,
                                                    const nw::snd::SoundStartable::StartInfo* info); // 0x006B28A0
    // the sound prepared in the handle
    nw::snd::SoundStartable::StartResult PrepareSound(nw::snd::SoundHandle* handle, u32 soundId); // 0x006B2BCC
    nw::snd::SoundStartable::StartResult PrepareSound(nw::snd::SoundHandle* handle, u32 soundId,
                                                      const nw::snd::SoundStartable::StartInfo* info); // 0x006B2BE8
    // the sound; the result is kept (true: started)
    bool StartSoundChecked(nw::snd::SoundHandle* handle, u32 soundId); // 0x006B4CC0
    bool ReadSoundInfo(nw::snd::SoundArchive::SoundInfo* info, u32 soundId); // 0x0013CFDC
    // the user parameter of the sound (0: none)
    DECOMP_EMIT_COPY DECOMP_NOINLINE u32 ReadSoundUserParam(u32 soundId); // 0x00767E80
    // the root of the external files of the archive (rom:/Sound/ or patch://Sound/)
    void SetExternalFileRoot(bool isRom); // 0x006B315C
    // a volume of SoundVolumeMgr (after the initialization)
    void SetVolume(u32 volume, f32 value, s32 frames); // 0x006B31B4
    // the time of the game is at night (before 7:00 or from 19:00)
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsNight(); // 0x00767EF0
    // y0..y1 for x in x0..x1 (y0 if x0 == x1)
    static f32 Lerp(s32 x, s32 x0, s32 x1, f32 y0, f32 y1); // 0x006B55F8
    // a flag of the BGM manager (until SoundBgmMgr is written)
    bool IsBgmFlag14() const { return m_BgmMgr.m_State != 0; }

    static SoundMgr* s_pInstance;

    u32 m_SingletonDisposerBuffer[4];        // 0x000
    u32 _10;                                 // 0x010
    u32 _14;                                 // 0x014
    u32 _18;                                 // 0x018
    nw::snd::SoundHandle m_Handles[8];       // 0x01C
    u8 _3C[8][20];                           // 0x03C
    sead::AudioSettingParameter m_Setting;   // 0x0DC
    sead::AudioResourceLoaderCtr m_Loader;   // 0x100
    u32 _130;                                // 0x130
    SoundVolumeMgr m_VolumeMgr;              // 0x134
    u8 m_DoubutsugoMgr[0xF8];                // 0x594 SoundDoubutsugoMgr
    SoundMelodyMgr m_MelodyMgr;              // 0x68C
    SoundBgmMgr m_BgmMgr;                    // 0x754
    SoundFxMgr m_FxMgr;                      // 0x860
    SoundEnvMgr m_EnvMgr;                    // 0xB00
    u8 m_SharedObjMgr[0x40];                 // 0xBC8 SoundSharedObjMgr
    sead::AudioPlayer* m_pAudioPlayer;       // 0xC08
    sead::AudioPlayerCtr* m_pPlayer;         // 0xC0C
    nw::snd::SoundArchive* m_pArchive;       // 0xC10
    sead::CheckedArray<nw::snd::SoundPlayer*, PLAYER_NUM> m_pSoundPlayers; // 0xC14
    bool m_IsInitialized;                    // 0xC5C
    u8 _C5D;                                 // 0xC5D
    u32 m_PauseFlags;                        // 0xC60
    sead::CheckedArray<u32, PLAYER_NUM> m_PlayerPauseFlags; // 0xC64
    u8 _CAC[0xCB8 - 0xCAC];                  // 0xCAC
    s32 m_State;                             // 0xCB8
    u8 _CBC[0xCC5 - 0xCBC];                  // 0xCBC
    bool _CC5;                               // 0xCC5
    u8 _CC6[0xCF4 - 0xCC6];                  // 0xCC6
    u8 m_LastStartResult;                    // 0xCF4 StartResult of StartSoundChecked
    u8 _CF5[0xD1C - 0xCF5];                  // 0xCF5
    SoundFader m_Fader;                      // 0xD1C
    struct Reaction
    {
        u32 soundId;  // 0x00
        u8 _4;        // 0x04
        u32 _8;       // 0x08
        f32 _C;       // 0x0C
        f32 _10;      // 0x10
    };
    Reaction m_Reactions[9];                 // 0xD30
};
ASSERT_OFFSET(SoundMgr, m_Setting, 0xDC);
ASSERT_OFFSET(SoundMgr, m_Loader, 0x100);
ASSERT_OFFSET(SoundMgr, m_pPlayer, 0xC0C);
ASSERT_OFFSET(SoundMgr, m_Fader, 0xD1C);
ASSERT_SIZE(SoundMgr, 0xDE4);
