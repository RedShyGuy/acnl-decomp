#include "Sound/dSoundMgr.h"
#include "sead/seadAudioPlayerCtr.h"
#include "time/time_Api.h"
#include "Sound/sound_Extern.h"
#include "Sv/dSvApi.h"
#include <cstring>
#include "Sound/dSoundFurnitureMgr.h"
#include "Sound/dSoundKKInfo.h"
#include "Sound/dSoundMgr_SingletonDisposer_.h"
#include "Sound/dSoundMuInfo.h"
#include "Sound/dSoundSeqParser.h"
#include "nw/snd/snd_SoundSystem.h"
#include "sead/seadAudioSystemCtr.h"
#include "sead/seadExpHeap.h"
#include "sead/seadNew.h"
#include "Sound/dSoundHistoryMgr.h"
#include "Sound/dSoundMicMgr.h"
#include "Sound/dSoundObjBase.h"
#include "Sound/dSoundSeaMgr.h"
#include "Sound/dSoundDataLoadMgr.h"
#include "Other/dHeapInit.h"
#include "nw/snd/internal/driver/snd_SequenceSoundPlayer.h"
#include "nw/snd/internal/driver/snd_HardwareManager.h"
#include "nw/snd/internal/snd_BasicSound.h"
#include "nw/snd/snd_SoundPlayer.h"
#include "nw/snd/internal/snd_SequenceSound.h"
#include "nw/snd/snd_SequenceSoundHandle.h"
#include "sead/seadSoundHandle.h"
#include "sead/seadAudioMgr.h"
#include "sead/seadAudioSoundDataMgrCtr.h"

namespace {
// the state of the callbacks over the sounds of a player (names are ours)
// 0x0094E07C
bool s_IsPlayerFull;
// 0x0094E07D
bool s_IsCallbackDone;
// 0x0094E07E
bool s_IsStopEqual;
} // namespace

// 0x0094E080
SoundMgr* SoundMgr::s_pInstance = NULL;

namespace {
// 0x0094E084
s32 s_NewPriority;
} // namespace

// 0x0094E088
SoundMgr::SingletonDisposer_* SoundMgr::s_pSingletonDisposer = NULL;

// 0x0012155C
SoundMgr* SoundMgr::createInstance(sead::Heap* heap)
{
    if (s_pInstance == NULL) {
        u8* buffer = new (heap, 4) u8[sizeof(SoundMgr)];
        SoundMgr* instance = NULL;
        if (buffer == NULL) {
            s_pSingletonDisposer = NULL;
        } else {
            s_pSingletonDisposer = new (buffer) SingletonDisposer_(heap);
            instance = new (buffer) SoundMgr();
        }
        s_pInstance = instance;
    }
    return s_pInstance;
}

// 0x0012674C
SoundMgr::SoundMgr()
    : m_pHeap(NULL), m_pMicHeap(NULL), m_GroupBaseLevel(0), m_pWorkBuffer(NULL), m_pAudioPlayer(NULL), m_pPlayer(NULL),
      m_pArchive(NULL), m_IsInitialized(false), m_IsStopped(false), m_PauseFlags(0), m_SkipIntervalTick(0),
      m_CurrentStage(0), m_PrevStage(0), m_State(0), _CBC(0), m_StageGroup(0xFFFFFFFF), m_IsPlantTree(false),
      _CC5(false), m_SystemSoundRepeat(0), m_SystemSoundFrames(0), m_LastStartResult(255), m_pListenerPos(NULL),
      m_IsBgmDucked(false), m_IsSpeechMuted(false), m_IsSystemMuted(false), m_IsEventMuted(false),
      m_IsPreviewActive(false), _D04(0), m_GroundCover(0), m_FadeState(0), m_FadeTimer(0), m_FadeFrames(0)
{
    for (u32 i = 0; i < PLAYER_NUM; i++) {
        m_pSoundPlayers[i] = NULL;
        m_PlayerPauseFlags[i] = 0;
    }
    m_TickScale = 4.0f / (m_SkipIntervalTick * 1000.0f);
    for (s32 i = 0; i < 9; i++) {
        m_Reactions[i].count = 0;
        m_Reactions[i].frames = 0;
        m_Reactions[i].maxVolume = 0.0f;
        m_Reactions[i].panSum = 0.0f;
    }
    m_Reactions[0].soundId = SE_MR_02_SMILE_LV_FLAT;
    m_Reactions[1].soundId = SE_MR_03_HAPPY_LV_FLAT;
    m_Reactions[2].soundId = SE_MR_13_ASERU_LV_FLAT;
    m_Reactions[3].soundId = SE_MR_14_NAKU_LV_FLAT;
    m_Reactions[4].soundId = SE_MR_20_BURU_LV_FLAT;
    m_Reactions[5].soundId = SE_MR_26_TERE_LV_FLAT;
    m_Reactions[6].soundId = SE_MR_31_CHEER_LV_FLAT;
    m_Reactions[7].soundId = SE_MR_36_WRY_SMILE_LV_FLAT;
    m_Reactions[8].soundId = SE_MR_39_HESITATE_LV_FLAT;
}

// 0x0013CCE8
SoundMgr::~SoundMgr()
{
    // (only the members are destroyed)
}

// 0x00121378
void SoundMgr::Setup(sead::Heap* heap, const sead::SafeStringBase<char>& archivePath)
{
    m_pHeap = heap;
    sead::AudioSystemCtr* system = new (heap, 4) sead::AudioSystemCtr();
    system->mIsSplitInitialize = true;
    m_Loader.mHeap = m_pHeap;
    m_Loader.mExtraStreamBufferSize = 0xC000;
    m_Setting.mSystem = system;
    m_Setting.mResourceLoader = &m_Loader;
    m_Setting.appendSubset(&m_VolumeMgr);
    m_Setting.appendSubset(&m_DoubutsugoMgr);
    m_Setting.appendSubset(&m_MelodyMgr);
    m_Setting.appendSubset(&m_BgmMgr);
    m_Setting.appendSubset(&m_FxMgr);
    m_Setting.appendSubset(&m_EnvMgr);
    m_Setting.appendSubset(&m_SharedObjMgr);
    system->mSoundThreadCoreNo = 1;
    nw::snd::SoundSystem::s_VoiceCount = 20;
    system->mEnableUserThread = false;
    m_Loader.setRomArchive(archivePath);
    SoundDataLoadMgr* loadMgr = SoundDataLoadMgr::createInstance(m_pHeap);
    SoundFurnitureMgr::createInstance(m_pHeap);
    SoundSeqParser::createInstance(m_pHeap);
    SoundHistoryMgr::createInstance(m_pHeap);
    SoundKKInfo::createInstance(m_pHeap);
    SoundMuInfo::createInstance(m_pHeap);
    SoundMicMgr::createInstance(m_pHeap);
    m_pMicHeap = sead::ExpHeap::create(0x80000, sead::SafeStringBase<char>("MicHeap"), m_pHeap,
                                       sead::Heap::HEAP_DIRECTION_FORWARD, false);
    SoundMicMgr::s_pInstance->Initialize(m_pMicHeap);
    loadMgr->CreateHeaps(m_pHeap);
    m_pWorkBuffer = new (m_pHeap, 4) u8[0x10000];
    sead::AudioMgr* audioMgr = sead::AudioMgr::createInstance(m_pHeap);
    if (audioMgr != NULL) {
        audioMgr->initialize(&m_Setting, m_pHeap);
        SoundDataLoadMgr::s_pInstance->Initialize();
        m_GroupBaseLevel = SoundDataLoadMgr::s_pInstance->GetLevel(5);
    }
}

// 0x0083C3B0
const u32 SoundMgr::s_ResetStopSounds[2] = {SE_ENV_THUNDER_OUTDOOR, SE_ENV_THUNDER_INDOOR};
// 0x0083C3B8
const f32 SoundMgr::s_SystemSoundVolumes[12] = {1.0f, 0.96f, 0.85f, 0.7f, 0.55f, 0.44f, 0.4f, 0.44f, 0.55f, 0.7f, 0.85f, 0.96f};
// 0x0083C3E8
const f32 SoundMgr::s_SystemSoundPitches[12] = {1.005f, 1.0043f, 1.0025f, 1.0f,    0.9975f, 0.9957f,
                                                 0.995f, 0.9957f, 0.9975f, 1.0f,    1.0025f, 1.0043f};
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
void SoundMgr::Unpause10(u32)
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
sead::SoundHandle* SoundMgr::HoldSound(u32 soundId)
{
    sead::SoundHandle* handle = NULL;
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
    for (u32 i = 0; i < PLAYER_NUM; i++) {
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
        for (u32 i = 0; i < PLAYER_NUM; i++) {
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

namespace {
// the sound group of a stage (-1: none) (name is ours)
inline u32 GetStageGroup(stage::Name stage)
{
    u32 group = stage::GetSoundGroup(stage);
    if (group != 0) {
        return group;
    }
    if (stage != stage::NAME_CLUB444_DJKK) {
        return 0xFFFFFFFF;
    }
    if (Func1668A0() == 1) {
        return GROUP_CLUB444_FC;
    }
    return GROUP_CLUB444_HS;
}
} // namespace

// 0x00768060
bool SoundMgr::IsLoadingStageGroup()
{
    bool isLoading = false;
    SoundDataLoadMgr* loadMgr = SoundDataLoadMgr::s_pInstance;
    if (loadMgr->IsBusy(5)) {
        isLoading = true;
    }
    if (loadMgr->IsBusy(4)) {
        isLoading = true;
    }
    return isLoading;
}

// 0x006B2D94
void SoundMgr::LoadStageGroups(stage::Name stage)
{
    if (!m_IsInitialized) {
        return;
    }
    if (IsLoadingStageGroup()) {
        return;
    }
    SoundDataLoadMgr* loadMgr = SoundDataLoadMgr::s_pInstance;
    u32 group = GetStageGroup(stage);
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

// 0x006B2C00
void SoundMgr::PlaySystemSound(u32 soundId, f32 pan)
{
    if (!m_IsInitialized || m_IsStopped || m_State != 2 || m_SystemSoundFrames == 0) {
        return;
    }
    if (soundId == SE_SYS_TELOP_NET) {
        StopHeldSound(SE_SYS_TELOP_NET);
    } else if (soundId == SE_SYS_SWK_ERROR) {
        StopHeldSound(SE_SYS_SWK_ERROR);
        m_DoubutsugoMgr.StopVoice();
    }
    nw::snd::SoundHandle* handle = m_HandleSet.FindFreeHandle(0xFFFFFFFF);
    if (handle == NULL) {
        return;
    }
    if (m_pPlayer != NULL) {
        static_cast<nw::snd::SoundArchivePlayer*>(m_pPlayer)->StartSound(handle, soundId, NULL);
    }
    static_cast<sead::SoundHandle*>(handle)->setPan(GetSystemSoundPan(soundId, pan));
    if (m_SystemSoundFrames > 6) {
        m_SystemSoundRepeat = 0;
    } else {
        if (handle->m_pSound != NULL) {
            handle->m_pSound->SetVolume(s_SystemSoundVolumes[m_SystemSoundRepeat % 12], 0);
        }
        if (handle->m_pSound != NULL) {
            handle->m_pSound->SetPitch(s_SystemSoundPitches[m_SystemSoundRepeat % 12]);
        }
    }
    m_SystemSoundFrames = 0;
    OnSystemSoundStarted(soundId);
}

// 0x006B30EC
void SoundMgr::PlayEditPreview(const f32* pos, f32 pan)
{
    if (_D04 == 0 || m_PreviewPos[0] != pos[0] || m_PreviewPos[1] != pos[1]) {
        PlaySystemSound(SE_SYS_EDIT_PREVIEW_ACTIVE, pan);
        m_PreviewPos[0] = pos[0];
        m_PreviewPos[1] = pos[1];
    }
    m_IsPreviewActive = true;
}

// 0x006B3C54
f32 SoundMgr::GetSystemSoundPan(u32 soundId, f32 pan)
{
    switch (soundId) {
    case SE_SYS_DECIDE:
    case SE_SYS_DECIDE_L:
    case SE_SYS_DECIDE_QUIT:
    case SE_SYS_CANCEL:
    case SE_SYS_ERASE:
    case SE_SYS_ERASE_L:
    case SE_SYS_COPY:
    case SE_SYS_WIN_SELECT_ITEM_CLOSE:
    case SE_SYS_WIN_ATM_MENU_DECIDE:
    case SE_SYS_WIN_ATM_MENU_CANCEL:
    case SE_SYS_WIN_ATM_BTN_DECIDE:
    case SE_SYS_WIN_ATM_BTN_CANCEL:
    case SE_SYS_FRIEND_SHOUT_ON:
    case SE_SYS_FRIEND_SHOUT_OFF:
    case SE_SYS_PARTY_PUT_IN_FRIEND:
    case SE_SYS_PARTY_ADD_BEST:
    case SE_SYS_CTLG_TOP_BTN_SELECTED:
    case SE_SYS_EXH_FAVORITE_ON:
    case SE_SYS_EXH_FAVORITE_OFF:
        return 0.0f;
    }
    if (pan < -1.0f) {
        pan = -1.0f;
    } else if (pan > 1.0f) {
        pan = 1.0f;
    }
    return pan * 0.8f;
}

// 0x006B4014
void SoundMgr::OnSystemSoundStarted(u32 soundId)
{
    if (soundId != SE_SYS_WIN_TEXT_PAGE_INC) {
        nw::snd::SoundHandle* handle = NULL;
        for (s32 i = 0; i < 8; i++) {
            if (m_HandleSet.m_Handles[i].m_pSound != NULL &&
                m_HandleSet.m_Handles[i].m_pSound->m_Id == SE_SYS_WIN_TEXT_PAGE_INC) {
                handle = &m_HandleSet.m_Handles[i];
                break;
            }
        }
        if (handle != NULL) {
            nw::snd::SequenceSoundHandle sequence(handle);
            if (sequence.m_pSound != NULL) {
                sequence.m_pSound->WriteVariable(0, 1);
            }
        }
    }
    switch (soundId) {
    case SE_SYS_BTN_DEC_ACTIVE:
    case SE_SYS_BTN_INC_ACTIVE:
        StopHeldSound(SE_SYS_BTN_ACTIVE_S);
        break;
    case SE_SYS_DECIDE:
    case SE_SYS_DECIDE_L:
    case SE_SYS_CANCEL:
    case SE_SYS_WIN_SELECT_ITEM_CLOSE:
    case SE_SYS_SWK_BACKSPACE:
    case SE_SYS_SWK_ERROR:
    case SE_SYS_SWK_CURSOR:
    case SE_SYS_ITEM_ICON_SELECTED:
    case SE_SYS_ITEM_WALLET_SELECTED:
    case SE_SYS_ITEM_LETTERBOX_SELECTED_ON:
    case SE_SYS_ITEM_LETTERBOX_SELECTED_OFF:
    case SE_SYS_ITEM_PLAYERVIEW_SELECTED:
        nw::snd::internal::SequenceSound::WriteGlobalVariable(15, -1);
        break;
    case SE_SYS_TOUR_NORMA_CLEAR:
        StopHeldSound(SE_SYS_TOUR_FISH_COUNT);
        StopHeldSound(SE_SYS_TOUR_INSECT_COUNT);
        StopHeldSound(SE_SYS_TOUR_DIVE_COUNT);
        break;
    case SE_SYS_PUBLIC_SET_PLACE:
        m_BgmMgr.Duck(true);
        m_IsBgmDucked = true;
        break;
    case SE_ACT_BEV_BELCH:
        m_VolumeMgr.SetPlayerVolume(4, 0.0f, 2);
        m_IsSpeechMuted = true;
        break;
    }
}

// 0x006B31D4
void SoundMgr::PlayCharacterMelody(const sead::SafeStringBase<char>& name, const u8* notes)
{
    if (!IsPaused() && !m_IsStopped) {
        m_MelodyMgr.PlayCharacter(name, notes);
    }
}

// 0x006B3218
f32 SoundMgr::GetHeightVolume(f32 height)
{
    if (!stage::IsKind(stage::KIND_VILLAGE, stage::GetCurrent()) || stage::GetCurrent() == stage::NAME_TITLE) {
        return 1.0f;
    }
    f32 difference = 0.0f;
    if (m_pListenerPos != NULL) {
        if (height > 112.0f) {
            height = 112.0f;
        }
        difference = height - m_pListenerPos->y;
        if (difference <= 0.0f) {
            difference = -difference;
        }
    }
    f32 volume = (60.0f - (difference - 20.0f) * 0.7f) * (1.0f / 60.0f);
    if (volume < 0.3f) {
        return 0.3f;
    }
    if (volume > 1.0f) {
        return 1.0f;
    }
    return volume;
}

// 0x006B32F0
void SoundMgr::UnloadStageGroups(u32)
{
    if (!m_IsInitialized || IsLoadingStageGroup()) {
        return;
    }
    SoundDataLoadMgr* loadMgr = SoundDataLoadMgr::s_pInstance;
    if (loadMgr->GetLevel(5) > m_GroupBaseLevel) {
        u32 group = GetStageGroup(stage::GetNext());
        if (m_IsPlantTree && group == GROUP_VILLAGE) {
            group = GROUP_PLANT_TREE;
        }
        if (group != 0xFFFFFFFF && m_StageGroup != group) {
            loadMgr->Unload(m_GroupBaseLevel, 5);
            m_StageGroup = 0xFFFFFFFF;
        }
    }
    if (loadMgr->HasHeap(4)) {
        loadMgr->Unload(0, 4);
        loadMgr->DestroyStageHeaps();
    }
}

// 0x006B3418
void SoundMgr::Pause1()
{
    if (!(m_PauseFlags & PAUSE_FLAG_1)) {
        Pause(PAUSE_FLAG_1);
    }
}

// 0x006B3430
void SoundMgr::Pause20()
{
    if (!(m_PauseFlags & PAUSE_FLAG_20)) {
        Pause(PAUSE_FLAG_20);
    }
}

// 0x006B3448
void SoundMgr::StartHeldSound(u32 soundId, f32 pan)
{
    for (s32 i = 0; i < 8; i++) {
        if (m_HandleSet.m_Handles[i].GetId() == soundId) {
            sead::SoundHandle* handle = &m_HandleSet.m_Handles[i];
            if (handle->m_pSound != NULL) {
                handle->m_pSound->StartPrepared();
            }
            handle->setPan(GetSystemSoundPan(soundId, pan));
            return;
        }
    }
}

// 0x006B34D8
void SoundMgr::PlayInsectSound(u32 insect)
{
    if (m_IsStopped) {
        return;
    }
    u32 soundId;
    switch (insect) {
    case 11:
        soundId = SE_SYS_BOOK_INSECT_HONEYBEE;
        break;
    case 12:
        soundId = SE_SYS_BOOK_INSECT_HORNET;
        break;
    case 13:
        soundId = SE_SYS_BOOK_INSECT_SHORYOBATTA;
        break;
    case 14:
        soundId = SE_SYS_BOOK_INSECT_TONOSAMABATTA;
        break;
    case 15:
        soundId = SE_SYS_BOOK_INSECT_INAGO;
        break;
    case 18:
        soundId = SE_SYS_BOOK_INSECT_ABURAZEMI;
        break;
    case 19:
        soundId = SE_SYS_BOOK_INSECT_MINMINZEMI;
        break;
    case 20:
        soundId = SE_SYS_BOOK_INSECT_KUMAZEMI;
        break;
    case 21:
        soundId = SE_SYS_BOOK_INSECT_TSUKUTSUKUHOSHI;
        break;
    case 22:
        soundId = SE_SYS_BOOK_INSECT_HIGURASHI;
        break;
    case 34:
        soundId = SE_SYS_BOOK_INSECT_KOROGI;
        break;
    case 35:
        soundId = SE_SYS_BOOK_INSECT_SUZUMUSHI;
        break;
    case 36:
        soundId = SE_SYS_BOOK_INSECT_KIRIGIRISU;
        break;
    case 37:
        soundId = SE_SYS_BOOK_INSECT_OKERA;
        break;
    case 65:
        soundId = SE_SYS_BOOK_INSECT_MOSQUITO;
        break;
    case 66:
        soundId = SE_SYS_BOOK_INSECT_FLY;
        break;
    default:
        return;
    }
    if (m_IsInitialized) {
        sead::SoundHandle* handle = HoldSound(soundId);
        if (handle != NULL && handle->m_pSound != NULL) {
            handle->m_pSound->SetPan(0.0f);
        }
    }
}

// 0x006B366C
void SoundMgr::Unpause1()
{
    if (m_PauseFlags & PAUSE_FLAG_1) {
        Unpause(PAUSE_FLAG_1);
    }
}

// 0x006B3684
void SoundMgr::Unpause20()
{
    if (m_PauseFlags & PAUSE_FLAG_20) {
        Unpause(PAUSE_FLAG_20);
    }
}

// 0x006B369C
void SoundMgr::PrepareStageChange(stage::Name next)
{
    if (!m_IsInitialized) {
        return;
    }
    m_State = 3;
    m_BgmMgr.ChangeMuseumRoom(next);
    m_FxMgr.PrepareStageChange(static_cast<stage::Name>(m_CurrentStage), next);
    m_EnvMgr.OnStageChange();
    m_VolumeMgr.StartDuckDemo();
    m_PrevStage = m_CurrentStage;
    m_CurrentStage = next;
}

// 0x006B3700
void SoundMgr::PlayMuffledSound(u32 soundId)
{
    if (!m_IsInitialized || m_IsStopped) {
        return;
    }
    sead::SoundHandle* handle = m_HandleSet.FindFreeHandle(0xFFFFFFFF);
    if (handle == NULL) {
        return;
    }
    if (m_pPlayer != NULL) {
        static_cast<nw::snd::SoundArchivePlayer*>(m_pPlayer)->StartSound(handle, soundId, NULL);
    }
    if (GetStageSoundKind(stage::GetCurrent()) == 1) {
        return;
    }
    if (handle->m_pSound != NULL) {
        handle->m_pSound->SetBiquadFilter(1, 0.7f);
    }
    if (handle->m_pSound != NULL) {
        handle->m_pSound->SetVolume(0.5f, 0);
    }
}

// 0x006B3858
void SoundMgr::EnterStage(stage::Name stage)
{
    if (!m_IsInitialized) {
        return;
    }
    m_State = 1;
    m_DoubutsugoMgr.OnEnterStage();
    if (m_CurrentStage != stage) {
        m_CurrentStage = stage;
    }
    m_MelodyMgr.m_TimeChime.SetMuffled(GetStageSoundKind(stage) == 2);
    m_BgmMgr.EnterStage(stage);
    m_EnvMgr.StartStage();
    if (SoundSeaMgr::s_pInstance != NULL) {
        SoundSeaMgr::s_pInstance->OnEnterStage();
    }
    if (stage::IsKind(stage::KIND_KOTOBUKI, stage::GetCurrent()) ||
        stage::IsKind(stage::KIND_INDOOR, stage::GetCurrent())) {
        m_GroundCover = 0;
    } else if (Func6305DC()) {
        m_GroundCover = 1;
    } else if (Func63061C()) {
        m_GroundCover = 2;
    } else {
        m_GroundCover = 0;
    }
    m_SharedObjMgr.OnEnterStage();
    m_FxMgr.ChangeStage(static_cast<stage::Name>(m_PrevStage), stage);
    nw::snd::internal::SequenceSound::WriteGlobalVariable(12, -1);
    nw::snd::internal::SequenceSound::WriteGlobalVariable(13, -1);
    nw::snd::internal::SequenceSound::WriteGlobalVariable(14, -1);
    m_Fader.SoundFader::Reset(1.0f);
    m_FadeState = 0;
    m_FadeTimer = 0;
    m_FadeFrames = 0;
    _CBC = 0;
    if (m_IsSystemMuted && m_IsInitialized) {
        m_VolumeMgr.SetPlayerVolume(5, 1.0f, 0);
        m_VolumeMgr.SetPlayerVolume(4, 1.0f, 0);
        m_IsSystemMuted = false;
    }
    if (m_IsEventMuted && m_IsInitialized) {
        m_VolumeMgr.SetPlayerVolume(4, 1.0f, 2);
        m_VolumeMgr.SetPlayerVolume(17, 1.0f, 2);
        m_IsEventMuted = false;
        m_MelodyMgr.m_TimeChime.m_IsDisabled = 0;
        m_SharedObjMgr.m_IsDisabled = false;
    }
    m_VolumeMgr.EndDuckDemo();
}

// 0x006B3B14
void SoundMgr::SetFade(bool isDown, s32 frames)
{
    if (isDown) {
        if (m_FadeState == 3 || m_FadeState == 0) {
            m_Fader.SetTarget(0.35f, frames < 20 ? frames : 20);
            m_FadeFrames = frames;
            m_FadeState = frames == 0 ? 2 : 1;
            m_FadeTimer = frames;
        }
    } else if (m_FadeState == 2 || m_FadeState == 1) {
        m_Fader.SetTarget(1.0f, frames < 24 ? frames : 24);
        m_FadeFrames = frames;
        m_FadeState = frames == 0 ? 0 : 3;
        m_FadeTimer = frames;
    }
}

// 0x006B3D44
void SoundMgr::UpdateTrainFaraway(const nn::math::VEC3* pos)
{
    if (m_IsStopped || pos->x < 511.0f || pos->x > 3072.0f) {
        return;
    }
    if (time::GetCurrentDate()->hour >= 7 && time::GetCurrentDate()->hour < 19) {
        return;
    }
    if (m_pListenerPos == NULL) {
        return;
    }
    f32 distanceX = pos->x - m_pListenerPos->x;
    distanceX = distanceX < 0.0f ? -distanceX : distanceX;
    if (distanceX >= 500.0f) {
        return;
    }
    f32 distanceZ = pos->z - m_pListenerPos->z;
    distanceZ = distanceZ < 0.0f ? -distanceZ : distanceZ;
    if (distanceZ <= 300.0f || distanceZ >= 1600.0f) {
        return;
    }
    f32 volume = 1.0f;
    if (distanceZ > 1000.0f) {
        volume = 1.0f - (distanceZ - 1000.0f) * (1.0f / 600.0f);
    } else if (distanceZ < 700.0f) {
        volume = (distanceZ - 300.0f) * 0.0025f;
    }
    sead::SoundHandle* handle = HoldSound(SE_OBJ_TRAIN_RUN_FARAWAY);
    if (handle == NULL) {
        return;
    }
    if (volume < 0.0f) {
        volume = 0.0f;
    } else if (volume > 1.0f) {
        volume = 1.0f;
    }
    if (handle->m_pSound != NULL) {
        handle->m_pSound->SetVolume(volume, 0);
    }
}

// 0x00767F38
s32 SoundMgr::GetStageSoundKind(stage::Name stage)
{
    if (stage::IsKind(stage::KIND_SCREEN, stage)) {
        if (!stage::IsKind(stage::KIND_TRAIN_STATION_ENVIRONMENT, stage)) {
            return 0;
        }
        if (stage == stage::NAME_STATION_PROLOGUE_DEMO) {
            return 0;
        }
    } else if (stage::IsKind(stage::KIND_KOTOBUKI, stage) || stage::IsKind(stage::KIND_ISLAND_DEMO, stage) ||
               stage::IsKind(stage::KIND_MUSEUM, stage) || stage::IsKind(stage::KIND_CLUB, stage) ||
               stage == stage::NAME_DREAM || stage == stage::NAME_FORTUNE_TENT || stage == stage::NAME_FORTUNE ||
               stage == stage::NAME_RESET_MONITORING_CENTER || stage == stage::NAME_PLAYER_HOUSE_0_BF ||
               stage == stage::NAME_PLAYER_HOUSE_1_BF || stage == stage::NAME_PLAYER_HOUSE_2_BF ||
               stage == stage::NAME_PLAYER_HOUSE_3_BF) {
        return 0;
    }
    if (stage::IsKind(stage::KIND_INDOOR, stage) || stage::IsKind(stage::KIND_EXHIBITION, stage) ||
        stage::IsKind(stage::KIND_AUTO_CAMP, stage)) {
        return 2;
    }
    return 1;
}

namespace {
// 0x006B44E0 (name is ours)
void StopFirstSoundCallback(nw::snd::SoundHandle& handle)
{
    if (!s_IsCallbackDone) {
        handle.Stop(0);
        s_IsCallbackDone = true;
    }
}

// 0x006B4844 (name is ours)
DECOMP_NOINLINE void StopLowerPriorityCallback(nw::snd::SoundHandle& handle)
{
    if (s_IsCallbackDone) {
        return;
    }
    s32 priority = handle.m_pSound->m_Priority;
    if (priority < s_NewPriority || (priority == s_NewPriority && s_IsStopEqual)) {
        handle.Stop(0);
        s_IsPlayerFull = false;
    }
    s_IsCallbackDone = true;
}
} // namespace

// 0x006B48A0
bool SoundMgr::CanPlaySystemSound(u32 soundId)
{
    if (m_IsEventMuted && soundId != SE_SYS_CAPTURE_PREPARE && soundId != SE_SYS_CAPTURE_SHUTTER &&
        soundId != SE_SYS_TELOP_NET && soundId != SE_SYS_TELOP_ERROR) {
        return false;
    }
    switch (soundId) {
    case SE_SYS_TOUR_FISH_COUNT:
    case SE_SYS_TOUR_INSECT_COUNT:
    case SE_SYS_TOUR_DIVE_COUNT:
        if (m_HandleSet.IsPlaying(SE_SYS_TOUR_NORMA_CLEAR)) {
            return false;
        }
        break;
    case SE_SYS_WIN_TEXT_CLOSE:
        if (m_HandleSet.IsPlaying(SE_SYS_ENTER_TITLE) || m_HandleSet.IsPlaying(SE_SYS_DECIDE_L) ||
            m_HandleSet.IsPlaying(SE_SYS_PUBLIC_GET_NEW_CHOICE) || m_HandleSet.IsPlaying(SE_SYS_PUBLIC_SET_PLACE) ||
            m_HandleSet.IsPlaying(SE_SYS_RETURN_TITLE)) {
            return false;
        }
        break;
    case SE_SYS_WIN_TEXT_PAGE_INC:
        if (m_HandleSet.IsPlaying(SE_SYS_PUBLIC_GET_NEW_CHOICE) || m_HandleSet.IsPlaying(SE_SYS_QUEST_START) ||
            m_HandleSet.IsPlaying(SE_SYS_QUEST_ACHIEVE)) {
            return false;
        }
        break;
    case SE_SYS_DIALOGUE_OPEN:
        if (m_HandleSet.IsPlaying(SE_SYS_WAITING_PROCESS)) {
            return false;
        }
        break;
    case SE_ENV_THUNDER_OUTDOOR:
        if (m_EnvMgr.m_RainArea != SoundEnvMgr::RAIN_AREA_OUTDOOR || m_HandleSet.IsPlaying(SE_ENV_THUNDER_OUTDOOR)) {
            return false;
        }
        break;
    case SE_ENV_THUNDER_INDOOR:
        if (m_EnvMgr.m_RainArea != SoundEnvMgr::RAIN_AREA_INDOOR || m_HandleSet.IsPlaying(SE_ENV_THUNDER_INDOOR)) {
            return false;
        }
        break;
    }
    return !SoundHistoryMgr::s_pInstance->Contains(soundId);
}

// (the start of the checked system sounds; name is ours)
DECOMP_ALWAYS_INLINE sead::SoundHandle* SoundMgr::StartSystemSound(u32 soundId, f32 pan)
{
    if (soundId == SE_SYS_TELOP_NET) {
        StopHeldSound(SE_SYS_TELOP_NET);
    } else if (soundId == SE_SYS_SWK_ERROR) {
        StopHeldSound(SE_SYS_SWK_ERROR);
        m_DoubutsugoMgr.StopVoice();
    }
    sead::SoundHandle* handle = m_HandleSet.FindFreeHandle(0xFFFFFFFF);
    if (handle == NULL) {
        return NULL;
    }
    if (m_pPlayer != NULL) {
        static_cast<nw::snd::SoundArchivePlayer*>(m_pPlayer)->StartSound(handle, soundId, NULL);
    }
    handle->setPan(GetSystemSoundPan(soundId, pan));
    return handle;
}

// 0x006B41D8
void SoundMgr::PlayCandyBreak(s16 chain, s16 value, f32 pan)
{
    if (!m_IsInitialized || m_IsStopped) {
        return;
    }
    s32 steps = (7 - chain) / 4;
    if (-steps > 0) {
        chain = static_cast<s16>(chain + steps * 4);
    }
    if (!CanPlaySystemSound(SE_PNP_CANDY_BREAK)) {
        return;
    }
    sead::SoundHandle* handle = StartSystemSound(SE_PNP_CANDY_BREAK, pan);
    if (handle == NULL) {
        return;
    }
    nw::snd::SequenceSoundHandle sequence(handle);
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->WriteVariable(15, chain);
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->WriteVariable(14, value);
        }
    }
    SoundHistoryMgr::s_pInstance->Add(SE_PNP_CANDY_BREAK, 1);
    OnSystemSoundStarted(SE_PNP_CANDY_BREAK);
}

// 0x006B4320
void SoundMgr::PlayPanelBreak(u32 size, s16 chain, s16 value, f32 pan)
{
    if (!m_IsInitialized || m_IsStopped) {
        return;
    }
    u32 soundId;
    if (size <= 1) {
        soundId = SE_PNP_PANEL_BREAK_C1;
    } else if (size == 2) {
        soundId = SE_PNP_PANEL_BREAK_C2;
    } else if (size == 3) {
        soundId = SE_PNP_PANEL_BREAK_C3;
    } else if (size == 4) {
        soundId = SE_PNP_PANEL_BREAK_C4;
    } else {
        soundId = SE_PNP_PANEL_BREAK_C5;
    }
    if (chain > 10) {
        chain = 10;
    }
    if (!CanPlaySystemSound(soundId)) {
        return;
    }
    sead::SoundHandle* handle = StartSystemSound(soundId, pan);
    if (handle == NULL) {
        return;
    }
    nw::snd::SequenceSoundHandle sequence(handle);
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->WriteVariable(15, chain);
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->WriteVariable(14, value);
        }
    }
    SoundHistoryMgr::s_pInstance->Add(soundId, 1);
    OnSystemSoundStarted(soundId);
}

// 0x006B4514
void SoundMgr::MuteForEvent(bool isMute, s32 frames)
{
    if (!m_IsInitialized) {
        return;
    }
    if (isMute) {
        m_VolumeMgr.SetPlayerVolume(4, 0.0f, frames);
        m_VolumeMgr.SetPlayerVolume(17, 0.0f, frames);
        m_IsEventMuted = true;
    } else {
        m_VolumeMgr.SetPlayerVolume(4, 1.0f, frames);
        m_VolumeMgr.SetPlayerVolume(17, 1.0f, frames);
        m_IsEventMuted = false;
    }
    m_MelodyMgr.m_TimeChime.m_IsDisabled = isMute;
    m_SharedObjMgr.m_IsDisabled = isMute;
}

// 0x006B45B4
void SoundMgr::SetFx6(bool isOn)
{
    if (isOn) {
        m_FxMgr.Start(6);
    } else {
        m_FxMgr.FadeOut(6, 60);
    }
    m_DoubutsugoMgr.m_IsFx6 = isOn;
}

// 0x006B4718
void SoundMgr::PlayPanelGarbage(s16 chain, f32 pan)
{
    if (!m_IsInitialized || m_IsStopped) {
        return;
    }
    s32 steps = (7 - chain) / 4;
    if (-steps > 0) {
        chain = static_cast<s16>(chain + steps * 4);
    }
    if (!CanPlaySystemSound(SE_PNP_PANEL_GARBAGE)) {
        return;
    }
    sead::SoundHandle* handle = StartSystemSound(SE_PNP_PANEL_GARBAGE, pan);
    if (handle == NULL) {
        return;
    }
    nw::snd::SequenceSoundHandle sequence(handle);
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->WriteVariable(15, chain);
    }
    SoundHistoryMgr::s_pInstance->Add(SE_PNP_PANEL_GARBAGE, 1);
    OnSystemSoundStarted(SE_PNP_PANEL_GARBAGE);
}

// 0x006B4C08
void SoundMgr::PlayFurnitureMove(f32 speed)
{
    nw::snd::SoundHandle* handle = HoldSound(SE_SYS_FUR_POLY_MOVE_FOR_SLT);
    if (handle == NULL) {
        return;
    }
    nw::snd::SequenceSoundHandle sequence(handle);
    f32 value = speed * (1.0f / 270.0f);
    if (value <= 0.0f) {
        value = -value;
    }
    if (value > 127.0f) {
        value = 127.0f;
    }
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->WriteVariable(15, static_cast<s16>(static_cast<s32>(value)));
    }
}

// 0x006B4C9C
void SoundMgr::SetFx8(bool isOn)
{
    if (isOn) {
        m_FxMgr.Start(8);
    } else {
        m_FxMgr.FadeOut(8, 60);
    }
}

// 0x006B4CF4
bool SoundMgr::StartSoundChecked(nw::snd::SoundHandle* handle, u32 soundId,
                                 const nw::snd::SoundStartable::StartInfo* info)
{
    if (m_pPlayer == NULL) {
        return false;
    }
    m_LastStartResult = static_cast<nw::snd::SoundArchivePlayer*>(m_pPlayer)->StartSound(handle, soundId, info);
    return m_LastStartResult == 0;
}

// 0x006B4D24
void SoundMgr::SetListenerPos(const nn::math::VEC3* pos)
{
    m_pListenerPos = pos;
    if (SoundSeaMgr::s_pInstance != NULL) {
        SoundSeaMgr::s_pInstance->m_pListenerPos = pos;
    }
}

// 0x006B4D40
bool SoundMgr::PlayAmiiboCameraTrans(f32 speed)
{
    if (speed <= 0.16f) {
        return false;
    }
    f32 volume = Func4DC39C(0, speed, 0.16f, 4.0f, 0.3f, 1.0f, 0.78f);
    if (volume <= 0.3f) {
        volume = 0.3f;
    } else if (volume > 1.0f) {
        volume = 1.0f;
    }
    f32 tempo = Func4DC39C(0, speed, 0.16f, 4.0f, 0.6f, 1.0f, 0.78f);
    sead::SoundHandle* handle = HoldSound(SE_SYS_AMIIBO_CAMERA_TRANS);
    if (handle == NULL) {
        return false;
    }
    if (handle->m_pSound != NULL) {
        handle->m_pSound->SetVolume(volume, 0);
    }
    nw::snd::SequenceSoundHandle sequence(handle);
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->SetTempoRatio(tempo);
    }
    return true;
}

// 0x006B4E6C
void SoundMgr::PlayAmiiboCameraSlider(f32 position, f32 filter)
{
    sead::SoundHandle* handle = HoldSound(SE_SYS_AMIIBO_CAMERA_SLIDER_MOVE);
    if (handle == NULL) {
        return;
    }
    f32 pitch = Func4DC39C(0, position, 0.0f, 1.0f, 0.5f, 1.26f, 0.705f);
    if (pitch <= 0.0f) {
        pitch = 0.1f;
    } else if (pitch > 16.0f) {
        pitch = 16.0f;
    }
    if (handle->m_pSound != NULL) {
        handle->m_pSound->SetPitch(pitch);
    }
    f32 value = Func4DC39C(0, filter, 1.0f, 0.0f, 0.0f, 1.0f, 0.705f);
    if (handle->m_pSound != NULL) {
        handle->m_pSound->SetBiquadFilter(3, value);
    }
}

// 0x006B4F54
bool SoundMgr::CheckPlayerRoom(u32 soundId, const SoundParam* param, s32 priorityOffset, bool isStopEqual,
                               const SoundObjBase* obj)
{
    nw::snd::SoundPlayer* player = m_pSoundPlayers[param->playerId];
    if (m_StageGroup == GROUP_AMIIBO_CAMERA) {
        Reaction* reaction = NULL;
        switch (soundId) {
        case SE_MR_02_SMILE_LV:
            reaction = &m_Reactions[0];
            break;
        case SE_MR_03_HAPPY_LV:
            reaction = &m_Reactions[1];
            break;
        case SE_MR_13_ASERU_LV:
            reaction = &m_Reactions[2];
            break;
        case SE_MR_14_NAKU_LV:
            reaction = &m_Reactions[3];
            break;
        case SE_MR_20_BURU_LV:
            reaction = &m_Reactions[4];
            break;
        case SE_MR_26_TERE_LV:
            reaction = &m_Reactions[5];
            break;
        case SE_MR_31_CHEER_LV:
            reaction = &m_Reactions[6];
            break;
        case SE_MR_36_WRY_SMILE_LV:
            reaction = &m_Reactions[7];
            break;
        case SE_MR_39_HESITATE_LV:
            reaction = &m_Reactions[8];
            break;
        }
        if (reaction != NULL) {
            reaction->count++;
            if (reaction->maxVolume <= obj->m_DistanceVolume) {
                reaction->maxVolume = obj->m_DistanceVolume;
            }
            reaction->panSum += obj->m_Pan;
            return false;
        }
        if (player->m_SoundList.size() >= player->m_PlayableCount) {
            s_IsCallbackDone = false;
            player->ForEachSound(StopFirstSoundCallback, false);
        }
        return true;
    }
    if (player->m_PlayableCount > player->m_SoundList.size()) {
        return true;
    }
    s_NewPriority = param->priority - priorityOffset;
    s_IsPlayerFull = true;
    s_IsCallbackDone = false;
    s_IsStopEqual = isStopEqual;
    void (*callback)(nw::snd::SoundHandle&) = StopLowerPriorityCallback;
    for (nw::snd::SoundPlayer::PriorityList::iterator it = player->m_PriorityList.begin();
         it != player->m_PriorityList.end();) {
        nw::snd::internal::BasicSound* sound = nw::snd::SoundPlayer::PriorityList::GetPointerFromNode(it.GetNode());
        ++it;
        nw::snd::SoundHandle handle;
        handle.detail_AttachSoundAsTempHandle(sound);
        callback(handle);
    }
    return !s_IsPlayerFull;
}

// 0x006B5160
void SoundMgr::Calc()
{
    if (IsPaused()) {
        return;
    }
    if (sead::AudioMgr::s_pInstance != NULL) {
        sead::AudioMgr::s_pInstance->calc();
    }
    if (!m_IsInitialized) {
        return;
    }
    if (m_State != 0) {
        _CBC++;
        m_SystemSoundRepeat++;
        m_SystemSoundFrames++;
        if (m_IsPreviewActive) {
            _D04++;
            m_IsPreviewActive = false;
        } else {
            _D04 = 0;
        }
        if (m_FadeTimer > 0) {
            m_FadeTimer--;
        }
        if (m_FadeState == 1) {
            m_Fader.SoundFader::Update();
            if (m_FadeTimer == 0) {
                m_FadeState = 2;
            }
        } else if (m_FadeState == 3) {
            m_Fader.SoundFader::Update();
            if (m_FadeTimer == 0) {
                m_FadeState = 0;
            }
        }
    }
    if (m_StageGroup == GROUP_AMIIBO_CAMERA) {
        for (s32 i = 0; i < 9; i++) {
            Reaction* reaction = &m_Reactions[i];
            if (reaction->count != 0) {
                bool isPlay = true;
                if (!m_HandleSet.IsPlaying(reaction->soundId)) {
                    nw::snd::SoundPlayer* player = m_pSoundPlayers[7];
                    if (player->m_PlayableCount <= player->m_SoundList.size()) {
                        if (reaction->frames != 0) {
                            isPlay = false;
                        } else {
                            s_IsCallbackDone = false;
                            player->ForEachSound(StopFirstSoundCallback, false);
                        }
                    }
                }
                if (isPlay) {
                    sead::SoundHandle* handle = HoldSound(reaction->soundId);
                    if (handle != NULL) {
                        if (handle->m_pSound != NULL) {
                            handle->m_pSound->SetVolume(reaction->maxVolume * m_VolumeMgr.m_MasterFader.GetValue(), 0);
                        }
                        if (handle->m_pSound != NULL) {
                            handle->m_pSound->SetPan(reaction->panSum / reaction->count);
                        }
                    }
                }
                reaction->frames++;
            } else {
                reaction->frames = 0;
            }
            reaction->maxVolume = 0.0f;
            reaction->panSum = 0.0f;
            reaction->count = 0;
        }
    }
    SoundMicMgr::s_pInstance->Update();
    SoundHistoryMgr::s_pInstance->Update();
    SoundDataLoadMgr::s_pInstance->Update();
    if (m_IsBgmDucked == true && !m_HandleSet.IsPlaying(SE_SYS_PUBLIC_SET_PLACE)) {
        m_BgmMgr.Duck(false);
        m_IsBgmDucked = false;
    }
    if (m_IsSpeechMuted == true && !m_HandleSet.IsPlaying(SE_ACT_BEV_BELCH)) {
        m_VolumeMgr.SetPlayerVolume(4, 1.0f, 2);
        m_IsSpeechMuted = false;
    }
}

// 0x006B5450
void SoundMgr::StopAll()
{
    m_BgmMgr.Stop(0);
    m_BgmMgr.StopJingle(0);
    m_BgmMgr.StopObjSong(0);
    for (u32 i = 0; i < PLAYER_NUM; i++) {
        m_pSoundPlayers[i]->StopAllSound(0);
    }
    SoundDataLoadMgr::s_pInstance->UnloadHeaps();
    nw::snd::internal::driver::HardwareManager::GetInstance().SetMasterVolume(1.0f, 0);
    m_IsStopped = false;
}

// 0x006B54E8
void SoundMgr::SetHeldSoundPan(u32 soundId, f32 pan)
{
    if (!m_IsInitialized || m_IsStopped) {
        return;
    }
    sead::SoundHandle* handle = HoldSound(soundId);
    if (handle == NULL) {
        return;
    }
    if (pan < -1.0f) {
        pan = -1.0f;
    } else if (pan > 1.0f) {
        pan = 1.0f;
    }
    if (handle->m_pSound != NULL) {
        handle->m_pSound->SetPan(pan * 0.8f);
    }
}

// 0x006B5574
void SoundMgr::StopSound(u32 soundId, s32 fadeFrames)
{
    if (!m_IsInitialized) {
        return;
    }
    for (s32 i = 0; i < 8; i++) {
        if (m_HandleSet.m_Handles[i].GetId() == soundId) {
            m_HandleSet.m_Handles[i].Stop(fadeFrames);
            return;
        }
    }
}

// 0x006B562C
s32 SoundMgr::LerpInt(s32 y0, s32 y1, f32 x, f32 x0, f32 x1)
{
    if (x0 == x1) {
        return y0;
    }
    return static_cast<s32>((x - x0) * (y1 - y0) / (x1 - x0) + y0);
}

// 0x006B566C
void SoundMgr::PlaySystemSoundChecked(u32 soundId, f32 pan)
{
    if (!m_IsInitialized || m_IsStopped || !CanPlaySystemSound(soundId)) {
        return;
    }
    if (StartSystemSound(soundId, pan) == NULL) {
        return;
    }
    SoundHistoryMgr::s_pInstance->Add(soundId, 1);
    OnSystemSoundStarted(soundId);
}

// 0x006B5788
void SoundMgr::PlaySystemSoundChecked(u32 soundId, s16 value, f32 pan)
{
    if (!m_IsInitialized || m_IsStopped || !CanPlaySystemSound(soundId)) {
        return;
    }
    sead::SoundHandle* handle = StartSystemSound(soundId, pan);
    if (handle == NULL) {
        return;
    }
    nw::snd::SequenceSoundHandle sequence(handle);
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->WriteVariable(15, value);
    }
    SoundHistoryMgr::s_pInstance->Add(soundId, 1);
    OnSystemSoundStarted(soundId);
}

// 0x006B58D4
void SoundMgr::Reset()
{
    if (!m_IsInitialized) {
        return;
    }
    m_State = 0;
    _CBC = 0;
    m_EnvMgr.Stop();
    m_BgmMgr.StopEvent();
    m_VolumeMgr.ResetDucks();
    for (s32 i = 0; i < 2; i++) {
        StopHeldSound(s_ResetStopSounds[i]);
    }
    SoundMicMgr::s_pInstance->Reset();
    m_pListenerPos = NULL;
    if (m_PauseFlags & PAUSE_FLAG_10) {
        Unpause(PAUSE_FLAG_10);
    }
}

// 0x006B5968
void SoundMgr::HoldSound(nw::snd::SoundHandle* handle, u32 soundId)
{
    if (m_pPlayer != NULL) {
        static_cast<nw::snd::SoundArchivePlayer*>(m_pPlayer)->HoldSound(handle, soundId, NULL);
    }
}

// 0x006B5984
void SoundMgr::PrepareSoundInFreeHandle(u32 soundId)
{
    if (!m_IsInitialized || m_IsStopped) {
        return;
    }
    sead::SoundHandle* handle = m_HandleSet.FindFreeHandle(0xFFFFFFFF);
    if (handle != NULL && m_pPlayer != NULL) {
        static_cast<nw::snd::SoundArchivePlayer*>(m_pPlayer)->PrepareSound(handle, soundId, NULL);
    }
}

// 0x00767EA4
bool SoundMgr::HasHeaps23()
{
    if (!m_IsInitialized) {
        return false;
    }
    return SoundDataLoadMgr::s_pInstance->HasHeap(2) && SoundDataLoadMgr::s_pInstance->HasHeap(3);
}

// 0x001B0D94
void SoundMgr::SetFx7(bool isOn)
{
    if (isOn) {
        m_FxMgr.Start(7);
    } else {
        m_FxMgr.FadeOut(7, 60);
    }
}
