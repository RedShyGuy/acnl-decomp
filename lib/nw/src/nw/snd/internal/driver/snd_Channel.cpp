#include "nw/snd/internal/driver/snd_Channel.h"

#include <cstring>
#include <new>

#include "nw/snd/internal/Util/Util_Api.h"
#include "nw/snd/internal/driver/snd_ChannelManager.h"
#include "nw/snd/internal/driver/snd_DisposeCallbackManager.h"
#include "nw/snd/internal/driver/snd_HardwareManager.h"
#include "nw/snd/internal/driver/snd_VoiceManager.h"
#include "nw/snd/internal/snd_WaveFileReader.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
namespace {
// the frames of a DSP ADPCM frame
const u32 ADPCM_FRAME_SAMPLES = 14;
// the default key of a note
const u8 KEY_DEFAULT = 60;
// the volume steps of the silence volume
const f32 SILENCE_VOLUME_SCALE = 1.0f / 255;
// the range of the volume LFO (dB)
const f32 LFO_VOLUME_RANGE_DB = 6.0f;
} // namespace

// 0x004CCC2C | fefates:bytes [tier B]
void nw::snd::internal::driver::Channel::VoiceCallbackFunc(MultiVoice* pVoice, MultiVoice::VoiceCallbackStatus status, void* pArg)
{
    Channel* pChannel = static_cast<Channel*>(pArg);
    ChannelCallbackStatus channelStatus = CALLBACK_STATUS_FINISH;
    switch (status) {
    case MultiVoice::STATUS_FINISH_WAVE:
        pVoice->Free();
        break;
    case MultiVoice::STATUS_CANCEL:
        channelStatus = CALLBACK_STATUS_CANCEL;
        pVoice->Free();
        break;
    case MultiVoice::STATUS_DROP_VOICE:
    case MultiVoice::STATUS_DROP_DSP:
        channelStatus = CALLBACK_STATUS_DROP;
        break;
    }
    if (pChannel->m_pCallback != nullptr) {
        pChannel->m_pCallback(pChannel, channelStatus, pChannel->m_CallbackUserData);
    }
    pChannel->m_pVoice = nullptr;
    pChannel->m_PauseFlag = false;
    pChannel->m_ActiveFlag = false;
    pChannel->m_AllocFlag = false;
    ChannelManager::GetInstance().Free(pChannel);
}

// 0x004D3C70 | nintendogs:callgraph [tier A]
void nw::snd::internal::driver::Channel::FreeChannel(Channel* pChannel)
{
    if (pChannel != nullptr) {
        pChannel->m_pCallback = nullptr;
        pChannel->m_CallbackUserData = 0;
    }
}

// 0x004D3C88 | fefates:bytes [tier B]
void nw::snd::internal::driver::Channel::SetLoopFlag(bool isLoop)
{
    if (m_pVoice == nullptr) {
        return;
    }
    const int voiceCount = m_pVoice->m_VoiceCount;
    for (int i = 0; i < voiceCount; ++i) {
        nn::snd::CTR::WaveBuffer& buffer = m_WaveBuffers[i][0];
        if (buffer.m_IsLoop != isLoop) {
            buffer.m_IsLoop = isLoop;
            m_pVoice->UpdateWaveBuffer(i, &buffer);
        }
    }
}

// 0x004D3CE4 | fefates:bytes [tier B]
void nw::snd::internal::driver::Channel::UpdateSweep(int count)
{
    m_SweepCounter += count;
    if (m_SweepCounter > m_SweepLength) {
        m_SweepCounter = m_SweepLength;
    }
}

// 0x004D3D04 | nintendogs:bytes-fuzzy [tier A]
nw::snd::internal::driver::Channel* nw::snd::internal::driver::Channel::AllocChannel(int voiceChannelCount, int priority, ChannelCallback pCallback,
                                                                                     u32 userData)
{
    Channel* pChannel = ChannelManager::GetInstance().Alloc();
    if (pChannel == nullptr) {
        return nullptr;
    }
    pChannel->m_AllocFlag = true;
    MultiVoice* pVoice = VoiceManager::GetInstance().AllocVoice(voiceChannelCount, priority, VoiceCallbackFunc, pChannel);
    if (pVoice == nullptr) {
        ChannelManager::GetInstance().Free(pChannel);
        return nullptr;
    }
    pChannel->m_pVoice = pVoice;
    pChannel->InitParam(pCallback, userData);
    return pChannel;
}

// 0x004D3D8C | fefates:bytes [tier B]
void nw::snd::internal::driver::Channel::SetSweepParam(f32 sweepPitch, int sweepTime, bool isAutoUpdate)
{
    m_SweepPitch = sweepPitch;
    m_SweepLength = sweepTime;
    m_SweepCounter = 0;
    m_IsAutoSweep = isAutoUpdate;
}

// 0x004D3DA4 | fefates:bytes [tier B]
void nw::snd::internal::driver::Channel::SetBiquadFilter(int type, f32 value)
{
    m_BiquadValue = value;
    m_BiquadType = type;
}

// 0x004D3DB0 | nintendogs:callseq [tier A]
void nw::snd::internal::driver::Channel::AppendWaveBuffer(const WaveInfo& waveInfo, u32 startOffset)
{
    m_IsLoop = waveInfo.isLoop;
    m_LoopStartFrame = waveInfo.loopStartFrame;
    m_OriginalLoopStartFrame = waveInfo.originalLoopStartFrame;
    // ADPCM starts at a frame
    if (startOffset != 0 && waveInfo.sampleFormat == SAMPLE_FORMAT_DSP_ADPCM) {
        startOffset = startOffset / ADPCM_FRAME_SAMPLES * ADPCM_FRAME_SAMPLES;
    }
    m_StartOffset = startOffset;
    const u32 loopStartByte = MultiVoice::FrameToByte(m_LoopStartFrame, waveInfo.sampleFormat);
    MultiVoice::FrameToByte(waveInfo.loopEndFrame, waveInfo.sampleFormat);

    const int voiceCount = m_pVoice->m_VoiceCount;
    for (int i = 0; i < voiceCount; ++i) {
        const WaveInfo::ChannelParam& channelParam = waveInfo.channelParam[i];
        const u8* pData = static_cast<const u8*>(channelParam.pDataAddress);
        const u8* pStart = pData;
        if (startOffset != 0) {
            pStart = pData + MultiVoice::FrameToByte(startOffset, waveInfo.sampleFormat);
        }
        nn::snd::CTR::AdpcmContext* pContext = &m_AdpcmContexts[i];
        nn::snd::CTR::AdpcmContext* pLoopContext = &m_AdpcmLoopContexts[i];
        if (waveInfo.sampleFormat == SAMPLE_FORMAT_DSP_ADPCM) {
            nn::snd::CTR::AdpcmParam param;
            std::memcpy(&param, channelParam.adpcmParam.coef, sizeof(param));
            if (startOffset != 0) {
                // the state at the start: decoded from the start of the data
                pContext->m_PredScale = *reinterpret_cast<const u16*>(pStart);
                pContext->m_History[0] = channelParam.adpcmParam.yn1;
                pContext->m_History[1] = channelParam.adpcmParam.yn2;
                MultiVoice::CalcOffsetAdpcmParam(pContext, param, startOffset, pData);
            } else {
                pContext->m_PredScale = channelParam.adpcmParam.predScale;
                pContext->m_History[0] = channelParam.adpcmParam.yn1;
                pContext->m_History[1] = channelParam.adpcmParam.yn2;
            }
            if (waveInfo.isLoop) {
                pLoopContext->m_PredScale = channelParam.adpcmLoopParam.loopPredScale;
                pLoopContext->m_History[0] = channelParam.adpcmLoopParam.loopYn1;
                pLoopContext->m_History[1] = channelParam.adpcmLoopParam.loopYn2;
            }
            m_pVoice->SetAdpcmParam(i, param);
        }

        nn::snd::CTR::WaveBuffer& startBuffer = m_WaveBuffers[i][0];
        nn::snd::CTR::InitializeWaveBuffer(&startBuffer);
        startBuffer.m_BufferAddress = pStart;
        startBuffer.m_SampleLength = waveInfo.loopEndFrame - startOffset;
        startBuffer.m_IsLoop = false;
        if (waveInfo.sampleFormat == SAMPLE_FORMAT_DSP_ADPCM) {
            startBuffer.m_AdpcmContext = pContext;
        }
        m_pVoice->AppendWaveBuffer(i, &startBuffer, !waveInfo.isLoop);

        if (waveInfo.isLoop) {
            nn::snd::CTR::WaveBuffer& loopBuffer = m_WaveBuffers[i][1];
            nn::snd::CTR::InitializeWaveBuffer(&loopBuffer);
            loopBuffer.m_BufferAddress = pData + loopStartByte;
            loopBuffer.m_SampleLength = waveInfo.loopEndFrame - m_LoopStartFrame;
            loopBuffer.m_IsLoop = true;
            if (waveInfo.sampleFormat == SAMPLE_FORMAT_DSP_ADPCM) {
                loopBuffer.m_AdpcmContext = pLoopContext;
            }
            m_pVoice->AppendWaveBuffer(i, &loopBuffer, true);
        }
    }
}

// 0x004D4018 | nintendogs:callseq [tier A]
void nw::snd::internal::driver::Channel::Stop()
{
    if (m_pVoice == nullptr) {
        return;
    }
    m_pVoice->Stop();
    m_pVoice->Free();
    m_pVoice = nullptr;
    m_PauseFlag = false;
    m_ActiveFlag = false;
    if (m_pCallback != nullptr) {
        m_pCallback(this, CALLBACK_STATUS_STOPPED, m_CallbackUserData);
    }
    if (m_AllocFlag) {
        m_AllocFlag = false;
        ChannelManager::GetInstance().Free(this);
    }
}

// 0x004D4088 | fefates:bytes [tier B]
void nw::snd::internal::driver::Channel::Start(const WaveInfo& waveInfo, int length, u32 startOffset)
{
    m_Length = length;
    for (int i = 0; i < LFO_COUNT; ++i) {
        m_Lfos[i].Reset();
    }
    m_CurveAdshr.Reset(CurveAdshr::VOLUME_INIT);
    m_SweepCounter = 0;
    m_pVoice->Initialize(waveInfo, startOffset);
    AppendWaveBuffer(waveInfo, startOffset);
    m_pVoice->SetInterpolationType(m_InterpolationType);
    m_pVoice->Start();
    m_ActiveFlag = true;
}

// 0x004D4118 | nintendogs:callseq [tier A]
void nw::snd::internal::driver::Channel::Update(bool isPeriodic)
{
    if (!m_ActiveFlag) {
        return;
    }
    if (m_PauseFlag) {
        isPeriodic = false;
    }

    // the volume
    m_SilenceVolume.Update();
    f32 volume = m_SilenceVolume.GetValue() * (m_Velocity * m_Velocity * (m_InstrumentVolume * m_UserVolume)) * SILENCE_VOLUME_SCALE;
    if (m_CurveAdshr.GetStatus() == CurveAdshr::STATUS_RELEASE && m_CurveAdshr.GetValue() < CurveAdshr::VOLUME_INIT) {
        // released to silence
        Stop();
        return;
    }

    // the pitch
    f32 sweep = 0.0f;
    if (m_SweepPitch != 0.0f && m_SweepLength > m_SweepCounter) {
        sweep = static_cast<f32>(m_SweepLength - m_SweepCounter) * m_SweepPitch / static_cast<f32>(m_SweepLength);
    }
    f32 cent = sweep + static_cast<f32>(m_Key - m_OriginalKey) + m_UserPitch;
    for (int i = 0; i < LFO_COUNT; ++i) {
        if (m_LfoTargets[i] == LFO_TARGET_PITCH) {
            cent += m_Lfos[i].GetValue();
        }
    }
    f32 pitchRatio;
    if (m_CachedCent == cent) {
        pitchRatio = m_CachedPitchRatio;
    } else {
        pitchRatio = Util::CalcPitchRatio(static_cast<int>(cent * 256.0f));
        m_CachedCent = cent;
        m_CachedPitchRatio = pitchRatio;
    }
    const f32 pitch = m_Tune * m_UserPitchRatio * pitchRatio;

    // the pan
    f32 pan = m_InitPan + m_UserPan;
    for (int i = 0; i < LFO_COUNT; ++i) {
        if (m_LfoTargets[i] == LFO_TARGET_PAN) {
            pan += m_Lfos[i].GetValue();
        }
    }
    const f32 surroundPan = m_InitSurroundPan + m_UserSurroundPan;

    if (isPeriodic) {
        if (m_IsAutoSweep) {
            UpdateSweep(UPDATE_INTERVAL_MSEC);
        }
        for (int i = 0; i < LFO_COUNT; ++i) {
            m_Lfos[i].Update(UPDATE_INTERVAL_MSEC);
        }
        m_CurveAdshr.Update(UPDATE_INTERVAL_MSEC);
    }

    const f32 lpfFreq = m_UserLpfFreq + 1.0f;
    volume *= Util::CalcVolumeRatio(m_CurveAdshr.GetValue());
    for (int i = 0; i < LFO_COUNT; ++i) {
        if (m_LfoTargets[i] == LFO_TARGET_VOLUME) {
            volume *= Util::CalcVolumeRatio(m_Lfos[i].GetValue() * LFO_VOLUME_RANGE_DB);
        }
    }

    if (m_pVoice == nullptr) {
        return;
    }
    m_pVoice->SetVolume(volume);
    m_pVoice->SetPitch(pitch);
    m_pVoice->SetPan(pan);
    m_pVoice->SetSurroundPan(surroundPan);
    m_pVoice->SetLpfFreq(lpfFreq);
    m_pVoice->SetBiquadFilter(m_BiquadType, m_BiquadValue);
    m_pVoice->SetMainSend(m_MainSend);
    for (int i = 0; i < 2; ++i) {
        m_pVoice->SetFxSend(static_cast<AuxBus>(i), m_FxSends[i]);
    }
}

// 0x004D44C4 | fefates:bytes [tier B]
void nw::snd::internal::driver::Channel::NoteOff()
{
    if (m_IsIgnoreNoteOff) {
        return;
    }
    if (m_CurveAdshr.GetStatus() != CurveAdshr::STATUS_RELEASE) {
        if (m_pVoice != nullptr && !m_IsReleasePriorityFix) {
            m_pVoice->SetPriority(MultiVoice::PRIORITY_RELEASE);
        }
        m_CurveAdshr.SetStatus(CurveAdshr::STATUS_RELEASE);
    }
    m_PauseFlag = false;
}

// 0x004D4514 | fefates:bytes [tier B]
void nw::snd::internal::driver::Channel::Release()
{
    if (m_CurveAdshr.GetStatus() != CurveAdshr::STATUS_RELEASE) {
        if (m_pVoice != nullptr && !m_IsReleasePriorityFix) {
            m_pVoice->SetPriority(MultiVoice::PRIORITY_RELEASE);
        }
        m_CurveAdshr.SetStatus(CurveAdshr::STATUS_RELEASE);
    }
    m_PauseFlag = false;
}

// 0x004D4558 slot 0x08 | nintendogs:callseq
void nw::snd::internal::driver::Channel::Disposer::InvalidateData(const void* pStart, const void* pEnd)
{
    if (m_pChannel->m_pVoice == nullptr) {
        return;
    }
    // a wave buffer in use in the memory
    bool isInvalid = false;
    const int voiceCount = m_pChannel->m_pVoice->m_VoiceCount;
    for (int i = 0; i < voiceCount; ++i) {
        for (int j = 0; j < WAVE_BUFFER_COUNT; ++j) {
            const nn::snd::CTR::WaveBuffer& buffer = m_pChannel->m_WaveBuffers[i][j];
            if (buffer.m_Status == nn::snd::CTR::WaveBuffer::STATUS_FREE || buffer.m_Status == nn::snd::CTR::WaveBuffer::STATUS_DONE) {
                continue;
            }
            const u8* pBuffer = static_cast<const u8*>(buffer.m_BufferAddress);
            const u8* pBufferEnd = pBuffer + Util::GetByteBySample(buffer.m_SampleLength, m_pChannel->m_pVoice->GetFormat());
            if (pStart > pBufferEnd || pBuffer > pEnd) {
                continue;
            }
            isInvalid = true;
            break;
        }
    }
    if (!isInvalid) {
        return;
    }
    if (m_pChannel->m_pCallback != nullptr) {
        m_pChannel->m_pCallback(m_pChannel, CALLBACK_STATUS_CANCEL, m_pChannel->m_CallbackUserData);
        m_pChannel->m_pCallback = nullptr;
    }
    m_pChannel->Stop();
}

// 0x004D46C4 slot 0x00
// 0x004D46C0 (deleting dtor)
nw::snd::internal::driver::Channel::Disposer::~Disposer()
{
}

// 0x004D46C8 | fefates:bytes [tier B]
void nw::snd::internal::driver::Channel::InitParam(ChannelCallback pCallback, u32 userData)
{
    m_Unknown0x1AC = 0;
    m_pCallback = pCallback;
    m_CallbackUserData = userData;
    m_PauseFlag = false;
    m_IsAutoSweep = true;
    m_IsReleasePriorityFix = false;
    m_IsIgnoreNoteOff = false;
    m_IsLoop = false;
    m_LoopStartFrame = 0;
    m_OriginalLoopStartFrame = 0;
    m_StartOffset = 0;
    m_Length = 0;
    m_Key = KEY_DEFAULT;
    m_OriginalKey = KEY_DEFAULT;
    m_InitPan = 0.0f;
    m_InitSurroundPan = 0.0f;
    m_Tune = 1.0f;
    m_CachedCent = 0.0f;
    m_CachedPitchRatio = 1.0f;
    m_UserVolume = 1.0f;
    m_UserPitch = 0.0f;
    m_UserPitchRatio = 1.0f;
    m_UserPan = 0.0f;
    m_UserSurroundPan = 0.0f;
    m_UserLpfFreq = 0.0f;
    m_BiquadType = 0;
    m_BiquadValue = 0.0f;
    m_MainSend = 0.0f;
    m_FxSends[0] = 0.0f;
    m_FxSends[1] = 0.0f;
    m_SilenceVolume.InitValue(255);
    m_SweepPitch = 0.0f;
    m_SweepLength = 0;
    m_SweepCounter = 0;
    m_CurveAdshr.Initialize(CurveAdshr::VOLUME_INIT);
    for (int i = 0; i < LFO_COUNT; ++i) {
        m_Lfos[i].m_Param.Initialize();
        m_LfoTargets[i] = LFO_TARGET_PITCH;
    }
    m_Unknown0x196 = 0;
    u8 interpolationType = 0;
    switch (HardwareManager::GetInstance().GetInterpolationType()) {
    case 0:
        interpolationType = 2;
        break;
    case 1:
        interpolationType = 1;
        break;
    }
    m_InterpolationType = interpolationType;
    m_InstrumentVolume = 1.0f;
    m_Velocity = 1.0f;
}

// 0x004D4800 | fefates:bytes [tier B]
nw::snd::internal::driver::Channel::Channel() : m_Disposer(this), m_PauseFlag(false), m_ActiveFlag(false), m_AllocFlag(false), m_pVoice(nullptr)
{
    DisposeCallbackManager::GetInstance().RegisterDisposeCallback(&m_Disposer);
}

// 0x004D489C | fefates:bytes [tier B]
nw::snd::internal::driver::Channel::~Channel()
{
    DisposeCallbackManager::GetInstance().UnregisterDisposeCallback(&m_Disposer);
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
