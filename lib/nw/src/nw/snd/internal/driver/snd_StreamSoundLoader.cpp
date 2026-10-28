#include "nw/snd/internal/driver/snd_StreamSoundLoader.h"

#include <cstring>
#include <new>

#include "nn/snd/CTR/CTR_Api.h"
#include "nw/snd/internal/Util/Util_Api.h"
#include "nw/snd/internal/driver/snd_StreamSoundPlayer.h"
#include "nw/snd/internal/driver/snd_Voice.h"
#include "nw/snd/internal/snd_DriverCommandPlayer.h"
#include "nw/snd/internal/snd_StreamSoundFileReader.h"
#include "nw/snd/internal/snd_WaveFileReader.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x0097F680 (name is ours)
ExternalStreamFileReader* g_pExternalStreamFileReader = nullptr;

namespace {
// the blocks of two channels, read by the tasks of all loaders
// 0x00AF0D00 (name is ours)
u8 s_StreamLoadBuffer[StreamSoundLoader::LOAD_BUFFER_SIZE];
} // namespace

// 0x004C6858 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundLoader::RequestLoadHeader()
{
    m_RegionIndex = 0;
    m_HeaderLoadTask.m_Id = reinterpret_cast<u32>(this);
    m_HeaderLoadTask.m_pLoader = this;
    m_FileLoader.m_pStream = m_pFileStream;
    TaskManager::GetInstance().AppendTask(&m_HeaderLoadTask, TaskManager::PRIORITY_MIDDLE);
}

// 0x004CEC48 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundLoader::Initialize()
{
    WaitFinalize();
    m_LoadingBlockIndex = 0;
    m_LastBlockIndex = 0xFFFFFFFF;
    m_LoopStartBlockIndex = 0;
    m_LoopStartFilePosition = 0;
    m_LoopStartInBlockOffset = 0;
    m_IsLoopJumped = false;
    m_IsLoadEnd = false;
    m_IsLoopFromStart = false;
    m_SampleFormat = SAMPLE_FORMAT_DSP_ADPCM;
    m_RegionCallback = nullptr;
    m_AdpcmContextPosition = 0xFFFFFFFF;
    m_RegionCallbackArg = nullptr;
}

// 0x004CEC90 | fefates:bytes [tier B]
bool nw::snd::internal::driver::StreamSoundLoader::LoadHeader1(DriverCommandStreamSoundLoadHeader* pCommand)
{
    StreamSoundFileReader reader;
    if (!m_FileLoader.LoadFileHeader(&reader, s_StreamLoadBuffer, LOAD_BUFFER_SIZE)) {
        return false;
    }
    pCommand->player = m_pPlayer;
    pCommand->requestId = m_RequestId;

    StreamSoundFile::StreamSoundInfo info;
    if (!reader.ReadStreamSoundInfo(&info)) {
        return false;
    }
    const u32 channelCount = reader.m_pInfoBlockBody->GetChannelInfoTable()->table.count;
    m_ChannelCount = channelCount;
    m_pDataInfo->channelCount = channelCount;
    m_pDataInfo->SetStreamSoundInfo(info);
    if (reader.IsTrackInfoAvailable()) {
        if (!ReadTrackInfoFromStreamSoundFile(reader)) {
            return false;
        }
    }

    m_SampleFormat = m_pDataInfo->sampleFormat;
    if (m_SampleFormat == SAMPLE_FORMAT_DSP_ADPCM) {
        for (u32 i = 0; i < channelCount; i++) {
            DspAdpcmParam param;
            DspAdpcmLoopParam loopParam;
            if (!reader.ReadDspAdpcmChannelInfo(&param, &loopParam, i)) {
                return false;
            }
            AdpcmInfo& adpcmInfo = m_AdpcmInfos[i];
            std::memcpy(&adpcmInfo.param, param.coef, sizeof(adpcmInfo.param));
            adpcmInfo.context.m_PredScale = param.predScale;
            adpcmInfo.context.m_History[0] = param.yn1;
            adpcmInfo.context.m_History[1] = param.yn2;
            adpcmInfo.loopContext.m_PredScale = loopParam.loopPredScale;
            adpcmInfo.loopContext.m_History[0] = loopParam.loopYn1;
            adpcmInfo.loopContext.m_History[1] = loopParam.loopYn2;
            pCommand->adpcmParams[i] = &adpcmInfo.param;
        }
    } else {
        for (u32 i = 0; i < channelCount; i++) {
            pCommand->adpcmParams[i] = nullptr;
        }
    }

    const u32 dataOffset = reader.GetSampleDataOffset();
    m_DataOffset = dataOffset;
    const StreamDataInfoDetail* pInfo = m_pDataInfo;
    m_LastBlockIndex = (pInfo->frameCount - 1) / pInfo->blockSampleCount;
    m_LoopStartBlockIndex = pInfo->loopStart / pInfo->blockSampleCount;
    m_LoopStartFilePosition = dataOffset + channelCount * (pInfo->blockSize * m_LoopStartBlockIndex);
    m_LoopStartInBlockOffset = 0;
    m_pFileStream->Seek(dataOffset, io::FileStream::SEEK_ORIGIN_BEGIN);

    if (m_RegionCallback != nullptr) {
        u32 regionIndex = 0;
        if (!m_RegionCallback(&regionIndex, m_RegionCallbackArg)) {
            return false;
        }
        m_RegionIndex = regionIndex;
        ChangeRegionInfo(regionIndex);
    } else {
        m_RegionStart = 0;
        m_RegionEnd = m_pDataInfo->frameCount;
    }
    m_Position = m_RegionStart;
    SeekToPositionBlock();
    return true;
}

// 0x004CEFB0 | fefates:bytes [tier B]
bool nw::snd::internal::driver::StreamSoundLoader::LoadHeader2(DriverCommandStreamSoundLoadHeader* pCommand)
{
    if (g_pExternalStreamFileReader == nullptr) {
        return false;
    }
    pCommand->player = m_pPlayer;
    pCommand->requestId = m_RequestId;

    ExternalStreamFileReader::HeaderInfo header;
    if (!g_pExternalStreamFileReader->ReadHeader(m_pFileStream, &header)) {
        return false;
    }
    const u32 channelCount = header.channelCount;
    m_ChannelCount = channelCount;
    m_pDataInfo->channelCount = channelCount;
    m_pDataInfo->sampleFormat = SAMPLE_FORMAT_PCM_S16;
    m_pDataInfo->sampleRate = header.sampleRate;
    m_pDataInfo->isLoop = m_IsLoop;
    m_pDataInfo->loopStart = m_LoopStart;
    m_pDataInfo->frameCount = m_LoopEnd;
    m_pDataInfo->originalLoopStart = m_LoopStart;
    m_pDataInfo->originalLoopEnd = m_LoopEnd;
    m_pDataInfo->blockSampleCount = header.blockSampleCount;
    m_pDataInfo->blockSize = EXTERNAL_BLOCK_SIZE;
    m_pDataInfo->lastBlockSize = EXTERNAL_BLOCK_SIZE;

    StreamDataInfoDetail* pInfo = m_pDataInfo;
    m_LastBlockIndex = (pInfo->frameCount - 1) / pInfo->blockSampleCount;
    if (pInfo->isLoop) {
        pInfo->lastBlockSampleCount = pInfo->frameCount - pInfo->blockSampleCount * m_LastBlockIndex;
    } else {
        pInfo->lastBlockSampleCount = EXTERNAL_LAST_BLOCK_SAMPLE_COUNT;
    }

    for (u32 i = 0; i < channelCount; i++) {
        pCommand->adpcmParams[i] = nullptr;
    }

    pInfo = m_pDataInfo;
    m_LoopStartBlockIndex = pInfo->loopStart / pInfo->blockSampleCount;
    m_LoopStartInBlockOffset = pInfo->loopStart - m_LoopStartBlockIndex * pInfo->blockSampleCount;

    if (m_RegionCallback != nullptr) {
        u32 regionIndex = 0;
        if (!m_RegionCallback(&regionIndex, m_RegionCallbackArg)) {
            return false;
        }
        m_RegionIndex = regionIndex;
        ChangeRegionInfo(regionIndex);
        m_Position = m_RegionStart;
    } else {
        m_RegionStart = 0;
        m_Position = 0;
        m_RegionEnd = m_pDataInfo->frameCount;
    }
    return true;
}

// 0x004CF194 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundLoader::WaitFinalize()
{
    m_HeaderLoadTask.m_Event.Wait();
    for (ut::internal::LinkListImpl::iterator it = m_DataLoadTaskList.begin(); it != m_DataLoadTaskList.end();) {
        StreamDataLoadTask* pTask = StreamDataLoadTaskList::GetPointerFromNode(it.GetNode());
        ++it;
        pTask->m_Event.Wait();
        m_DataLoadTaskList.Erase(pTask);
        if (pTask != nullptr) {
            pTask->~StreamDataLoadTask();
            m_DataLoadTaskPool.FreeImpl(pTask);
        }
    }
    if (m_pFileStream != nullptr) {
        m_pFileStream->Close();
        m_pFileStream = nullptr;
    }
}

// 0x004CF22C | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundLoader::CancelRequest()
{
    TaskManager::GetInstance().RemoveTaskById(reinterpret_cast<u32>(this));
}

// 0x004CF244 | fefates:bytes
void nw::snd::internal::driver::StreamSoundLoader::RequestLoadData(void** buffers, u32 blockIndex, u32 startOffset,
                                                                  u32 prefetchOffset, TaskManager::TaskPriority priority,
                                                                  s8 isLoopFromStart)
{
    void* pMemory = m_DataLoadTaskPool.AllocImpl();
    StreamDataLoadTask* pTask = pMemory != nullptr ? new (pMemory) StreamDataLoadTask() : nullptr;
    pTask->m_pLoader = this;
    pTask->m_BlockIndex = blockIndex;
    pTask->m_StartOffset = startOffset;
    pTask->m_PrefetchOffset = prefetchOffset;
    pTask->m_IsLoopFromStart = isLoopFromStart;
    pTask->m_Id = reinterpret_cast<u32>(this);
    for (u32 i = 0; i < m_ChannelCount; i++) {
        pTask->m_Buffers[i] = buffers[i];
    }
    m_DataLoadTaskList.PushBack(pTask);
    TaskManager::GetInstance().AppendTask(pTask, priority);
}

// 0x004CF358 | fefates:bytes [tier B]
bool nw::snd::internal::driver::StreamSoundLoader::ApplyStartOffset(u32 offset, u32* pLoopCount)
{
    while (m_Position + offset >= m_RegionEnd) {
        offset -= m_RegionEnd - m_Position;
        if (!MoveToNextRegion()) {
            return false;
        }
        (*pLoopCount)++;
    }
    m_Position += offset;
    return true;
}

// 0x004CF490 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundLoader::ChangeRegionInfo(u32 regionIndex)
{
    StreamSoundFile::RegionInfo regionInfo;
    if (m_FileLoader.ReadRegionInfo(&regionInfo, regionIndex)) {
        m_RegionStart = regionInfo.start;
        m_RegionEnd = regionInfo.end;
        if (m_SampleFormat == SAMPLE_FORMAT_DSP_ADPCM) {
            for (u32 i = 0; i < m_ChannelCount; i++) {
                m_AdpcmContexts[i].m_PredScale = regionInfo.adpcmContext[i].loopPredScale;
                m_AdpcmContexts[i].m_History[0] = regionInfo.adpcmContext[i].loopYn1;
                m_AdpcmContexts[i].m_History[1] = regionInfo.adpcmContext[i].loopYn2;
            }
            m_AdpcmContextPosition = regionInfo.start;
        }
    } else {
        m_RegionStart = 0;
        m_RegionEnd = m_pDataInfo->frameCount;
    }
}

// 0x004CF728 | fefates:bytes [tier B]
bool nw::snd::internal::driver::StreamSoundLoader::ReadTrackInfoFromStreamSoundFile(StreamSoundFileReader& reader)
{
    u32 trackCount = reader.m_pInfoBlockBody->GetTrackInfoTable()->table.count;
    if (trackCount > StreamDataInfoDetail::TRACK_MAX) {
        trackCount = StreamDataInfoDetail::TRACK_MAX;
    }
    for (u32 i = 0; i < trackCount; i++) {
        StreamSoundFileReader::TrackInfo trackInfo = {};
        if (!reader.ReadStreamTrackInfo(&trackInfo, i)) {
            return false;
        }
        m_pDataInfo->trackInfo[i].volume = trackInfo.volume;
        m_pDataInfo->trackInfo[i].pan = trackInfo.pan;
        m_pDataInfo->trackInfo[i].span = trackInfo.unknown2;
        m_pDataInfo->trackInfo[i].flags = trackInfo.unknown3;
        m_pDataInfo->trackInfo[i].channelCount = trackInfo.channelCount;
        for (int j = 0; j < trackInfo.channelCount; j++) {
            m_pDataInfo->trackInfo[i].globalChannelIndex[j] = trackInfo.globalChannelIndex[j];
        }
        m_pDataInfo->trackInfo[i].mainSend = 127;
        m_pDataInfo->trackInfo[i].fxSend[0] = 0;
        m_pDataInfo->trackInfo[i].fxSend[1] = 0;
        m_pDataInfo->trackInfo[i].lpfFreq = 64;
        m_pDataInfo->trackInfo[i].biquadType = 0;
        m_pDataInfo->trackInfo[i].biquadValue = 0;
    }
    return true;
}

// 0x004CF88C | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundLoader::Update()
{
    for (ut::internal::LinkListImpl::iterator it = m_DataLoadTaskList.begin(); it != m_DataLoadTaskList.end();) {
        StreamDataLoadTask* pTask = StreamDataLoadTaskList::GetPointerFromNode(it.GetNode());
        ++it;
        if (pTask->m_Status != Task::STATUS_DONE) {
            break;
        }
        m_DataLoadTaskList.Erase(pTask);
        if (pTask != nullptr) {
            pTask->~StreamDataLoadTask();
            m_DataLoadTaskPool.FreeImpl(pTask);
        }
    }
}

// 0x004CF8F8 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundLoader::Finalize()
{
    TaskManager::GetInstance().RemoveTaskById(reinterpret_cast<u32>(this));
}

// 0x004CF910 | fefates:bytes [tier B]
bool nw::snd::internal::driver::StreamSoundLoader::LoadData1(DriverCommandStreamSoundLoadData* pCommand, void** buffers,
                                                            u32 blockIndex, u32 startOffset, u32 prefetchOffset)
{
    LoadDataParam& param = pCommand->param;
    u32 loopCount = 0;
    u32 sampleOffset = 0;
    bool isSeekAdpcm = false;
    if (startOffset != 0 || prefetchOffset != 0) {
        if (!ApplyStartOffset(startOffset + prefetchOffset, &loopCount)) {
            param.sampleCount = 0;
            return true;
        }
        SeekToPositionBlock();
        if (m_SampleFormat == SAMPLE_FORMAT_DSP_ADPCM) {
            isSeekAdpcm = true;
        }
    }

    u32 sampleCount = 0;
    u32 dataOffset = 0;
    bool isFirstBlock = prefetchOffset == 0;
    const u32 samplePosition = m_Position;
    do {
        u32 blockSampleCount;
        u32 blockSize;
        if (m_LoadingBlockIndex == m_LastBlockIndex) {
            blockSize = m_pDataInfo->lastBlockSize;
            blockSampleCount = m_pDataInfo->lastBlockSampleCount;
        } else {
            blockSampleCount = m_pDataInfo->blockSampleCount;
            blockSize = m_pDataInfo->blockSize;
        }
        // the samples before the position: whole ADPCM frames are skipped, the rest by the context
        const u32 skipSamples = m_Position % m_pDataInfo->blockSampleCount;
        u32 skipFrameSamples = skipSamples;
        if (m_SampleFormat == SAMPLE_FORMAT_DSP_ADPCM) {
            skipFrameSamples = skipSamples / 14 * 14;
        }
        const u32 skipSize = Util::GetByteBySample(skipFrameSamples, m_SampleFormat);
        u32 loadSamples = blockSampleCount - skipSamples;
        if (m_Position + loadSamples > m_RegionEnd) {
            loadSamples = m_RegionEnd - m_Position;
        }
        const u32 copySize = blockSize - skipSize;

        if (isFirstBlock && isSeekAdpcm) {
            // the ADPCM history at the start of the block
            const u32 position = m_pFileStream->Tell();
            u16 yn1[CHANNEL_MAX];
            u16 yn2[CHANNEL_MAX];
            if (!m_FileLoader.ReadSeekBlockData(yn1, yn2, m_LoadingBlockIndex, m_ChannelCount)) {
                return false;
            }
            m_pFileStream->Seek(position, io::FileStream::SEEK_ORIGIN_BEGIN);
            for (u32 i = 0; i < m_ChannelCount; i++) {
                m_AdpcmContexts[i].m_History[0] = yn1[i];
                m_AdpcmContexts[i].m_History[1] = yn2[i];
            }
        }

        // two channels per read
        const bool isCalcAdpcmContext = isSeekAdpcm && isFirstBlock;
        for (u32 channel = 0; channel < m_ChannelCount;) {
            if (m_pPlayer->m_IsFinalizing) {
                return false;
            }
            u32 readCount = 2;
            if (m_ChannelCount < channel + 2) {
                readCount = m_ChannelCount - channel;
            }
            const u32 readSize = blockSize * readCount;
            if (static_cast<u32>(m_pFileStream->Read(s_StreamLoadBuffer, readSize)) != readSize) {
                return false;
            }
            for (u32 i = 0; i < readCount; i++, channel++) {
                const u8* pBlock = s_StreamLoadBuffer + blockSize * i;
                void* pDest = static_cast<u8*>(buffers[channel]) + dataOffset;
                std::memcpy(pDest, pBlock + skipSize, copySize);
                nn::snd::CTR::FlushDataCache(reinterpret_cast<uptr>(pDest), copySize);
                if (isCalcAdpcmContext) {
                    m_AdpcmContexts[channel].m_PredScale = *pBlock;
                    Voice::CalcOffsetAdpcmParam(&m_AdpcmContexts[channel], m_AdpcmInfos[channel].param, skipFrameSamples,
                                                pBlock);
                    m_AdpcmContextPosition = m_Position;
                }
            }
        }

        sampleCount += loadSamples;
        m_LoadingBlockIndex++;
        m_Position += loadSamples;
        dataOffset += copySize;
        if (m_Position == m_RegionEnd) {
            if (MoveToNextRegion()) {
                loopCount++;
                SeekToPositionBlock();
            }
            break;
        }
        isFirstBlock = false;
    } while (sampleCount == 0);

    // the ADPCM state at the start of the samples, if known
    param.isAdpcmContextValid = false;
    if (m_SampleFormat == SAMPLE_FORMAT_DSP_ADPCM) {
        if (samplePosition == 0) {
            for (u32 i = 0; i < m_ChannelCount; i++) {
                param.adpcmContext[i] = m_AdpcmInfos[i].context;
            }
            param.isAdpcmContextValid = true;
        } else if (m_pDataInfo->isLoop && m_pDataInfo->loopStart == samplePosition) {
            for (u32 i = 0; i < m_ChannelCount; i++) {
                param.adpcmContext[i] = m_AdpcmInfos[i].loopContext;
            }
            param.isAdpcmContextValid = true;
        } else if (m_AdpcmContextPosition == samplePosition) {
            for (u32 i = 0; i < m_ChannelCount; i++) {
                param.adpcmContext[i] = m_AdpcmContexts[i];
            }
            param.isAdpcmContextValid = true;
        }
    }
    param.blockIndex = blockIndex;
    param.sampleCount = sampleCount;
    param.samplePosition = samplePosition;
    param.sampleOffset = sampleOffset;
    param.loopCount = loopCount;
    param.isLastBlock = m_IsLoadEnd;
    return true;
}

// 0x004D00EC | fefates:bytes [tier B]
bool nw::snd::internal::driver::StreamSoundLoader::LoadData2(DriverCommandStreamSoundLoadData* pCommand, void** buffers,
                                                            u32 blockIndex, u32 startOffset, u32 /*prefetchOffset*/)
{
    LoadDataParam& param = pCommand->param;
    u32 loopCount = 0;
    u32 sampleOffset = 0;

    // past the last block of a loop: back to the loop start
    if (m_LoadingBlockIndex > m_LastBlockIndex && m_pDataInfo->isLoop) {
        m_LoadingBlockIndex = m_LoopStartBlockIndex;
        m_pFileStream->Seek(m_LoopStartFilePosition, io::FileStream::SEEK_ORIGIN_BEGIN);
        if (m_LoopStartBlockIndex != 0) {
            // the block before the loop start again (the reader continues from it)
            void* channelBuffers[CHANNEL_MAX];
            for (u32 i = 0; i < m_ChannelCount; i++) {
                channelBuffers[i] = buffers[i];
            }
            if (!g_pExternalStreamFileReader->ReadBlock(m_pFileStream, m_ChannelCount, channelBuffers)) {
                return false;
            }
        }
        m_Position = m_pDataInfo->loopStart;
        m_IsLoopJumped = true;
    }

    if (startOffset != 0) {
        if (!ApplyStartOffset(startOffset, &loopCount)) {
            param.sampleCount = 0;
            return true;
        }
        SkipToPositionBlock();
        sampleOffset = m_Position - m_LoadingBlockIndex * m_pDataInfo->blockSampleCount;
    } else if (m_IsLoopJumped) {
        sampleOffset = m_LoopStartInBlockOffset;
    }
    UpdateLoopStartFilePosition();

    const u32 samplePosition = m_Position;
    u32 sampleCount;
    if (m_LoadingBlockIndex != m_LastBlockIndex) {
        sampleCount = m_pDataInfo->blockSampleCount;
    } else {
        sampleCount = m_pDataInfo->lastBlockSampleCount;
    }
    if (m_pFileStream->Tell() >= m_pFileStream->GetSize()) {
        sampleCount = 0;
    } else if (sampleCount != 0) {
        void* channelBuffers[CHANNEL_MAX];
        for (u32 i = 0; i < m_ChannelCount; i++) {
            channelBuffers[i] = buffers[i];
        }
        if (!g_pExternalStreamFileReader->ReadBlock(m_pFileStream, m_ChannelCount, channelBuffers)) {
            return false;
        }
        if (m_pFileStream->Tell() >= m_pFileStream->GetSize()) {
            m_LastBlockIndex = m_LoadingBlockIndex;
        }
    }

    m_IsLoopJumped = false;
    m_LoadingBlockIndex++;
    m_Position += sampleCount;
    if (m_LoadingBlockIndex > m_LastBlockIndex) {
        if (m_IsLoopFromStart) {
            m_Position = 0;
            SkipToPositionBlock();
        } else if (!m_pDataInfo->isLoop) {
            m_IsLoadEnd = true;
        }
    }

    param.isAdpcmContextValid = false;
    param.blockIndex = blockIndex;
    param.sampleCount = sampleCount;
    param.samplePosition = samplePosition;
    param.sampleOffset = sampleOffset;
    param.loopCount = loopCount;
    param.isLastBlock = m_IsLoadEnd;
    return true;
}

// 0x004D0510 (symbols.json: StreamSoundPlayer::StreamSoundPlayer)
nw::snd::internal::driver::StreamSoundLoader::StreamSoundLoader()
    : m_pPlayer(nullptr), m_pFileStream(nullptr), m_Position(0), m_RegionStart(0), m_RegionEnd(0)
{
    m_FileLoader.m_pStream = nullptr;
    m_DataLoadTaskPool.CreateImpl(m_DataLoadTasks, sizeof(m_DataLoadTasks), sizeof(StreamDataLoadTask));
}

// 0x004D058C | fefates:bytes [tier B]
nw::snd::internal::driver::StreamSoundLoader::~StreamSoundLoader()
{
    WaitFinalize();
    m_DataLoadTaskPool.DestroyImpl(m_DataLoadTasks, sizeof(m_DataLoadTasks));
}

// 0x00741534 | fefates:bytes [tier B]
bool nw::snd::internal::driver::StreamSoundLoader::IsBusy() const
{
    if (m_IsLoadEnd) {
        return false;
    }
    return !m_DataLoadTaskList.empty();
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
