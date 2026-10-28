#include "nw/snd/internal/snd_BasicSound.h"

#include <cstring>

#include "nw/snd/internal/driver/snd_BasicSoundPlayer.h"
#include "nw/snd/internal/snd_DriverCommandManager.h"
#include "nw/snd/internal/snd_ExternalSoundPlayer.h"
#include "nw/snd/snd_SoundHandle.h"
#include "nw/snd/snd_SoundPlayer.h"

namespace nw {
namespace snd {
namespace internal {
namespace {
// the instance id of the next initialized sound
// 0x00976170
u32 s_LastInstanceId = 0;

// (inline) a command for the player of a sound
template <typename TCommand>
TCommand* AllocPlayerCommand(DriverCommandManager& manager, DriverCommand::Id id, BasicSound* pSound)
{
    TCommand* pCommand = manager.AllocCommand<TCommand>();
    pCommand->id = id;
    pCommand->player = pSound->GetBasicSoundPlayerHandle();
    return pCommand;
}
} // namespace

// 0x00976174
const TypeInfo nw::snd::internal::BasicSound::s_TypeInfo(nullptr);

// 0x004C49AC slot 0x0C | fefates:bytes
void nw::snd::internal::BasicSound::Initialize()
{
    m_InstanceId = s_LastInstanceId++;
    m_Unknown0x04 = 0;
    m_pGeneralHandle = nullptr;
    m_pTempGeneralHandle = nullptr;
    m_pSoundPlayer = nullptr;
    m_pSoundActor = nullptr;
    m_pExtSoundPlayer = nullptr;
    m_Id = 0xFFFFFFFF;
    m_AmbientInfo.paramUpdateCallback = nullptr;
    m_AmbientInfo.argUpdateCallback = nullptr;
    m_AmbientInfo.argAllocatorCallback = nullptr;
    m_AmbientInfo.arg = nullptr;
    m_AmbientInfo.argSize = 0;
    m_PlayerState = PLAYER_STATE_INIT;
    m_PauseState = PAUSE_STATE_NORMAL;
    m_MuteState = MUTE_STATE_NORMAL;
    m_UnPauseFlag = false;
    m_StartFlag = false;
    m_StartedFlag = false;
    m_AutoStopFlag = false;
    m_FadeOutFlag = false;
    m_IsPlayerAvailable = false;
    m_AutoStopCounter = 0;
    m_UpdateCounter = 0;
    m_PlayingCounter = 0;
    m_FadeVolume.InitValue(1.0f);
    m_PauseFadeVolume.InitValue(1.0f);
    m_MuteFadeVolume.InitValue(1.0f);
    m_InitVolume = 1.0f;
    m_Pitch = 1.0f;
    m_Pan = 0.0f;
    m_ExtMoveVolume.InitValue(1.0f);
    m_LpfFreq = 0.0f;
    m_BiquadType = -1;
    m_BiquadValue = 0.0f;
    m_SurroundPan = 0.0f;
    m_MainSend = 0.0f;
    m_FxSend[0] = 0.0f;
    m_FxSend[1] = 0.0f;
    m_AmbientParam.volume = 1.0f;
    m_AmbientParam.pitch = 1.0f;
    m_AmbientParam.pan = 0.0f;
    m_AmbientParam.surroundPan = 0.0f;
    m_AmbientParam.fxSend = 0.0f;
    m_AmbientParam.lpfFreq = 0.0f;
    m_AmbientParam.biquadValue = 0.0f;
    m_AmbientParam.priority = 0;
    m_AmbientParam.biquadType = -1;
    m_AmbientParam.userData = 0;
    m_ActorParam.pitch = 1.0f;
    m_ActorParam.volume = 1.0f;
    m_ActorParam.pan = 0.0f;
    m_ActorParam.lpfFreq = 0.0f;
    if (m_UserParamSize != 0) {
        std::memset(m_pUserParam, 0, m_UserParamSize);
    }

    driver::BasicSoundPlayer* pPlayer = GetBasicSoundPlayerHandle();
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    DriverCommandPlayerInit* pCommand = manager.AllocCommand<DriverCommandPlayerInit>();
    pCommand->id = DriverCommand::ID_PLAYER_INIT;
    pCommand->player = pPlayer;
    pCommand->availableFlag = &m_IsPlayerAvailable;
    manager.PushCommand(pCommand);
    pPlayer->m_Event.ClearSignal();
    m_State = STATE_INITIALIZED;
}

// 0x004C4B54 | fefates:bytes [tier B]
void nw::snd::internal::BasicSound::SetPanCurve(PanCurve panCurve)
{
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    DriverCommandPlayerPanParam* pCommand = AllocPlayerCommand<DriverCommandPlayerPanParam>(manager, DriverCommand::ID_PLAYER_PANCURVE, this);
    pCommand->panCurve = panCurve;
    manager.PushCommand(pCommand);
}

// 0x004C4BB0 | fefates:callgraph [tier C]
void nw::snd::internal::BasicSound::SetPriority(int priority, int ambientPriority)
{
    m_AmbientParam.priority = ambientPriority;
    m_Priority = priority;
}

// 0x004C4BBC slot 0x30 | fefates:bytes
void nw::snd::internal::BasicSound::UpdateParam()
{
    OnUpdateParam();

    f32 volume;
    if (m_MuteState == MUTE_STATE_MUTED) {
        volume = 0.0f;
    } else {
        volume = 1.0f;
        volume *= m_InitVolume;
        volume *= m_pSoundPlayer->m_Volume;
        volume *= m_ExtMoveVolume.GetValue();
        volume *= m_FadeVolume.GetValue();
        volume *= m_PauseFadeVolume.GetValue();
        volume *= m_MuteFadeVolume.GetValue();
        volume *= m_AmbientParam.volume;
        volume *= m_ActorParam.volume;
    }

    const f32 pitch = m_ActorParam.pitch * (m_AmbientParam.pitch * m_Pitch);
    const f32 pan = m_ActorParam.pan + (m_AmbientParam.pan + m_Pan);
    const f32 surroundPan = m_AmbientParam.surroundPan + m_SurroundPan;
    const f32 lpfFreq = m_ActorParam.lpfFreq + (m_pSoundPlayer->m_LpfFreq + (m_AmbientParam.lpfFreq + m_LpfFreq));

    // the biquad filter of the sound, else of the player, else of the ambient parameters
    int biquadType = m_BiquadType;
    f32 biquadValue = m_BiquadValue;
    if (biquadType == -1) {
        biquadType = m_pSoundPlayer->m_BiquadType;
        biquadValue = m_pSoundPlayer->m_BiquadValue;
        if (biquadType == -1) {
            biquadType = m_AmbientParam.biquadType;
            biquadValue = m_AmbientParam.biquadValue;
        }
    }

    const f32 mainSend = m_pSoundPlayer->m_MainSend + m_MainSend;
    f32 fxSend[2];
    for (int i = 0; i < 2; ++i) {
        fxSend[i] = 0.0f;
        fxSend[i] += m_FxSend[i];
        fxSend[i] += m_pSoundPlayer->m_FxSend[i];
    }
    fxSend[0] += m_AmbientParam.fxSend;

    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    DriverCommandPlayerParam* pCommand = AllocPlayerCommand<DriverCommandPlayerParam>(manager, DriverCommand::ID_PLAYER_PARAM, this);
    pCommand->volume = volume;
    pCommand->pitch = pitch;
    pCommand->lpfFreq = lpfFreq;
    pCommand->pan = pan;
    pCommand->surroundPan = surroundPan;
    pCommand->biquadType = biquadType;
    pCommand->biquadValue = biquadValue;
    pCommand->mainSend = mainSend;
    pCommand->fxSend[0] = fxSend[0];
    pCommand->fxSend[1] = fxSend[1];
    manager.PushCommand(pCommand);
}

// 0x004C4D70 slot 0x2C
void nw::snd::internal::BasicSound::OnUpdateParam()
{
    // empty (in the original too)
}

// 0x004C4D74 | nintendogs:callgraph [tier A]
void nw::snd::internal::BasicSound::StartPrepared()
{
    m_StartFlag = true;
}

// 0x004C4D80 | nintendogs:bytes [tier A]
void nw::snd::internal::BasicSound::SetAmbientInfo(const AmbientInfo& info)
{
    // a copy of the argument for the sound
    void* pArg = info.argAllocatorCallback->AllocAmbientArg(info.argSize);
    if (pArg == nullptr) {
        return;
    }
    std::memcpy(pArg, info.arg, info.argSize);
    m_AmbientInfo.paramUpdateCallback = info.paramUpdateCallback;
    m_AmbientInfo.argUpdateCallback = info.argUpdateCallback;
    m_AmbientInfo.argAllocatorCallback = info.argAllocatorCallback;
    m_AmbientInfo.arg = pArg;
    m_AmbientInfo.argSize = info.argSize;
}

// 0x004C4DCC | fefates:bytes [tier B]
void nw::snd::internal::BasicSound::SetFrontBypass(bool isFrontBypass)
{
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    DriverCommandPlayerState* pCommand = AllocPlayerCommand<DriverCommandPlayerState>(manager, DriverCommand::ID_PLAYER_FRONTBYPASS, this);
    pCommand->flag = isFrontBypass;
    manager.PushCommand(pCommand);
}

// 0x004C4E28 (name is ours)
void nw::snd::internal::BasicSound::SetSurroundPan(f32 surroundPan)
{
    m_SurroundPan = surroundPan;
}

// 0x004C4E30 (name is ours)
void nw::snd::internal::BasicSound::SetBiquadFilter(int type, f32 value)
{
    m_BiquadValue = value;
    m_BiquadType = type;
}

// 0x004C4E3C slot 0x28 | fefates:bytes
void nw::snd::internal::BasicSound::UpdateMoveValue()
{
    m_FadeVolume.Update();
    m_MuteFadeVolume.Update();
    m_ExtMoveVolume.Update();
}

// 0x004C4E7C | fefates:bytes [tier B]
void nw::snd::internal::BasicSound::SetInitialVolume(f32 volume)
{
    if (volume < 0.0f) {
        volume = 0.0f;
    }
    m_InitVolume = volume;
}

// 0x004C4ECC (name is ours)
void nw::snd::internal::BasicSound::DetachSoundPlayer(SoundPlayer*)
{
    m_pSoundPlayer = nullptr;
}

// 0x004C4ED8 | fefates:bytes [tier B]
void nw::snd::internal::BasicSound::SetPlayerPriority(int priority)
{
    m_Priority = priority;
    if (m_pSoundPlayer != nullptr) {
        m_pSoundPlayer->detail_SortPriorityList(this);
    }
    OnUpdatePlayerPriority();
}

// 0x004C4F08 | nintendogs:bytes [tier A]
int nw::snd::internal::BasicSound::GetAmbientPriority(const AmbientInfo& info, u32 soundId)
{
    if (info.paramUpdateCallback == nullptr) {
        return 0;
    }
    return info.paramUpdateCallback->GetAmbientPriority(info.arg, soundId);
}

// 0x004C4F30 (name is ours)
void nw::snd::internal::BasicSound::SetAutoStopCounter(int frames)
{
    m_AutoStopCounter = frames;
    m_AutoStopFlag = frames > 0;
}

// 0x004C4F48 slot 0x24
void nw::snd::internal::BasicSound::OnUpdatePlayerPriority()
{
    // empty (in the original too)
}

// 0x004C4F4C | nintendogs:callgraph [tier A]
void nw::snd::internal::BasicSound::DetachTempGeneralHandle()
{
    m_pTempGeneralHandle->DetachSound();
}

// 0x004C4F54 | nintendogs:callgraph [tier A]
bool nw::snd::internal::BasicSound::IsAttachedGeneralHandle()
{
    return m_pGeneralHandle != nullptr;
}

// 0x004C4F88 (name is ours)
void nw::snd::internal::BasicSound::DetachExternalSoundPlayer(ExternalSoundPlayer*)
{
    m_pExtSoundPlayer = nullptr;
}

// 0x004C4F94 | nintendogs:callgraph [tier A]
bool nw::snd::internal::BasicSound::IsAttachedTempGeneralHandle()
{
    return m_pTempGeneralHandle != nullptr;
}

// 0x004C4FA4 | mk7dlp:callgraph [tier A]
void nw::snd::internal::BasicSound::Pause(bool isPause, int fadeFrames)
{
    if (isPause) {
        switch (m_PauseState) {
        case PAUSE_STATE_NORMAL:
        case PAUSE_STATE_PAUSING:
        case PAUSE_STATE_UNPAUSING: {
            int frames = static_cast<int>(m_PauseFadeVolume.GetValue() * fadeFrames);
            if (frames <= 0) {
                frames = 1;
            }
            m_PauseFadeVolume.SetTarget(0.0f, frames);
            m_PauseState = PAUSE_STATE_PAUSING;
            m_UnPauseFlag = false;
            break;
        }
        default:
            break;
        }
    } else {
        switch (m_PauseState) {
        case PAUSE_STATE_PAUSING:
        case PAUSE_STATE_PAUSED:
        case PAUSE_STATE_UNPAUSING: {
            int frames = static_cast<int>((1.0f - m_PauseFadeVolume.GetValue()) * fadeFrames);
            if (frames <= 0) {
                frames = 1;
            }
            m_PauseFadeVolume.SetTarget(1.0f, frames);
            m_PauseState = PAUSE_STATE_UNPAUSING;
            m_UnPauseFlag = true;
            break;
        }
        default:
            break;
        }
    }
}

// 0x004C50AC | fefates:bytes [tier B]
void nw::snd::internal::BasicSound::FadeIn(int frames)
{
    if (m_FadeOutFlag || m_UpdateCounter != 0) {
        return;
    }
    m_FadeVolume.InitValue(0.0f);
    m_FadeVolume.SetTarget(1.0f, frames);
}

// 0x004C5114 (name is ours)
void nw::snd::internal::BasicSound::SetPan(f32 pan)
{
    m_Pan = pan;
}

// 0x004C511C | fefates:bytes [tier B]
void nw::snd::internal::BasicSound::Update()
{
    driver::BasicSoundPlayer* pPlayer = GetBasicSoundPlayerHandle();
    if (m_IsPlayerAvailable && GetBasicSoundPlayerHandle()->m_IsFinished) {
        Finalize();
        return;
    }
    if (pPlayer->m_Unknown0x10) {
        DriverCommandManager& manager = DriverCommandManager::GetInstance();
        DriverCommandPlayer* pCommand = AllocPlayerCommand<DriverCommandPlayer>(manager, DriverCommand::ID_PLAYER_CLEAR_UNKNOWN0x10, this);
        manager.PushCommand(pCommand);
        Finalize();
        return;
    }
    if (!IsPrepared()) {
        return;
    }

    // the auto stop
    if (m_AutoStopFlag) {
        if (m_AutoStopCounter != 0) {
            --m_AutoStopCounter;
        } else if (m_PauseState == PAUSE_STATE_NORMAL || m_PauseState == PAUSE_STATE_UNPAUSING) {
            Finalize();
            return;
        }
    }

    // the start
    bool isStart = false;
    if (m_StartedFlag) {
        if (m_PlayerState == PLAYER_STATE_PLAY) {
            if (m_UpdateCounter != 0xFFFFFFFF) {
                ++m_UpdateCounter;
            }
            if (m_PauseState != PAUSE_STATE_PAUSED && m_PlayingCounter != 0xFFFFFFFF) {
                ++m_PlayingCounter;
            }
        }
    } else {
        if (!m_StartFlag || !IsPrepared()) {
            return;
        }
        isStart = true;
        m_PlayerState = PLAYER_STATE_PLAY;
        if (m_UpdateCounter != 0xFFFFFFFF) {
            ++m_UpdateCounter;
        }
        if (m_PauseState != PAUSE_STATE_PAUSED && m_PlayingCounter != 0xFFFFFFFF) {
            ++m_PlayingCounter;
        }
    }

    // the move values
    switch (m_PauseState) {
    case PAUSE_STATE_NORMAL:
        UpdateMoveValue();
        break;
    case PAUSE_STATE_PAUSING:
        m_PauseFadeVolume.Update();
        break;
    case PAUSE_STATE_UNPAUSING:
        m_PauseFadeVolume.Update();
        UpdateMoveValue();
        break;
    default:
        break;
    }

    // the ambient parameters
    if (m_AmbientInfo.argUpdateCallback != nullptr) {
        m_AmbientInfo.argUpdateCallback->UpdateAmbientArg(m_AmbientInfo.arg, this);
    }
    if (m_AmbientInfo.paramUpdateCallback != nullptr) {
        AmbientParam param;
        if (m_UpdateCounter != 0) {
            param = m_AmbientParam;
        }
        m_AmbientInfo.paramUpdateCallback->UpdateAmbientParam(m_AmbientInfo.arg, m_Id, &param);
        m_AmbientParam = param;
    }
    if (m_pSoundActor != nullptr) {
        m_ActorParam = m_pSoundActor->m_ActorParam;
    }
    UpdateParam();

    // the end of a fade out
    if (m_FadeOutFlag && m_FadeVolume.IsFinished()) {
        m_FadeOutFlag = false;
        Finalize();
        return;
    }

    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    if (isStart) {
        DriverCommandPlayer* pCommand = AllocPlayerCommand<DriverCommandPlayer>(manager, DriverCommand::ID_PLAYER_START, this);
        manager.PushCommand(pCommand);
        m_StartedFlag = true;
        m_StartFlag = false;
    }

    // the pause
    switch (m_PauseState) {
    case PAUSE_STATE_PAUSING:
        if (m_PauseFadeVolume.IsFinished()) {
            DriverCommandPlayerState* pCommand = AllocPlayerCommand<DriverCommandPlayerState>(manager, DriverCommand::ID_PLAYER_PAUSE, this);
            pCommand->flag = true;
            manager.PushCommand(pCommand);
            m_PauseState = PAUSE_STATE_PAUSED;
        }
        break;
    case PAUSE_STATE_UNPAUSING:
        if (m_PauseFadeVolume.IsFinished()) {
            m_PauseState = PAUSE_STATE_NORMAL;
        }
        break;
    default:
        break;
    }
    if (m_UnPauseFlag) {
        DriverCommandPlayerState* pCommand = AllocPlayerCommand<DriverCommandPlayerState>(manager, DriverCommand::ID_PLAYER_PAUSE, this);
        pCommand->flag = false;
        manager.PushCommand(pCommand);
        m_UnPauseFlag = false;
    }

    // the mute
    switch (m_MuteState) {
    case MUTE_STATE_MUTING:
        if (m_MuteFadeVolume.IsFinished()) {
            m_MuteState = MUTE_STATE_MUTED;
        }
        break;
    case MUTE_STATE_UNMUTING:
        if (m_MuteFadeVolume.IsFinished()) {
            m_MuteState = MUTE_STATE_NORMAL;
        }
        break;
    default:
        break;
    }
}

// 0x004C5604 slot 0x10 | fefates:bytes
void nw::snd::internal::BasicSound::Finalize()
{
    if (m_State != STATE_INITIALIZED) {
        return;
    }
    m_Id = 0xFFFFFFFF;
    if (m_pGeneralHandle != nullptr) {
        m_pGeneralHandle->DetachSound();
    }
    if (m_pTempGeneralHandle != nullptr) {
        m_pTempGeneralHandle->DetachSound();
    }
    if (IsAttachedTempSpecialHandle()) {
        DetachTempSpecialHandle();
    }
    if (m_pSoundPlayer != nullptr) {
        m_pSoundPlayer->detail_RemoveSound(this);
    }
    if (m_pExtSoundPlayer != nullptr) {
        m_pExtSoundPlayer->RemoveSound(this);
    }
    if (m_AmbientInfo.argAllocatorCallback != nullptr) {
        m_AmbientInfo.argAllocatorCallback->FreeAmbientArg(m_AmbientInfo.arg, this);
        m_AmbientInfo.arg = nullptr;
    }

    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    if (m_StartedFlag) {
        DriverCommandPlayerState* pCommand = AllocPlayerCommand<DriverCommandPlayerState>(manager, DriverCommand::ID_PLAYER_STOP, this);
        pCommand->flag = m_FadeOutFlag;
        manager.PushCommand(pCommand);
        m_StartedFlag = false;
    }
    m_IsPlayerAvailable = false;
    m_PlayerState = PLAYER_STATE_STOP;
    DriverCommandManager& manager2 = DriverCommandManager::GetInstance();
    DriverCommandPlayerState* pCommand = AllocPlayerCommand<DriverCommandPlayerState>(manager2, DriverCommand::ID_PLAYER_FINALIZE, this);
    manager2.PushCommand(pCommand);

    if (m_StopCallback != nullptr) {
        m_StopCallback();
        m_StopCallback = nullptr;
    }
    m_FadeOutFlag = false;
    m_State = STATE_FINALIZED;
}

// 0x004C5784 (name is ours)
void nw::snd::internal::BasicSound::SetPitch(f32 pitch)
{
    m_Pitch = pitch;
}

// 0x004C578C (name is ours)
void nw::snd::internal::BasicSound::SetFxSend(AuxBus bus, f32 send)
{
    m_FxSend[bus] = send;
}

// 0x004C5798 | fefates:bytes [tier B]
void nw::snd::internal::BasicSound::SetVolume(f32 volume, int frames)
{
    if (volume < 0.0f) {
        volume = 0.0f;
    }
    m_ExtMoveVolume.SetTarget(volume, frames);
}

// 0x004C57E4 (name is ours)
nw::snd::internal::BasicSound::BasicSound()
    : m_State(STATE_CONSTRUCTED), m_pUserParam(nullptr), m_UserParamSize(0), m_StopCallback(nullptr)
{
}

// 0x004C58C8 slot 0x04
// 0x004C58B0 (deleting dtor)
nw::snd::internal::BasicSound::~BasicSound()
{
    m_State = STATE_DESTRUCTED;
}

// 0x00137B28 | nintendogs:callgraph [tier A]
void nw::snd::internal::BasicSound::DetachGeneralHandle()
{
    m_pGeneralHandle->DetachSound();
}

// 0x00137CC0 | fefates:bytes [tier B]
void nw::snd::internal::BasicSound::Stop(int fadeFrames)
{
    if (fadeFrames <= 0 || m_PauseState == PAUSE_STATE_PAUSED || (!m_StartFlag && !m_StartedFlag)) {
        Finalize();
        return;
    }
    const int frames = static_cast<int>(m_FadeVolume.GetValue() * fadeFrames);
    m_FadeVolume.SetTarget(0.0f, frames);
    SetPlayerPriority(0);
    m_AutoStopFlag = false;
    m_PauseState = PAUSE_STATE_NORMAL;
    m_UnPauseFlag = false;
    m_FadeOutFlag = true;
    m_MuteState = MUTE_STATE_NORMAL;
}

// 0x004C9150 | nintendogs:callseq [tier A]
void nw::snd::internal::BasicSound::SetPanMode(PanMode panMode)
{
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    DriverCommandPlayerPanParam* pCommand = AllocPlayerCommand<DriverCommandPlayerPanParam>(manager, DriverCommand::ID_PLAYER_PANMODE, this);
    pCommand->panMode = panMode;
    manager.PushCommand(pCommand);
}

// 0x0073F2E8 slot 0x00 (name after nw::gfx)
const TypeInfo* nw::snd::internal::BasicSound::GetRuntimeTypeInfo() const
{
    return &s_TypeInfo;
}

// 0x0073F2F4 | fefates:bytes [tier B]
bool nw::snd::internal::BasicSound::IsPause() const
{
    return m_PauseState == PAUSE_STATE_PAUSING || m_PauseState == PAUSE_STATE_PAUSED;
}

} // namespace internal
} // namespace snd
} // namespace nw
