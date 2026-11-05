#include "Sound/dSoundVolumeMgr.h"
#include "Sound/dSoundMgr.h"
#include "nw/snd/snd_SoundHandle.h"
#include "nw/snd/snd_SoundPlayer.h"

// 0x0083EE92
const bool SoundVolumeMgr::s_IsFadedPause[PLAYER_NUM] = {true,  true,  true,  true,  true,  true,  false, false, false,
                                                         false, true,  true,  true,  true,  true,  true,  true,  true};
// 0x0094E640
SoundVolumeMgr::DuckParam SoundVolumeMgr::s_DemoDuckParam = {0.0f, 0, 30, 0, 30, 0xF6};

// 0x00128A5C
SoundVolumeMgr::SoundVolumeMgr() : _1F0(0), m_IsSeOff(false), m_IsBgmOff(false)
{
    for (s32 i = 0; i < DUCK_NUM; i++) {
        m_Ducks[i].SoundFaderEx::Reset(1.0f);
    }
    for (s32 i = 0; i < GROUP_NUM; i++) {
        m_Scales[i] = 1.0f;
    }
    m_DuckHandles.allocBuffer(30, NULL, 4);
}

// 0x0027A3AC
// 0x0027A360 (deleting dtor)
SoundVolumeMgr::~SoundVolumeMgr()
{
    // (only the members are destroyed)
}

// 0x00279CAC
void SoundVolumeMgr::Initialize()
{
    SoundMgr* mgr = SoundMgr::s_pInstance;
    for (u32 i = 0; i < BGM_PLAYER_NUM; i++) {
        f32 volume = m_IsBgmOff ? 0.0f : 1.0f;
        m_PlayerFaders[i].Initialize(mgr->GetSoundPlayer(i), volume);
        m_PlayerVolumes[i] = volume;
        m_IsPlayerPaused[i] = false;
        if (i == 0) {
            m_PlayerFaders[i].SetScale(&m_Scales[GROUP_BGM]);
        } else if (i == 2) {
            m_PlayerFaders[i].SetScale(&m_Scales[GROUP_BGM_MU]);
        }
    }
    for (u32 i = BGM_PLAYER_NUM; i < PLAYER_NUM; i++) {
        f32 volume = m_IsSeOff ? 0.0f : 1.0f;
        m_PlayerFaders[i].Initialize(mgr->GetSoundPlayer(i), volume);
        m_PlayerVolumes[i] = volume;
        m_IsPlayerPaused[i] = false;
        switch (i) {
        case PLAYER_SE_ACT & 0xFF:
        case PLAYER_SE_FOOT & 0xFF:
            m_PlayerFaders[i].SetScale(&m_Scales[GROUP_ACT]);
            break;
        case PLAYER_SE_OBJ & 0xFF:
        case PLAYER_SE_FUR_LV & 0xFF:
        case PLAYER_SE_FUR_TRG & 0xFF:
        case PLAYER_SE_MELODY & 0xFF:
        case PLAYER_SE_TV & 0xFF:
            m_PlayerFaders[i].SetScale(&m_Scales[GROUP_OBJ]);
            break;
        case PLAYER_SE_ENV & 0xFF:
            m_PlayerFaders[i].SetScale(&m_Scales[GROUP_ENV]);
            break;
        case PLAYER_BGM_RM & 0xFF:
            m_PlayerFaders[i].SetScale(&m_Scales[GROUP_BGM_RM]);
            break;
        case PLAYER_SE_HANIWA & 0xFF:
        case PLAYER_SE_SESSION & 0xFF:
            m_PlayerFaders[i].SetScale(&m_Scales[GROUP_HANIWA]);
            break;
        }
    }
    m_MasterFader.Reset(1.0f);
}

// 0x00279E8C
void SoundVolumeMgr::Finalize()
{
    // empty (in the original too)
}

// 0x00279C5C
void SoundVolumeMgr::EndDuck(const s32& index, bool now)
{
    DuckingFader& duck = m_Ducks[index];
    if (duck.GetState() == DuckingFader::STATE_UP_WAIT || duck.GetState() == DuckingFader::STATE_UP) {
        return;
    }
    if (now) {
        duck.SoundFaderEx::Reset(1.0f);
    } else {
        duck.SetTarget(1.0f, m_DuckParams[index].upFrames, m_DuckParams[index].upDelay);
    }
}

// 0x00279E70
void SoundVolumeMgr::EndDuckDemo()
{
    s32 index = 6;
    EndDuck(index, false);
}

// 0x00279E90
void SoundVolumeMgr::StartDuckDemo()
{
    m_PlayerFaders[PLAYER_DOUBUTSUGO & 0xFF].SetScale(&m_Scales[GROUP_ACT]);
    m_DuckParams[6] = s_DemoDuckParam;
    m_Ducks[6].SetTarget(m_DuckParams[6].value, m_DuckParams[6].downFrames, m_DuckParams[6].downDelay);
}

// 0x00279ED0
void SoundVolumeMgr::ResetDucks()
{
    for (s32 i = 0; i < DUCK_NUM; i++) {
        if (i == 6 || i == 10) {
            continue;
        }
        if (m_Ducks[i].GetState() == DuckingFader::STATE_UP_WAIT || m_Ducks[i].GetState() == DuckingFader::STATE_UP) {
            continue;
        }
        m_Ducks[i].SoundFaderEx::Reset(1.0f);
    }
    _1F0 = 0;
}

// 0x00279F40
void SoundVolumeMgr::PausePlayer(u32 player)
{
    if (player >= PLAYER_NUM) {
        return;
    }
    if (!s_IsFadedPause[player]) {
        SoundMgr::s_pInstance->GetSoundPlayer(player)->PauseAllSound(true, 0);
        return;
    }
    if (m_IsPlayerPaused[player]) {
        return;
    }
    m_PlayerVolumes[player] = m_PlayerFaders[player].GetTarget();
    m_PlayerFaders[player].SetTarget(0.0f, 0);
    m_IsPlayerPaused[player] = true;
}

// 0x00279FC4
void SoundVolumeMgr::UnpausePlayer(u32 player, s32 frames)
{
    if (player >= PLAYER_NUM) {
        return;
    }
    if (!s_IsFadedPause[player]) {
        SoundMgr::s_pInstance->GetSoundPlayer(player)->PauseAllSound(false, frames);
        return;
    }
    m_PlayerFaders[player].SetTarget(m_PlayerVolumes[player], frames);
    m_IsPlayerPaused[player] = false;
}

// 0x0027A328
void SoundVolumeMgr::SetPlayerVolume(u32 player, f32 volume, s32 frames)
{
    if (player >= PLAYER_NUM) {
        return;
    }
    if (m_IsPlayerPaused[player]) {
        m_PlayerVolumes[player] = volume;
    } else {
        m_PlayerFaders[player].SetTarget(volume, frames);
    }
}

// 0x002BD234
void SoundVolumeMgr::SetVolume(u32 volume, f32 value, s32 frames)
{
    switch (volume) {
    case VOLUME_BGM:
        if (!m_IsBgmOff) {
            SetPlayerVolume(PLAYER_BGM & 0xFF, value, frames);
        }
        break;
    case VOLUME_BGM_HORNET:
        if (!m_IsBgmOff) {
            SetPlayerVolume(PLAYER_BGM_HORNET & 0xFF, value, frames);
        }
        break;
    case VOLUME_BGM_MU:
        if (!m_IsBgmOff) {
            SetPlayerVolume(PLAYER_BGM_MU & 0xFF, value, frames);
        }
        break;
    case VOLUME_MASTER:
        if (!m_IsSeOff) {
            m_MasterFader.SetTarget(value, frames);
        }
        break;
    case VOLUME_ENV:
        if (!m_IsSeOff) {
            SetPlayerVolume(PLAYER_SE_ENV & 0xFF, value, frames);
        }
        break;
    }
}

// 0x0027A03C
void SoundVolumeMgr::calc()
{
    SoundMgr* mgr = SoundMgr::s_pInstance;
    if (mgr == NULL || mgr->IsPaused()) {
        return;
    }
    bool wasDemoDuckUp = m_Ducks[6].GetState() == DuckingFader::STATE_UP;

    // the handles that hold ducking fader 7 down: the ones whose sound ended go
    s32 i = 0;
    while (i < m_DuckHandles.size()) {
        nw::snd::SoundHandle* handle = i < m_DuckHandles.size() ? m_DuckHandles.at(i) : NULL;
        if (handle != NULL && handle->IsAttached()) {
            i++;
            continue;
        }
        m_DuckHandles.erase(i, 1);
        if (m_DuckHandles.size() == 0 && m_Ducks[7].GetState() != DuckingFader::STATE_UP_WAIT &&
            m_Ducks[7].GetState() != DuckingFader::STATE_UP) {
            m_Ducks[7].SetTarget(1.0f, m_DuckParams[7].upFrames, m_DuckParams[7].upDelay);
        }
    }

    for (s32 group = 0; group < GROUP_NUM; group++) {
        m_Scales[group] = 1.0f;
    }
    for (s32 duck = 0; duck < DUCK_NUM; duck++) {
        m_Ducks[duck].Update();
        f32 value = m_Ducks[duck].GetValue();
        for (s32 group = 0; group < GROUP_NUM; group++) {
            if (duck == 9 && group == GROUP_BGM) {
                // the BGM never goes below 0.3125 with this one
                if (value < 0.3125f) {
                    m_Scales[GROUP_BGM] = 0.3125f;
                } else if (value < 1.0f) {
                    m_Scales[GROUP_BGM] = value;
                }
            } else if (m_DuckParams[duck].groupMask & (1 << group)) {
                if (m_Scales[group] > value) {
                    m_Scales[group] = value;
                }
            }
        }
    }
    if (mgr->m_State == 0) {
        m_Scales[GROUP_BGM_MU] = 0.0f;
        m_Scales[GROUP_BGM_RM] = 0.0f;
        m_Scales[GROUP_OBJ] = 0.0f;
        m_Scales[GROUP_HANIWA] = 0.0f;
        m_Scales[GROUP_ENV] = 0.0f;
        m_Scales[GROUP_ACT] = 0.0f;
    }
    if (wasDemoDuckUp && m_Ducks[6].GetState() == DuckingFader::STATE_NONE) {
        m_PlayerFaders[PLAYER_DOUBUTSUGO & 0xFF].SetScale(NULL);
    }
    if (!m_IsBgmOff) {
        for (u32 player = 0; player < BGM_PLAYER_NUM; player++) {
            m_PlayerFaders[player].Update();
        }
    }
    if (!m_IsSeOff) {
        for (u32 player = BGM_PLAYER_NUM; player < PLAYER_NUM; player++) {
            m_PlayerFaders[player].Update();
        }
    }
    m_MasterFader.Update();
}
