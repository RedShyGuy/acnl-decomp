#include "nw/snd/internal/snd_StreamSoundFileReader.h"

#include "nw/snd/internal/snd_StreamSoundFile_ChannelInfo.h"

namespace nw {
namespace snd {
namespace internal {
namespace {
// the size of the header of a block
const u32 BLOCK_HEADER_SIZE = 8;
} // namespace

// 0x004C97A8 | fefates:bytes [tier B]
void nw::snd::internal::StreamSoundFileReader::Initialize(const void* pStreamSoundFile)
{
    if (!IsValidFileHeader(pStreamSoundFile)) {
        return;
    }
    m_pHeader = static_cast<const StreamSoundFile::FileHeader*>(pStreamSoundFile);
    m_pInfoBlockBody = reinterpret_cast<const StreamSoundFile::InfoBlockBody*>(
        static_cast<const u8*>(pStreamSoundFile) + m_pHeader->GetInfoBlockOffset() + BLOCK_HEADER_SIZE);
}

// 0x004C97FC | fefates:bytes [tier B]
nw::snd::internal::StreamSoundFileReader::StreamSoundFileReader() : m_pHeader(nullptr), m_pInfoBlockBody(nullptr)
{
}

// 0x00740A60 | fefates:bytes [tier B]
bool nw::snd::internal::StreamSoundFileReader::IsValidFileHeader(const void* pStreamSoundFile) const
{
    const StreamSoundFile::FileHeader* pHeader = static_cast<const StreamSoundFile::FileHeader*>(pStreamSoundFile);
    if (pHeader->signature != StreamSoundFile::SIGNATURE_FILE) {
        return false;
    }
    return pHeader->version - StreamSoundFile::VERSION_MIN <= StreamSoundFile::VERSION_MAX - StreamSoundFile::VERSION_MIN;
}

// 0x00740A98 | fefates:bytes [tier B]
bool nw::snd::internal::StreamSoundFileReader::ReadStreamSoundInfo(StreamSoundFile::StreamSoundInfo* pInfo) const
{
    const StreamSoundFile::StreamSoundInfo* pSrc = m_pInfoBlockBody->GetStreamSoundInfo();
    pInfo->encoding = pSrc->encoding;
    pInfo->isLoop = pSrc->isLoop;
    pInfo->channelCount = pSrc->channelCount;
    pInfo->unknown3 = pSrc->unknown3;
    pInfo->sampleRate = pSrc->sampleRate;
    pInfo->loopStart = pSrc->loopStart;
    pInfo->frameCount = pSrc->frameCount;
    pInfo->blockCount = pSrc->blockCount;
    pInfo->blockSize = pSrc->blockSize;
    pInfo->blockSampleCount = pSrc->blockSampleCount;
    pInfo->lastBlockSize = pSrc->lastBlockSize;
    pInfo->lastBlockSampleCount = pSrc->lastBlockSampleCount;
    pInfo->lastBlockPaddedSize = pSrc->lastBlockPaddedSize;
    pInfo->seekSize = pSrc->seekSize;
    pInfo->seekInterval = pSrc->seekInterval;
    pInfo->sampleDataReference = pSrc->sampleDataReference;
    pInfo->regionInfoSize = pSrc->regionInfoSize;
    pInfo->regionDataReference = pSrc->regionDataReference;
    // older files: the loop is the original one
    if (m_pHeader->version >= StreamSoundFile::VERSION_ORIGINAL_LOOP) {
        pInfo->originalLoopStart = pSrc->originalLoopStart;
        pInfo->originalLoopEnd = pSrc->originalLoopEnd;
    } else {
        pInfo->originalLoopStart = pSrc->loopStart;
        pInfo->originalLoopEnd = pSrc->frameCount;
    }
    return true;
}

// 0x00740B80 | fefates:bytes [tier B]
bool nw::snd::internal::StreamSoundFileReader::ReadStreamTrackInfo(TrackInfo* pInfo, int index) const
{
    const StreamSoundFile::TrackInfoTable* pTable = m_pInfoBlockBody->GetTrackInfoTable();
    if (pTable == nullptr) {
        return false;
    }
    if (static_cast<int>(pTable->table.count) <= index) {
        return false;
    }
    const StreamSoundFile::TrackInfo* pTrack = pTable->GetTrackInfo(index);
    pInfo->volume = pTrack->volume;
    pInfo->pan = pTrack->pan;
    pInfo->unknown2 = pTrack->unknown2;
    pInfo->unknown3 = pTrack->unknown3;
    pInfo->channelCount = static_cast<const BinaryTable<u8>*>(GetReferedPointer(pTrack, pTrack->globalChannelIndexTable))->count;
    const u8 count = pInfo->channelCount > TrackInfo::CHANNEL_COUNT_MAX ? TrackInfo::CHANNEL_COUNT_MAX : pInfo->channelCount;
    for (u8 i = 0; i < count; ++i) {
        pInfo->globalChannelIndex[i] = static_cast<const BinaryTable<u8>*>(GetReferedPointer(pTrack, pTrack->globalChannelIndexTable))->items[i];
    }
    return true;
}

// 0x00740C24 | fefates:bytes [tier B]
bool nw::snd::internal::StreamSoundFileReader::IsTrackInfoAvailable() const
{
    return m_pHeader->version <= StreamSoundFile::VERSION_TRACK_INFO;
}

// 0x00740C44 | fefates:bytes [tier B]
bool nw::snd::internal::StreamSoundFileReader::ReadDspAdpcmChannelInfo(DspAdpcmParam* pParam, DspAdpcmLoopParam* pLoopParam, int channelIndex) const
{
    const StreamSoundFile::DspAdpcmChannelInfo* pInfo =
        m_pInfoBlockBody->GetChannelInfoTable()->GetChannelInfo(channelIndex)->GetDspAdpcmChannelInfo();
    if (pInfo == nullptr) {
        return false;
    }
    *pParam = pInfo->param;
    *pLoopParam = pInfo->loopParam;
    return true;
}

} // namespace internal
} // namespace snd
} // namespace nw
