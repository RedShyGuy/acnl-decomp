#include "nw/snd/internal/snd_StreamSoundFileLoader.h"

#include <cstring>

#include "nw/snd/internal/snd_StreamSoundFileReader.h"
#include "nw/snd/internal/snd_WaveFileReader.h"
#include "nw/ut/ut_Api.h"

namespace nw {
namespace snd {
namespace internal {
namespace {
// the size of the header of a block
const u32 BLOCK_HEADER_SIZE = 8;
// the alignment of the buffers the file system reads into
const u32 READ_ALIGNMENT = 32;
// the seek data of a block (the history of up to 8 channels)
const u32 SEEK_BLOCK_READ_SIZE = 32;

// the buffer ReadSeekBlockData reads into
// 0x00AF63A0
u16 s_SeekBlockBuffer[SEEK_BLOCK_READ_SIZE / sizeof(u16)] DECOMP_ALIGN(32);
} // namespace

// 0x004C9534 | fefates:bytes [tier B]
bool nw::snd::internal::StreamSoundFileLoader::LoadFileHeader(StreamSoundFileReader* pReader, void* pBuffer, u32 bufferSize)
{
    u8 headerBuffer[HEADER_READ_SIZE + READ_ALIGNMENT];
    void* pHeader = ut::RoundUp(headerBuffer, READ_ALIGNMENT);
    m_pStream->Seek(0, io::FileStream::SEEK_ORIGIN_BEGIN);
    if (m_pStream->Read(pHeader, HEADER_READ_SIZE) != static_cast<s32>(HEADER_READ_SIZE)) {
        return false;
    }
    if (!pReader->IsValidFileHeader(pHeader)) {
        return false;
    }
    const StreamSoundFile::FileHeader* pFileHeader = static_cast<const StreamSoundFile::FileHeader*>(pHeader);
    const u32 loadSize = pFileHeader->GetInfoBlockOffset() + pFileHeader->GetInfoBlockSize();
    if (loadSize > bufferSize) {
        return false;
    }
    m_pStream->Seek(0, io::FileStream::SEEK_ORIGIN_BEGIN);
    if (m_pStream->Read(pBuffer, loadSize) != static_cast<s32>(loadSize)) {
        return false;
    }
    pReader->Initialize(pBuffer);

    if (pReader->m_pHeader != nullptr && pReader->m_pHeader->HasRegionBlock()) {
        const u32 regionDataOffset = pReader->m_pInfoBlockBody->GetStreamSoundInfo()->regionDataReference.offset;
        m_RegionDataOffset = pReader->m_pHeader->GetRegionBlockOffset() + BLOCK_HEADER_SIZE + regionDataOffset;
    } else {
        m_RegionDataOffset = 0;
    }
    m_RegionInfoSize = pReader->m_pInfoBlockBody->GetStreamSoundInfo()->regionInfoSize;

    StreamSoundFile::StreamSoundInfo info;
    if (!pReader->ReadStreamSoundInfo(&info)) {
        return false;
    }
    if (WaveFileReader::GetSampleFormat(info.encoding) == SAMPLE_FORMAT_DSP_ADPCM) {
        if (pReader->m_pHeader != nullptr && pReader->m_pHeader->HasSeekBlock()) {
            m_SeekBlockOffset = pReader->m_pHeader->GetSeekBlockOffset();
        } else {
            m_SeekBlockOffset = 0;
        }
    }
    return true;
}

// 0x004C96D8 | fefates:bytes [tier B]
bool nw::snd::internal::StreamSoundFileLoader::ReadSeekBlockData(u16* pYn1, u16* pYn2, int blockIndex, int channelCount)
{
    const u32 offset = m_SeekBlockOffset + BLOCK_HEADER_SIZE + blockIndex * (channelCount * sizeof(u16) * 2);
    m_pStream->Seek(offset, io::FileStream::SEEK_ORIGIN_BEGIN);
    if (m_pStream->Read(s_SeekBlockBuffer, SEEK_BLOCK_READ_SIZE) != static_cast<s32>(SEEK_BLOCK_READ_SIZE)) {
        return false;
    }
    for (int i = 0; i < channelCount; ++i) {
        pYn1[i] = s_SeekBlockBuffer[i * 2];
        pYn2[i] = s_SeekBlockBuffer[i * 2 + 1];
    }
    return true;
}

// 0x007409E8 | fefates:bytes [tier B]
bool nw::snd::internal::StreamSoundFileLoader::ReadRegionInfo(StreamSoundFile::RegionInfo* pInfo, u32 index) const
{
    u8 buffer[sizeof(StreamSoundFile::RegionInfo) + READ_ALIGNMENT];
    m_pStream->Seek(m_RegionDataOffset + m_RegionInfoSize * index, io::FileStream::SEEK_ORIGIN_BEGIN);
    void* pRegionInfo = ut::RoundUp(buffer, READ_ALIGNMENT);
    if (m_pStream->Read(pRegionInfo, sizeof(StreamSoundFile::RegionInfo)) != static_cast<s32>(sizeof(StreamSoundFile::RegionInfo))) {
        return false;
    }
    std::memcpy(pInfo, pRegionInfo, sizeof(StreamSoundFile::RegionInfo));
    return true;
}

} // namespace internal
} // namespace snd
} // namespace nw
