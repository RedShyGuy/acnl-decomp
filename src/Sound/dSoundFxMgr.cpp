#include "Sound/dSoundFxMgr.h"
#include "Sound/dSoundMgr.h"
#include "netgame/netgame_Api.h"
#include "nn/snd/CTR/CTR_Api.h"
#include "nw/snd/snd_SoundHeap.h"
#include "nw/snd/snd_SoundPlayer.h"
#include "sead/seadAudioSoundHeapCtr.h"
#include "sead/seadNew.h"

// 0x0083B8EC
const f32 SoundFxMgr::s_SendLevels[FX_NUM] = {0.4f, 0.4f, 0.5f, 0.55f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
// 0x0083B914
const s8 SoundFxMgr::s_IsFrontBypass[FX_NUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// 0x001284CC
SoundFxMgr::SoundFxMgr()
    : m_pHeap(NULL), m_HeapLevel(-1), m_pCurrent(NULL), m_IsFxCreated(false), m_FxType(FX_NONE), m_State(STATE_NONE),
      m_Counter(0)
{
    for (s32 i = 0; i < FX_REVERB_NUM; i++) {
        nn::snd::CTR::FxReverb::Param& param = m_ReverbParams[i];
        param.m_EarlyReflectionTime = 60;
        param.m_FusedTime = 4000;
        param.m_PreDelayTime = 100;
        param.m_Coloration = 0.5f;
        param.m_Damping = 0.4f;
        param.m_FilterSize = &nn::snd::CTR::s_DefaultReverbFilterSize;
        param.m_EarlyGain = 0.6f;
        param.m_FusedGain = 0.4f;
        param.m_IsEnableSurround = false;
    }
    for (s32 i = 0; i < 5; i++) {
        nn::snd::CTR::FxDelay::Param& param = m_DelayParams[i];
        param.m_DelayTime = 250;
        param.m_FeedbackGain = 0.4f;
        param.m_Damping = 0.5f;
        param.m_IsEnableSurround = false;
    }
    m_pParams[0] = &m_ReverbParams[0];
    m_pParams[1] = &m_ReverbParams[1];
    m_pParams[2] = &m_ReverbParams[2];
    m_pParams[3] = &m_DelayParams[0];
    m_pParams[4] = &m_DelayParams[0];
    m_pParams[5] = &m_DelayParams[1];
    m_pParams[6] = &m_DelayParams[0];
    m_pParams[7] = &m_DelayParams[2];
    m_pParams[8] = &m_DelayParams[3];
    m_pParams[9] = &m_DelayParams[4];
}

// 0x0013E1AC
// 0x001B1560 (deleting dtor)
SoundFxMgr::~SoundFxMgr()
{
    // (only the members are destroyed)
}

// 0x001B0B4C
void SoundFxMgr::initialize(sead::AudioMgr*, sead::Heap* heap)
{
    nn::snd::CTR::FxReverb::Param& reverb0 = m_ReverbParams[0];
    reverb0.m_EarlyReflectionTime = 10;
    reverb0.m_FusedTime = 1200;
    reverb0.m_PreDelayTime = 40;
    reverb0.m_Coloration = 0.1f;
    reverb0.m_Damping = 0.6f;
    reverb0.m_EarlyGain = 0.5f;
    reverb0.m_FusedGain = 0.4f;
    reverb0.m_IsEnableSurround = false;
    nn::snd::CTR::FxReverb::Param& reverb1 = m_ReverbParams[1];
    reverb1.m_EarlyReflectionTime = 100;
    reverb1.m_FusedTime = 3000;
    reverb1.m_PreDelayTime = 40;
    reverb1.m_Coloration = 1.0f;
    reverb1.m_Damping = 0.97f;
    reverb1.m_EarlyGain = 0.15f;
    reverb1.m_FusedGain = 0.8f;
    reverb1.m_IsEnableSurround = false;
    nn::snd::CTR::FxReverb::Param& reverb2 = m_ReverbParams[2];
    reverb2.m_EarlyReflectionTime = 2;
    reverb2.m_FusedTime = 2000;
    reverb2.m_PreDelayTime = 5;
    reverb2.m_Coloration = 1.0f;
    reverb2.m_Damping = 0.87f;
    reverb2.m_EarlyGain = 0.2f;
    reverb2.m_FusedGain = 0.9f;
    reverb2.m_IsEnableSurround = false;
    m_DelayParams[0].m_DelayTime = 200;
    m_DelayParams[1].m_DelayTime = 250;
    m_DelayParams[1].m_FeedbackGain = 0.75f;
    m_DelayParams[1].m_Damping = 0.1f;
    m_DelayParams[1].m_IsEnableSurround = false;
    m_DelayParams[4].m_DelayTime = 250;
    m_DelayParams[4].m_FeedbackGain = 0.8f;
    m_DelayParams[4].m_Damping = 0.8f;

    m_SendFader.Reset(0.0f);
    m_ReturnFader.Reset(1.0f);
    if (!m_IsFxCreated) {
        m_Fx[0].initialize(sead::AudioFxCtr::TYPE_FX_REVERB, m_pParams[0], heap);
        m_Fx[1].initialize(sead::AudioFxCtr::TYPE_FX_REVERB, m_pParams[1], heap);
        m_Fx[2].initialize(sead::AudioFxCtr::TYPE_FX_REVERB, m_pParams[2], heap);
        m_Fx[3].initialize(sead::AudioFxCtr::TYPE_FX_DELAY, m_pParams[3], heap);
        // (the original passes the parameters of effect 3 again)
        m_Fx[4].initialize(sead::AudioFxCtr::TYPE_FX_DELAY, m_pParams[3], heap);
        m_Fx[5].initialize(sead::AudioFxCtr::TYPE_FX_DELAY, m_pParams[5], heap);
        m_Fx[6].initialize(sead::AudioFxCtr::TYPE_FX_DELAY, m_pParams[6], heap);
        m_Fx[7].initialize(sead::AudioFxCtr::TYPE_FX_DELAY, m_pParams[7], heap);
        m_Fx[8].initialize(sead::AudioFxCtr::TYPE_FX_DELAY, m_pParams[8], heap);
        m_Fx[9].initialize(sead::AudioFxCtr::TYPE_FX_DELAY, m_pParams[9], heap);
        m_IsFxCreated = true;
    }
    if (m_pHeap == NULL) {
        m_pHeap = new (heap, 32) sead::AudioSoundHeapCtr(0x28000, heap);
    }
}

// 0x001B13E4
void SoundFxMgr::finalize()
{
    Clear();
    delete m_pHeap;
    m_pHeap = NULL;
}

inline void SoundFxMgr::Clear()
{
    if (m_FxSet.getNum(0) > 0) {
        m_FxSet.reset(0);
        m_pCurrent = NULL;
        m_pHeap->LoadState(m_HeapLevel);
        m_SendFader.SetTarget(0.0f, 0);
        ApplySend(m_FxType, m_SendFader.GetValue());
    }
}

// 0x001B1460
void SoundFxMgr::SetFx(s32 fx)
{
    if (static_cast<u32>(fx + 1) >= FX_NUM + 1) {
        return;
    }
    Clear();
    if (fx == FX_NONE) {
        m_pCurrent = NULL;
    } else {
        m_FxSet.append(0, &m_Fx[fx]);
        m_pCurrent = &m_Fx[fx];
    }
    nn::snd::CTR::SetAuxFrontBypass(nn::snd::CTR::AUX_BUS_0, s_IsFrontBypass[fx]);
    if (m_FxSet.getNum(0) > 0) {
        m_HeapLevel = m_pHeap->GetCurrentLevel();
        m_FxSet.setup(m_pHeap);
    }
}

// 0x001B0F94
void SoundFxMgr::ApplySend(s32 fx, f32 send)
{
    SoundMgr* mgr = SoundMgr::s_pInstance;
    switch (fx) {
    case FX_CLUB_DJKK:
        mgr->GetSoundPlayer(PLAYER_BGM & 0xFF)->SetFxSend(nw::snd::AUX_BUS_A, send);
        mgr->GetSoundPlayer(PLAYER_BGM_MU & 0xFF)->SetFxSend(nw::snd::AUX_BUS_A, send);
        mgr->GetSoundPlayer(PLAYER_SE_FOOT & 0xFF)->SetFxSend(nw::snd::AUX_BUS_A, send);
        mgr->GetSoundPlayer(PLAYER_SE_HANIWA & 0xFF)->SetFxSend(nw::snd::AUX_BUS_A, send);
        break;
    case FX_CLUB:
    case FX_MUSEUM:
        mgr->GetSoundPlayer(PLAYER_SE_FOOT & 0xFF)->SetFxSend(nw::snd::AUX_BUS_A, send);
        break;
    case FX_DREAM:
        mgr->GetSoundPlayer(PLAYER_BGM & 0xFF)->SetFxSend(nw::snd::AUX_BUS_A, send);
        mgr->GetSoundPlayer(PLAYER_BGM_HORNET & 0xFF)->SetFxSend(nw::snd::AUX_BUS_A, send);
        break;
    }
}

DECOMP_ALWAYS_INLINE s32 SoundFxMgr::GetFxType(stage::Name stage)
{
    if (stage == stage::NAME_CLUB444 || stage == stage::NAME_CLUB444_LIVE) {
        return FX_CLUB;
    }
    if (stage == stage::NAME_CLUB444_DJKK) {
        return FX_CLUB_DJKK;
    }
    if (stage::IsKind(stage::KIND_MUSEUM, stage)) {
        return FX_MUSEUM;
    }
    if (stage::IsKind(stage::KIND_PLAYER_HOUSE, stage) || stage::IsKind(stage::KIND_NPC_HOUSE, stage) ||
        stage::IsKind(stage::KIND_MODEL_HOUSE, stage)) {
        return FX_HOUSE;
    }
    if (stage == stage::NAME_TRAIN) {
        return FX_TRAIN;
    }
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return FX_DREAM;
    }
    return FX_NONE;
}

// 0x001B0E6C
void SoundFxMgr::Start(s32 fx)
{
    if (m_State == STATE_FADE_OUT && m_FxType != FX_NONE) {
        Clear();
        m_FxType = FX_NONE;
        m_State = STATE_NONE;
    }
    if (fx <= FX_NONE || fx >= FX_NUM) {
        return;
    }
    if (fx < FX_6 || m_FxType == FX_NONE || m_FxType == FX_HOUSE) {
        SetFx(fx);
        m_FxType = fx;
    }
    m_SendFader.SetTarget(s_SendLevels[m_FxType], 0);
    ApplySend(m_FxType, m_SendFader.GetValue());
    m_ReturnFader.SetTarget(1.0f, 0);
    nn::snd::CTR::SetAuxReturnVolume(nn::snd::CTR::AUX_BUS_0, 1.0f);
    m_State = STATE_ON;
}

// 0x001B0DB8
void SoundFxMgr::FadeOut(s32 fx, s32 frames)
{
    if (m_FxType == FX_NONE || m_FxType != fx) {
        return;
    }
    if (frames != 0) {
        m_ReturnFader.SetTarget(0.0f, frames);
        m_State = STATE_FADE_OUT;
        m_Counter = frames;
    } else {
        Clear();
        m_FxType = FX_NONE;
        m_State = STATE_NONE;
    }
}

// 0x001B1090
void SoundFxMgr::PrepareStageChange(stage::Name current, stage::Name next)
{
    if (m_FxType == FX_NONE) {
        return;
    }
    s32 fx = GetFxType(next);
    if (m_FxType != fx && m_FxType != FX_NONE) {
        m_ReturnFader.SetTarget(0.0f, 20);
        m_State = STATE_FADE_OUT;
        m_Counter = 20;
    }
    if ((current == stage::NAME_MUSEUM_INSECT_0 || current == stage::NAME_MUSEUM_INSECT_1 ||
         current == stage::NAME_MUSEUM_INSECT_2) &&
        stage::IsKind(stage::KIND_MUSEUM, next)) {
        m_SendFader.SetTarget(s_SendLevels[m_FxType], 20);
        ApplySend(m_FxType, m_SendFader.GetValue());
    }
}

// 0x001B11DC
void SoundFxMgr::ChangeStage(stage::Name prev, stage::Name current)
{
    s32 fx = GetFxType(current);
    if (m_FxType != fx && fx != FX_NONE) {
        Start(fx);
    }
    if (current == stage::NAME_MUSEUM_INSECT_0 || current == stage::NAME_MUSEUM_INSECT_1 ||
        current == stage::NAME_MUSEUM_INSECT_2) {
        if (stage::IsKind(stage::KIND_MUSEUM, prev)) {
            m_SendFader.SetTarget(0.0f, 20);
        } else {
            m_SendFader.SetTarget(0.0f, 0);
        }
        ApplySend(fx, m_SendFader.GetValue());
    }
}

// 0x001B1308
void SoundFxMgr::calc()
{
    if (SoundMgr::s_pInstance->IsPaused()) {
        return;
    }
    if (m_State == STATE_FADE_OUT && --m_Counter <= 0) {
        Clear();
        m_FxType = FX_NONE;
        m_State = STATE_NONE;
    }
    m_SendFader.Update();
    if (m_FxType != FX_NONE) {
        ApplySend(m_FxType, m_SendFader.GetValue());
    }
    m_ReturnFader.Update();
    nn::snd::CTR::SetAuxReturnVolume(nn::snd::CTR::AUX_BUS_0, m_ReturnFader.GetValue());
}
