#pragma once

#include "decomp.h"
#include "Sound/dSoundFader.h"
#include "nw/snd/snd_SoundHandle.h"
#include "sead/seadAudioSubsetBase.h"
#include "sead/seadSoundHandle.h"
#include "stage/stage_Api.h"

class SoundObjBase;

// RTTI 11SoundBgmMgr @ 0x008CB330
// vtable 0x008ECCD4 (vptr 0x008ECCDC), offset_to_top 0, 20 entries
// The background music: the BGM of the stage (prepared or started), its kind with special handling (track mixes,
// weather variants, fades), a second sound for jingles and the sounds of events (the carnival end, the prologue)
// (the class name is from the RTTI, everything else is ours).
class SoundBgmMgr : public ::sead::AudioSubsetBase
{
public:
    // m_State
    enum State
    {
        STATE_NONE = 0,
        STATE_PREPARING = 1,        // prepared, starts when it is ready
        STATE_PREPARING_START = 2,  // started while it is prepared
        STATE_PREPARED = 3,
        STATE_PLAYING = 4,
        STATE_STOPPING = 5,
    };

    SoundBgmMgr(); // 0x001286DC
    virtual ~SoundBgmMgr();
    virtual void calc() override; // 0x001D7DE4 slot 0x1C

    // the BGM (mode 1: prepare only; param 1: keep the variant/volume)
    void Play(u32 soundId); // 0x001D67CC
    void Play(u32 soundId, u8 mode, s8 param); // 0x001D67D8
    // the prepared BGM starts
    void StartPrepared(); // 0x001D6E24
    void Update(); // 0x001D643C
    void SetVolume(f32 volume, s32 frames); // 0x001D5B6C
    // the kind of the BGM (m_Kind) for its sound id
    void SetKind(u32 soundId); // 0x001D6204
    // the tracks for the kind after the start
    void ApplyTracks(); // 0x001D5D80
    // the track mix of the museum from one room to another
    void CrossfadeTracks(s32 from, s32 to, s32 frames); // 0x001D7150
    // the BGM of the museum follows the room
    void ChangeMuseumRoom(stage::Name stage); // 0x001D6D70
    void EnterStage(stage::Name stage); // 0x001D6E84
    // the two tracks of the Able Sisters' shops
    void SetShopTracks(s32 frames); // 0x001D6CA0
    // the two tracks of T.I.Y. and the gardening store
    void SetHomeCenterTracks(s32 frames); // 0x001D7CB0
    void SetBoost(bool isBoost); // 0x001D58B0
    void SetDowntownLevel(u8 level); // 0x001D6430
    void UpdateDowntown(); // 0x001D72D4

    void StopJingle(s32 fadeFrames); // 0x001D5904
    void StopSub(s32 fadeFrames); // 0x001D597C
    void StopSubNow(s32 fadeFrames); // 0x001D5BB0
    void ResetSub(); // 0x001D5BC4
    void PlayCarnival(u8 mode); // 0x001D59E0
    void UpdateCarnivalEnd(); // 0x001D6F94
    void PlayBingo(bool isOffset); // 0x001D5C5C
    void PlayPrologue(); // 0x001D6120
    void SetPrologueStep(u8 step); // 0x001D6168
    void PlaySymbolTree(); // 0x001D63C8

    void Stop(); // 0x001D7FC4
    void Stop(s32 fadeFrames); // 0x001D7FCC
    void Pause(bool isPause, s32 frames); // 0x001D80FC
    void PlayNow(u32 soundId, s8 param); // 0x001D8140
    void StopEvent(); // 0x001D8160
    // the volume down at once (back after 30 frames when !isDown)
    void Duck(bool isDown); // 0x001D762C
    void PlayJingle(u32 soundId); // 0x001D7660
    void PlayJingle(u32 soundId, u8 mode); // 0x001D7668
    // PlayJingle with mode 2
    void PlayJingleStartPrepared(u32 soundId); // 0x001D63C0
    void StartJinglePrepared(); // 0x001D7BCC
    void StartSubPrepared(); // 0x001D7C0C
    void StartCafe(); // 0x001D7B68
    // the puzzle music by the height (tracks and pitch)
    void SetPuzzleLevel(f32 level); // 0x001D7A44
    void SetTrackVolume(u32 trackBitFlag, f32 volume, s32 frames); // 0x001D7D80
    // the K.K. song of an object
    void PlayObjSong(u32 song); // 0x001D7C38
    void StopObjSong(s32 fadeFrames); // 0x001D7784
    void UpdateObjSong(); // 0x001D8164
    void ClearObj(SoundObjBase* obj); // 0x001D814C
    // the two tracks by the distance of the object
    void UpdateDistanceTracks(); // 0x001D77BC
    void UpdateSubNone(); // 0x001D74C8
    void UpdateSymbolTree(); // 0x001D74CC
    void UpdatePrologue(); // 0x001D79A0

    // the museum room of a stage: 2 main hall, 3 fish, 4 insects, 5 fossils, 6 art
    static s32 GetMuseumRoom(stage::Name stage);

    // the updates of the sub sounds by m_SubKind
    typedef void (SoundBgmMgr::*SubUpdate)();
    static SubUpdate s_SubUpdates[4];
    static const s8 s_IsTrackSurround[2];
    static const f32 s_MinVolumes[2];
    static const f32 s_NearDistances[2];
    static const f32 s_FarDistances[2];

    // the tracks of each museum room in the stream and in the sequence
    static const u32 s_StreamTracks[7];
    static const u32 s_SeqTracks[7];
    static const s32 s_FadeInFrames[2];
    static const s32 s_FadeOutFrames[2];

    u8 m_Kind;                           // 0x00C
    sead::SoundHandle m_Handle;          // 0x010
    u8 m_State;                          // 0x014 State
    u32 m_SoundId;                       // 0x018
    SoundFader m_TrackFader;             // 0x01C
    bool m_IsBiquad;                     // 0x030
    sead::SoundHandle m_JingleHandle;    // 0x034
    u8 m_JingleState;                    // 0x038
    u32 m_JingleId;                      // 0x03C
    nw::snd::SoundHandle m_ObjHandle;    // 0x040 a K.K. song from an object
    s32 m_ObjSong;                       // 0x044 SoundMuInfo song (-1: none)
    u8 m_ObjState;                       // 0x048
    s32 m_ObjLoadLevel;                  // 0x04C the level of the sound heap before the song (-1: none)
    nw::snd::SoundHandle m_MixHandle;    // 0x050
    u32 m_MixId;                         // 0x054
    u8 m_DowntownLevel;                  // 0x058 the variant of the music of Main Street (STRM_BGM_DWNTWN_*_1/2/3)
    s8 m_DowntownVariant;                 // 0x059 (-1: none)
    u32 m_DowntownIds[3];                 // 0x05C
    SoundFader m_DowntownFader;           // 0x068
    bool m_IsKeepVariant;                // 0x07C
    s32 m_MuseumRoom;                    // 0x080
    u8 m_MixCounter;                     // 0x084
    u32 m_StreamTracksOn;                // 0x088
    u32 m_SeqTracksOn;                   // 0x08C
    u32 m_StreamTracksOff;               // 0x090
    u32 m_SeqTracksOff;                  // 0x094
    SoundObjBase* m_pObj;                // 0x098 the object of m_ObjHandle and of the distance fade
    bool m_IsFadeInRequested;            // 0x09C for this frame (SoundObjNpc::RequestBgmFadeIn): states 8/9 fade m_FaderA0 in
    SoundFader m_FaderA0;                // 0x0A0
    u8 _B4;                              // 0x0B4
    SoundFader m_BoostFader;             // 0x0B8
    s32 m_BoostTimer;                    // 0x0CC
    nw::snd::SoundHandle m_SubHandles[2]; // 0x0D0
    u8 m_SubState;                       // 0x0D8
    s32 m_SubKind;                       // 0x0DC
    u8 m_SubMode;                        // 0x0E0
    u8 _E1;                              // 0x0E1
    u8 _E2;                              // 0x0E2
    s32 m_LoopStart;                     // 0x0E4
    s32 m_LoopLength;                    // 0x0E8
    u8 m_PrologueStep;                   // 0x0EC
    s32 _F0;                             // 0x0F0
    SoundFader m_VolumeFader;            // 0x0F4
    s32 m_VolumeDelay;                   // 0x108

private:
    void Reset();
    void ApplyBiquad();
};
ASSERT_OFFSET(SoundBgmMgr, m_TrackFader, 0x1C);
ASSERT_OFFSET(SoundBgmMgr, m_DowntownFader, 0x68);
ASSERT_OFFSET(SoundBgmMgr, m_BoostFader, 0xB8);
ASSERT_OFFSET(SoundBgmMgr, m_SubHandles, 0xD0);
ASSERT_OFFSET(SoundBgmMgr, m_VolumeFader, 0xF4);
ASSERT_SIZE(SoundBgmMgr, 0x10C);
