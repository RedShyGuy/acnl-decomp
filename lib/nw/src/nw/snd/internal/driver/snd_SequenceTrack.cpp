#include "nw/snd/internal/driver/snd_SequenceTrack.h"

#include "nw/snd/internal/driver/snd_Bank.h"
#include "nw/snd/internal/driver/snd_NoteOnCallback.h"
#include "nw/snd/internal/driver/snd_SequenceSoundPlayer.h"
#include "nw/snd/internal/driver/snd_Voice.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
namespace {
// the steps of the track parameters
const f32 VOLUME_STEP = 1.0f / (127.0f * 127.0f * 127.0f);
const f32 PITCH_BEND_STEP = 1.0f / 128.0f;
const f32 PAN_STEP = 1.0f / 63.0f;
const f32 SEND_STEP = 1.0f / 127.0f;
// the silence volume of a channel
const u8 SILENCE_VOLUME_MAX = 255;
// the release of a note of the same key group
const int KEY_GROUP_RELEASE = 126;
} // namespace

// 0x004CBBE8 (name is ours)
void nw::snd::internal::driver::SequenceTrack::SetLpfFreq(f32 lpfFreq)
{
    m_LpfFreq = lpfFreq;
}

// 0x004CBBF0 | nintendogs:callseq-callee [tier A]
void nw::snd::internal::driver::SequenceTrack::SetSeqData(const void* pBase, s32 offset)
{
    m_pSeqBase = static_cast<const u8*>(pBase);
    m_pCurrentAddr = m_pSeqBase + offset;
}

// 0x004CBC00 | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceTrack::SetSilence(bool isSilence, int fadeTimes)
{
    m_SilenceFlag = isSilence;
    for (Channel* pChannel = m_pChannelList; pChannel != nullptr; pChannel = pChannel->m_pNextLink) {
        pChannel->m_SilenceVolume.SetTarget(isSilence ? 0 : SILENCE_VOLUME_MAX,
                                            static_cast<u16>((fadeTimes + Channel::UPDATE_INTERVAL_MSEC - 1) / static_cast<u32>(Channel::UPDATE_INTERVAL_MSEC)));
    }
}

// 0x004CBC94 (name is ours)
void nw::snd::internal::driver::SequenceTrack::SetPanRange(f32 panRange)
{
    m_PanRange = panRange;
}

// 0x004CBC9C (name is ours)
void nw::snd::internal::driver::SequenceTrack::SetBankIndex(int bankIndex)
{
    m_BankIndex = bankIndex;
}

// 0x004CBCA4 (name is ours)
void nw::snd::internal::driver::SequenceTrack::SetTranspose(s8 transpose)
{
    m_Transpose = transpose;
}

// 0x004CBCAC | fefates:bytes [tier B]
int nw::snd::internal::driver::SequenceTrack::ParseNextTick(bool doNoteOn)
{
    if (!m_OpenFlag) {
        return 0;
    }
    m_Volume.Update();
    m_Volume2.Update();
    m_Pan.Update();
    m_SurroundPan.Update();
    m_PitchBend.Update();

    if (m_NoteWaitFlag) {
        if (m_pChannelList != nullptr) {
            return 1;
        }
        m_NoteWaitFlag = false;
    }
    if (m_Wait > 0) {
        --m_Wait;
        if (m_Wait > 0) {
            return 1;
        }
    }
    if (m_pCurrentAddr == nullptr) {
        return 1;
    }
    for (int count = 0; m_Wait == 0 && !m_NoteWaitFlag;) {
        ++count;
        if (count > PARSE_COUNT_MAX) {
            break;
        }
        if (Parse(doNoteOn) == PARSE_RESULT_FINISH) {
            return -1;
        }
    }
    return 1;
}

// 0x004CBDDC | nintendogs:callseq-callee [tier A]
void nw::snd::internal::driver::SequenceTrack::FreeAllChannel()
{
    for (Channel* pChannel = m_pChannelList; pChannel != nullptr; pChannel = pChannel->m_pNextLink) {
        Channel::FreeChannel(pChannel);
    }
    m_pChannelList = nullptr;
}

// 0x004CBE10 | nintendogs:callseq-callee [tier A]
s16* nw::snd::internal::driver::SequenceTrack::GetVariablePtr(int index)
{
    if (index < VARIABLE_COUNT) {
        return &m_TrackVariable[index];
    }
    return nullptr;
}

// 0x004CBE24 (name is ours)
void nw::snd::internal::driver::SequenceTrack::SetSurroundPan(f32 surroundPan)
{
    m_ExtSurroundPan = surroundPan;
}

// 0x004CBE2C | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceTrack::PauseAllChannel(bool isPause)
{
    for (Channel* pChannel = m_pChannelList; pChannel != nullptr; pChannel = pChannel->m_pNextLink) {
        pChannel->Pause(isPause);
    }
}

// 0x004CBE80 (name is ours)
void nw::snd::internal::driver::SequenceTrack::SetBiquadFilter(int type, f32 value)
{
    m_BiquadValue = value;
    m_BiquadType = type;
}

// 0x004CBE8C (name is ours)
void nw::snd::internal::driver::SequenceTrack::SetTrackVariable(int index, s16 value)
{
    m_TrackVariable[index] = value;
}

// 0x004CBE9C (name is ours)
void nw::snd::internal::driver::SequenceTrack::SetVelocityRange(u8 velocityRange)
{
    m_VelocityRange = velocityRange;
}

// 0x004CBEA4 | nintendogs:callseq-callee [tier A]
void nw::snd::internal::driver::SequenceTrack::ReleaseAllChannel(int release)
{
    UpdateChannelParam();
    for (Channel* pChannel = m_pChannelList; pChannel != nullptr; pChannel = pChannel->m_pNextLink) {
        if (pChannel->m_ActiveFlag) {
            if (release >= 0) {
                pChannel->m_CurveAdshr.SetRelease(static_cast<u8>(release));
            }
            pChannel->Release();
        }
    }
}

// 0x004CBEF8 | fefates:bytes [tier B]
DECOMP_NOINLINE void nw::snd::internal::driver::SequenceTrack::UpdateChannelParam()
{
    if (!m_OpenFlag || m_pChannelList == nullptr) {
        return;
    }

    // the volume: the track, the second track volume and the player as a cubic curve
    SequenceSoundPlayer* pPlayer = m_pSequenceSoundPlayer;
    const u32 volume = m_Volume.GetValue() * m_Volume2.GetValue() * pPlayer->m_SequenceVolume.GetValue();
    f32 volumeRatio = volume * VOLUME_STEP;
    volumeRatio = m_ExtVolume * pPlayer->m_Volume * (volumeRatio * volumeRatio);

    const f32 pitchBend = m_PitchBend.GetValue() * PITCH_BEND_STEP * m_BendRange;
    const f32 pitchRatio = pPlayer->m_Pitch * m_ExtPitch;

    f32 pan = m_Pan.GetValue() * PAN_STEP;
    if (pan > 1.0f) {
        pan = 1.0f;
    } else if (pan < -1.0f) {
        pan = -1.0f;
    }
    pan = (m_ExtPan + pPlayer->m_Pan) + pan * (m_PanRange * pPlayer->m_PanRange);

    f32 surroundPan = m_SurroundPan.GetValue() * PAN_STEP;
    if (surroundPan > 2.0f) {
        surroundPan = 2.0f;
    } else if (surroundPan < 0.0f) {
        surroundPan = 0.0f;
    }
    surroundPan = surroundPan + (m_ExtSurroundPan + pPlayer->m_SurroundPan);

    const f32 lpfFreq = m_LpfFreq + pPlayer->m_LpfFreq;

    int biquadType = m_BiquadType;
    f32 biquadValue = m_BiquadValue;
    if (static_cast<s8>(pPlayer->m_BiquadType) != -1) {
        biquadType = static_cast<s8>(pPlayer->m_BiquadType);
        biquadValue = pPlayer->m_BiquadValue;
    }

    const f32 mainSend = (m_MainSend * SEND_STEP - 1.0f) + pPlayer->m_MainSend;
    f32 fxSend[2];
    for (int i = 0; i < 2; ++i) {
        fxSend[i] = pPlayer->GetFxSend(static_cast<AuxBus>(i)) + m_FxSend[i] * SEND_STEP;
    }

    for (Channel* pChannel = m_pChannelList; pChannel != nullptr; pChannel = pChannel->m_pNextLink) {
        pChannel->m_UserVolume = volumeRatio;
        pChannel->m_UserPitch = pitchBend;
        pChannel->m_UserPitchRatio = pitchRatio;
        pChannel->m_UserPan = pan;
        pChannel->m_UserSurroundPan = surroundPan;
        pChannel->m_UserLpfFreq = lpfFreq;
        pChannel->SetBiquadFilter(biquadType, biquadValue);
        pChannel->m_MainSend = mainSend;
        pChannel->m_FxSends[0] = fxSend[0];
        pChannel->m_FxSends[1] = fxSend[1];
        for (int i = 0; i < LFO_COUNT; ++i) {
            pChannel->m_Lfos[i].m_Param = m_LfoParam[i];
            pChannel->m_LfoTargets[i] = static_cast<Channel::LfoTarget>(m_LfoTarget[i]);
        }
    }
}

// 0x004CC290 | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceTrack::ChannelCallbackFunc(Channel* pChannel, Channel::ChannelCallbackStatus status, u32 userData)
{
    SequenceTrack* pTrack = reinterpret_cast<SequenceTrack*>(userData);
    if (status == Channel::CALLBACK_STATUS_STOPPED || status == Channel::CALLBACK_STATUS_FINISH) {
        Channel::FreeChannel(pChannel);
    }
    if (pTrack->m_pSequenceSoundPlayer != nullptr) {
        pTrack->m_pSequenceSoundPlayer->ChannelCallback(pChannel);
    }

    // the channel leaves the list
    if (pTrack->m_pChannelList == pChannel) {
        pTrack->m_pChannelList = pChannel->m_pNextLink;
        return;
    }
    for (Channel* pPrev = pTrack->m_pChannelList; pPrev->m_pNextLink != nullptr; pPrev = pPrev->m_pNextLink) {
        if (pPrev->m_pNextLink == pChannel) {
            pPrev->m_pNextLink = pChannel->m_pNextLink;
            return;
        }
    }
}

// 0x004CC30C | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceTrack::UpdateChannelLength()
{
    if (!m_OpenFlag) {
        return;
    }
    for (Channel* pChannel = m_pChannelList; pChannel != nullptr; pChannel = pChannel->m_pNextLink) {
        if (pChannel->m_Length > 0) {
            --pChannel->m_Length;
        }
        if (pChannel->m_Length == 0 && pChannel->m_CurveAdshr.GetStatus() != CurveAdshr::STATUS_RELEASE && !m_DamperFlag) {
            pChannel->NoteOff();
        }
        if (!pChannel->m_IsAutoSweep) {
            pChannel->UpdateSweep(1);
        }
    }
}

// 0x004CC388 | nintendogs:callseq-callee [tier A]
void nw::snd::internal::driver::SequenceTrack::Open()
{
    m_NoteWaitFlag = false;
    m_CallStackDepth = 0;
    m_Wait = 0;
    m_OpenFlag = true;
}

// 0x004CC3A4 | nintendogs:callseq-callee [tier A]
void nw::snd::internal::driver::SequenceTrack::Close()
{
    ReleaseAllChannel(-1);
    FreeAllChannel();
    m_OpenFlag = false;
}

// 0x004CC408 | nintendogs:callgraph [tier A]
nw::snd::internal::driver::Channel* nw::snd::internal::driver::SequenceTrack::NoteOn(int key, int velocity, s32 length, bool tieFlag)
{
    const int channelVelocity = m_VelocityRange * velocity / 127;
    Channel* pChannel = nullptr;

    // a tie: the channel of the last note goes on with the new key
    if (tieFlag && (pChannel = m_pChannelList) != nullptr) {
        pChannel->m_Key = key;
        pChannel->m_Velocity = Bank::CalcChannelVelocityVolume(channelVelocity);
    }

    bool isNewChannel = false;
    if (m_MonophonicFlag) {
        pChannel = m_pChannelList;
        if (pChannel != nullptr) {
            if (pChannel->m_CurveAdshr.GetStatus() == CurveAdshr::STATUS_RELEASE) {
                pChannel->Stop();
                isNewChannel = true;
            } else {
                pChannel->m_Key = key;
                pChannel->m_Velocity = Bank::CalcChannelVelocityVolume(channelVelocity);
                pChannel->m_Length = length;
            }
        } else {
            isNewChannel = true;
        }
    } else if (pChannel == nullptr) {
        isNewChannel = true;
    }

    if (isNewChannel) {
        // a new channel from the bank
        NoteOnInfo info = {0, 0, 0, 0, 0, 0, ChannelCallbackFunc, 0};
        info.prgNo = m_PrgNo;
        info.key = key;
        info.velocity = channelVelocity;
        info.length = tieFlag ? -1 : length;
        info.initPan = m_InitPan;
        info.priority = m_pSequenceSoundPlayer->m_Priority + m_Priority;
        info.channelCallbackData = reinterpret_cast<u32>(this);
        pChannel = m_pSequenceSoundPlayer->NoteOn(m_BankIndex, info);
        if (pChannel == nullptr) {
            return nullptr;
        }
        // the other notes of the key group are released
        if (pChannel->m_KeyGroup != 0) {
            for (Channel* pOther = m_pChannelList; pOther != nullptr; pOther = pOther->m_pNextLink) {
                if (pOther->m_KeyGroup == pChannel->m_KeyGroup) {
                    pOther->m_CurveAdshr.SetRelease(KEY_GROUP_RELEASE);
                    pOther->Release();
                }
            }
        }
        pChannel->m_pNextLink = m_pChannelList;
        m_pChannelList = pChannel;
    }

    // the envelope of the track (above 127: from the bank)
    if (m_Attack <= 127) {
        pChannel->m_CurveAdshr.SetAttack(m_Attack);
    }
    if (m_Decay <= 127) {
        pChannel->m_CurveAdshr.SetDecay(m_Decay);
    }
    if (m_Sustain <= 127) {
        pChannel->m_CurveAdshr.SetSustain(m_Sustain);
    }
    if (m_Release <= 127) {
        pChannel->m_CurveAdshr.SetRelease(m_Release);
    }
    if (m_Hold <= 127) {
        pChannel->m_CurveAdshr.SetHold(m_Hold);
    }

    // the portamento
    f32 sweepPitch = m_SweepPitch;
    if (m_PortaFlag) {
        sweepPitch += m_PortaKey - key;
    }
    if (m_PortaTime != 0) {
        const f32 time = static_cast<f32>(m_PortaTime * m_PortaTime);
        const int sweepTime = static_cast<int>((sweepPitch < 0.0f ? -sweepPitch : sweepPitch) * time) >> 5;
        pChannel->SetSweepParam(sweepPitch, sweepTime * Channel::UPDATE_INTERVAL_MSEC, true);
    } else {
        pChannel->SetSweepParam(sweepPitch, length, false);
    }
    m_PortaKey = key;

    pChannel->m_SilenceVolume.SetTarget(m_SilenceFlag ? 0 : SILENCE_VOLUME_MAX, 0);
    pChannel->m_IsReleasePriorityFix = m_pSequenceSoundPlayer->m_IsReleasePriorityFix;
    pChannel->m_pVoice->SetPanMode(m_pSequenceSoundPlayer->m_PanMode);
    pChannel->m_pVoice->SetPanCurve(m_pSequenceSoundPlayer->m_PanCurve);
    pChannel->m_pVoice->SetFrontBypass(m_FrontBypassFlag);
    return pChannel;
}

// 0x004CC718 (name is ours)
void nw::snd::internal::driver::SequenceTrack::SetPan(f32 pan)
{
    m_ExtPan = pan;
}

// 0x004CC720 | nintendogs:callseq-callee [tier A]
void nw::snd::internal::driver::SequenceTrack::SetMute(SeqMute mute)
{
    switch (mute) {
    case SEQ_MUTE_OFF:
        m_MuteFlag = false;
        break;
    case SEQ_MUTE_NO_STOP:
        m_MuteFlag = true;
        break;
    case SEQ_MUTE_RELEASE:
        ReleaseAllChannel(-1);
        FreeAllChannel();
        m_MuteFlag = true;
        break;
    case SEQ_MUTE_STOP:
        for (Channel* pChannel = m_pChannelList; pChannel != nullptr; pChannel = pChannel->m_pNextLink) {
            Channel::FreeChannel(pChannel);
            pChannel->Stop();
        }
        m_pChannelList = nullptr;
        m_MuteFlag = true;
        break;
    default:
        break;
    }
}

// 0x004CC7E4 (name is ours)
void nw::snd::internal::driver::SequenceTrack::SetPitch(f32 pitch)
{
    m_ExtPitch = pitch;
}

// 0x004CC7EC | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceTrack::InitParam()
{
    m_ExtVolume = 1.0f;
    m_ExtPitch = 1.0f;
    m_PanRange = 1.0f;
    m_CmpFlag = true;
    m_WaitByNoteLengthFlag = true;
    m_ExtPan = 0.0f;
    m_ExtSurroundPan = 0.0f;
    m_pSeqBase = nullptr;
    m_pCurrentAddr = nullptr;
    m_TieFlag = false;
    m_MonophonicFlag = false;
    m_CallStackDepth = 0;
    m_Wait = 0;
    m_MuteFlag = false;
    m_SilenceFlag = false;
    m_NoteWaitFlag = false;
    m_PortaFlag = false;
    m_DamperFlag = false;
    m_BankIndex = 0;
    m_PrgNo = 0;
    for (int i = 0; i < LFO_COUNT; ++i) {
        m_LfoParam[i].Initialize();
        m_LfoTarget[i] = Channel::LFO_TARGET_PITCH;
    }
    m_SweepPitch = 0.0f;
    m_Volume.InitValue(127);
    m_Volume2.InitValue(127);
    m_Pan.InitValue(0);
    m_SurroundPan.InitValue(0);
    m_VelocityRange = 127;
    m_PitchBend.InitValue(0);
    m_BendRange = 2;
    m_InitPan = 0;
    m_Transpose = 0;
    m_Priority = 64;
    m_PortaKey = 60;
    m_PortaTime = 0;
    m_Attack = 255;
    m_Decay = 255;
    m_Sustain = 255;
    m_Release = 255;
    m_Hold = 255;
    m_MainSend = 127;
    m_FxSend[0] = 0;
    m_FxSend[1] = 0;
    m_LpfFreq = 0.0f;
    m_BiquadType = 0;
    m_BiquadValue = 0.0f;
    for (int i = 0; i < VARIABLE_COUNT; ++i) {
        m_TrackVariable[i] = -1;
    }
}

// 0x004CC964 (name is ours)
void nw::snd::internal::driver::SequenceTrack::SetVolume(f32 volume)
{
    m_ExtVolume = volume;
}

// 0x004CC96C | fefates:bytes [tier B]
nw::snd::internal::driver::SequenceTrack::SequenceTrack() : m_OpenFlag(false), m_pSequenceSoundPlayer(nullptr), m_pChannelList(nullptr)
{
    InitParam();
}

// 0x004CCA8C slot 0x00 | fefates:bytes
// 0x004CCA0C (deleting dtor)
nw::snd::internal::driver::SequenceTrack::~SequenceTrack()
{
    Close();
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
