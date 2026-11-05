#include "Sound/dSoundMgr.h"
#include "sead/seadAudioPlayerCtr.h"
#include "time/time_Api.h"
#include "Sound/sound_Extern.h"
#include "Sound/dSoundDataLoadMgr.h"
#include "Other/dHeapInit.h"
#include "nw/snd/internal/driver/snd_SequenceSoundPlayer.h"
#include "nw/snd/internal/driver/snd_HardwareManager.h"
#include "nw/snd/internal/snd_BasicSound.h"
#include "nw/snd/snd_SoundPlayer.h"
#include "sead/seadAudioMgr.h"
#include "sead/seadAudioSoundDataMgrCtr.h"

// 0x0094E080
SoundMgr* SoundMgr::s_pInstance = NULL;

// 0x0083C418
const u32 SoundMgr::s_PlayerPauseMasks[PLAYER_NUM] = {
    0x07, 0x27, 0x07, 0x27, 0x00, 0x07, 0x37, 0x37, 0x37, 0x37, 0x00, 0x07, 0x14, 0x04, 0x04, 0x04, 0x04, 0x04,
};

namespace {
// 0x0094E090
const sead::SafeStringBase<char> s_RomSoundRoot("rom:/Sound/");
// 0x0094E098
const sead::SafeStringBase<char> s_PatchSoundRoot("patch://Sound/");
} // namespace
// (builds s_RomSoundRoot and s_PatchSoundRoot)
// 0x007859B4 (static initializer)

// 0x0058DE9C | libgarden [tier A]
void SoundMgr::PlaySound(SeID, float)
{
}

// 0x006B2884
nw::snd::SoundStartable::StartResult SoundMgr::StartSound(nw::snd::SoundHandle* handle, u32 soundId)
{
    if (m_pPlayer == NULL) {
        return static_cast<nw::snd::SoundStartable::StartResult>(0);
    }
    return static_cast<nw::snd::SoundArchivePlayer*>(m_pPlayer)->StartSound(handle, soundId, NULL);
}

// 0x00767EF0
bool SoundMgr::IsNight()
{
    if (time::GetCurrentDate()->hour < 7) {
        return true;
    }
    return time::GetCurrentDate()->hour >= 19;
}

// 0x006B55F8
f32 SoundMgr::Lerp(s32 x, s32 x0, s32 x1, f32 y0, f32 y1)
{
    if (x0 == x1) {
        return y0;
    }
    return static_cast<f32>(x - x0) * (y1 - y0) / static_cast<f32>(x1 - x0) + y0;
}

// 0x006B28A0
nw::snd::SoundStartable::StartResult SoundMgr::StartSound(nw::snd::SoundHandle* handle, u32 soundId,
                                                          const nw::snd::SoundStartable::StartInfo* info)
{
    if (m_pPlayer == NULL) {
        return static_cast<nw::snd::SoundStartable::StartResult>(0);
    }
    return static_cast<nw::snd::SoundArchivePlayer*>(m_pPlayer)->StartSound(handle, soundId, info);
}

// 0x006B2BCC
nw::snd::SoundStartable::StartResult SoundMgr::PrepareSound(nw::snd::SoundHandle* handle, u32 soundId)
{
    if (m_pPlayer == NULL) {
        return static_cast<nw::snd::SoundStartable::StartResult>(0);
    }
    return static_cast<nw::snd::SoundArchivePlayer*>(m_pPlayer)->PrepareSound(handle, soundId, NULL);
}

// 0x006B2BE8
nw::snd::SoundStartable::StartResult SoundMgr::PrepareSound(nw::snd::SoundHandle* handle, u32 soundId,
                                                            const nw::snd::SoundStartable::StartInfo* info)
{
    if (m_pPlayer == NULL) {
        return static_cast<nw::snd::SoundStartable::StartResult>(0);
    }
    return static_cast<nw::snd::SoundArchivePlayer*>(m_pPlayer)->PrepareSound(handle, soundId, info);
}

// 0x006B4CC0
bool SoundMgr::StartSoundChecked(nw::snd::SoundHandle* handle, u32 soundId)
{
    if (m_pPlayer == NULL) {
        return false;
    }
    m_LastStartResult = static_cast<nw::snd::SoundArchivePlayer*>(m_pPlayer)->StartSound(handle, soundId, NULL);
    return m_LastStartResult == 0;
}

// 0x0013CFDC
bool SoundMgr::ReadSoundInfo(nw::snd::SoundArchive::SoundInfo* info, u32 soundId)
{
    return m_pArchive->ReadSoundInfo(soundId, info);
}

// 0x00767E80
u32 SoundMgr::ReadSoundUserParam(u32 soundId)
{
    u32 value = 0;
    if (m_pArchive->ReadSoundUserParam(soundId, 0, value)) {
        return value;
    }
    return 0;
}

// 0x006B31B4
void SoundMgr::SetVolume(u32 volume, f32 value, s32 frames)
{
    if (m_IsInitialized && volume < 5) {
        m_VolumeMgr.SetVolume(volume, value, frames);
    }
}

// 0x006B315C
void SoundMgr::SetExternalFileRoot(bool isRom)
{
    const char* root;
    if (isRom) {
        root = s_RomSoundRoot.getCString();
    } else {
        root = s_PatchSoundRoot.getCString();
    }
    m_pArchive->SetExternalFileRoot(root);
}

namespace {
// the sounds that pause with the players (the songs of DJ K.K. and K.K. Slider, the ship, the train and two sounds of the
// escape game) (name is ours)
inline bool IsPausedWithPlayer(u32 soundId)
{
    if (soundId - SEQ_BGM_DJ01_HS_SANBIKA <= SEQ_BGM_KAPPEI_BACK_RANDOMMATCHING - SEQ_BGM_DJ01_HS_SANBIKA) {
        return true;
    }
    if (soundId - SEQ_BGM_KK01_SANBIKA <= SEQ_BGM_KK94_DRIVE - SEQ_BGM_KK01_SANBIKA) {
        return true;
    }
    switch (soundId) {
    case SE_SYS_MAP_ICON_BLINK:
    case SE_OBJ_SHIP_RUN_FAST:
    case SE_OBJ_SHIP_RUN_FAST_SONG_FIN:
    case SE_OBJ_SHIP_RUN_SLOW:
    case SE_OBJ_SHIP_RUN_SLOW_SONG_FIN:
    case SE_OBJ_SHIP_RUN_LAST_SONG:
    case SE_OBJ_SHIP_RUN_LAST_SONG_2:
    case SE_OBJ_SHIP_RUN_LAST:
    case SE_OBJ_SHIP_RUN_LAST_2:
    case SE_ENV_TRAIN_DEPARTURE_L:
    case SE_ENV_TRAIN_DEPARTURE_R:
    case SE_ENV_TRAIN_ARRIVE_LO_R:
    case SE_ENV_TRAIN_ARRIVE_LO_L:
    case SE_ENV_TRAIN_ARRIVE_SH_R:
    case SE_ENV_TRAIN_ARRIVE_SH_L:
    case SE_OBJ_TRAIN_RUN_SPEED_DOWN_DEMO_L:
    case SE_OBJ_TRAIN_RUN_SPEED_DOWN_DEMO_R:
    case SE_ESCAPE_IMP_ITM_HKR:
    case SE_ESCAPE_RLT_MAN_RLT:
        return true;
    }
    return false;
}

// 0x006B45EC (name is ours)
void PauseSoundCallback(nw::snd::SoundHandle& handle)
{
    if (handle.m_pSound != NULL && handle.m_pSound->IsPause()) {
        return;
    }
    if (IsPausedWithPlayer(handle.GetId()) && handle.m_pSound != NULL) {
        handle.m_pSound->Pause(true, 0);
    }
}

// 0x006B4ADC (name is ours)
void UnpauseSoundCallback(nw::snd::SoundHandle& handle)
{
    if (handle.m_pSound == NULL || !handle.m_pSound->IsPause()) {
        return;
    }
    if (IsPausedWithPlayer(handle.GetId()) && handle.m_pSound != NULL) {
        handle.m_pSound->Pause(false, 30);
    }
}
} // namespace

// 0x006B26D0
void SoundMgr::Unpause10()
{
    if (m_PauseFlags & PAUSE_FLAG_10) {
        Unpause(PAUSE_FLAG_10);
    }
}

// 0x006B26E8
void SoundMgr::Initialize()
{
    if (!sead::DynamicCast<sead::AudioPlayerCtr>(sead::AudioMgr::s_pInstance->mPlayer)->initializeAfterSoundSystem()) {
        return;
    }
    m_pAudioPlayer = sead::AudioMgr::s_pInstance->mPlayer;
    m_pPlayer = sead::DynamicCast<sead::AudioPlayerCtr>(m_pAudioPlayer);
    m_pArchive = m_pPlayer->m_pSoundDataMgr->getSoundArchive();
    for (u32 i = 0; i < PLAYER_NUM; i++) {
        m_pSoundPlayers[i] = &m_pPlayer->GetSoundPlayer(i | PLAYER_BGM);
    }
    m_pArchive->SetExternalFileRoot(s_RomSoundRoot.getCString());
    m_VolumeMgr.Initialize();
    Func321FD4();
    m_IsInitialized = true;
}

// 0x006B2870
void SoundMgr::SetRainState(u32 state)
{
    if (state < 7) {
        m_EnvMgr.SetRainState(state);
    }
}

// 0x006B28B8
void SoundMgr::BeginPlay()
{
    if (m_IsInitialized) {
        m_State = 2;
        m_EnvMgr.StartSea();
        m_VolumeMgr.Finalize();
    }
}

// 0x006B28EC
nw::snd::SoundHandle* SoundMgr::HoldSound(u32 soundId)
{
    nw::snd::SoundHandle* handle = NULL;
    for (s32 i = 0; i < 8; i++) {
        if (m_HandleSet.m_Handles[i].GetId() == soundId) {
            handle = &m_HandleSet.m_Handles[i];
            break;
        }
    }
    if (handle == NULL) {
        handle = m_HandleSet.FindFreeHandle(0xFFFFFFFF);
        if (handle == NULL) {
            return handle;
        }
    }
    if (m_pPlayer != NULL) {
        static_cast<nw::snd::SoundArchivePlayer*>(m_pPlayer)->HoldSound(handle, soundId, NULL);
    }
    return handle;
}

// 0x006B29A4
void SoundMgr::Pause10()
{
    if (!(m_PauseFlags & PAUSE_FLAG_10)) {
        Pause(PAUSE_FLAG_10);
    }
}

// 0x006B29BC
void SoundMgr::StopHeldSound(u32 soundId)
{
    for (;;) {
        nw::snd::SoundHandle* handle = NULL;
        for (s32 i = 0; i < 8; i++) {
            if (m_HandleSet.m_Handles[i].GetId() == soundId) {
                handle = &m_HandleSet.m_Handles[i];
                break;
            }
        }
        if (handle == NULL) {
            return;
        }
        handle->Stop(0);
    }
}

// 0x006B2A78
void SoundMgr::MuteSystemAndSpeech(bool isMute)
{
    if (!m_IsInitialized) {
        return;
    }
    if (isMute) {
        m_VolumeMgr.SetPlayerVolume(5, 0.0f, 0);
        m_VolumeMgr.SetPlayerVolume(4, 0.0f, 0);
        m_IsSystemMuted = true;
    } else {
        m_VolumeMgr.SetPlayerVolume(5, 1.0f, 0);
        m_VolumeMgr.SetPlayerVolume(4, 1.0f, 0);
        m_IsSystemMuted = false;
    }
}

namespace {
// (name is ours)
inline void PauseSounds(SoundMgr* mgr, u32 flag, void (*callback)(nw::snd::SoundHandle&))
{
    switch (flag) {
    case SoundMgr::PAUSE_FLAG_4:
        if (mgr->m_pSoundPlayers[0] != NULL) {
            mgr->m_pSoundPlayers[0]->ForEachSound(callback, false);
        }
        // fall through
    case SoundMgr::PAUSE_FLAG_1:
    case SoundMgr::PAUSE_FLAG_2:
    case SoundMgr::PAUSE_FLAG_10:
        if (mgr->m_pSoundPlayers[5] != NULL) {
            mgr->m_pSoundPlayers[5]->ForEachSound(callback, false);
        }
        if (mgr->m_pSoundPlayers[11] != NULL) {
            mgr->m_pSoundPlayers[11]->ForEachSound(callback, false);
        }
        break;
    case SoundMgr::PAUSE_FLAG_20:
        if (mgr->m_pSoundPlayers[5] != NULL) {
            mgr->m_pSoundPlayers[5]->ForEachSound(callback, false);
        }
        break;
    }
}
} // namespace

// 0x006B37B8
void SoundMgr::PauseSounds(u32 flag)
{
    ::PauseSounds(this, flag, PauseSoundCallback);
}

// 0x006B3ED4
void SoundMgr::UnpauseSounds(u32 flag)
{
    ::PauseSounds(this, flag, UnpauseSoundCallback);
}

// 0x006B2B0C
void SoundMgr::Pause(u32 flag)
{
    if (m_PauseFlags & flag) {
        return;
    }
    for (s32 i = 0; i < PLAYER_NUM; i++) {
        if (s_PlayerPauseMasks[i] & flag) {
            m_VolumeMgr.PausePlayer(i);
            m_PlayerPauseFlags[i] |= flag;
        }
    }
    PauseSounds(flag);
    m_PauseFlags |= flag;
    if (flag != PAUSE_FLAG_10 && flag != PAUSE_FLAG_20) {
        sead::AudioMgr::s_pInstance->calc();
    }
}

// 0x006B2BB4
void SoundMgr::Pause2()
{
    if (!(m_PauseFlags & PAUSE_FLAG_2)) {
        Pause(PAUSE_FLAG_2);
    }
}

// 0x006B3024
void SoundMgr::Unpause(u32 flag)
{
    if (m_PauseFlags & flag) {
        for (s32 i = 0; i < PLAYER_NUM; i++) {
            if (m_PlayerPauseFlags[i] & flag) {
                if (m_PlayerPauseFlags[i] == flag) {
                    m_VolumeMgr.UnpausePlayer(i, 30);
                }
                m_PlayerPauseFlags[i] ^= flag;
            }
        }
        UnpauseSounds(flag);
    }
    m_PauseFlags ^= flag;
}

// 0x006B30CC
void SoundMgr::Unpause2()
{
    if (m_PauseFlags & PAUSE_FLAG_2) {
        Unpause(PAUSE_FLAG_2);
    }
}

// 0x006B30E4
void SoundMgr::SetFlagCC5(bool flag)
{
    _CC5 = flag;
}

// 0x006B3A58
void SoundMgr::ReadSoundParam(SoundParam* param, u32 soundId)
{
    if (!m_pArchive->ReadSoundInfo(soundId, &m_SoundInfo)) {
        return;
    }
    param->soundId = soundId;
    param->playerId = m_SoundInfo.playerId & 0xFFFFFF;
    param->priority = m_SoundInfo.playerPriority;
    u32 userParam = 0;
    if (!m_pArchive->ReadSoundUserParam(soundId, 0, userParam)) {
        userParam = 0;
    }
    param->_D = 0;
    param->kind = userParam & 3;
    u32 flags = 0;
    if (m_pArchive->ReadSound3DInfo(soundId, &m_Sound3DInfo)) {
        flags = m_Sound3DInfo.flags;
    }
    param->isPan = (flags & 4) >> 2;
    param->isSurroundPan = (flags & 8) >> 3;
    param->isPriority = (flags & 2) >> 1;
}

// 0x006B3F74
void SoundMgr::PauseHard(bool isStop)
{
    if (isStop) {
        m_IsStopped = true;
        m_MelodyMgr.Stop();
        nw::snd::internal::driver::HardwareManager::GetInstance().SetMasterVolume(0.0f, 0);
        if (!(m_PauseFlags & PAUSE_FLAG_8)) {
            Pause(PAUSE_FLAG_8);
        }
    } else if (!(m_PauseFlags & PAUSE_FLAG_4)) {
        Pause(PAUSE_FLAG_4);
    }
}

// 0x006B3FEC
void SoundMgr::UnpauseHard()
{
    if (m_PauseFlags & PAUSE_FLAG_8) {
        Unpause(PAUSE_FLAG_8);
    } else if (m_PauseFlags & PAUSE_FLAG_4) {
        Unpause(PAUSE_FLAG_4);
    }
}

// 0x006B2D94
void SoundMgr::LoadStageGroups(stage::Name stage)
{
    if (!m_IsInitialized) {
        return;
    }
    SoundDataLoadMgr* loadMgr = SoundDataLoadMgr::s_pInstance;
    bool isBusy = false;
    if (loadMgr->IsBusy(5)) {
        isBusy = true;
    }
    if (loadMgr->IsBusy(4) || isBusy) {
        return;
    }
    u32 group = stage::GetSoundGroup(stage);
    if (group == 0) {
        group = 0xFFFFFFFF;
        if (stage == stage::NAME_CLUB444_DJKK) {
            s32 kind = Func1668A0();
            if (kind == 0) {
                group = GROUP_CLUB444_HS;
            } else if (kind == 1) {
                group = GROUP_CLUB444_FC;
            } else {
                group = GROUP_CLUB444_HS;
            }
        }
    }
    if (m_IsPlantTree && group == GROUP_VILLAGE) {
        group = GROUP_PLANT_TREE;
    } else if (group == 0xFFFFFFFF) {
        goto extra;
    }
    if (loadMgr->GetLevel(5) == m_GroupBaseLevel) {
    load:
        loadMgr->Load(group, 5);
        m_StageGroup = group;
    } else if (m_StageGroup != group) {
        loadMgr->Unload(m_GroupBaseLevel, 5);
        m_StageGroup = 0xFFFFFFFF;
        goto load;
    }
    if (m_IsPlantTree && group == GROUP_PLANT_TREE) {
        m_IsPlantTree = false;
    }
    {
        s32 tick = group == GROUP_HOUSE ? 36 : 72;
        if (m_SkipIntervalTick != tick && tick > 0) {
            m_TickScale = 4.0f / (tick * 1000.0f);
            nw::snd::internal::driver::SequenceSoundPlayer::SetSkipIntervalTickEntry(tick);
            m_SkipIntervalTick = tick;
        }
    }
extra:
    if (HeapInit::s_pSoundDataHeap == NULL) {
        return;
    }
    if (group == GROUP_ESCAPE) {
        loadMgr->CreateStageHeaps(false);
        loadMgr->Load(GROUP_ESCAPE_EXT, 4);
        return;
    }
    loadMgr->CreateStageHeaps(true);
    if (group == GROUP_HOUSE) {
        loadMgr->Load(GROUP_HOUSE_EXT, 4);
    } else if (group == GROUP_CLUB444_HS) {
        loadMgr->Load(GROUP_CLUB444_HS_EXT, 4);
    } else if (group == GROUP_CLUB444_FC) {
        loadMgr->Load(GROUP_CLUB444_FC_EXT, 4);
    } else if (stage::IsKind(stage::KIND_ISLAND_DEMO | stage::KIND_SCREEN | stage::KIND_OUTDOOR |
                                 stage::KIND_WORLD_CURVE,
                             stage)) {
        loadMgr->Load(GROUP_SEA_EXT, 4);
    }
}
