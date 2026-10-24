#include "nw/snd/internal/snd_SequenceSoundFileReader.h"

#include "nw/snd/internal/snd_SequenceSoundFile_FileHeader.h"
#include "nw/snd/internal/snd_SequenceSoundFile_LabelBlockBody.h"

namespace nw {
namespace snd {
namespace internal {
namespace {
// the size of the header of a block
const u32 BLOCK_HEADER_SIZE = 8;
} // namespace

// 0x004C9998 | fefates:bytes [tier B]
nw::snd::internal::SequenceSoundFileReader::SequenceSoundFileReader(const void* pSequenceSoundFile)
    : m_pHeader(nullptr), m_pDataBlockBody(nullptr), m_pLabelBlockBody(nullptr)
{
    const SequenceSoundFile::FileHeader* pHeader = static_cast<const SequenceSoundFile::FileHeader*>(pSequenceSoundFile);
    if (pHeader->signature != SequenceSoundFile::SIGNATURE_FILE) {
        return;
    }
    if (pHeader->version - SequenceSoundFile::VERSION_MIN > SequenceSoundFile::VERSION_MAX - SequenceSoundFile::VERSION_MIN) {
        return;
    }
    m_pHeader = pHeader;
    m_pDataBlockBody = static_cast<const u8*>(m_pHeader->GetDataBlock()) + BLOCK_HEADER_SIZE;
    m_pLabelBlockBody = reinterpret_cast<const SequenceSoundFile::LabelBlockBody*>(static_cast<const u8*>(m_pHeader->GetLabelBlock()) + BLOCK_HEADER_SIZE);
}

// 0x007404D8 (name is ours)
bool nw::snd::internal::SequenceSoundFileReader::GetOffsetByLabel(const char* pLabel, u32* pOffset) const
{
    return m_pLabelBlockBody->GetOffsetByLabel(pLabel, pOffset);
}

} // namespace internal
} // namespace snd
} // namespace nw
