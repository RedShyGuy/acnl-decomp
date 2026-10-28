#include "nw/snd/internal/snd_DriverCommand.h"

#include "nw/snd/internal/driver/snd_DisposeCallbackManager.h"
#include "nw/snd/internal/driver/snd_HardwareManager.h"
#include "nw/snd/internal/driver/snd_SequenceTrack.h"
#include "nw/snd/internal/driver/snd_VoiceManager.h"
#include "nw/snd/internal/snd_DriverCommandPlayer.h"

namespace nw {
namespace snd {
namespace internal {
// 0x004C7300 | nintendogs:callgraph [tier A]
void nw::snd::internal::DriverCommand::ProcessCommandList(DriverCommand* pList)
{
    for (DriverCommand* pCommand = pList; pCommand != nullptr; pCommand = pCommand->next) {
        switch (pCommand->id) {
        case ID_DUMMY:
        case 1:
            break;
        case ID_REPLY: {
            DriverCommandReply* pReply = static_cast<DriverCommandReply*>(pCommand);
            *pReply->pFlag = true;
            break;
        }

        // the players
        case ID_PLAYER_INIT: {
            DriverCommandPlayerInit* pInit = static_cast<DriverCommandPlayerInit*>(pCommand);
            pInit->player->Initialize();
            *pInit->availableFlag = true;
            break;
        }
        case ID_PLAYER_FINALIZE:
            static_cast<DriverCommandPlayer*>(pCommand)->player->Finalize();
            break;
        case ID_PLAYER_START:
            static_cast<DriverCommandPlayer*>(pCommand)->player->Start();
            break;
        case ID_PLAYER_STOP: {
            DriverCommandPlayerState* pState = static_cast<DriverCommandPlayerState*>(pCommand);
            if (pState->flag) {
                pState->player->m_Volume = 0.0f;
            }
            pState->player->Stop();
            break;
        }
        case ID_PLAYER_PAUSE: {
            DriverCommandPlayerState* pState = static_cast<DriverCommandPlayerState*>(pCommand);
            pState->player->Pause(pState->flag);
            break;
        }
        case ID_PLAYER_PANMODE: {
            DriverCommandPlayerPanParam* pParam = static_cast<DriverCommandPlayerPanParam*>(pCommand);
            pParam->player->m_PanMode = pParam->panMode;
            break;
        }
        case ID_PLAYER_PANCURVE: {
            DriverCommandPlayerPanParam* pParam = static_cast<DriverCommandPlayerPanParam*>(pCommand);
            pParam->player->m_PanCurve = pParam->panCurve;
            break;
        }
        case ID_PLAYER_FRONTBYPASS: {
            DriverCommandPlayerState* pState = static_cast<DriverCommandPlayerState*>(pCommand);
            pState->player->m_IsFrontBypass = pState->flag;
            break;
        }
        case ID_PLAYER_PARAM: {
            DriverCommandPlayerParam* pParam = static_cast<DriverCommandPlayerParam*>(pCommand);
            pParam->player->m_Volume = pParam->volume;
            pParam->player->m_Pitch = pParam->pitch;
            pParam->player->m_Pan = pParam->pan;
            pParam->player->m_SurroundPan = pParam->surroundPan;
            pParam->player->m_LpfFreq = pParam->lpfFreq;
            pParam->player->SetBiquadFilter(pParam->biquadType, pParam->biquadValue);
            pParam->player->m_MainSend = pParam->mainSend;
            for (int i = 0; i < 2; i++) {
                pParam->player->SetFxSend(static_cast<AuxBus>(i), pParam->fxSend[i]);
            }
            break;
        }
        case ID_PLAYER_CLEAR_UNKNOWN0x10:
            static_cast<DriverCommandPlayer*>(pCommand)->player->m_Unknown0x10 = false;
            break;

        // the sequence sounds
        case ID_SEQSOUND_SETUP: {
            DriverCommandSequenceSoundSetup* pSetup = static_cast<DriverCommandSequenceSoundSetup*>(pCommand);
            pSetup->player->Setup(pSetup->arg);
            pSetup->player->m_Priority = pSetup->channelPriority;
            pSetup->player->m_IsReleasePriorityFix = pSetup->isReleasePriorityFix;
            pSetup->player->SetSeqUserprocCallback(pSetup->userproc, pSetup->userprocArg);
            break;
        }
        case ID_SEQSOUND_LOAD: {
            DriverCommandSequenceSoundLoad* pLoad = static_cast<DriverCommandSequenceSoundLoad*>(pCommand);
            pLoad->player->RequestLoad(pLoad->startInfo, pLoad->arg);
            break;
        }
        case ID_SEQSOUND_PREPARE: {
            DriverCommandSequenceSoundPrepare* pPrepare = static_cast<DriverCommandSequenceSoundPrepare*>(pCommand);
            pPrepare->player->Prepare(pPrepare->arg);
            break;
        }
        case ID_SEQSOUND_SKIP: {
            DriverCommandSequenceSoundSkip* pSkip = static_cast<DriverCommandSequenceSoundSkip*>(pCommand);
            pSkip->player->Skip(static_cast<driver::SequenceSoundPlayer::OffsetType>(static_cast<u8>(pSkip->offsetType)), pSkip->offset);
            break;
        }
        case ID_SEQSOUND_TEMPORATIO: {
            DriverCommandSequenceSoundTempoRatio* pTempo = static_cast<DriverCommandSequenceSoundTempoRatio*>(pCommand);
            pTempo->player->m_TempoRatio = pTempo->tempoRatio;
            break;
        }
        case ID_SEQSOUND_CHANNELPRIO: {
            DriverCommandSequenceSoundChannelPrio* pPrio = static_cast<DriverCommandSequenceSoundChannelPrio*>(pCommand);
            pPrio->player->m_Priority = pPrio->priority;
            break;
        }
        case ID_SEQSOUND_SETVAR: {
            DriverCommandSequenceSoundSetVar* pVar = static_cast<DriverCommandSequenceSoundSetVar*>(pCommand);
            pVar->player->SetLocalVariable(pVar->varNo, static_cast<s16>(pVar->value));
            break;
        }
        case ID_SEQSOUND_SETGVAR: {
            DriverCommandSequenceSoundSetVar* pVar = static_cast<DriverCommandSequenceSoundSetVar*>(pCommand);
            driver::SequenceSoundPlayer::SetGlobalVariable(pVar->varNo, static_cast<s16>(pVar->value));
            break;
        }
        case ID_SEQSOUND_SETTVAR: {
            DriverCommandSequenceSoundSetVar* pVar = static_cast<DriverCommandSequenceSoundSetVar*>(pCommand);
            driver::SequenceTrack* pTrack = pVar->player->GetPlayerTrack(pVar->trackNo);
            if (pTrack != nullptr) {
                pTrack->SetTrackVariable(pVar->varNo, static_cast<s16>(pVar->value));
            }
            break;
        }
        case ID_SEQTRACK_MUTE: {
            DriverCommandSequenceTrackMute* pMute = static_cast<DriverCommandSequenceTrackMute*>(pCommand);
            pMute->player->SetTrackMute(pMute->trackBitFlag, static_cast<SeqMute>(pMute->mute));
            break;
        }
        case ID_SEQTRACK_SILENCE: {
            DriverCommandSequenceTrackSilence* pSilence = static_cast<DriverCommandSequenceTrackSilence*>(pCommand);
            pSilence->player->SetTrackSilence(pSilence->trackBitFlag, pSilence->isSilence, pSilence->fadeTimes);
            break;
        }
        case ID_SEQTRACK_VOLUME: {
            DriverCommandSequenceTrackParam* pParam = static_cast<DriverCommandSequenceTrackParam*>(pCommand);
            pParam->player->SetTrackVolume(pParam->trackBitFlag, pParam->value);
            break;
        }
        case ID_SEQTRACK_PITCH: {
            DriverCommandSequenceTrackParam* pParam = static_cast<DriverCommandSequenceTrackParam*>(pCommand);
            pParam->player->SetTrackPitch(pParam->trackBitFlag, pParam->value);
            break;
        }
        case ID_SEQTRACK_PAN: {
            DriverCommandSequenceTrackParam* pParam = static_cast<DriverCommandSequenceTrackParam*>(pCommand);
            pParam->player->SetTrackPan(pParam->trackBitFlag, pParam->value);
            break;
        }
        case ID_SEQTRACK_SURROUND_PAN: {
            DriverCommandSequenceTrackParam* pParam = static_cast<DriverCommandSequenceTrackParam*>(pCommand);
            pParam->player->SetTrackSurroundPan(pParam->trackBitFlag, pParam->value);
            break;
        }
        case ID_SEQTRACK_LPF: {
            DriverCommandSequenceTrackParam* pParam = static_cast<DriverCommandSequenceTrackParam*>(pCommand);
            pParam->player->SetTrackLpfFreq(pParam->trackBitFlag, pParam->value);
            break;
        }
        case ID_SEQTRACK_PAN_RANGE: {
            DriverCommandSequenceTrackParam* pParam = static_cast<DriverCommandSequenceTrackParam*>(pCommand);
            pParam->player->SetTrackPanRange(pParam->trackBitFlag, pParam->value);
            break;
        }
        case ID_SEQTRACK_BIQUAD: {
            DriverCommandSequenceTrackBiquad* pParam = static_cast<DriverCommandSequenceTrackBiquad*>(pCommand);
            pParam->player->SetTrackBiquadFilter(pParam->trackBitFlag, pParam->type, pParam->value);
            break;
        }
        case ID_SEQTRACK_BANK_INDEX: {
            DriverCommandSequenceTrackValue* pParam = static_cast<DriverCommandSequenceTrackValue*>(pCommand);
            pParam->player->SetTrackBankIndex(pParam->trackBitFlag, pParam->bankIndex);
            break;
        }
        case ID_SEQTRACK_TRANSPOSE: {
            DriverCommandSequenceTrackValue* pParam = static_cast<DriverCommandSequenceTrackValue*>(pCommand);
            pParam->player->SetTrackTranspose(pParam->trackBitFlag, pParam->transpose);
            break;
        }
        case ID_SEQTRACK_VELOCITY_RANGE: {
            DriverCommandSequenceTrackValue* pParam = static_cast<DriverCommandSequenceTrackValue*>(pCommand);
            pParam->player->SetTrackVelocityRange(pParam->trackBitFlag, pParam->velocityRange);
            break;
        }

        // the wave sounds
        case ID_WAVESOUND_PREPARE: {
            DriverCommandWaveSoundPrepare* pPrepare = static_cast<DriverCommandWaveSoundPrepare*>(pCommand);
            pPrepare->player->Prepare(pPrepare->startInfo, pPrepare->arg);
            break;
        }
        case ID_WAVESOUND_LOAD: {
            DriverCommandWaveSoundLoad* pLoad = static_cast<DriverCommandWaveSoundLoad*>(pCommand);
            pLoad->player->RequestLoad(pLoad->startInfo, pLoad->arg);
            break;
        }
        case ID_WAVESOUND_CHANNEL_PRIO: {
            DriverCommandWaveSoundChannelParam* pParam = static_cast<DriverCommandWaveSoundChannelParam*>(pCommand);
            pParam->player->m_Priority = pParam->priority;
            break;
        }
        case ID_WAVESOUND_CHANNEL_PARAM: {
            DriverCommandWaveSoundChannelParam* pParam = static_cast<DriverCommandWaveSoundChannelParam*>(pCommand);
            pParam->player->m_Priority = pParam->priority;
            pParam->player->m_IsReleasePriorityFix = pParam->isReleasePriorityFix;
            break;
        }
        case ID_WAVESOUND_LOOP: {
            DriverCommandPlayerState* pState = static_cast<DriverCommandPlayerState*>(pCommand);
            static_cast<driver::WaveSoundPlayer*>(pState->player)->m_IsLoop = pState->flag;
            break;
        }

        // the stream sounds
        case ID_STRMSOUND_SETUP: {
            DriverCommandStreamSoundSetup* pSetup = static_cast<DriverCommandStreamSoundSetup*>(pCommand);
            pSetup->player->Setup(pSetup->arg);
            break;
        }
        case ID_STRMSOUND_PREPARE: {
            DriverCommandStreamSoundPrepare* pPrepare = static_cast<DriverCommandStreamSoundPrepare*>(pCommand);
            pPrepare->player->Prepare(pPrepare->arg);
            break;
        }
        case ID_STRMSOUND_PREPARE_PREFETCH: {
            DriverCommandStreamSoundPreparePrefetch* pPrepare = static_cast<DriverCommandStreamSoundPreparePrefetch*>(pCommand);
            pPrepare->player->PreparePrefetch(pPrepare->arg);
            break;
        }
        case ID_STRMSOUND_LOAD_HEADER: {
            DriverCommandStreamSoundLoadHeader* pLoad = static_cast<DriverCommandStreamSoundLoadHeader*>(pCommand);
            pLoad->player->LoadHeader(pLoad->result, pLoad->adpcmParams, pLoad->requestId);
            break;
        }
        case ID_STRMSOUND_LOAD_DATA: {
            DriverCommandStreamSoundLoadData* pLoad = static_cast<DriverCommandStreamSoundLoadData*>(pCommand);
            pLoad->player->LoadStreamData(pLoad->result, pLoad->param);
            break;
        }
        case ID_STRMSOUND_LOOP_FROM_START: {
            DriverCommandPlayerState* pState = static_cast<DriverCommandPlayerState*>(pCommand);
            static_cast<driver::StreamSoundPlayer*>(pState->player)->m_IsLoopFromStart = pState->flag;
            break;
        }
        case ID_STRMTRACK_VOLUME: {
            DriverCommandStreamSoundTrackParam* pParam = static_cast<DriverCommandStreamSoundTrackParam*>(pCommand);
            pParam->player->SetTrackVolume(pParam->trackBitFlag, pParam->value);
            break;
        }
        case ID_STRMTRACK_INITIAL_VOLUME: {
            DriverCommandStreamSoundTrackInitialVolume* pParam = static_cast<DriverCommandStreamSoundTrackInitialVolume*>(pCommand);
            pParam->player->SetTrackInitialVolume(pParam->trackBitFlag, pParam->volume);
            break;
        }
        case ID_STRMTRACK_PAN: {
            DriverCommandStreamSoundTrackParam* pParam = static_cast<DriverCommandStreamSoundTrackParam*>(pCommand);
            pParam->player->SetTrackPan(pParam->trackBitFlag, pParam->value);
            break;
        }
        case ID_STRMTRACK_SURROUND_PAN: {
            DriverCommandStreamSoundTrackParam* pParam = static_cast<DriverCommandStreamSoundTrackParam*>(pCommand);
            pParam->player->SetTrackSurroundPan(pParam->trackBitFlag, pParam->value);
            break;
        }

        // the memory and the dispose callbacks
        case ID_DISPOSE: {
            DriverCommandDispose* pDispose = static_cast<DriverCommandDispose*>(pCommand);
            driver::DisposeCallbackManager::GetInstance().Dispose(pDispose->pMemory, pDispose->size);
            break;
        }
        case ID_REGISTER_DISPOSE_CALLBACK:
            driver::DisposeCallbackManager::GetInstance().RegisterDisposeCallback(
                static_cast<DriverCommandDisposeCallback*>(pCommand)->pCallback);
            break;
        case ID_UNREGISTER_DISPOSE_CALLBACK:
            driver::DisposeCallbackManager::GetInstance().UnregisterDisposeCallback(
                static_cast<DriverCommandDisposeCallback*>(pCommand)->pCallback);
            break;

        // the effects
        case ID_APPEND_EFFECT: {
            DriverCommandEffect* pEffect = static_cast<DriverCommandEffect*>(pCommand);
            driver::HardwareManager::GetInstance().AppendEffect(pEffect->bus, pEffect->effect);
            break;
        }
        case ID_APPEND_EFFECT_FX_DELAY: {
            DriverCommandEffectFxDelay* pEffect = static_cast<DriverCommandEffectFxDelay*>(pCommand);
            driver::HardwareManager::GetInstance().AppendEffect(pEffect->bus, pEffect->effect);
            break;
        }
        case ID_APPEND_EFFECT_FX_REVERB: {
            DriverCommandEffectFxReverb* pEffect = static_cast<DriverCommandEffectFxReverb*>(pCommand);
            driver::HardwareManager::GetInstance().AppendEffect(pEffect->bus, pEffect->effect);
            break;
        }
        case ID_APPEND_EFFECT_DSP_DELAY: {
            DriverCommandEffectDspDelay* pEffect = static_cast<DriverCommandEffectDspDelay*>(pCommand);
            driver::HardwareManager::GetInstance().AppendEffect(pEffect->bus, pEffect->effect, pEffect->param);
            break;
        }
        case ID_APPEND_EFFECT_DSP_REVERB: {
            DriverCommandEffectDspReverb* pEffect = static_cast<DriverCommandEffectDspReverb*>(pCommand);
            driver::HardwareManager::GetInstance().AppendEffect(pEffect->bus, pEffect->effect, pEffect->param);
            break;
        }
        case ID_CLEAR_EFFECT: {
            DriverCommandEffectClear* pEffect = static_cast<DriverCommandEffectClear*>(pCommand);
            driver::HardwareManager::GetInstance().ClearEffect(pEffect->bus, pEffect->fadeTimes);
            break;
        }

        case ID_VOICE_SYNC:
            driver::VoiceManager::GetInstance().UpdateAllVoicesSync(static_cast<DriverCommandVoiceSync*>(pCommand)->syncFlag);
            break;
        default:
            break;
        }
    }
}

} // namespace internal
} // namespace snd
} // namespace nw
