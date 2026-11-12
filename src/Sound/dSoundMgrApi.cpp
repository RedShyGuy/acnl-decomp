#include "Sound/dSoundMgr.h"
#include "Audio/bgm/dBgmMgr.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Audio/bgm/dBgmTrackMixer.h"
#include "Other/dHeapInit.h"
#include "Other/dTourResult.h"
#include "Sound/dSoundBarcarollePlayer.h"
#include "Sound/dSoundDJPlayer.h"
#include "Sound/dSoundDataLoadThread.h"
#include "Sound/dSoundMicMgr.h"
#include "Sound/dSoundLivePlayer.h"
#include "Sound/dSoundRiverMgr.h"
#include "Sound/dSoundRoomMusicPlayer.h"
#include "Sound/dSoundSeaMgr.h"
#include "Sound/sound_Extern.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "sead/seadExpHeap.h"
#include "Sv/dSvPlayer.h"
#include "netgame/dGameFunctions.h"
#include "netgame/netgame_Api.h"
#include "stage/stage_Api.h"
#include "time/time_Api.h"
#include <cstring>

// The static interface of the sound layer (a code block of its own in the original).

// the interface of the game ---------------------------------------------------------------------------------------------

// 0x0094F438
s16 SoundMgr::s_NextSoundValue = -1;
// 0x0094F434
bool SoundMgr::s_IsJinglePending = false;
// 0x0094F436
s16 SoundMgr::s_NextBgmParam = -1;
// 0x0094F43C
u32 SoundMgr::s_PendingJingle = 0;
// 0x0094F454
bool SoundMgr::s_IsRiverCreated = false;
// 0x0094F448
bool SoundMgr::s_IsBarcarolleCreated = false;
// nothing sets it
// 0x0094F449
bool SoundMgr::s_IsBarcarolleFlag = false;
// 0x0094F452
bool SoundMgr::s_IsLiveCreated = false;
// 0x0094F453
bool SoundMgr::s_IsLiveRequested = false;
// 0x0094F455
bool SoundMgr::s_IsRoomMusicCreated = false;
// 0x0094F456
bool SoundMgr::s_IsSeaCreated = false;

namespace {
// 0x00987A80
SoundMelody s_PlayMelodyNotes;
// 0x00987A60
SoundMelody s_TownMelody;

// (name is ours)
inline void PlayMelodyCopy(u32 soundId, const u8* notes)
{
    SoundMgr* mgr = SoundMgr::s_pInstance;
    if (mgr->m_MelodyMgr.m_Player.m_State != 0 || mgr->m_MelodyMgr.m_Delay > 0) {
        return;
    }
    std::memcpy(s_PlayMelodyNotes.notes, notes, sizeof(s_PlayMelodyNotes.notes));
    mgr = SoundMgr::s_pInstance;
    if (!mgr->IsPaused() && !mgr->m_IsStopped) {
        mgr->m_MelodyMgr.Play(soundId, s_PlayMelodyNotes.notes);
    }
}
} // namespace

// 0x00583BE0
void SoundMgr::SPlayTownMelody(u32 soundId)
{
    SoundMelody melody;
    UnpackMelody(reinterpret_cast<const u8*>(GetGardenData()) + 0x6E21A, &melody);
    PlayMelodyCopy(soundId, melody.notes);
}

// 0x00583C88
void SoundMgr::SPlayPackedMelody(u32 soundId, const u8* packed)
{
    SoundMelody melody;
    UnpackMelody(packed, &melody);
    PlayMelodyCopy(soundId, melody.notes);
}

// 0x0058E014
void SoundMgr::SUpdateTownMelody()
{
    UnpackMelody(reinterpret_cast<const u8*>(GetGardenPlus()) + 0x80 + 0x6E21A, &s_TownMelody);
    s_pInstance->m_MelodyMgr.SetMelody(s_TownMelody);
}


// 0x005838B8
void SoundMgr::SUnpause10(u32 arg)
{
    s_pInstance->Unpause10(arg);
}

// 0x005838CC
void SoundMgr::SPlayBgm(u32 soundId)
{
    s_pInstance->m_BgmMgr.Play(soundId);
}

// 0x00583920
void SoundMgr::SSetRainState(u32 state)
{
    s_pInstance->SetRainState(state);
}

// 0x005839E4
void SoundMgr::SBeginPlay()
{
    if (s_pInstance != NULL) {
        s_pInstance->BeginPlay();
    }
}

// 0x00583D20
void SoundMgr::SPlayMelody(u32 soundId, const u8* notes)
{
    PlayMelodyCopy(soundId, notes);
}

// 0x00583DA8
void SoundMgr::SStopJingle(s32 fadeFrames)
{
    s_pInstance->m_BgmMgr.StopJingle(fadeFrames);
}

// 0x00583DCC
void SoundMgr::SStopSub(s32 fadeFrames)
{
    s_pInstance->m_BgmMgr.StopSub(fadeFrames);
}

// 0x00583DE8
void SoundMgr::SPause10()
{
    s_pInstance->Pause10();
}

// 0x00583DF8
void SoundMgr::SPlayCarnival(u8 mode)
{
    s_pInstance->m_BgmMgr.PlayCarnival(mode);
}

// 0x00583E14
bool SoundMgr::SIsBgmActive()
{
    return s_pInstance->m_BgmMgr.m_State != 0;
}

// 0x00583E68
void SoundMgr::SMuteSystemAndSpeech(bool isMute)
{
    s_pInstance->MuteSystemAndSpeech(isMute);
}

// 0x00583E7C
void SoundMgr::SSetBgmVolume(f32 volume, s32 frames)
{
    s_pInstance->m_BgmMgr.SetVolume(volume, frames);
}

// 0x00583E98
void SoundMgr::SPlaySystemSound(u32 soundId, f32 pan)
{
    s_pInstance->PlaySystemSound(soundId, pan);
}

// 0x00583EAC
void SoundMgr::SPlayJingle(u32 soundId)
{
    s_pInstance->m_BgmMgr.PlayJingle(soundId);
}

// 0x00583ECC
void SoundMgr::SStartJinglePrepared()
{
    s_pInstance->m_BgmMgr.StartJinglePrepared();
}

// 0x00583F08
void SoundMgr::SStopSubNow(s32 fadeFrames)
{
    s_pInstance->m_BgmMgr.StopSubNow(fadeFrames);
}

// 0x00583F28
void SoundMgr::SResetSub()
{
    s_pInstance->m_BgmMgr.ResetSub();
}

// 0x00583F40
void SoundMgr::SStopTimeChime()
{
    s_pInstance->m_MelodyMgr.m_TimeChime.Stop(20);
}

// 0x00583FA4
bool SoundMgr::SHasHeaps23()
{
    return s_pInstance->HasHeaps23();
}

// 0x00583FB4
void SoundMgr::SLoadStageGroups(stage::Name stage)
{
    s_pInstance->LoadStageGroups(stage);
}

// 0x00583FE4
void SoundMgr::SSetEnvPosition(s32 x, s32 y)
{
    s_pInstance->m_EnvMgr.SetPosition(x, y);
}

// 0x00584004
void SoundMgr::SPlayBingo()
{
    s_pInstance->m_BgmMgr.PlayBingo(s_NextBgmParam != -1);
    s_NextBgmParam = -1;
}

// 0x00584048
void SoundMgr::SPlayPrologue()
{
    s_pInstance->m_BgmMgr.PlayPrologue();
}

// 0x00584060
void SoundMgr::SPlayTimeChime()
{
    SoundMgr* mgr = s_pInstance;
    if (!mgr->IsPaused() && !mgr->m_IsStopped) {
        mgr->m_MelodyMgr.PlayTimeChime();
    }
}

// 0x005840A8
void SoundMgr::SSetBoost(bool isBoost)
{
    s_pInstance->m_BgmMgr.SetBoost(isBoost);
}

// 0x005840C4
void SoundMgr::SSetPrologueStep(u8 step)
{
    s_pInstance->m_BgmMgr.SetPrologueStep(step);
}

// 0x005840E0
const SoundMelody* SoundMgr::SGetExtraDefaultMelody()
{
    return s_pInstance->m_MelodyMgr.GetExtraDefaultMelody();
}

// 0x005840F8
void SoundMgr::SPlayEditPreview(const f32* pos, f32 pan)
{
    s_pInstance->PlayEditPreview(pos, pan);
}

// 0x00584128
void SoundMgr::SPlayJingleDelayed(u32 soundId)
{
    s_PendingJingle = soundId;
    s_IsJinglePending = true;
}

// 0x00584140
void SoundMgr::SPlaySymbolTree()
{
    s_pInstance->m_BgmMgr.PlaySymbolTree();
}

// 0x00584158
void SoundMgr::SSetDowntownLevel(u8 level)
{
    s_pInstance->m_BgmMgr.SetDowntownLevel(level);
}

// 0x00584174
void SoundMgr::SSetVolume(u32 volume, f32 value, s32 frames)
{
    s_pInstance->SetVolume(volume, value, frames);
}

// 0x005841A8
void SoundMgr::SPlayCharacterMelody(const sead::SafeStringBase<char>& name)
{
    s_pInstance->PlayCharacterMelody(name, NULL);
}

// 0x005841C0
void SoundMgr::SUnloadStageGroups(u32 arg)
{
    s_pInstance->UnloadStageGroups(arg);
}

// 0x005841D4
u32 SoundMgr::SGetBgmId()
{
    return s_pInstance->m_BgmMgr.m_Handle.getId();
}

// 0x005841EC
bool SoundMgr::SIsMelodyBusy()
{
    SoundMelodyMgr& melodyMgr = s_pInstance->m_MelodyMgr;
    return melodyMgr.m_Player.m_State != 0 || melodyMgr.m_Delay > 0;
}

// 0x00584224
void SoundMgr::SPause20()
{
    s_pInstance->Pause20();
}

// 0x00584234
void SoundMgr::SStartHeldSound(u32 soundId, f32 pan)
{
    s_pInstance->StartHeldSound(soundId, pan);
}

// 0x00584248
void SoundMgr::SStopEnvLoops(s32 fadeFrames)
{
    s_pInstance->m_EnvMgr.StopLoops(fadeFrames);
}

// 0x00584260
const SoundMelody* SoundMgr::SGetRandomDefaultMelody()
{
    return s_pInstance->m_MelodyMgr.GetRandomDefaultMelody();
}

// 0x00584278
s32 SoundMgr::SGetMelodyNoteIndex()
{
    return s_pInstance->m_MelodyMgr.m_Player.m_NoteIndex;
}

// 0x0058428C
void SoundMgr::SPlayInsectSound(u32 insect)
{
    s_pInstance->PlayInsectSound(insect);
}

// 0x005842A0
bool SoundMgr::SIsJingleAttached()
{
    return s_pInstance->m_BgmMgr.m_JingleHandle.m_pSound != NULL;
}

// 0x005842BC
void SoundMgr::SUnpause20()
{
    s_pInstance->Unpause20();
}

// 0x005842CC
void SoundMgr::SPrepareStageChange(stage::Name next)
{
    s_pInstance->PrepareStageChange(next);
}

// 0x005842E0
void SoundMgr::SPlayMuffledSound(u32 soundId)
{
    s_pInstance->PlayMuffledSound(soundId);
}

// 0x005842F4
void SoundMgr::SStartShaper()
{
    s_pInstance->m_EnvMgr.StartShaper();
}

// 0x0058430C
void SoundMgr::SStartBgmPrepared()
{
    s_pInstance->m_BgmMgr.StartPrepared();
}

// 0x00584324
bool SoundMgr::SIsTimeChimeActive()
{
    return s_pInstance->m_MelodyMgr.m_TimeChime.m_State != 0;
}

// 0x00584340
void SoundMgr::SEnterStage(stage::Name stage)
{
    if (s_pInstance != NULL) {
        s_pInstance->EnterStage(stage);
    }
}

// 0x00584360
void SoundMgr::SSetFade(bool isDown, s32 frames)
{
    s_pInstance->SetFade(isDown, frames);
}

// 0x00584378
void SoundMgr::SetNextBgmParam(s16 param)
{
    s_NextBgmParam = param;
}

// 0x00584388
s32 SoundMgr::SGetHaniwaPlayableCount()
{
    return s_pInstance->GetHaniwaPlayableCount();
}

// 0x00584398
void SoundMgr::SUpdateTrainFaraway(const nn::math::VEC3* pos)
{
    s_pInstance->UpdateTrainFaraway(pos);
}

// 0x005843AC
bool SoundMgr::SIsLoadingStageGroup()
{
    return s_pInstance->IsLoadingStageGroup();
}

// 0x005843BC
s8 SoundMgr::SGetBgmE2()
{
    return s_pInstance->m_BgmMgr._E2;
}

// 0x005843D4
void SoundMgr::SSetEnvLoopParam(f32 distance, f32 pan)
{
    s_pInstance->m_EnvMgr.SetLoopParam(distance, pan);
}

// 0x0058441C
u32 SoundMgr::SGetJingleId()
{
    return s_pInstance->m_BgmMgr.m_JingleHandle.getId();
}

// 0x00584500
void SoundMgr::SPlayEditorNote(u32 soundId, u8 note)
{
    s_pInstance->m_MelodyMgr.PlayEditorNote(soundId, &note);
}

// 0x0058452C
void SoundMgr::SPlayCandyBreak(s16 chain, s16 value, f32 pan)
{
    s_pInstance->PlayCandyBreak(chain, value, pan);
}

// 0x00584544
void SoundMgr::SPlayPanelBreak(u32 size, s16 chain, s16 value, f32 pan)
{
    s_pInstance->PlayPanelBreak(size, chain, value, pan);
}

// 0x00584560
void SoundMgr::SStopBgm()
{
    s_pInstance->m_BgmMgr.Stop();
}

// 0x00584580
void SoundMgr::SStopObjSong(s32 fadeFrames)
{
    s_pInstance->m_BgmMgr.StopObjSong(fadeFrames);
}

// 0x0058459C
void SoundMgr::SMuteForEvent(bool isMute, s32 frames)
{
    s_pInstance->MuteForEvent(isMute, frames);
}

// 0x005845D0
void SoundMgr::SSetPlantTree()
{
    s_pInstance->m_IsPlantTree = true;
}

// 0x005845F0
void SoundMgr::SSetFx6(bool isOn)
{
    s_pInstance->SetFx6(isOn);
}

// 0x00584604
void SoundMgr::SSetPuzzleLevel(f32 level)
{
    s_pInstance->m_BgmMgr.SetPuzzleLevel(level);
}

// 0x0058461C
void SoundMgr::SStartCafe()
{
    s_pInstance->m_BgmMgr.StartCafe();
}

// 0x00584638
void SoundMgr::SStartSubPrepared()
{
    s_pInstance->m_BgmMgr.StartSubPrepared();
}

// 0x00584658
void SoundMgr::SPlayObjSong(u32 song)
{
    s_pInstance->m_BgmMgr.PlayObjSong(song);
}

// 0x00584674
s8 SoundMgr::SGetBgmE1()
{
    return s_pInstance->m_BgmMgr._E1;
}

// 0x005846A8
void SoundMgr::SPlayPanelGarbage(s16 chain, f32 pan)
{
    s_pInstance->PlayPanelGarbage(chain, pan);
}

// 0x005846BC
void SoundMgr::SSetFx7(bool isOn)
{
    s_pInstance->SetFx7(isOn);
}

// 0x005846D0
bool SoundMgr::SIsRainAudible()
{
    return s_pInstance != NULL && s_pInstance->m_EnvMgr.m_RainArea != 0;
}

// 0x005846F4
void SoundMgr::SSetBgmTrackVolume(u32 trackBitFlag, f32 volume, s32 frames)
{
    s_pInstance->m_BgmMgr.SetTrackVolume(trackBitFlag, volume, frames);
}

// 0x00584714
bool SoundMgr::SIsObjSongActive()
{
    return s_pInstance->m_BgmMgr.m_ObjState != 0;
}

// 0x00584730
void SoundMgr::SPlayFurnitureMove(f32 speed)
{
    s_pInstance->PlayFurnitureMove(speed);
}

// 0x00584740
void SoundMgr::SSetFx8(bool isOn)
{
    s_pInstance->SetFx8(isOn);
}

// 0x00583A00
bool SoundMgr::SIsBarcarolleBetween()
{
    return SoundBarcarollePlayer::s_pInstance->m_State == SoundBarcarollePlayer::STATE_BETWEEN;
}

// 0x00583A20
bool SoundMgr::SIsBarcarolleSinging()
{
    return SoundBarcarollePlayer::s_pInstance->m_IsSinging;
}

// 0x00583A38
s16 SoundMgr::SGetBarcarolleNoteIndex()
{
    return SoundBarcarollePlayer::s_pInstance->m_NoteIndex;
}

// 0x00583A4C
s16 SoundMgr::SGetBarcarollePhrase()
{
    return SoundBarcarollePlayer::s_pInstance->m_Phrase;
}

// 0x00583A64
void SoundMgr::SStopBarcarolleAndClear()
{
    SoundBarcarollePlayer::s_pInstance->Stop();
    s_IsBarcarolleFlag = false;
}

// 0x00583A8C
s32 SoundMgr::SGetBarcarolleFrames()
{
    return SoundBarcarollePlayer::s_pInstance->m_Frames;
}

// 0x00583ABC
void SoundMgr::SPlayBarcarollePacked(u32 param)
{
    SoundBarcarollePlayer::s_pInstance->Play((param & 0x1000000) >> 24, (param << 16) >> 24, (param << 8) >> 24);
}

// 0x00583AE4
void SoundMgr::SCreateBarcarolle()
{
    SoundBarcarollePlayer::createInstance(HeapInit::s_pStageHeap);
    s_IsBarcarolleCreated = true;
}

// 0x00583B0C
void SoundMgr::SDestroyBarcarolle()
{
    SoundBarcarollePlayer::destroyInstance();
    s_IsBarcarolleCreated = false;
}

// 0x00583B28
void SoundMgr::SPlayBarcarolleSecond()
{
    SoundBarcarollePlayer::s_pInstance->PlaySecond();
}

// 0x00583B38
void SoundMgr::SStopBarcarolle()
{
    SoundBarcarollePlayer::s_pInstance->Stop();
}

// 0x00583B48
void SoundMgr::SPlayBarcarolle(u32 kind, u32 song, u32 variation)
{
    SoundBarcarollePlayer::s_pInstance->Play(kind, song, variation);
}

// 0x00583B64
u8 SoundMgr::SGetBarcarollePart()
{
    return SoundBarcarollePlayer::s_pInstance->GetPart();
}

// 0x00583B74
void SoundMgr::SStopBarcarolleNow(s32 arg)
{
    SoundBarcarollePlayer::s_pInstance->StopNow(arg);
}

// 0x00583B88
bool SoundMgr::SIsBarcarolleActive()
{
    return SoundBarcarollePlayer::s_pInstance->m_State != SoundBarcarollePlayer::STATE_NONE;
}

// 0x00583BA4
void SoundMgr::SPlayBarcarolleRandomMatching(u32 kind)
{
    SoundBarcarollePlayer::s_pInstance->PlayRandomMatching(kind);
}

// 0x00583BB8
void SoundMgr::SUpdateBarcarolle()
{
    SoundBarcarollePlayer::s_pInstance->Update();
}

// 0x00583BC8
s16 SoundMgr::SGetBarcarolleNoteKind()
{
    return SoundBarcarollePlayer::s_pInstance->m_NoteKind;
}

// 0x00584770
bool SoundMgr::SPlayAmiiboCameraTrans(f32 speed)
{
    return s_pInstance->PlayAmiiboCameraTrans(speed);
}

// 0x00584780
void SoundMgr::SPlayAmiiboCameraSlider(f32 position, f32 filter)
{
    s_pInstance->PlayAmiiboCameraSlider(position, filter);
}

// 0x0058D6FC
void SoundMgr::SCreateEnvMgrs()
{
    SCreateRiverMgr();
    SCreateSeaMgr();
}

// 0x0058D70C
void SoundMgr::SCreateSeaMgr()
{
    if ((!stage::IsKind(stage::KIND_VILLAGE, stage::GetCurrent()) || stage::GetCurrent() == stage::NAME_VILLAGE_PROLOGUE) &&
        !stage::IsKind(stage::KIND_DOWNTOWN, stage::GetCurrent()) &&
        !stage::IsKind(stage::KIND_ISLAND, stage::GetCurrent()) && !stage::IsKind(stage::KIND_TOUR, stage::GetCurrent())) {
        return;
    }
    SoundSeaMgr::createInstance(HeapInit::s_pStageHeap);
    s_IsSeaCreated = true;
    if (stage::IsKind(stage::KIND_VILLAGE, stage::GetCurrent())) {
        if (s_IsSeaCreated) {
            SoundSeaMgr::s_pInstance->m_Type = SoundSeaMgr::TYPE_VILLAGE;
        }
        bool side = GetGardenPlus()->data.townData.unk_0x5C.oceanSide == 1;
        if (s_IsSeaCreated) {
            SoundSeaMgr::s_pInstance->SetOceanSide(side);
        }
    } else if (stage::IsKind(stage::KIND_DOWNTOWN, stage::GetCurrent())) {
        if (s_IsSeaCreated) {
            SoundSeaMgr::s_pInstance->m_Type = SoundSeaMgr::TYPE_DOWNTOWN;
        }
        bool side = GetGardenPlus()->data.townData.unk_0x5C.oceanSide == 1;
        if (s_IsSeaCreated) {
            SoundSeaMgr::s_pInstance->SetOceanSide(side);
        }
    } else if (stage::IsKind(stage::KIND_TOUR, stage::GetCurrent())) {
        u8 state = TourResult::s_pInstance->m_State;
        if (state == 0) {
            if (s_IsSeaCreated) {
                SoundSeaMgr::s_pInstance->m_Type = SoundSeaMgr::TYPE_VILLAGE;
            }
            bool side = TourResult::s_pInstance->m_OceanSide == 1;
            if (s_IsSeaCreated) {
                SoundSeaMgr::s_pInstance->SetOceanSide(side);
            }
        } else if (state == 1) {
            if (s_IsSeaCreated) {
                SoundSeaMgr::s_pInstance->SetType(SoundSeaMgr::TYPE_TOUR_1);
            }
        } else if (state == 2) {
            if (s_IsSeaCreated) {
                SoundSeaMgr::s_pInstance->SetType(SoundSeaMgr::TYPE_TOUR_2);
            }
        }
    } else if (stage::IsKind(stage::KIND_ISLAND, stage::GetCurrent())) {
        if (s_IsSeaCreated) {
            SoundSeaMgr::s_pInstance->SetType(SoundSeaMgr::TYPE_ISLAND);
        }
    }
}

// 0x0058D968
void SoundMgr::SDestroyEnvMgrs()
{
    SDestroyRiverMgr();
    SDestroySeaMgr();
}

// 0x0058D978
void SoundMgr::SDestroySeaMgr()
{
    if (s_IsSeaCreated) {
        s_IsSeaCreated = false;
        SoundSeaMgr::destroyInstance();
    }
}

// 0x0058D99C
void SoundMgr::SUpdateSeaMgr()
{
    if (s_IsSeaCreated) {
        SoundSeaMgr::s_pInstance->Update();
    }
}

// 0x0058D9C4
void SoundMgr::SSetSeaListenerPos(const nn::math::VEC3* pos)
{
    if (s_IsSeaCreated) {
        SoundSeaMgr::s_pInstance->m_pListenerPos = pos;
    }
}

namespace {
const u8 LIVE_REQUEST_KIND = 8;
const u8 LIVE_EVENT_INDEX = 18;

// (name is ours)
inline void RequestLive()
{
    SoundMgr::s_IsLiveRequested = true;
    audio::bgm::RequestMgr::EventParam param = {20, 0, -1, 0};
    audio::bgm::GetRequestMgr()->RequestEvent(LIVE_REQUEST_KIND, 0, LIVE_EVENT_INDEX, param, 0);
}
} // namespace

// 0x0058DA40
s16 SoundMgr::SGetLiveBeatsPerBar()
{
    return SoundLivePlayer::s_pInstance->GetBeatsPerBar();
}

// 0x0058DA50
s16 SoundMgr::SGetLiveVariable2()
{
    return SoundLivePlayer::s_pInstance->m_Variable2;
}

// 0x0058DA68
bool SoundMgr::SIsLivePlaying()
{
    return SoundLivePlayer::s_pInstance->m_IsPlaying;
}

// 0x0058DA7C
bool SoundMgr::SPlayLiveExtra(u32 index, u32 milliSeconds)
{
    bool isStarted = SoundLivePlayer::s_pInstance->PlayExtra(index, milliSeconds);
    if (isStarted) {
        RequestLive();
    }
    return isStarted;
}

// 0x0058DAF8
void SoundMgr::SCreateLive()
{
    SoundLivePlayer::createInstance(HeapInit::s_pStageHeap);
    SoundLivePlayer::s_pInstance->AllocateBuffers();
    s_IsLiveCreated = true;
}

// 0x0058DB24
void SoundMgr::SDestroyLive()
{
    if (s_IsLiveRequested) {
        s_IsLiveRequested = false;
        audio::bgm::GetRequestMgr()->CancelEvent(LIVE_REQUEST_KIND, LIVE_EVENT_INDEX);
    }
    SoundLivePlayer::s_pInstance->FreeBuffers();
    SoundLivePlayer::destroyInstance();
    s_IsLiveCreated = false;
}

// 0x0058DB78
f32 SoundMgr::SGetLiveBeatPhase()
{
    return SoundLivePlayer::s_pInstance->m_BeatPhase;
}

// 0x0058DB90
u32 SoundMgr::SGetLiveEndMs()
{
    return SoundLivePlayer::s_pInstance->m_EndMs;
}

// 0x0058DBA4
f32 SoundMgr::SGetLiveMeasurePhase()
{
    return SoundLivePlayer::s_pInstance->m_MeasurePhase;
}

// 0x0058DBBC
f32 SoundMgr::SGetLiveBarPhase()
{
    return SoundLivePlayer::s_pInstance->m_BarPhase;
}

// 0x0058DBD4
void SoundMgr::SUpdateLive()
{
    SoundLivePlayer::s_pInstance->Update();
    if (s_IsLiveRequested && !SoundLivePlayer::s_pInstance->m_IsPlaying) {
        s_IsLiveRequested = false;
        audio::bgm::GetRequestMgr()->CancelEvent(LIVE_REQUEST_KIND, LIVE_EVENT_INDEX);
    }
}

// 0x0058DC2C
s16 SoundMgr::SGetLiveNoteKind()
{
    return SoundLivePlayer::s_pInstance->m_NoteKind;
}

// 0x0058DC44
s16 SoundMgr::SGetLiveVariable0()
{
    return SoundLivePlayer::s_pInstance->m_Variable0;
}

// 0x0058DC5C
s16 SoundMgr::SGetLiveMark()
{
    return SoundLivePlayer::s_pInstance->m_Mark;
}

// 0x0058DC74
s16 SoundMgr::SGetLiveVariable1()
{
    return SoundLivePlayer::s_pInstance->m_Variable1;
}

// 0x0058DC8C
bool SoundMgr::SPlayLive(u32 song, u32 milliSeconds)
{
    bool isStarted = SoundLivePlayer::s_pInstance->Play(song, milliSeconds);
    if (isStarted) {
        RequestLive();
    }
    return isStarted;
}

// 0x0058DD08
void SoundMgr::SInitializeRoomMusic()
{
    if (s_IsRoomMusicCreated) {
        SoundRoomMusicPlayer::s_pInstance->Initialize();
    }
}

// 0x0058DD30
void SoundMgr::SCreateRoomMusic()
{
    SoundRoomMusicPlayer::createInstance(HeapInit::s_pStageHeap);
    s_IsRoomMusicCreated = true;
}

// 0x0058DD58
void SoundMgr::SDestroyRoomMusic()
{
    s_IsRoomMusicCreated = false;
    SoundRoomMusicPlayer::destroyInstance();
}

// 0x0058DD6C
void SoundMgr::SUpdateRoomMusic()
{
    if (s_IsRoomMusicCreated) {
        SoundRoomMusicPlayer::s_pInstance->Update();
    }
}

// 0x0058DD94
void SoundMgr::SCreateRiverMgr()
{
    SoundRiverMgr::createInstance(HeapInit::s_pStageHeap);
    s_IsRiverCreated = true;
}

// 0x0058DDBC
void SoundMgr::SDestroyRiverMgr()
{
    s_IsRiverCreated = false;
    SoundRiverMgr::destroyInstance();
}

// 0x0058DDD0
void SoundMgr::SUpdateRiverMgr()
{
    if (s_IsRiverCreated) {
        SoundRiverMgr::s_pInstance->Update();
    }
}

// 0x0058D9EC
void SoundMgr::SCalc()
{
    if (s_IsJinglePending && s_pInstance->m_BgmMgr.m_JingleHandle.m_pSound == NULL) {
        s_pInstance->m_BgmMgr.PlayJingleStartPrepared(s_PendingJingle);
        s_IsJinglePending = false;
    }
    s_pInstance->Calc();
}

// 0x0058DDF8
void SoundMgr::SSetHeldSoundPan(u32 soundId, f32 pan)
{
    s_pInstance->SetHeldSoundPan(soundId, pan);
}

// 0x0058DE0C
void SoundMgr::SStopSound(u32 soundId, s32 fadeFrames)
{
    s_pInstance->StopSound(soundId, fadeFrames);
}

// 0x0058DE24
void SoundMgr::SPause2()
{
    s_pInstance->Pause2();
}

// 0x0058DE34
void SoundMgr::SUnpause2()
{
    s_pInstance->Unpause2();
}

// 0x0058DE44
void SoundMgr::SPause1()
{
    s_pInstance->Pause1();
}

// 0x0058DE54
void SoundMgr::SUnpause1()
{
    s_pInstance->Unpause1();
}

// 0x0058DE64
void SoundMgr::SPauseHard(bool isStop)
{
    s_pInstance->PauseHard(isStop);
}

// 0x0058DE78
void SoundMgr::SUnpauseHard()
{
    s_pInstance->UnpauseHard();
}

// 0x0058DE8C
void SoundMgr::SStopAll()
{
    s_pInstance->StopAll();
}

// 0x0058DECC
void SoundMgr::SPlaySystemSoundChecked(u32 soundId, s16 value, f32 pan)
{
    s_pInstance->PlaySystemSoundChecked(soundId, value, pan);
}

// 0x0058DEE4
void SoundMgr::SStopBgmFade(s32 fadeFrames)
{
    s_pInstance->m_BgmMgr.Stop(fadeFrames);
}

// 0x0058DF00
void SoundMgr::SPauseBgm(bool isPause, s32 frames)
{
    s_pInstance->m_BgmMgr.Pause(isPause, frames);
}

// 0x0058DF28
void SoundMgr::SPlayBgmNow(u32 soundId)
{
    s_pInstance->m_BgmMgr.PlayNow(soundId, static_cast<s8>(s_NextBgmParam));
    s_NextBgmParam = -1;
}

// 0x0058DF60
void SoundMgr::SInitialize()
{
    s_pInstance->Initialize();
}

// 0x0058DF70
void SoundMgr::SReset()
{
    s_pInstance->Reset();
}

// 0x0058DFCC
void SoundMgr::SPrepareSoundInFreeHandle(u32 soundId)
{
    s_pInstance->PrepareSoundInFreeHandle(soundId);
}

// 0x0058DE9C | libgarden [tier A]
void SoundMgr::PlaySound(SeID id, float pan)
{
    s_pInstance->PlaySystemSoundChecked(id, s_NextSoundValue, pan);
    s_NextSoundValue = -1;
}

// 0x005844F0
void SoundMgr::SetNextSoundValue(s16 value)
{
    s_NextSoundValue = value;
}

// the rest of the interface -----------------------------------------------------------------------------------------------

// 0x0094F44A
bool SoundMgr::s_IsDJCreated = false;
// 0x0094F44B
SoundMgr::InterfaceState SoundMgr::s_Interface = {0, 0, 0, 0, 0, 0, 1};

// 0x005838E8
void SoundMgr::SSetSpeechMood(u32 mood)
{
    s_pInstance->m_DoubutsugoMgr.SetMood(mood);
}

// 0x00583904
void SoundMgr::SSetSpeechVoices(const sead::CheckedArray<u8, 4>& voices)
{
    s_pInstance->m_DoubutsugoMgr.SetVoices(voices);
}

// 0x00583934
void SoundMgr::SPlaySharedSound(u32 soundId, const nn::math::VEC3* world, const void* owner, bool isIgnoreMaster)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    s_pInstance->m_SharedObjMgr.PlaySound(soundId, world, &relative, owner, isIgnoreMaster);
}

// 0x00583984
void SoundMgr::SPlaySharedSoundWithValue(u32 soundId, const nn::math::VEC3* world, const void* owner, s16 value, bool isIgnoreMaster)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    s_pInstance->m_SharedObjMgr.PlaySoundWithValue(soundId, world, &relative, owner, value, isIgnoreMaster);
}

// 0x00583E30
bool SoundMgr::SIsSpeechSyllableEntry()
{
    return SIsSpeechSyllable();
}

// 0x00583E34
bool SoundMgr::SIsSpeechSyllable()
{
    return s_pInstance->m_DoubutsugoMgr.m_IsSyllable;
}

// 0x00583EC8
void SoundMgr::SStartJinglePreparedEntry()
{
    SStartJinglePrepared();
}

// 0x00583EE4
void SoundMgr::SStopSharedSounds(s32 fadeFrames)
{
    s_pInstance->m_SharedObjMgr.StopSounds(fadeFrames);
}

// 0x00583F5C
void SoundMgr::SSetSharedSoundPosition(const nn::math::VEC3* world, const void* owner)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    s_pInstance->m_SharedObjMgr.SetPosition(world, &relative, owner);
}

// 0x00583FC8
void SoundMgr::SSetSpeechCharFrames(u32 frames)
{
    s_pInstance->m_DoubutsugoMgr.SetCharFrames(frames);
}

// 0x005840A0
void SoundMgr::SSetBoostOn()
{
    SSetBoost(true);
}

// 0x0058410C
bool SoundMgr::SIsSharedSoundUsedBy(const void* owner)
{
    return s_pInstance->m_SharedObjMgr.IsUsedBy(owner);
}

// 0x0058418C
void SoundMgr::SSetSharedVolumeScale(const void* owner, f32 scale)
{
    s_pInstance->m_SharedObjMgr.SetVolumeScale(owner, scale);
}

// 0x005843E8
void SoundMgr::SWriteSharedVariable(u32 soundId, const void* owner, s16 value, s32 variable)
{
    s_pInstance->m_SharedObjMgr.WriteVariable(soundId, owner, value, variable);
}

// 0x00584434
bool SoundMgr::SIsChimeStage(stage::Name stage)
{
    if (stage == stage::NAME_STATION_ENTRY_DEMO || stage == 157 || stage == 158) {
        return false;
    }
    SoundMgr* mgr = s_pInstance;
    if (mgr->GetStageSoundKind(stage) != 0) {
        SvPlayer* player = g_pSvCurrentPlayer;
        if (player == NULL) {
            return true;
        }
        if (!player->playerFlags.finishedFirstDay || player->playerFlags.unk_0x2B_7) {
            return false;
        }
        return true;
    }
    if (audio::bgm::GetStageOpusMgr()->m_Flags & audio::bgm::StageOpusMgr::FLAG_CEREMONY) {
        return false;
    }
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return false;
    }
    return mgr->GetStageSoundKind(stage) != 0;
}

// 0x005845B4
void SoundMgr::SPlaySpeechChar(u32 c, f32 pan)
{
    s_pInstance->m_DoubutsugoMgr.PlayChar(c, pan);
}

// 0x005845E8
void SoundMgr::SSetFx6On()
{
    SSetFx6(true);
}

// 0x0058468C
void SoundMgr::SSetSpeechFilterOn()
{
    SSetSpeechFilter(true);
}

// 0x00584694
void SoundMgr::SSetSpeechFilter(bool isOn)
{
    s_pInstance->SetSpeechFilter(isOn);
}

// 0x00584754
void SoundMgr::SPlaySpeechLongChar(u32 c, f32 pan)
{
    s_pInstance->m_DoubutsugoMgr.PlayLongChar(c, pan);
}

// 0x00584790
u8 SoundMgr::SGetDJPart()
{
    return SoundDJPlayer::s_pInstance->m_Part;
}

// 0x005847A4
s32 SoundMgr::SGetDJSong()
{
    return SoundDJPlayer::s_pInstance->GetSong();
}

// 0x005847B4
void SoundMgr::SSetDJForceFirstSong(bool isForced)
{
    SoundDJPlayer::s_pInstance->SetForceFirstSong(isForced);
}

// 0x005847C8
s32 SoundMgr::SGetDJNextSong()
{
    return SoundDJPlayer::s_pInstance->GetNextSong();
}

// 0x005847D8
void SoundMgr::SSetDJForceSecondSong(bool isForced)
{
    SoundDJPlayer::s_pInstance->SetForceSecondSong(isForced);
}

// 0x005847EC
void SoundMgr::SCreateDJPlayer()
{
    SoundDJPlayer::createInstance(HeapInit::s_pStageHeap);
    SoundDJPlayer::s_pInstance->Initialize();
    s_IsDJCreated = true;
}

// 0x00584824
void SoundMgr::SDestroyDJPlayer()
{
    SoundDJPlayer::s_pInstance->Finalize();
    SoundDJPlayer::destroyInstance();
    s_IsDJCreated = false;
}

// 0x00584850
u8 SoundMgr::SGetDJPlayType()
{
    return SoundDJPlayer::s_pInstance->m_PlayType;
}

// 0x00584864
u8 SoundMgr::SGetDJBeatsPerBar()
{
    return SoundDJPlayer::s_pInstance->GetBeatsPerBar();
}

// 0x00584874
f32 SoundMgr::SGetDJBarPhase()
{
    return SoundDJPlayer::s_pInstance->m_BarPhase;
}

// 0x00584888
const SoundSongQueueData* SoundMgr::SGetDJQueueData()
{
    return SoundDJPlayer::s_pInstance->GetQueueData();
}

// 0x00584898
u8 SoundMgr::SGetDJBeatsPerBar(u32 song)
{
    return SoundDJPlayer::s_pInstance->GetBeatsPerBar(song);
}

// 0x005848AC
f32 SoundMgr::SGetDJBeatPhase()
{
    return SoundDJPlayer::s_pInstance->m_BeatPhase;
}

// 0x005848C0
s32 SoundMgr::SGetDJPartRemainingFrames()
{
    return SoundDJPlayer::s_pInstance->m_PartRemainingFrames;
}

// 0x005848D4
f32 SoundMgr::SGetDJPartBeats()
{
    return SoundDJPlayer::s_pInstance->GetPartBeats();
}

// 0x005848E4
f32 SoundMgr::SGetDJPartRemaining()
{
    return SoundDJPlayer::s_pInstance->m_PartRemaining;
}

// 0x005848F8
s32 SoundMgr::SGetDJPartElapsedFrames()
{
    return SoundDJPlayer::s_pInstance->m_PartElapsedFrames;
}

// 0x0058490C
f32 SoundMgr::SGetDJPartElapsed()
{
    return SoundDJPlayer::s_pInstance->m_PartElapsed;
}

// 0x00584920
s16 SoundMgr::SUpdateDJ()
{
    return SoundDJPlayer::s_pInstance->Update();
}

// 0x00584930
void SoundMgr::SStopDJ()
{
    SoundDJPlayer::s_pInstance->Stop();
}

// 0x00584940
void SoundMgr::SPlayDJ(const SoundSongQueueData* data, u32 milliSeconds)
{
    SoundDJPlayer::s_pInstance->Play(data, milliSeconds);
}

// 0x00584D10
void SoundMgr::SStartCarnival()
{
    audio::bgm::GetStageOpusMgr()->StartCarnival();
}

// 0x0058503C
void SoundMgr::SEndCarnivalLong()
{
    audio::bgm::GetStageOpusMgr()->EndCarnivalLong();
}

// 0x0058584C
void SoundMgr::SNothing()
{
    audio::bgm::GetStageOpusMgr()->Nothing();
}

// 0x00585A84
void SoundMgr::SStartCarnival2()
{
    audio::bgm::GetStageOpusMgr()->StartCarnival2();
}

// 0x00585B20
void SoundMgr::SStartCarnival4()
{
    audio::bgm::GetStageOpusMgr()->StartCarnival4();
}

// 0x00585D48
void SoundMgr::SStartClub444Live()
{
    audio::bgm::GetStageOpusMgr()->StartClub444Live();
}

// 0x00585D8C
void SoundMgr::SStopCarnival()
{
    audio::bgm::GetStageOpusMgr()->StopCarnival();
}

// 0x00585E58
void SoundMgr::SStopClub444Live()
{
    audio::bgm::GetStageOpusMgr()->StopClub444Live();
}

// 0x00585E98
void SoundMgr::SEndCarnivalShort()
{
    audio::bgm::GetStageOpusMgr()->EndCarnivalShort();
}

// 0x0058AF64
void SoundMgr::SSetFanfareGetWait(s32 wait)
{
    audio::bgm::GetFieldMusic()->SetFanfareGetWait(wait);
}

// 0x0058D5F8
void SoundMgr::SStopMicAsr()
{
    SoundMicMgr::s_pInstance->StopAsr();
}

// 0x0058D608
s8 SoundMgr::SIsMicUser1Detected()
{
    return SoundMicMgr::s_pInstance->m_User1.m_IsDetected;
}

// 0x0058D620
bool SoundMgr::SStartMicSampling(u32 client)
{
    return SoundMicMgr::s_pInstance->StartSampling(client);
}

// 0x0058D634
s32 SoundMgr::SGetMicAsrResult()
{
    return SoundMicMgr::s_pInstance->m_Asr.GetResult();
}

// 0x0058D648
void SoundMgr::SStopMicSampling(u32 client)
{
    SoundMicMgr::s_pInstance->StopSampling(client);
}

// 0x0058D65C
void SoundMgr::SLoadMicAsr()
{
    SoundMicMgr::s_pInstance->LoadAsr();
}

// 0x0058D66C
void SoundMgr::SSetMicAsrParam(const void* data, s32 size)
{
    SoundMicMgr::s_pInstance->SetAsrParam(data, size);
}

// 0x0058D684
bool SoundMgr::SIsMicAsrLoaded()
{
    return SoundMicMgr::s_pInstance->m_Asr.m_State >= 2;
}

// 0x0058D6A4
u8 SoundMgr::SGetMicAsrProgress()
{
    return SoundMicMgr::s_pInstance->m_Asr.m_Progress;
}

// 0x0058D6BC
void SoundMgr::SStopMicUser1()
{
    SoundMicMgr::s_pInstance->StopUser1();
}

// 0x0058D6CC
bool SoundMgr::SStartMicUser1()
{
    return SoundMicMgr::s_pInstance->StartUser1();
}

// 0x0058D6DC
bool SoundMgr::SStartMicAsr()
{
    return SoundMicMgr::s_pInstance->StartAsr();
}

// 0x0058D6EC
void SoundMgr::SResetMicAsr()
{
    SoundMicMgr::s_pInstance->ResetAsr();
}

// 0x0058DE88
void SoundMgr::SStopAllEntry()
{
    SStopAll();
}

// 0x0058DF80
void SoundMgr::SHoldSharedSound(u32 soundId, const nn::math::VEC3* world, const void* owner)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    s_pInstance->m_SharedObjMgr.HoldSound(soundId, world, &relative, owner);
}

// 0x0058DFE0
void SoundMgr::SQueueSpeechChar(u16 c, s32 level)
{
    s_pInstance->m_DoubutsugoMgr.Queue(c, level);
}

// 0x0058E000
void SoundMgr::SOnGameLoaded()
{
    netgame::Func5C69C4();
    time::LoadOffset();
    SUpdateTownMelody();
}

// 0x0058E058
void SoundMgr::SStopSharedSoundsOf(const void* owner, s32 fadeFrames)
{
    s_pInstance->m_SharedObjMgr.StopSounds(owner, fadeFrames);
}

// 0x0058E078
s8 SoundMgr::SIsReset()
{
    return s_Interface.isReset;
}

// 0x0058E088
void SoundMgr::SBeginStageEnter()
{
    if (!s_Interface.isInitialized || s_Interface.isDisabled || s_Interface.isEntering) {
        return;
    }
    s_Interface.isEntering = 1;
    if (s_Interface.isExiting) {
        s_Interface.isExiting = 0;
        stage::Name next = stage::GetNext();
        if (s_Interface.isFull) {
            SReset();
            audio::bgm::StopWeather();
        }
        audio::bgm::Leave(next);
        s_Interface.isReset = 0;
    }
    stage::Name current = stage::GetCurrent();
    if (s_Interface.isFull) {
        audio::bgm::RefreshWeather();
        SEnterStage(current);
    }
    audio::bgm::ChangeStage(current);
}

// 0x0058E13C
void SoundMgr::SEndStageEnterEntry()
{
    SEndStageEnter();
}

// 0x0058E140
void SoundMgr::SEndStageEnter()
{
    if (!s_Interface.isInitialized || s_Interface.isDisabled || !s_Interface.isEntering) {
        return;
    }
    s_Interface.isEntering = 0;
    if (s_Interface.isFull) {
        SBeginPlay();
    }
    audio::bgm::Ready();
    s_Interface.isFull = 0;
    s_Interface.flag5 = 0;
}

// 0x0058E194
bool SoundMgr::SIsStageGroupLoaded()
{
    return !SIsLoadingStageGroup();
}

// 0x0058E1A4
void SoundMgr::SBeginStageExit()
{
    if (!s_Interface.isInitialized || s_Interface.isDisabled || s_Interface.isExiting) {
        return;
    }
    s_Interface.isExiting = 1;
    if (s_Interface.isEntering) {
        s_Interface.isEntering = 0;
        if (s_Interface.isFull) {
            SBeginPlay();
        }
        audio::bgm::Ready();
        s_Interface.isFull = 0;
        s_Interface.flag5 = 0;
    }
    stage::Name next = stage::GetNext();
    if (s_Interface.isFull) {
        SPrepareStageChange(next);
    }
    audio::bgm::Begin(next, s_Interface.isFull);
}

// 0x0058E238
void SoundMgr::SEndStageExitEntry()
{
    SEndStageExit();
}

// 0x0058E23C
void SoundMgr::SEndStageExit()
{
    if (!s_Interface.isInitialized || s_Interface.isDisabled || !s_Interface.isExiting) {
        return;
    }
    s_Interface.isExiting = 0;
    stage::Name next = stage::GetNext();
    if (s_Interface.isFull) {
        SReset();
        audio::bgm::StopWeather();
    }
    audio::bgm::Leave(next);
    s_Interface.isReset = 0;
}

// 0x0058E2A4
void SoundMgr::SInitializeInterface()
{
    if (!s_Interface.isInitialized) {
        SInitialize();
        audio::bgm::Initialize();
        s_Interface.isInitialized = 1;
    }
}

// 0x0058E2D0
void SoundMgr::SSetInterfaceFlag5()
{
    s_Interface.flag5 = 1;
}

// 0x0058E2E4
void SoundMgr::SSetFullStageChange()
{
    s_Interface.isFull = 1;
}

// 0x0058E2F8
void SoundMgr::SResetMusic()
{
    s_Interface.isReset = 1;
    audio::bgm::Reset();
}

// 0x0058E30C
void SoundMgr::SLoadCurrentStageGroups()
{
    if (s_Interface.isInitialized && !s_Interface.isDisabled) {
        SLoadStageGroups(stage::GetCurrent());
    }
}

// 0x0058E33C
void SoundMgr::SUnloadCurrentStageGroups()
{
    if (s_Interface.isInitialized && !s_Interface.isDisabled) {
        SUnloadStageGroups(stage::GetCurrent());
    }
}

// 0x0058E36C
void SoundMgr::SCalcInterface()
{
    if (s_pInstance == NULL || !s_Interface.isInitialized || s_Interface.isDisabled) {
        return;
    }
    SoundDataLoadThread::Wake();
    SCalc();
    SUpdateRiverMgr();
    SUpdateSeaMgr();
    audio::bgm::Update();
    audio::bgm::UpdateWeather();
}
