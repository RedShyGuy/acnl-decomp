#include "nw/snd/internal/snd_StreamSoundPrefetchFileReader.h"

#include "nw/snd/internal/snd_StreamSoundFile_ChannelInfo.h"

namespace nw {
namespace snd {
namespace internal {
// 0x004C9A08 | fefates:bytes [tier B]
void nw::snd::internal::StreamSoundPrefetchFileReader::Initialize(const void* pPrefetchFile)
{
    const StreamSoundPrefetchFile::FileHeader* pHeader = static_cast<const StreamSoundPrefetchFile::FileHeader*>(pPrefetchFile);
    if (pHeader->signature != StreamSoundPrefetchFile::SIGNATURE_FILE) {
        return;
    }
    if (pHeader->version - StreamSoundPrefetchFile::VERSION_MIN > StreamSoundPrefetchFile::VERSION_MAX - StreamSoundPrefetchFile::VERSION_MIN) {
        return;
    }
    m_pHeader = pHeader;
    const ut::BinaryBlockHeader* pInfoBlock = static_cast<const ut::BinaryBlockHeader*>(pHeader->GetInfoBlock());
    if (pInfoBlock->kind != StreamSoundPrefetchFile::SIGNATURE_INFO_BLOCK) {
        return;
    }
    const ut::BinaryBlockHeader* pPrefetchDataBlock = static_cast<const ut::BinaryBlockHeader*>(m_pHeader->GetPrefetchDataBlock());
    if (pPrefetchDataBlock->kind != StreamSoundPrefetchFile::SIGNATURE_PREFETCH_DATA_BLOCK) {
        return;
    }
    m_pInfoBlockBody = reinterpret_cast<const StreamSoundFile::InfoBlockBody*>(pInfoBlock + 1);
    m_pPrefetchDataBlockBody = reinterpret_cast<const BinaryTable<StreamSoundPrefetchFile::PrefetchData>*>(pPrefetchDataBlock + 1);
}

// 0x004C9A90 | fefates:bytes [tier B]
nw::snd::internal::StreamSoundPrefetchFileReader::StreamSoundPrefetchFileReader()
    : m_pHeader(nullptr), m_pInfoBlockBody(nullptr), m_pPrefetchDataBlockBody(nullptr)
{
}

// 0x0074135C | fefates:bytes [tier B]
bool nw::snd::internal::StreamSoundPrefetchFileReader::ReadStreamSoundInfo(StreamSoundFile::StreamSoundInfo* pInfo) const
{
    *pInfo = *m_pInfoBlockBody->GetStreamSoundInfo();
    return true;
}

// 0x00741384 | fefates:bytes [tier B]
bool nw::snd::internal::StreamSoundPrefetchFileReader::ReadPrefetchDataInfo(PrefetchDataInfo* pInfo, int index) const
{
    const StreamSoundPrefetchFile::PrefetchData& data = m_pPrefetchDataBlockBody->items[index];
    pInfo->startFrame = data.startFrame;
    pInfo->prefetchSize = data.prefetchSize;
    pInfo->pPrefetchSample = data.GetPrefetchSample();
    return true;
}

// 0x007413C0 | fefates:bytes [tier B]
bool nw::snd::internal::StreamSoundPrefetchFileReader::ReadDspAdpcmChannelInfo(DspAdpcmParam* pParam, DspAdpcmLoopParam* pLoopParam, int channelIndex) const
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
