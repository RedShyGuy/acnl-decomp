#include "nw/snd/internal/driver/snd_StreamSoundPlayer.h"

#include <cstring>

#include "nn/snd/CTR/CTR_Api.h"
#include "nw/snd/internal/driver/snd_Voice.h"
#include "nw/snd/internal/driver/snd_VoiceManager.h"
#include "nw/snd/internal/snd_WaveFileReader.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
namespace {
// the steps of the parameters of the tracks (pan 63 to the side, the volume and the sends 127)
const f32 VOLUME_STEP = 1.0f / 127;
const f32 PAN_STEP = 1.0f / 63;
const f32 SURROUND_PAN_STEP_HIGH = 1.0f / 64;
const f32 LPF_STEP = 1.0f / 64;
const f32 SEND_STEP = 1.0f / 127;

// the next id of a header request
// 0x0097F684 (name is ours)
u16 s_StreamRequestIdCounter = 0;
} // namespace

// 0x004D2324 slot 0x00 | fefates:bytes
// 0x004D230C (deleting dtor)
// 0x004D231C (thunk)
// 0x007789A4 (deleting thunk)
nw::snd::internal::driver::StreamSoundPlayer::~StreamSoundPlayer()
{
    Finalize();
}

// 0x004D05D0 slot 0x08 | fefates:bytes
void nw::snd::internal::driver::StreamSoundPlayer::Initialize()
{
    BasicSoundPlayer::Initialize();
    m_IsInitialized = false;
    m_IsPrepared = false;
    m_IsPausing = false;
    m_IsLoadingDelayState = false;
    m_IsLoadFinished = false;
    m_IsFinalizing = false;
    m_IsPrefetch = false;
    m_IsLoopFromStart = false;
    m_LoopCounter = 0;
    m_PrefetchOffset = 0;
    m_StreamPitch = 1.0f;
    m_StreamMainSend = 1.0f;
    m_StreamFxSend[0] = 0.0f;
    m_StreamFxSend[1] = 0.0f;
    for (u32 i = 0; i < TRACK_MAX; i++) {
        StreamTrack& track = m_Tracks[i];
        track.m_IsActive = false;
        track.m_ChannelCount = 0;
        track.m_Volume = 127;
        track.m_Pan = 64;
        track.m_UserVolume = 1.0f;
        track.m_UserPan = 0.0f;
        track.m_UserSurroundPan = 0.0f;
    }
    for (u32 i = 0; i < CHANNEL_MAX; i++) {
        m_Channels[i].m_pBuffer = nullptr;
        m_Channels[i].m_pVoice = nullptr;
    }
}

// 0x004D0694 | fefates:bytes [tier B]
bool nw::snd::internal::driver::StreamSoundPlayer::LoadHeader(bool result, nn::snd::CTR::AdpcmParam** ppAdpcmParams,
                                                             u16 requestId)
{
    if (!m_IsInitialized) {
        return false;
    }
    if (m_RequestId != requestId) {
        return false;
    }
    if (!result) {
        m_IsFinished = true;
        Stop();
        return false;
    }

    m_ChannelCount = m_DataInfo.channelCount;
    if (m_ChannelCount > CHANNEL_MAX) {
        m_ChannelCount = CHANNEL_MAX;
    }
    for (u32 i = 0; i < m_ChannelCount; i++) {
        void* pBuffer = m_pBufferPool->Alloc();
        if (pBuffer == nullptr) {
            for (u32 j = 0; j < i; j++) {
                m_pBufferPool->Free(m_Channels[j].m_pBuffer);
                m_Channels[j].m_pBuffer = nullptr;
            }
            Finalize();
            m_Unknown0x10 = true;
            return false;
        }
        m_Channels[i].m_pBuffer = pBuffer;
    }
    if (!ApplyStreamDataInfo(m_DataInfo)) {
        return false;
    }

    if (!m_IsPrefetch) {
        if (!SetupBufferBlocks()) {
            return false;
        }
        if (!AllocVoices()) {
            FreeBuffers();
            return false;
        }
        m_PrepareCounter = 0;
        for (u32 i = 0; i < m_BufferBlockCount; i++) {
            RequestLoadData();
            m_PrepareCounter++;
        }
    }

    for (u32 i = 0; i < m_TrackCount; i++) {
        StreamTrack& track = m_Tracks[i];
        for (int j = 0; j < track.m_ChannelCount; j++) {
            MultiVoice* pVoice = track.m_pChannels[j]->m_pVoice;
            switch (track.m_Flags) {
            case 0:
                pVoice->SetFrontBypass(false);
                pVoice->SetSurroundPreProcessed(false);
                break;
            case 1:
                pVoice->SetFrontBypass(true);
                pVoice->SetSurroundPreProcessed(false);
                break;
            case 2:
                pVoice->SetFrontBypass(false);
                pVoice->SetSurroundPreProcessed(true);
                break;
            }
        }
    }

    for (u32 i = 0; i < m_ChannelCount; i++) {
        if (ppAdpcmParams[i] != nullptr) {
            m_Channels[i].m_pVoice->SetAdpcmParam(0, *ppAdpcmParams[i]);
        }
    }
    return true;
}

// 0x004D0B74 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::SetTrackPan(u32 trackBitFlag, f32 pan)
{
    for (u32 i = 0; i < m_TrackCount && trackBitFlag != 0; i++, trackBitFlag >>= 1) {
        if (trackBitFlag & 1) {
            m_Tracks[i].m_UserPan = pan;
        }
    }
}

// 0x004D0BB4 slot 0x20
// 0x0077899C (thunk)
void nw::snd::internal::driver::StreamSoundPlayer::OnUpdateBuffer()
{
    UpdateBuffer();
}

// 0x004D0BB8 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::UpdateBuffer()
{
    if (!m_IsStarted) {
        return;
    }
    if (!m_Tracks[0].m_IsActive) {
        return;
    }
    MultiVoice* pVoice = m_Channels[0].m_pVoice;
    if (pVoice == nullptr) {
        return;
    }
    if (!pVoice->IsRun()) {
        return;
    }

    // the played blocks are loaded again
    while (m_Channels[0].m_WaveBuffers[m_PlayingBufferBlockIndex].m_Status == nn::snd::CTR::WaveBuffer::STATUS_DONE) {
        nn::snd::CTR::InitializeWaveBuffer(&m_Channels[0].m_WaveBuffers[m_PlayingBufferBlockIndex]);
        m_LoopCounter += m_BlockInfos[m_PlayingBufferBlockIndex].loopCount;
        RequestLoadData();
        if (++m_PlayingBufferBlockIndex >= m_BufferBlockCount) {
            m_PlayingBufferBlockIndex = 0;
        }
    }

    if (!m_IsLoadingDelayState && m_pLoader->IsBusy()) {
        bool isPlaying = false;
        for (u32 i = 0; i < m_BufferBlockCount; i++) {
            const u8 status = m_Channels[0].m_WaveBuffers[i].m_Status;
            if (status == nn::snd::CTR::WaveBuffer::STATUS_PLAY || status == nn::snd::CTR::WaveBuffer::STATUS_WAIT) {
                isPlaying = true;
                break;
            }
        }
        if (!isPlaying) {
            // the loader is late: the voices wait for it
            m_IsLoadingDelay = true;
            m_IsLoadingDelayState = true;
            UpdatePauseStatus();
            return;
        }
    }

    u32 position = m_BlockInfos[m_PlayingBufferBlockIndex].samplePosition + pVoice->GetPlayPosition();
    m_PlaySamplePosition = position;
    if (m_DataInfo.isLoop && position > m_DataInfo.originalLoopEnd) {
        const u32 loopStart = m_DataInfo.originalLoopStart;
        const u32 loopLength = m_DataInfo.originalLoopEnd - loopStart;
        // the samples of 2048 bytes in units of 8 bytes
        u32 frameSamples;
        switch (m_DataInfo.sampleFormat) {
        case SAMPLE_FORMAT_PCM_S8:
            frameSamples = 8;
            break;
        case SAMPLE_FORMAT_PCM_S16:
            frameSamples = 4;
            break;
        default:
            frameSamples = 14;
            break;
        }
        if (loopLength >= frameSamples * 2048) {
            position = position - m_DataInfo.originalLoopEnd + loopStart;
        } else {
            position = (position - loopStart) % loopLength + loopStart;
        }
    }
    m_OriginalPlaySamplePosition = position;
}

// 0x004D0EB8 | fefates:bytes [tier B]
bool nw::snd::internal::driver::StreamSoundPlayer::LoadStreamData(bool result, const LoadDataParam& param)
{
    return LoadStreamData(result, param, false, 0, 0);
}

// 0x004D0ED8 | fefates:bytes [tier B]
bool nw::snd::internal::driver::StreamSoundPlayer::LoadStreamData(bool result, const LoadDataParam& param,
                                                                 bool isPrefetch, u32 prefetchBlockIndex,
                                                                 u32 prefetchBlockSize)
{
    if (!m_IsInitialized) {
        return false;
    }
    if (!result) {
        m_IsFinished = true;
        Stop();
        return false;
    }

    if (param.sampleCount != 0) {
        for (u32 i = 0; i < m_ChannelCount; i++) {
            const u8* pData;
            if (isPrefetch) {
                pData = static_cast<const u8*>(m_PrefetchDataInfo.pPrefetchSample) +
                        (m_DataInfo.blockSize * m_ChannelCount * prefetchBlockIndex + prefetchBlockSize * i);
            } else {
                pData = static_cast<const u8*>(m_Channels[i].m_pBuffer) + m_DataInfo.blockSize * param.blockIndex;
            }
            u32 sampleCount = param.sampleCount;
            if (param.sampleOffset != 0) {
                pData += Voice::FrameToByte(param.sampleOffset, m_DataInfo.sampleFormat);
                sampleCount -= param.sampleOffset;
            }

            StreamChannel& channel = m_Channels[i];
            nn::snd::CTR::WaveBuffer* pWaveBuffer = &channel.m_WaveBuffers[param.blockIndex];
            nn::snd::CTR::AdpcmContext* pAdpcmContext = nullptr;
            if (param.isAdpcmContextValid) {
                pAdpcmContext = &channel.m_AdpcmContexts[param.blockIndex];
                *pAdpcmContext = param.adpcmContext[i];
            }
            nn::snd::CTR::InitializeWaveBuffer(pWaveBuffer);
            pWaveBuffer->m_AdpcmContext = pAdpcmContext;
            pWaveBuffer->m_SampleLength = sampleCount;
            pWaveBuffer->m_BufferAddress = pData;
            channel.AppendWaveBuffer(pWaveBuffer, param.isLastBlock);
        }
    }

    m_BlockInfos[param.blockIndex].samplePosition = param.samplePosition;
    m_BlockInfos[param.blockIndex].loopCount = param.loopCount;
    if (param.isLastBlock) {
        m_IsLoadFinished = true;
    }
    if (!m_IsPrepared) {
        if (isPrefetch || --m_PrepareCounter == 0 || m_IsLoadFinished) {
            m_IsPrepared = true;
        }
    }
    return true;
}

// 0x004D10A4 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::SetTrackVolume(u32 trackBitFlag, f32 volume)
{
    for (u32 i = 0; i < m_TrackCount && trackBitFlag != 0; i++, trackBitFlag >>= 1) {
        if (trackBitFlag & 1) {
            m_Tracks[i].m_UserVolume = volume;
        }
    }
}

// 0x004D10E4 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::PreparePrefetch(const PreparePrefetchArg& arg)
{
    if (!m_IsInitialized) {
        return;
    }
    m_pPrefetchData = arg.pPrefetchData;
    StreamSoundPrefetchFileReader reader;
    reader.Initialize(m_pPrefetchData);
    StreamSoundPrefetchFileReader::PrefetchDataInfo prefetchDataInfo;
    if (!reader.ReadPrefetchDataInfo(&prefetchDataInfo, 0)) {
        return;
    }
    m_PrefetchDataInfo = prefetchDataInfo;

    StreamSoundFile::StreamSoundInfo info;
    reader.ReadStreamSoundInfo(&info);
    m_DataInfo.channelCount = reader.m_pInfoBlockBody->GetChannelInfoTable()->table.count;
    m_DataInfo.SetStreamSoundInfo(info);
    m_ChannelCount = m_DataInfo.channelCount;
    if (m_ChannelCount > CHANNEL_MAX) {
        m_ChannelCount = CHANNEL_MAX;
    }
    m_StartOffsetType = arg.baseArg.startOffsetType;
    m_StartOffset = arg.baseArg.startOffset;
    m_IsActive = true;
    if (!ApplyStreamDataInfo(m_DataInfo)) {
        return;
    }
    if (!SetupBufferBlocks()) {
        return;
    }
    if (!AllocVoices()) {
        FreeBuffers();
        return;
    }
    m_IsPrefetch = true;
    LoadPrefetchBlocks(reader);
}

// 0x004D1318 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::UpdateVoiceParams(StreamTrack* pTrack)
{
    if (!pTrack->m_IsActive) {
        return;
    }

    // the parameters of the track
    f32 volume = pTrack->m_Volume * VOLUME_STEP;
    f32 lpfFreq = pTrack->m_LpfFreq * LPF_STEP;
    int biquadType = pTrack->m_BiquadType;
    f32 biquadValue = pTrack->m_BiquadValue * SEND_STEP;
    f32 pan;
    if (pTrack->m_Pan <= 1) {
        pan = static_cast<int>(pTrack->m_Pan - 63) * PAN_STEP;
    } else {
        pan = static_cast<int>(pTrack->m_Pan - 64) * PAN_STEP;
    }
    f32 surroundPan;
    if (pTrack->m_SurroundPan <= 63) {
        surroundPan = pTrack->m_SurroundPan * PAN_STEP;
    } else {
        surroundPan = static_cast<u32>(pTrack->m_SurroundPan + 1) * SURROUND_PAN_STEP_HIGH;
    }
    f32 mainSend = pTrack->m_MainSend * SEND_STEP - 1.0f;
    f32 fileFxSends[2];
    fileFxSends[0] = pTrack->m_FxSend[0] * SEND_STEP;
    fileFxSends[1] = pTrack->m_FxSend[1] * SEND_STEP;

    // with the parameters of the player
    volume = pTrack->m_UserVolume * (volume * m_Volume);
    const f32 pitch = m_StreamPitch * m_Pitch;
    lpfFreq = m_LpfFreq + lpfFreq;
    pan = pTrack->m_UserPan + (pan + m_Pan);
    surroundPan = pTrack->m_UserSurroundPan + (surroundPan + m_SurroundPan);
    if (static_cast<s8>(m_BiquadType) != -1) {
        biquadValue = m_BiquadValue;
        biquadType = static_cast<s8>(m_BiquadType);
    }
    mainSend = mainSend + (m_StreamMainSend + m_MainSend);
    f32 fxSends[2];
    for (int i = 0; i < 2; i++) {
        fxSends[i] = 0.0f;
        fxSends[i] += GetFxSend(static_cast<AuxBus>(i));
        fxSends[i] += m_StreamFxSend[i];
        fxSends[i] += fileFxSends[i];
    }

    for (int i = 0; i < pTrack->m_ChannelCount; i++) {
        MultiVoice* pVoice = pTrack->m_pChannels[i]->m_pVoice;
        if (pVoice == nullptr) {
            continue;
        }
        pVoice->SetVolume(volume);
        pVoice->SetPitch(pitch);
        pVoice->SetLpfFreq(lpfFreq);
        pVoice->SetBiquadFilter(biquadType, biquadValue);
        pVoice->SetMainSend(mainSend);
        for (int j = 0; j < 2; j++) {
            pVoice->SetFxSend(static_cast<AuxBus>(j), fxSends[j]);
        }
        switch (pTrack->m_ChannelCount) {
        case 1:
            pVoice->SetPan(pan);
            break;
        case 2: {
            // stereo: the channels to the sides
            f32 channelPan = pan;
            if (i == 0) {
                channelPan = pan - 1.0f;
            } else if (i == 1) {
                channelPan = pan + 1.0f;
            }
            pVoice->SetPan(channelPan);
            break;
        }
        }
        pVoice->SetSurroundPan(surroundPan);
    }
}

// 0x004D1618 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::VoiceCallbackFunc(MultiVoice* pVoice,
                                                                    MultiVoice::VoiceCallbackStatus status, void* pArg)
{
    StreamChannel* pChannel = static_cast<StreamChannel*>(pArg);
    switch (status) {
    case MultiVoice::STATUS_FINISH_WAVE:
    case MultiVoice::STATUS_CANCEL:
        pVoice->Free();
        pChannel->m_pVoice = nullptr;
        break;
    case MultiVoice::STATUS_DROP_VOICE:
    case MultiVoice::STATUS_DROP_DSP:
        pChannel->m_pVoice = nullptr;
        break;
    }
}

// 0x004D1650 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::ApplyTrackDataInfo(const StreamDataInfoDetail& info)
{
    for (u32 i = 0; i < m_TrackCount; i++) {
        const StreamDataInfoDetail::TrackInfo& trackInfo = info.trackInfo[i];
        StreamTrack& track = m_Tracks[i];
        track.m_Volume = trackInfo.volume;
        track.m_Pan = trackInfo.pan;
        track.m_SurroundPan = trackInfo.span;
        track.m_MainSend = trackInfo.mainSend;
        track.m_FxSend[0] = trackInfo.fxSend[0];
        track.m_FxSend[1] = trackInfo.fxSend[1];
        track.m_LpfFreq = trackInfo.lpfFreq;
        track.m_BiquadType = trackInfo.biquadType;
        track.m_BiquadValue = trackInfo.biquadValue;
        track.m_Flags = trackInfo.flags;
        track.m_ChannelCount = trackInfo.channelCount;
        for (int j = 0; j < trackInfo.channelCount; j++) {
            track.m_pChannels[j] = &m_Channels[trackInfo.globalChannelIndex[j]];
        }
    }
}

// 0x004D1774 | fefates:bytes [tier B]
bool nw::snd::internal::driver::StreamSoundPlayer::LoadPrefetchBlocks(StreamSoundPrefetchFileReader& reader)
{
    const u32 blockSampleCount = m_DataInfo.blockSampleCount;
    const u32 lastBlockIndex = (m_DataInfo.frameCount - 1) / blockSampleCount;
    const u32 loopStartOffset = m_DataInfo.loopStart % blockSampleCount;
    const u32 loopStartBlockIndex = (m_DataInfo.loopStart - loopStartOffset) / blockSampleCount;
    const u32 loopBlockCount = lastBlockIndex - loopStartBlockIndex + 1;

    for (u32 i = 0; i < m_BufferBlockCount; i++) {
        LoadDataParam param;
        u32 prefetchBlockIndex = 0;
        u32 prefetchBlockSize = 0;
        u32 loopIndex = 0;
        if (i > lastBlockIndex) {
            loopIndex = (i - lastBlockIndex) % loopBlockCount;
        }

        if (lastBlockIndex == 0 || lastBlockIndex == i || (i > lastBlockIndex && loopIndex == 0)) {
            // the last block
            param.sampleCount = m_DataInfo.lastBlockSampleCount;
            prefetchBlockIndex = lastBlockIndex;
            prefetchBlockSize = m_DataInfo.lastBlockSize;
            if (m_DataInfo.isLoop) {
                param.loopCount = 1;
                m_PrefetchOffset = m_DataInfo.loopStart;
            } else {
                param.isLastBlock = true;
                m_PrefetchOffset = 0;
            }
        } else if (!m_DataInfo.isLoop || i <= lastBlockIndex) {
            // a block before the end
            param.sampleCount = m_DataInfo.blockSampleCount;
            prefetchBlockIndex = i;
            prefetchBlockSize = m_DataInfo.blockSize;
            m_PrefetchOffset += m_DataInfo.blockSampleCount;
            if (m_DataInfo.sampleFormat == SAMPLE_FORMAT_DSP_ADPCM && prefetchBlockIndex == 0) {
                for (u32 j = 0; j < m_DataInfo.channelCount; j++) {
                    DspAdpcmParam adpcmParam;
                    DspAdpcmLoopParam adpcmLoopParam;
                    if (!reader.ReadDspAdpcmChannelInfo(&adpcmParam, &adpcmLoopParam, j)) {
                        return false;
                    }
                    std::memcpy(&m_AdpcmParams[j], adpcmParam.coef, sizeof(m_AdpcmParams[j]));
                    m_Channels[j].m_pVoice->SetAdpcmParam(0, m_AdpcmParams[j]);
                    param.adpcmContext[j].m_PredScale = adpcmParam.predScale;
                    param.adpcmContext[j].m_History[0] = adpcmParam.yn1;
                    param.adpcmContext[j].m_History[1] = adpcmParam.yn2;
                }
                param.isAdpcmContextValid = true;
            }
        } else {
            // the loop after the end
            param.sampleCount = m_DataInfo.blockSampleCount;
            if (loopIndex == 1) {
                // the block of the loop start
                prefetchBlockSize = m_DataInfo.blockSize;
                prefetchBlockIndex = loopStartBlockIndex;
                param.samplePosition = loopStartOffset;
                m_PrefetchOffset = m_DataInfo.blockSampleCount * (loopStartBlockIndex + 1);
                if (m_DataInfo.sampleFormat == SAMPLE_FORMAT_DSP_ADPCM) {
                    for (u32 j = 0; j < m_DataInfo.channelCount; j++) {
                        DspAdpcmParam adpcmParam;
                        DspAdpcmLoopParam adpcmLoopParam;
                        if (!reader.ReadDspAdpcmChannelInfo(&adpcmParam, &adpcmLoopParam, j)) {
                            return false;
                        }
                        std::memcpy(&m_AdpcmParams[j], adpcmParam.coef, sizeof(m_AdpcmParams[j]));
                        m_Channels[j].m_pVoice->SetAdpcmParam(0, m_AdpcmParams[j]);
                        param.adpcmContext[j].m_PredScale = adpcmLoopParam.loopPredScale;
                        param.adpcmContext[j].m_History[0] = adpcmLoopParam.loopYn1;
                        param.adpcmContext[j].m_History[1] = adpcmLoopParam.loopYn2;
                    }
                    param.isAdpcmContextValid = true;
                }
            } else {
                prefetchBlockIndex = loopStartBlockIndex + (loopIndex - 1);
                prefetchBlockSize = m_DataInfo.blockSize;
                m_PrefetchOffset += m_DataInfo.blockSampleCount;
            }
        }

        param.blockIndex = m_LoadingBufferBlockIndex;
        param.sampleOffset = 0;
        LoadStreamData(true, param, true, prefetchBlockIndex, prefetchBlockSize);
        if (++m_LoadingBufferBlockIndex >= m_BufferBlockCount) {
            m_LoadingBufferBlockIndex = 0;
        }
        if (param.isLastBlock) {
            break;
        }
    }
    return true;
}

// 0x004D1B14 | fefates:bytes [tier B]
bool nw::snd::internal::driver::StreamSoundPlayer::ApplyStreamDataInfo(const StreamDataInfoDetail& info)
{
    if (!info.isLoop) {
        if (GetStartOffsetSamples(info.sampleRate) >= info.frameCount) {
            m_IsFinished = true;
            Stop();
            return false;
        }
    }
    ApplyTrackDataInfo(info);
    return true;
}

// 0x004D1BE4 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::SetTrackSurroundPan(u32 trackBitFlag, f32 surroundPan)
{
    for (u32 i = 0; i < m_TrackCount && trackBitFlag != 0; i++, trackBitFlag >>= 1) {
        if (trackBitFlag & 1) {
            m_Tracks[i].m_UserSurroundPan = surroundPan;
        }
    }
}

// 0x004D1C24 slot 0x24
// 0x00778988 (thunk)
void nw::snd::internal::driver::StreamSoundPlayer::OnShutdown()
{
    Stop();
}

// 0x004D1C30 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::SetTrackInitialVolume(u32 trackBitFlag, u32 volume)
{
    for (u32 i = 0; i < m_TrackCount && trackBitFlag != 0; i++, trackBitFlag >>= 1) {
        if (trackBitFlag & 1) {
            m_Tracks[i].m_Volume = static_cast<u8>(volume);
        }
    }
}

// 0x004D1C74 slot 0x14 | nintendogs:callseq
void nw::snd::internal::driver::StreamSoundPlayer::Stop()
{
    m_pLoader->CancelRequest();
    for (u32 i = 0; i < m_ChannelCount; i++) {
        if (m_Channels[i].m_pVoice != nullptr) {
            m_Channels[i].m_pVoice->Stop();
        }
    }
    if (m_IsStarted) {
        SoundThread::GetInstance().UnregisterPlayerCallback(this);
        m_IsStarted = false;
    }
}

// 0x004D1CE4 slot 0x18 | fefates:bytes
void nw::snd::internal::driver::StreamSoundPlayer::Pause(bool isPause)
{
    m_IsPause = isPause;
    if (isPause) {
        m_IsLoadingDelayState = true;
    }
    UpdatePauseStatus();
}

// 0x004D1D64 | fefates:bytes [tier B]
nw::snd::internal::driver::StreamSoundPlayer::SetupResult
nw::snd::internal::driver::StreamSoundPlayer::Setup(const SetupArg& arg)
{
    m_FileType = arg.fileType;
    m_IsLoop = arg.isLoop;
    m_LoopStart = arg.loopStart;
    m_LoopEnd = arg.loopEnd;
    m_StreamPitch = arg.pitch;
    m_StreamMainSend = arg.mainSend * SEND_STEP - 1.0f;
    m_StreamFxSend[0] = arg.fxSend[0] * SEND_STEP;
    m_StreamFxSend[1] = arg.fxSend[1] * SEND_STEP;

    u32 trackCount = 0;
    for (u32 flags = arg.allocateTrackFlags; flags != 0; flags >>= 1, trackCount++) {
        if (flags & 1) {
            if (trackCount >= TRACK_MAX) {
                break;
            }
            m_Tracks[trackCount].m_IsActive = true;
        }
    }
    m_TrackCount = trackCount > TRACK_MAX ? TRACK_MAX : trackCount;
    if (m_TrackCount == 0) {
        return SETUP_ERR_NO_TRACK;
    }
    for (int i = 0; i < TRACK_MAX; i++) {
        m_DataInfo.trackInfo[i] = arg.trackInfo[i];
    }
    m_pBufferPool = arg.pBufferPool;
    m_IsInitialized = true;
    return SETUP_SUCCESS;
}

// 0x004D1E90 slot 0x10 | fefates:bytes
void nw::snd::internal::driver::StreamSoundPlayer::Start()
{
    if (m_IsStarted) {
        return;
    }
    for (u32 i = 0; i < m_TrackCount; i++) {
        StreamTrack& track = m_Tracks[i];
        if (!track.m_IsActive) {
            continue;
        }
        WaveInfo waveInfo;
        waveInfo.sampleFormat = m_DataInfo.sampleFormat;
        waveInfo.channelCount = track.m_ChannelCount;
        waveInfo.sampleRate = m_DataInfo.sampleRate;
        for (int j = 0; j < track.m_ChannelCount; j++) {
            StreamChannel* pChannel = track.m_pChannels[j];
            if (pChannel == nullptr) {
                continue;
            }
            MultiVoice* pVoice = pChannel->m_pVoice;
            if (pVoice == nullptr) {
                continue;
            }
            pVoice->Initialize(waveInfo, 0);
            pVoice->Start();
        }
    }
    UpdatePauseStatus();
    SoundThread::GetInstance().RegisterPlayerCallback(this);
    m_IsStarted = true;
}

// 0x004D1FDC slot 0x1C
// 0x00778994 (thunk)
void nw::snd::internal::driver::StreamSoundPlayer::OnUpdateFrame()
{
    Update();
}

// 0x004D1FE0 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::Update()
{
    m_pLoader->Update();
    if (m_IsStarted) {
        for (u32 i = 0; i < m_ChannelCount; i++) {
            if (m_Channels[i].m_pVoice == nullptr) {
                // a voice was dropped
                Stop();
                m_IsFinished = true;
                return;
            }
        }
        for (u32 i = 0; i < m_TrackCount; i++) {
            UpdateVoiceParams(&m_Tracks[i]);
        }
    }
    if (m_IsLoadingDelayState && !m_pLoader->IsBusy()) {
        m_IsLoadingDelayState = false;
        UpdatePauseStatus();
    }
    if (m_IsLoadingDelay) {
        m_IsLoadingDelay = false;
    }
}

// 0x004D211C | fefates:bytes [tier B]
bool nw::snd::internal::driver::StreamSoundPlayer::Prepare(const PrepareArg& arg)
{
    if (!m_IsInitialized || arg.pLoader == nullptr) {
        return false;
    }
    m_pLoader = arg.pLoader;
    m_StartOffsetType = arg.baseArg.startOffsetType;
    m_StartOffset = arg.baseArg.startOffset;
    m_IsActive = true;
    m_IsLoadingDelay = false;
    m_RequestId = s_StreamRequestIdCounter++;
    m_pLoader->m_RegionCallback = arg.baseArg.regionCallback;
    m_pLoader->m_RegionCallbackArg = arg.baseArg.regionCallbackArg;
    m_pLoader->m_pPlayer = this;
    m_pLoader->m_pDataInfo = &m_DataInfo;
    m_pLoader->m_FileType = static_cast<StreamSoundLoader::FileType>(m_FileType);
    m_pLoader->m_IsLoop = m_IsLoop;
    m_pLoader->m_LoopStart = m_LoopStart;
    m_pLoader->m_LoopEnd = m_LoopEnd;
    m_pLoader->m_RequestId = m_RequestId;
    m_pLoader->RequestLoadHeader();
    return true;
}

// 0x004D21F0 slot 0x0C | fefates:bytes
void nw::snd::internal::driver::StreamSoundPlayer::Finalize()
{
    if (!m_IsInitialized) {
        return;
    }
    m_IsFinalizing = true;
    FreeBuffers();
    FreeVoices();
    m_pBufferPool = nullptr;
    m_IsActive = false;
    m_IsInitialized = false;
    BasicSoundPlayer::Finalize();
}

// 0x004D22B4 | fefates:bytes [tier B]
nw::snd::internal::driver::StreamSoundPlayer::StreamSoundPlayer()
    : m_IsLoopFromStart(false), m_pPrefetchData(nullptr), m_ChannelCount(0), m_TrackCount(0)
{
}

// 0x00741554 | fefates:bytes [tier B]
bool nw::snd::internal::driver::StreamSoundPlayer::ReadStreamDataInfo(StreamDataInfo* pInfo) const
{
    SoundThreadLock lock;
    if (!m_IsPrepared) {
        return false;
    }
    pInfo->isLoop = m_DataInfo.isLoop;
    pInfo->sampleRate = m_DataInfo.sampleRate;
    pInfo->loopStart = m_DataInfo.loopStart;
    pInfo->loopEnd = m_DataInfo.frameCount;
    return true;
}

// 0x007415C8 | fefates:bytes [tier B]
long nw::snd::internal::driver::StreamSoundPlayer::GetPlaySamplePosition(bool isOriginal) const
{
    SoundThreadLock lock;
    if (!m_IsActive || !m_Tracks[0].m_IsActive) {
        return -1;
    }
    if (!m_IsPrepared) {
        return 0;
    }
    if (isOriginal) {
        return m_OriginalPlaySamplePosition;
    }
    return m_PlaySamplePosition;
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
