#pragma once

#include "decomp.h"
#include "Other/dDuckingFader.h"
#include "Sound/dSoundFaderPlayer.h"
#include "Sound/dSoundLabel.h"
#include "sead/seadAudioSubsetBase.h"
#include "sead/seadPtrArrayImpl.h"

namespace nw { namespace snd { class SoundHandle; } }

// RTTI 14SoundVolumeMgr @ 0x008CBE20
// vtable 0x008F0928 (vptr 0x008F0930), offset_to_top 0, 20 entries
// The volumes of the 18 sound players: a fader per player, scaled by the volume of its group, which ducking faders
// lower (the class name is from the RTTI, everything else is ours). Players 0..3 are the BGM players, 4..17 the
// others.
class SoundVolumeMgr : public ::sead::AudioSubsetBase
{
public:
    // the groups of players with a common scale (m_Scales)
    enum Group
    {
        GROUP_BGM = 0,          // PLAYER_BGM
        GROUP_BGM_MU = 1,       // PLAYER_BGM_MU
        GROUP_BGM_RM = 2,       // PLAYER_BGM_RM
        GROUP_3 = 3,
        GROUP_OBJ = 4,          // PLAYER_SE_OBJ, _FUR_LV, _FUR_TRG, _MELODY, _TV
        GROUP_HANIWA = 5,       // PLAYER_SE_HANIWA, _SESSION
        GROUP_ENV = 6,          // PLAYER_SE_ENV
        GROUP_ACT = 7,          // PLAYER_SE_ACT, _FOOT
        GROUP_NUM = 8,
    };
    enum
    {
        DUCK_NUM = 11,
        BGM_PLAYER_NUM = 4,
    };
    // the volumes SetVolume sets
    enum Volume
    {
        VOLUME_BGM = 0,         // player PLAYER_BGM
        VOLUME_BGM_HORNET = 1,  // player PLAYER_BGM_HORNET
        VOLUME_BGM_MU = 2,      // player PLAYER_BGM_MU
        VOLUME_MASTER = 3,      // m_MasterFader
        VOLUME_ENV = 4,         // player PLAYER_SE_ENV
    };

    // how a ducking fader moves: down to value, back up to 1 (delays and frames in frames)
    struct DuckParam
    {
        f32 value;       // 0x0
        u8 downDelay;    // 0x4
        u8 downFrames;   // 0x5
        u8 upDelay;      // 0x6
        u8 upFrames;     // 0x7
        u8 groupMask;    // 0x8 the groups it lowers
    };

    SoundVolumeMgr(); // 0x00128A5C
    virtual ~SoundVolumeMgr();
    // the faders, the ducking and the volumes of the players
    virtual void calc() override; // 0x0027A03C slot 0x1C

    // the players of the faders (after the sound archive player is there)
    void Initialize(); // 0x00279CAC
    void Finalize(); // 0x00279E8C
    // a ducking fader back to 1 (now: at once)
    DECOMP_NOINLINE void EndDuck(const s32& index, bool now); // 0x00279C5C
    void EndDuckDemo(); // 0x00279E70
    // ducking fader 6 with s_DemoDuckParam; the doubutsugo player is scaled by GROUP_ACT
    void StartDuckDemo(); // 0x00279E90
    // all ducking faders except 6 and 10 at 1
    void ResetDucks(); // 0x00279ED0
    // the player silent (or paused if it has no fader)
    void PausePlayer(u32 player); // 0x00279F40
    void UnpausePlayer(u32 player, s32 frames); // 0x00279FC4
    void SetPlayerVolume(u32 player, f32 volume, s32 frames); // 0x0027A328
    // the volume of the player (its volume before a pause while it is paused)
    DECOMP_EMIT_COPY DECOMP_NOINLINE f32 GetPlayerVolume(u32 player) const // 0x0071ABA4
    {
        if (player >= PLAYER_NUM) {
            return 0.0f;
        }
        if (m_IsPlayerPaused[player]) {
            return m_PlayerVolumes[player];
        }
        return m_PlayerFaders[player].GetValue();
    }
    void SetVolume(u32 volume, f32 value, s32 frames); // 0x002BD234

    // the players that keep their pause (true) or are paused with their fader (false) (name is ours)
    static const bool s_IsFadedPause[PLAYER_NUM];
    static DuckParam s_DemoDuckParam;

    DuckingFader m_Ducks[DUCK_NUM];                     // 0x00C
    DuckParam m_DuckParams[DUCK_NUM];                   // 0x140
    f32 m_Scales[GROUP_NUM];                            // 0x1C4
    sead::PtrArray<nw::snd::SoundHandle> m_DuckHandles; // 0x1E4 keep ducking fader 7 down while they play
    u8 _1F0;                                            // 0x1F0
    SoundFaderPlayer m_PlayerFaders[PLAYER_NUM];        // 0x1F4
    f32 m_PlayerVolumes[PLAYER_NUM];                    // 0x3EC the volume to go back to after a pause
    bool m_IsPlayerPaused[PLAYER_NUM];                  // 0x434
    SoundFader m_MasterFader;                           // 0x448
    bool m_IsSeOff;                                     // 0x45C players 4..17 at 0
    bool m_IsBgmOff;                                    // 0x45D players 0..3 at 0
};
ASSERT_OFFSET(SoundVolumeMgr, m_DuckParams, 0x140);
ASSERT_OFFSET(SoundVolumeMgr, m_DuckHandles, 0x1E4);
ASSERT_OFFSET(SoundVolumeMgr, m_PlayerFaders, 0x1F4);
ASSERT_OFFSET(SoundVolumeMgr, m_MasterFader, 0x448);
ASSERT_SIZE(SoundVolumeMgr, 0x460);
