#pragma once

#include "decomp.h"
#include "Sound/dSoundFader.h"
#include "nn/snd/CTR/snd_FxDelay.h"
#include "nn/snd/CTR/snd_FxReverb.h"
#include "sead/seadAudioFxCtr.h"
#include "sead/seadAudioFxSet.h"
#include "sead/seadAudioSubsetBase.h"
#include "stage/stage_Api.h"

namespace sead { class AudioSoundHeapCtr; }

// RTTI 10SoundFxMgr @ 0x008CB1F0
// vtable 0x008EC4B8 (vptr 0x008EC4C0), offset_to_top 0, 20 entries
// The room effect of the stage on aux bus A: one of ten effects (three reverbs, seven delays) with the send level of
// the players that use it and a fade of the return (the class name is from the RTTI, everything else is ours).
class SoundFxMgr : public ::sead::AudioSubsetBase
{
public:
    // the effects (for the kind of stage, see GetFxType; names are ours)
    enum Fx
    {
        FX_NONE = -1,
        FX_CLUB_DJKK = 0,   // stage CLUB444_DJKK
        FX_CLUB = 1,        // stages CLUB444, CLUB444_Live
        FX_MUSEUM = 2,      // KIND_MUSEUM
        FX_DREAM = 3,       // a dream (netgame::TYPE_DREAM: visiting a dream town in the Dream Suite)
        FX_4 = 4,
        FX_HOUSE = 5,       // KIND_NPC_HOUSE, KIND_PLAYER_HOUSE, KIND_MODEL_HOUSE
        FX_6 = 6,
        FX_7 = 7,
        FX_8 = 8,
        FX_TRAIN = 9,       // stage Train
        FX_NUM = 10,
        FX_REVERB_NUM = 3,  // effects 0..2 are reverbs, the others delays
    };
    enum State
    {
        STATE_NONE = 0,
        STATE_ON = 1,
        STATE_FADE_OUT = 2,  // the effect goes when the counter runs out
    };

    SoundFxMgr(); // 0x001284CC
    virtual ~SoundFxMgr();
    // the parameters of the effects, the effects (once) and the sound heap of the effects
    virtual void initialize(sead::AudioMgr* mgr, sead::Heap* heap) override; // 0x001B0B4C slot 0x14
    virtual void finalize() override; // 0x001B13E4 slot 0x18
    virtual void calc() override; // 0x001B1308 slot 0x1C

    // the effect on (effects 6..9 not over another one except FX_HOUSE)
    void Start(s32 fx); // 0x001B0E6C
    // the effect off, faded over frames (only if it is the current one)
    void FadeOut(s32 fx, s32 frames); // 0x001B0DB8
    // the stage changes: the effect fades out if the next stage has another one
    void PrepareStageChange(stage::Name current, stage::Name next); // 0x001B1090
    // the stage changed: the effect of the new stage
    void ChangeStage(stage::Name prev, stage::Name current); // 0x001B11DC

    // the effect of the kind of stage
    static DECOMP_ALWAYS_INLINE s32 GetFxType(stage::Name stage);

    // the send level of the players for each effect (name is ours)
    static const f32 s_SendLevels[FX_NUM];
    // whether the front channels bypass the effect (name is ours)
    static const s8 s_IsFrontBypass[FX_NUM];

    sead::AudioSoundHeapCtr* m_pHeap;                  // 0x00C the memory of the effects
    s32 m_HeapLevel;                                   // 0x010 the level of m_pHeap before them
    sead::AudioFxSet m_FxSet;                          // 0x014
    sead::AudioFxCtr* m_pCurrent;                      // 0x040
    sead::AudioFxCtr m_Fx[FX_NUM];                     // 0x044
    bool m_IsFxCreated;                                // 0x184
    const void* m_pParams[FX_NUM];                     // 0x188
    nn::snd::CTR::FxReverb::Param m_ReverbParams[FX_REVERB_NUM]; // 0x1B0
    nn::snd::CTR::FxDelay::Param m_DelayParams[5];     // 0x21C
    s32 m_FxType;                                      // 0x26C Fx
    s32 m_State;                                       // 0x270 State
    s32 m_Counter;                                     // 0x274 frames until the effect goes (STATE_FADE_OUT)
    SoundFader m_SendFader;                            // 0x278
    SoundFader m_ReturnFader;                          // 0x28C

private:
    // the effect off at once (the send level to 0)
    void Clear();
    // the effect into the set of bus A (FX_NONE: none)
    void SetFx(s32 fx); // 0x001B1460
    // the send level of the players of the effect
    void ApplySend(s32 fx, f32 send); // 0x001B0F94
};
ASSERT_OFFSET(SoundFxMgr, m_FxSet, 0x14);
ASSERT_OFFSET(SoundFxMgr, m_Fx, 0x44);
ASSERT_OFFSET(SoundFxMgr, m_ReverbParams, 0x1B0);
ASSERT_OFFSET(SoundFxMgr, m_FxType, 0x26C);
ASSERT_SIZE(SoundFxMgr, 0x2A0);
