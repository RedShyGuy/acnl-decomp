#include "nw/snd/internal/snd_WaveArchiveFileReader.h"

namespace nw {
namespace snd {
namespace internal {
namespace {
// the size of the header of a block
const u32 BLOCK_HEADER_SIZE = 8;
} // namespace

// 0x0013F310 | nintendogs:callseq-callee [tier A]
const void* nw::snd::internal::WaveArchiveFileReader::SetWaveFile(u32 waveIndex, const void* pWaveFile)
{
    if (!m_IsInitialized || m_pLoadTable == nullptr) {
        return nullptr;
    }
    if (waveIndex >= m_pInfoBlockBody->count) {
        return nullptr;
    }
    const void* pOld = m_pLoadTable[waveIndex];
    m_pLoadTable[waveIndex] = pWaveFile;
    return pOld;
}

// 0x0013F344 | fefates:bytes [tier B]
nw::snd::internal::WaveArchiveFileReader::WaveArchiveFileReader(const void* pWaveArchiveFile, bool isIndividual)
    : m_pHeader(nullptr), m_pInfoBlockBody(nullptr), m_pLoadTable(nullptr), m_IsInitialized(false)
{
    Initialize(pWaveArchiveFile, isIndividual);
}

// 0x0013FE8C | nintendogs:callseq-callee [tier A]
const void* nw::snd::internal::WaveArchiveFileReader::GetWaveFile(u32 waveIndex) const
{
    if (!m_IsInitialized || waveIndex >= m_pInfoBlockBody->count) {
        return nullptr;
    }
    if (m_pLoadTable != nullptr) {
        return m_pLoadTable[waveIndex];
    }
    return static_cast<const u8*>(m_pHeader->GetFileBlock()) + BLOCK_HEADER_SIZE + m_pInfoBlockBody->items[waveIndex].offset;
}

// 0x0013FEE4 | fefates:bytes [tier B]
bool nw::snd::internal::WaveArchiveFileReader::HasIndividualLoadTable() const
{
    if (!m_IsInitialized) {
        return false;
    }
    const u32 signature = *reinterpret_cast<const u32*>(reinterpret_cast<const u8*>(m_pHeader) + m_pHeader->GetFileBlockOffset());
    return signature == WaveArchiveFile::SIGNATURE_INDIVIDUAL_LOAD_TABLE;
}

// 0x00141914 | nintendogs:callseq-callee [tier A]
void nw::snd::internal::WaveArchiveFileReader::InitializeFileTable()
{
    for (u32 i = 0; i < (m_IsInitialized ? m_pInfoBlockBody->count : 0); ++i) {
        m_pLoadTable[i] = nullptr;
    }
}

// 0x00141F30 | fefates:bytes [tier B]
u32 nw::snd::internal::WaveArchiveFileReader::GetWaveFileSize(u32 waveIndex) const
{
    if (!m_IsInitialized) {
        return 0;
    }
    return m_pInfoBlockBody->items[waveIndex].size;
}

// 0x00141F58 | fefates:bytes [tier B]
u32 nw::snd::internal::WaveArchiveFileReader::GetWaveFileOffsetFromFileHead(u32 waveIndex) const
{
    if (!m_IsInitialized) {
        return 0;
    }
    return m_pInfoBlockBody->items[waveIndex].offset + m_pHeader->GetFileBlockOffset() + BLOCK_HEADER_SIZE;
}

// 0x004C980C | fefates:bytes [tier B]
void nw::snd::internal::WaveArchiveFileReader::Initialize(const void* pWaveArchiveFile, bool isIndividual)
{
    if (pWaveArchiveFile == nullptr) {
        return;
    }
    const WaveArchiveFile::FileHeader* pHeader = static_cast<const WaveArchiveFile::FileHeader*>(pWaveArchiveFile);
    if (pHeader->signature != WaveArchiveFile::SIGNATURE_FILE || pHeader->version != WaveArchiveFile::VERSION) {
        return;
    }
    m_pHeader = pHeader;
    m_pInfoBlockBody = reinterpret_cast<const WaveArchiveFile::InfoBlockBody*>(static_cast<const u8*>(pHeader->GetInfoBlock()) + BLOCK_HEADER_SIZE);
    m_IsInitialized = true;
    m_pLoadTable = nullptr;
    // the table of the loaded waves after the signature at the start of the file block
    if (isIndividual && m_IsInitialized) {
        if (*reinterpret_cast<const u32*>(reinterpret_cast<const u8*>(m_pHeader) + m_pHeader->GetFileBlockOffset()) ==
            WaveArchiveFile::SIGNATURE_INDIVIDUAL_LOAD_TABLE) {
            m_pLoadTable = reinterpret_cast<const void**>(static_cast<u8*>(const_cast<void*>(pWaveArchiveFile)) +
                                                          m_pHeader->GetFileBlockOffset() + sizeof(u32));
        }
    }
}

// 0x004C98B4 | fefates:bytes [tier B]
void nw::snd::internal::WaveArchiveFileReader::Finalize()
{
    if (!m_IsInitialized) {
        return;
    }
    m_pHeader = nullptr;
    m_pInfoBlockBody = nullptr;
    m_pLoadTable = nullptr;
    m_IsInitialized = false;
}

// 0x004C98D8 (name after the other constructor)
nw::snd::internal::WaveArchiveFileReader::WaveArchiveFileReader()
    : m_pHeader(nullptr), m_pInfoBlockBody(nullptr), m_pLoadTable(nullptr), m_IsInitialized(false)
{
}

} // namespace internal
} // namespace snd
} // namespace nw
