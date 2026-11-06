#pragma once

#include "decomp.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundFader.h"
#include "Sound/dSoundVolumeMgr.h"
#include "Sound/dSoundFxMgr.h"
#include "Sound/dSoundEnvMgr.h"
#include "nn/math/math_Vector3.h"
#include "nw/snd/snd_SoundStartable.h"
#include "nw/snd/snd_SoundArchive.h"
#include "Sound/dSoundBgmMgr.h"
#include "Sound/dSoundMelodyMgr.h"
#include "Sound/dSoundHandleSet.h"
#include "Sound/dSoundDoubutsugoMgr.h"
#include "Sound/dSoundSharedObjMgr.h"
#include "sead/seadCheckedArray.h"
#include "nw/snd/snd_SoundHandle.h"
#include "nw/snd/snd_SoundPlayer.h"
#include "sead/seadAudioSettingParameter.h"
#include "sead/seadAudioResourceLoaderCtr.h"

namespace nw { namespace snd { class SoundArchive; class SoundPlayer; } }
namespace sead { class AudioPlayer; class AudioPlayerCtr; class ExpHeap; class Heap; }

// The sound manager of the game: the sound players of the archive, the managers of the kinds of sounds (volume,
// animal speech, melodies, BGM, effects, environment, shared objects) and pausing (the class name and PlaySound are
// from libgarden, everything else is ours). (being written: the members of the managers are still raw memory)
class SoundObjBase;

struct SoundSongQueueData;

class SoundMgr
{
public:
    class SingletonDisposer_;

    // the pause reasons of m_PauseFlags (a player is paused while one of its reasons is set)
    enum PauseFlag
    {
        PAUSE_FLAG_1 = 1 << 0,
        PAUSE_FLAG_2 = 1 << 1,
        PAUSE_FLAG_4 = 1 << 2,
        PAUSE_FLAG_8 = 1 << 3,
        PAUSE_FLAG_10 = 1 << 4,
        PAUSE_FLAG_20 = 1 << 5,
    };

    SoundMgr(); // 0x0012674C
    // 0x0013CCE8
    ~SoundMgr();
    static SoundMgr* createInstance(sead::Heap* heap); // 0x0012155C

    // creates the audio system, the subsets and the singletons of the sound layer
    void Setup(sead::Heap* heap, const sead::SafeStringBase<char>& archivePath); // 0x00121378

    // a checked system sound; the value of SetNextSoundValue goes into its sequence variable 15 (-1: none)
    static void PlaySound(SeID id, float pan = 0.0f); // 0x0058DE9C | libgarden [tier A]
    static void SetNextSoundValue(s16 value); // 0x005844F0

    // the static interface of the game: the same as the members (the S is ours; only PlaySound has a known name)
    static void SUnpause10(u32 arg); // 0x005838B8
    static void SPlayBgm(u32 soundId); // 0x005838CC
    static void SSetRainState(u32 state); // 0x00583920
    static void SBeginPlay(); // 0x005839E4
    static void SPlayMelody(u32 soundId, const u8* notes); // 0x00583D20
    static void SStopJingle(s32 fadeFrames); // 0x00583DA8
    static void SStopSub(s32 fadeFrames); // 0x00583DCC
    static void SPause10(); // 0x00583DE8
    static void SPlayCarnival(u8 mode); // 0x00583DF8
    static bool SIsBgmActive(); // 0x00583E14
    static void SMuteSystemAndSpeech(bool isMute); // 0x00583E68
    static void SSetBgmVolume(f32 volume, s32 frames); // 0x00583E7C
    static void SPlaySystemSound(u32 soundId, f32 pan); // 0x00583E98
    static void SPlayJingle(u32 soundId); // 0x00583EAC
    static void SStartJinglePrepared(); // 0x00583ECC
    static void SStopSubNow(s32 fadeFrames); // 0x00583F08
    static void SResetSub(); // 0x00583F28
    static void SStopTimeChime(); // 0x00583F40
    static bool SHasHeaps23(); // 0x00583FA4
    static void SLoadStageGroups(stage::Name stage); // 0x00583FB4
    static void SSetEnvPosition(s32 x, s32 y); // 0x00583FE4
    static void SPlayBingo(); // 0x00584004
    static void SPlayPrologue(); // 0x00584048
    static void SPlayTimeChime(); // 0x00584060
    static void SSetBoost(bool isBoost); // 0x005840A8
    static void SSetPrologueStep(u8 step); // 0x005840C4
    static const SoundMelody* SGetExtraDefaultMelody(); // 0x005840E0
    static void SPlayEditPreview(const f32* pos, f32 pan); // 0x005840F8
    static void SPlayJingleDelayed(u32 soundId); // 0x00584128
    static void SPlaySymbolTree(); // 0x00584140
    static void SSetDowntownLevel(u8 level); // 0x00584158
    static void SSetVolume(u32 volume, f32 value, s32 frames); // 0x00584174
    static void SPlayCharacterMelody(const sead::SafeStringBase<char>& name); // 0x005841A8
    static void SUnloadStageGroups(u32 arg); // 0x005841C0
    static u32 SGetBgmId(); // 0x005841D4
    static bool SIsMelodyBusy(); // 0x005841EC
    static void SPause20(); // 0x00584224
    static void SStartHeldSound(u32 soundId, f32 pan); // 0x00584234
    static void SStopEnvLoops(s32 fadeFrames); // 0x00584248
    static const SoundMelody* SGetRandomDefaultMelody(); // 0x00584260
    static s32 SGetMelodyNoteIndex(); // 0x00584278
    static void SPlayInsectSound(u32 insect); // 0x0058428C
    static bool SIsJingleAttached(); // 0x005842A0
    static void SUnpause20(); // 0x005842BC
    static void SPrepareStageChange(stage::Name next); // 0x005842CC
    static void SPlayMuffledSound(u32 soundId); // 0x005842E0
    static void SStartShaper(); // 0x005842F4
    static void SStartBgmPrepared(); // 0x0058430C
    static bool SIsTimeChimeActive(); // 0x00584324
    static void SEnterStage(stage::Name stage); // 0x00584340
    static void SSetFade(bool isDown, s32 frames); // 0x00584360
    static void SetNextBgmParam(s16 param); // 0x00584378
    static s32 SGetHaniwaPlayableCount(); // 0x00584388
    static void SUpdateTrainFaraway(const nn::math::VEC3* pos); // 0x00584398
    static bool SIsLoadingStageGroup(); // 0x005843AC
    static s8 SGetBgmE2(); // 0x005843BC
    static void SSetEnvLoopParam(f32 distance, f32 pan); // 0x005843D4
    static u32 SGetJingleId(); // 0x0058441C
    static void SPlayEditorNote(u32 soundId, u8 note); // 0x00584500
    static void SPlayCandyBreak(s16 chain, s16 value, f32 pan); // 0x0058452C
    static void SPlayPanelBreak(u32 size, s16 chain, s16 value, f32 pan); // 0x00584544
    static void SStopBgm(); // 0x00584560
    static void SStopObjSong(s32 fadeFrames); // 0x00584580
    static void SMuteForEvent(bool isMute, s32 frames); // 0x0058459C
    static void SSetPlantTree(); // 0x005845D0
    static void SSetFx6(bool isOn); // 0x005845F0
    static void SSetPuzzleLevel(f32 level); // 0x00584604
    static void SStartCafe(); // 0x0058461C
    static void SStartSubPrepared(); // 0x00584638
    static void SPlayObjSong(u32 song); // 0x00584658
    static s8 SGetBgmE1(); // 0x00584674
    static void SPlayPanelGarbage(s16 chain, f32 pan); // 0x005846A8
    static void SSetFx7(bool isOn); // 0x005846BC
    static bool SIsRainAudible(); // 0x005846D0
    static void SSetBgmTrackVolume(u32 trackBitFlag, f32 volume, s32 frames); // 0x005846F4
    static bool SIsObjSongActive(); // 0x00584714
    static void SPlayFurnitureMove(f32 speed); // 0x00584730
    static void SSetFx8(bool isOn); // 0x00584740
    static bool SPlayAmiiboCameraTrans(f32 speed); // 0x00584770
    static void SPlayAmiiboCameraSlider(f32 position, f32 filter); // 0x00584780
    // the sound of the river and of the sea exist in the stages that have them
    static void SCreateEnvMgrs(); // 0x0058D6FC
    static void SCreateSeaMgr(); // 0x0058D70C
    static void SDestroyEnvMgrs(); // 0x0058D968
    static void SDestroySeaMgr(); // 0x0058D978
    static void SUpdateSeaMgr(); // 0x0058D99C
    static void SSetSeaListenerPos(const nn::math::VEC3* pos); // 0x0058D9C4
    static void SCreateRiverMgr(); // 0x0058DD94
    static void SDestroyRiverMgr(); // 0x0058DDBC
    static void SUpdateRiverMgr(); // 0x0058DDD0
    static void SInitializeRoomMusic(); // 0x0058DD08
    static void SCreateRoomMusic(); // 0x0058DD30
    static void SDestroyRoomMusic(); // 0x0058DD58
    static void SUpdateRoomMusic(); // 0x0058DD6C
    static s16 SGetLiveBeatsPerBar(); // 0x0058DA40
    static s16 SGetLiveVariable2(); // 0x0058DA50
    static bool SIsLivePlaying(); // 0x0058DA68
    static bool SPlayLiveExtra(u32 index, u32 milliSeconds); // 0x0058DA7C
    static void SCreateLive(); // 0x0058DAF8
    static void SDestroyLive(); // 0x0058DB24
    static f32 SGetLiveBeatPhase(); // 0x0058DB78
    static u32 SGetLiveEndMs(); // 0x0058DB90
    static f32 SGetLiveMeasurePhase(); // 0x0058DBA4
    static f32 SGetLiveBarPhase(); // 0x0058DBBC
    static void SUpdateLive(); // 0x0058DBD4
    static s16 SGetLiveNoteKind(); // 0x0058DC2C
    static s16 SGetLiveVariable0(); // 0x0058DC44
    static s16 SGetLiveMark(); // 0x0058DC5C
    static s16 SGetLiveVariable1(); // 0x0058DC74
    static bool SPlayLive(u32 song, u32 milliSeconds); // 0x0058DC8C
    static bool SIsBarcarolleBetween(); // 0x00583A00
    static bool SIsBarcarolleSinging(); // 0x00583A20
    static s16 SGetBarcarolleNoteIndex(); // 0x00583A38
    static s16 SGetBarcarollePhrase(); // 0x00583A4C
    static void SStopBarcarolleAndClear(); // 0x00583A64
    static s32 SGetBarcarolleFrames(); // 0x00583A8C
    // (the first two arguments are not used) the low byte 1: a song of Kapp'n packed as kind << 24 | variation << 16 | song << 8
    DECOMP_NOINLINE static void SPlayBarcarollePacked(u32 param); // 0x00583ABC
    static void SCreateBarcarolle(); // 0x00583AE4
    static void SDestroyBarcarolle(); // 0x00583B0C
    static void SPlayBarcarolleSecond(); // 0x00583B28
    static void SStopBarcarolle(); // 0x00583B38
    static void SPlayBarcarolle(u32 kind, u32 song, u32 variation); // 0x00583B48
    static u8 SGetBarcarollePart(); // 0x00583B64
    static void SStopBarcarolleNow(s32 arg); // 0x00583B74
    static bool SIsBarcarolleActive(); // 0x00583B88
    static void SPlayBarcarolleRandomMatching(u32 kind); // 0x00583BA4
    static void SUpdateBarcarolle(); // 0x00583BB8
    static s16 SGetBarcarolleNoteKind(); // 0x00583BC8
    static void SCalc(); // 0x0058D9EC
    static void SSetHeldSoundPan(u32 soundId, f32 pan); // 0x0058DDF8
    static void SStopSound(u32 soundId, s32 fadeFrames); // 0x0058DE0C
    static void SPause2(); // 0x0058DE24
    static void SUnpause2(); // 0x0058DE34
    static void SPause1(); // 0x0058DE44
    static void SUnpause1(); // 0x0058DE54
    static void SPauseHard(bool isStop); // 0x0058DE64
    static void SUnpauseHard(); // 0x0058DE78
    static void SStopAll(); // 0x0058DE8C
    static void SPlaySystemSoundChecked(u32 soundId, s16 value, f32 pan); // 0x0058DECC
    static void SStopBgmFade(s32 fadeFrames); // 0x0058DEE4
    static void SPauseBgm(bool isPause, s32 frames); // 0x0058DF00
    static void SPlayBgmNow(u32 soundId); // 0x0058DF28
    static void SInitialize(); // 0x0058DF60
    static void SReset(); // 0x0058DF70
    static void SPrepareSoundInFreeHandle(u32 soundId); // 0x0058DFCC
    // the melody of the town from the save data
    static void SPlayTownMelody(u32 soundId); // 0x00583BE0
    // the melody of a packed tune (8 bytes)
    static void SPlayPackedMelody(u32 soundId, const u8* packed); // 0x00583C88
    // the town tune of the save data becomes the melody
    static void SUpdateTownMelody(); // 0x0058E014

    static void SSetSpeechMood(u32 mood); // 0x005838E8
    static void SSetSpeechVoices(const sead::CheckedArray<u8, 4>& voices); // 0x00583904
    static void SPlaySharedSound(u32 soundId, const nn::math::VEC3* world, const void* owner, bool isIgnoreMaster); // 0x00583934
    static void SPlaySharedSoundWithValue(u32 soundId, const nn::math::VEC3* world, const void* owner, s16 value, bool isIgnoreMaster); // 0x00583984
    static bool SIsSpeechSyllableEntry(); // 0x00583E30
    static bool SIsSpeechSyllable(); // 0x00583E34
    static void SStartJinglePreparedEntry(); // 0x00583EC8
    static void SStopSharedSounds(s32 fadeFrames); // 0x00583EE4
    static void SSetSharedSoundPosition(const nn::math::VEC3* world, const void* owner); // 0x00583F5C
    static void SSetSpeechCharFrames(u32 frames); // 0x00583FC8
    static void SSetBoostOn(); // 0x005840A0
    static bool SIsSharedSoundUsedBy(const void* owner); // 0x0058410C
    static void SSetSharedVolumeScale(const void* owner, f32 scale); // 0x0058418C
    static void SWriteSharedVariable(u32 soundId, const void* owner, s16 value, s32 variable); // 0x005843E8
    static bool SIsChimeStage(stage::Name stage); // 0x00584434
    static void SPlaySpeechChar(u32 c, f32 pan); // 0x005845B4
    static void SSetFx6On(); // 0x005845E8
    static void SSetSpeechFilterOn(); // 0x0058468C
    static void SSetSpeechFilter(bool isOn); // 0x00584694
    static void SPlaySpeechLongChar(u32 c, f32 pan); // 0x00584754
    static u8 SGetDJPart(); // 0x00584790
    static s32 SGetDJSong(); // 0x005847A4
    static void SSetDJForceFirstSong(bool isForced); // 0x005847B4
    static s32 SGetDJNextSong(); // 0x005847C8
    static void SSetDJForceSecondSong(bool isForced); // 0x005847D8
    static void SCreateDJPlayer(); // 0x005847EC
    static void SDestroyDJPlayer(); // 0x00584824
    static u8 SGetDJPlayType(); // 0x00584850
    static u8 SGetDJBeatsPerBar(); // 0x00584864
    static f32 SGetDJBarPhase(); // 0x00584874
    static const SoundSongQueueData* SGetDJQueueData(); // 0x00584888
    static u8 SGetDJBeatsPerBar(u32 song); // 0x00584898
    static f32 SGetDJBeatPhase(); // 0x005848AC
    static s32 SGetDJPartRemainingFrames(); // 0x005848C0
    static f32 SGetDJPartBeats(); // 0x005848D4
    static f32 SGetDJPartRemaining(); // 0x005848E4
    static s32 SGetDJPartElapsedFrames(); // 0x005848F8
    static f32 SGetDJPartElapsed(); // 0x0058490C
    static s16 SUpdateDJ(); // 0x00584920
    static void SStopDJ(); // 0x00584930
    static void SPlayDJ(const SoundSongQueueData* data, u32 milliSeconds); // 0x00584940
    static void SStartCarnival(); // 0x00584D10
    static void SEndCarnivalLong(); // 0x0058503C
    static void SNothing(); // 0x0058584C
    static void SStartCarnival2(); // 0x00585A84
    static void SStartCarnival4(); // 0x00585B20
    static void SStartClub444Live(); // 0x00585D48
    static void SStopCarnival(); // 0x00585D8C
    static void SStopClub444Live(); // 0x00585E58
    static void SEndCarnivalShort(); // 0x00585E98
    static void SSetFanfareGetWait(s32 wait); // 0x0058AF64
    static void SStopMicAsr(); // 0x0058D5F8
    static s8 SIsMicUser1Detected(); // 0x0058D608
    static bool SStartMicSampling(u32 client); // 0x0058D620
    static s32 SGetMicAsrResult(); // 0x0058D634
    static void SStopMicSampling(u32 client); // 0x0058D648
    static void SLoadMicAsr(); // 0x0058D65C
    static void SSetMicAsrParam(const void* data, s32 size); // 0x0058D66C
    static bool SIsMicAsrLoaded(); // 0x0058D684
    static u8 SGetMicAsrProgress(); // 0x0058D6A4
    static void SStopMicUser1(); // 0x0058D6BC
    static bool SStartMicUser1(); // 0x0058D6CC
    static bool SStartMicAsr(); // 0x0058D6DC
    static void SResetMicAsr(); // 0x0058D6EC
    static void SStopAllEntry(); // 0x0058DE88
    static void SHoldSharedSound(u32 soundId, const nn::math::VEC3* world, const void* owner); // 0x0058DF80
    static void SQueueSpeechChar(u16 c, s32 level); // 0x0058DFE0
    static void SOnGameLoaded(); // 0x0058E000
    static void SStopSharedSoundsOf(const void* owner, s32 fadeFrames); // 0x0058E058
    static s8 SIsReset(); // 0x0058E078
    static void SBeginStageEnter(); // 0x0058E088
    static void SEndStageEnterEntry(); // 0x0058E13C
    static void SEndStageEnter(); // 0x0058E140
    static bool SIsStageGroupLoaded(); // 0x0058E194
    static void SBeginStageExit(); // 0x0058E1A4
    static void SEndStageExitEntry(); // 0x0058E238
    static void SEndStageExit(); // 0x0058E23C
    static void SInitializeInterface(); // 0x0058E2A4
    static void SSetInterfaceFlag5(); // 0x0058E2D0
    static void SSetFullStageChange(); // 0x0058E2E4
    static void SResetMusic(); // 0x0058E2F8
    static void SLoadCurrentStageGroups(); // 0x0058E30C
    static void SUnloadCurrentStageGroups(); // 0x0058E33C
    static void SCalcInterface(); // 0x0058E36C

    // the state of the interface during the change of a stage (0x0094F44B)
    struct InterfaceState
    {
        u8 isInitialized; // 0x0
        u8 isEntering;    // 0x1
        u8 isExiting;     // 0x2
        s8 isReset;       // 0x3 the requests are not kept (SResetMusic)
        u8 isDisabled;    // 0x4 (nothing sets it in the interface)
        u8 flag5;         // 0x5
        s8 isFull;        // 0x6 the stage groups change too
    };
    static InterfaceState s_Interface;
    static bool s_IsDJCreated;

    // the state of the interface
    static bool s_IsJinglePending;
    static s16 s_NextBgmParam;
    static u32 s_PendingJingle;
    static bool s_IsRiverCreated;
    static bool s_IsSeaCreated;
    static bool s_IsRoomMusicCreated;
    static bool s_IsLiveCreated;
    static bool s_IsBarcarolleCreated;
    static bool s_IsBarcarolleFlag;
    static bool s_IsLiveRequested;

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
    // after the sound system: the players of the archive, the volume manager and the biquad filters
    void Initialize(); // 0x006B26E8
    void SetRainState(u32 state); // 0x006B2870
    // the sea sound starts and the volume manager is finalized (m_State 2)
    void BeginPlay(); // 0x006B28B8
    // a handle that holds the sound (the one that has it already, else a free one; NULL: none)
    sead::SoundHandle* HoldSound(u32 soundId); // 0x006B28EC
    // stops the sound in all handles
    DECOMP_NOINLINE void StopHeldSound(u32 soundId); // 0x006B29BC
    // the system sounds and the animal speech silent (or back)
    void MuteSystemAndSpeech(bool isMute); // 0x006B2A78
    // pauses the players of the reason (s_PlayerPauseMasks) and the sounds that pause with it
    DECOMP_NOINLINE void Pause(u32 flag); // 0x006B2B0C
    DECOMP_NOINLINE void Unpause(u32 flag); // 0x006B3024
    void Pause2(); // 0x006B2BB4
    void Unpause2(); // 0x006B30CC
    void Pause10(); // 0x006B29A4
    void Unpause10(u32 arg); // 0x006B26D0
    void SetFlagCC5(bool flag); // 0x006B30E4
    // isStop: the melodies stop, the master volume goes to 0 and the pause has reason 8, else reason 4
    void PauseHard(bool isStop); // 0x006B3F74
    // undoes PauseHard
    void UnpauseHard(); // 0x006B3FEC
    DECOMP_NOINLINE void PauseSounds(u32 flag); // 0x006B37B8
    DECOMP_NOINLINE void UnpauseSounds(u32 flag); // 0x006B3ED4
    // the sound group of the stage into data heap 5 and its extra group into heap 4
    void LoadStageGroups(stage::Name stage); // 0x006B2D94
    // a system sound (SE_SYS_*) in a free handle; quick repeats get lower volumes and pitches
    void PlaySystemSound(u32 soundId, f32 pan); // 0x006B2C00
    // the pan of a system sound (0 for the centered ones, else 0.8 of the clamped pan)
    DECOMP_NOINLINE f32 GetSystemSoundPan(u32 soundId, f32 pan); // 0x006B3C54
    // what a system sound does to other sounds
    DECOMP_NOINLINE void OnSystemSoundStarted(u32 soundId); // 0x006B4014
    // SE_SYS_EDIT_PREVIEW_ACTIVE when the position changed
    void PlayEditPreview(const f32* pos, f32 pan); // 0x006B30EC
    // the melody of a character while nothing pauses the sounds
    void PlayCharacterMelody(const sead::SafeStringBase<char>& name, const u8* notes); // 0x006B31D4
    // the volume of a sound object by the height difference to the listener (in the village)
    f32 GetHeightVolume(f32 height); // 0x006B3218
    // unloads the stage group if the next stage has another one, and the extra group
    void UnloadStageGroups(u32 arg); // 0x006B32F0
    void Pause1(); // 0x006B3418
    void Pause20(); // 0x006B3430
    void Unpause1(); // 0x006B366C
    void Unpause20(); // 0x006B3684
    void StartHeldSound(u32 soundId, f32 pan); // 0x006B3448
    // the sound of an insect of the encyclopedia by its index
    void PlayInsectSound(u32 insect); // 0x006B34D8
    // before a stage change: fades, effects and the environment follow the next stage
    void PrepareStageChange(stage::Name next); // 0x006B369C
    // a sound in a free handle, muffled outside of indoor stages
    void PlayMuffledSound(u32 soundId); // 0x006B3700
    void EnterStage(stage::Name stage); // 0x006B3858
    // the fader of m_Fader down to 0.35 (isDown) or back
    void SetFade(bool isDown, s32 frames); // 0x006B3B14
    // the train in the distance, by the position of the player (only at night)
    void UpdateTrainFaraway(const nn::math::VEC3* pos); // 0x006B3D44
    // 0 no outside sounds, 1 outdoors, 2 indoors
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetStageSoundKind(stage::Name stage); // 0x00767F38
    // data is being loaded into a stage heap
    DECOMP_EMIT_COPY bool IsLoadingStageGroup(); // 0x00768060
    // the system sounds of the Puzzle League game: the value goes into sequence variable 15 (and 14)
    void PlayPanelBreak(u32 size, s16 chain, s16 value, f32 pan); // 0x006B4320
    void PlayCandyBreak(s16 chain, s16 value, f32 pan); // 0x006B41D8
    void PlayPanelGarbage(s16 chain, f32 pan); // 0x006B4718
    // the camera of amiibo Camera: false if the speed is too low
    bool PlayAmiiboCameraTrans(f32 speed); // 0x006B4D40
    void PlayAmiiboCameraSlider(f32 position, f32 filter); // 0x006B4E6C
    // the sound of a furniture model that is moved, by the speed
    void PlayFurnitureMove(f32 speed); // 0x006B4C08
    // false: the system sound must not start (the event mute, another sound in front of it, played a moment ago)
    DECOMP_NOINLINE bool CanPlaySystemSound(u32 soundId); // 0x006B48A0
    // PlaySystemSound with the test of CanPlaySystemSound and the history
    DECOMP_NOINLINE void PlaySystemSoundChecked(u32 soundId, f32 pan); // 0x006B566C
    void PlaySystemSoundChecked(u32 soundId, s16 value, f32 pan); // 0x006B5788
    // the time chime and the shared objects off (players 4 and 17 at 0)
    void MuteForEvent(bool isMute, s32 frames); // 0x006B4514
    void SetFx6(bool isOn); // 0x006B45B4
    void SetFx7(bool isOn); // 0x001B0D94
    void SetFx8(bool isOn); // 0x006B4C9C
    // false: the player has no room and the new sound does not replace one (the objects of amiibo Camera share SE_MR_*)
    bool CheckPlayerRoom(u32 soundId, const SoundParam* param, s32 priorityOffset, bool isStopEqual,
                         const SoundObjBase* obj); // 0x006B4F54
    bool StartSoundChecked(nw::snd::SoundHandle* handle, u32 soundId,
                           const nw::snd::SoundStartable::StartInfo* info); // 0x006B4CF4
    void SetListenerPos(const nn::math::VEC3* pos); // 0x006B4D24
    // the biquad filter of the speech (in the file of SoundDoubutsugoMgr)
    void SetSpeechFilter(bool isOn); // 0x002E7A30
    void Calc(); // 0x006B5160
    // the BGM, all players and the data heaps stop, the master volume is back
    void StopAll(); // 0x006B5450
    // a held sound with a pan
    void SetHeldSoundPan(u32 soundId, f32 pan); // 0x006B54E8
    void StopSound(u32 soundId, s32 fadeFrames); // 0x006B5574
    // y0..y1 for x in x0..x1 as an integer (y0 if x0 == x1)
    static s32 LerpInt(s32 y0, s32 y1, f32 x, f32 x0, f32 x1); // 0x006B562C
    // back in the title: all stops
    void Reset(); // 0x006B58D4
    void HoldSound(nw::snd::SoundHandle* handle, u32 soundId); // 0x006B5968
private:
    sead::SoundHandle* StartSystemSound(u32 soundId, f32 pan);

public:
    // the sound prepared in a free handle
    void PrepareSoundInFreeHandle(u32 soundId); // 0x006B5984
    // the playable sounds of PLAYER_SE_HANIWA
    DECOMP_EMIT_COPY s32 GetHaniwaPlayableCount() const // 0x00768054
    {
        return m_pSoundPlayers[13]->m_PlayableCount;
    }
    // the stage heaps 2 and 3 exist (false before the initialization)
    DECOMP_EMIT_COPY bool HasHeaps23(); // 0x00767EA4
    // the parameters of a sound from the archive
    void ReadSoundParam(SoundParam* param, u32 soundId); // 0x006B3A58
    // the time of the game is at night (before 7:00 or from 19:00)
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsNight(); // 0x00767EF0
    // y0..y1 for x in x0..x1 (y0 if x0 == x1)
    DECOMP_NOINLINE static f32 Lerp(s32 x, s32 x0, s32 x1, f32 y0, f32 y1); // 0x006B55F8
    // a flag of the BGM manager (until SoundBgmMgr is written)
    bool IsBgmFlag14() const { return m_BgmMgr.m_State != 0; }

    static const f32 s_SystemSoundVolumes[12];
    static const f32 s_SystemSoundPitches[12];
    // the sounds Reset stops
    static const u32 s_ResetStopSounds[2];
    // the pause reasons of each player
    static const u32 s_PlayerPauseMasks[PLAYER_NUM];
    static s16 s_NextSoundValue;
    static SoundMgr* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;

    u32 m_SingletonDisposerBuffer[4];        // 0x000
    sead::Heap* m_pHeap;                     // 0x010
    sead::ExpHeap* m_pMicHeap;               // 0x014
    s32 m_GroupBaseLevel;                    // 0x018 the level of data heap 5 without a stage group
    SoundHandleSet<8> m_HandleSet;           // 0x01C
    sead::AudioSettingParameter m_Setting;   // 0x0DC
    sead::AudioResourceLoaderCtr m_Loader;   // 0x100
    void* m_pWorkBuffer;                     // 0x130
    SoundVolumeMgr m_VolumeMgr;              // 0x134
    SoundDoubutsugoMgr m_DoubutsugoMgr;      // 0x594
    SoundMelodyMgr m_MelodyMgr;              // 0x68C
    SoundBgmMgr m_BgmMgr;                    // 0x754
    SoundFxMgr m_FxMgr;                      // 0x860
    SoundEnvMgr m_EnvMgr;                    // 0xB00
    SoundSharedObjMgr m_SharedObjMgr;        // 0xBC8
    sead::AudioPlayer* m_pAudioPlayer;       // 0xC08
    sead::AudioPlayerCtr* m_pPlayer;         // 0xC0C
    nw::snd::SoundArchive* m_pArchive;       // 0xC10
    sead::CheckedArray<nw::snd::SoundPlayer*, PLAYER_NUM> m_pSoundPlayers; // 0xC14
    bool m_IsInitialized;                    // 0xC5C
    bool m_IsStopped;                        // 0xC5D set by PauseHard(true)
    u32 m_PauseFlags;                        // 0xC60
    sead::CheckedArray<u32, PLAYER_NUM> m_PlayerPauseFlags; // 0xC64
    s32 m_SkipIntervalTick;                  // 0xCAC of the sequences (36 in the player houses, else 72)
    f32 m_TickScale;                         // 0xCB0 4 / (m_SkipIntervalTick * 1000)
    u8 m_CurrentStage;                       // 0xCB4 stage::Name
    u8 m_PrevStage;                          // 0xCB5 stage::Name
    u8 _CB6[0xCB8 - 0xCB6];                  // 0xCB6
    s32 m_State;                             // 0xCB8
    u32 _CBC;                                // 0xCBC
    u32 m_StageGroup;                        // 0xCC0 the loaded stage group (GROUP_*, -1: none)
    bool m_IsPlantTree;                      // 0xCC4 the next village group is GROUP_PLANT_TREE
    bool _CC5;                               // 0xCC5
    u8 _CC6[0xCC8 - 0xCC6];                  // 0xCC6
    u32 m_SystemSoundRepeat;                 // 0xCC8 a system sound repeated quickly gets volume/pitch by it (mod 12)
    s32 m_SystemSoundFrames;                 // 0xCCC the frames since the last system sound (0: one this frame)
    nw::snd::SoundArchive::SoundInfo m_SoundInfo;     // 0xCD0 (ReadSoundParam)
    nw::snd::SoundArchive::Sound3DInfo m_Sound3DInfo; // 0xCE8 (ReadSoundParam)
    u8 m_LastStartResult;                    // 0xCF4 StartResult of StartSoundChecked
    u8 _CF5[0xCF8 - 0xCF5];                  // 0xCF5
    const nn::math::VEC3* m_pListenerPos;    // 0xCF8
    bool m_IsBgmDucked;                      // 0xCFC by SE_SYS_PUBLIC_SET_PLACE
    bool m_IsSpeechMuted;                    // 0xCFD by SE_ACT_BEV_BELCH
    bool m_IsSystemMuted;                    // 0xCFE MuteSystemAndSpeech
    bool m_IsEventMuted;                     // 0xCFF the time chime and the shared objects are off
    bool m_IsPreviewActive;                  // 0xD00
    s32 _D04;                                // 0xD04
    f32 m_PreviewPos[2];                     // 0xD08 the position of the last SE_SYS_EDIT_PREVIEW_ACTIVE
    u8 m_GroundCover;                    // 0xD10 of grass and soil for the footsteps: 0 none, 1 leaves, 2 snow (SoundFootstep)
    u8 m_FadeState;                          // 0xD11 0 none, 1 fading down, 2 down, 3 fading up
    s32 m_FadeTimer;                         // 0xD14
    s32 m_FadeFrames;                        // 0xD18
    SoundFader m_Fader;                      // 0xD1C
    struct Reaction
    {
        u32 soundId;     // 0x00
        u8 count;        // 0x04 the objects that wanted the sound this frame
        s32 frames;      // 0x08 the frames in a row with a request
        f32 maxVolume;   // 0x0C of the objects
        f32 panSum;      // 0x10 of the objects
    };
    Reaction m_Reactions[9];                 // 0xD30
};
ASSERT_OFFSET(SoundMgr, m_Setting, 0xDC);
ASSERT_OFFSET(SoundMgr, m_Loader, 0x100);
ASSERT_OFFSET(SoundMgr, m_pPlayer, 0xC0C);
ASSERT_OFFSET(SoundMgr, m_Fader, 0xD1C);
ASSERT_SIZE(SoundMgr, 0xDE4);
