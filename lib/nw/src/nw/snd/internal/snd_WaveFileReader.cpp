#include "nw/snd/internal/snd_WaveFileReader.h"

#include <cstring>

#include "nw/snd/internal/snd_WaveFile_ChannelInfo.h"
#include "nw/snd/internal/snd_WaveFile_InfoBlockBody.h"

namespace nw {
namespace snd {
namespace internal {
namespace {
// the size of the header of a block
const u32 BLOCK_HEADER_SIZE = 8;
// the versions of the file the reader knows
const u32 VERSION_MIN = 0x02000000;
const u32 VERSION_MAX = 0x02010100;
} // namespace

// 0x004C81EC | nintendogs:bytes [tier A]
nw::snd::SampleFormat nw::snd::internal::WaveFileReader::GetSampleFormat(u8 encoding)
{
    switch (encoding) {
    case WaveFile::ENCODING_PCM8:
        return SAMPLE_FORMAT_PCM_S8;
    case WaveFile::ENCODING_PCM16:
        return SAMPLE_FORMAT_PCM_S16;
    default:
        return SAMPLE_FORMAT_DSP_ADPCM;
    }
}

// 0x004C8200 | fefates:bytes [tier B]
nw::snd::internal::WaveFileReader::WaveFileReader(const void* pWaveFile, s8 waveType)
    : m_pHeader(nullptr), m_pInfoBlockBody(nullptr), m_WaveType(waveType)
{
    if (waveType != 0) {
        return;
    }
    const WaveFile::FileHeader* pHeader = static_cast<const WaveFile::FileHeader*>(pWaveFile);
    if (pHeader->signature != WaveFile::SIGNATURE_FILE || pHeader->version - VERSION_MIN > VERSION_MAX - VERSION_MIN) {
        return;
    }
    m_pHeader = pHeader;
    const ut::BinaryBlockHeader* pInfoBlock = static_cast<const ut::BinaryBlockHeader*>(m_pHeader->GetInfoBlock());
    const ut::BinaryBlockHeader* pDataBlock = static_cast<const ut::BinaryBlockHeader*>(m_pHeader->GetDataBlock());
    if (pInfoBlock == nullptr || pDataBlock == nullptr) {
        return;
    }
    if (pInfoBlock->kind != WaveFile::SIGNATURE_INFO_BLOCK || pDataBlock->kind != WaveFile::SIGNATURE_DATA_BLOCK) {
        return;
    }
    m_pDataBlockBody = reinterpret_cast<const u8*>(pDataBlock) + BLOCK_HEADER_SIZE;
    m_pInfoBlockBody = reinterpret_cast<const WaveFile::InfoBlockBody*>(reinterpret_cast<const u8*>(pInfoBlock) + BLOCK_HEADER_SIZE);
}

// 0x0073F86C | nintendogs:callseq [tier A]
bool nw::snd::internal::WaveFileReader::ReadWaveInfo(WaveInfo* pInfo, const void* /*pWaveDataAddress*/) const
{
    if (m_WaveType != 0) {
        return false;
    }
    pInfo->sampleFormat = GetSampleFormat(m_pInfoBlockBody->encoding);
    pInfo->channelCount = m_pInfoBlockBody->channelInfoTable.count;
    pInfo->sampleRate = m_pInfoBlockBody->sampleRate;
    pInfo->isLoop = m_pInfoBlockBody->isLoop == 1;
    pInfo->loopStartFrame = m_pInfoBlockBody->loopStartFrame;
    pInfo->loopEndFrame = m_pInfoBlockBody->loopEndFrame;
    if (m_pHeader->version >= WaveFile::VERSION_ORIGINAL_LOOP_START) {
        pInfo->originalLoopStartFrame = m_pInfoBlockBody->originalLoopStartFrame;
    } else {
        pInfo->originalLoopStartFrame = m_pInfoBlockBody->loopStartFrame;
    }

    for (int i = 0; i < static_cast<int>(m_pInfoBlockBody->channelInfoTable.count); ++i) {
        if (i >= WaveInfo::CHANNEL_MAX) {
            continue;
        }
        WaveInfo::ChannelParam& param = pInfo->channelParam[i];
        const WaveFile::ChannelInfo* pChannelInfo = m_pInfoBlockBody->GetChannelInfo(i);
        if (pChannelInfo->dspAdpcmInfo.offset != 0) {
            const u8* pAdpcmInfo = static_cast<const u8*>(pChannelInfo->GetDspAdpcmInfo());
            std::memcpy(&param.adpcmParam, pAdpcmInfo, sizeof(DspAdpcmParam));
            std::memcpy(&param.adpcmLoopParam, pAdpcmInfo + sizeof(DspAdpcmParam), sizeof(DspAdpcmLoopParam));
        }
        param.pDataAddress = pChannelInfo->GetSamplesAddress(m_pDataBlockBody);
    }
    return true;
}

} // namespace internal
} // namespace snd
} // namespace nw
