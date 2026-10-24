#include "nw/snd/internal/snd_GroupFileReader.h"

#include "nw/snd/internal/snd_GroupFile_FileHeader.h"

namespace nw {
namespace snd {
namespace internal {
namespace {
// the size of the header of a block
const u32 BLOCK_HEADER_SIZE = 8;
} // namespace

// 0x0013BB9C | nintendogs:bytes [tier A]
nw::snd::internal::GroupFileReader::GroupFileReader(const void* pGroupFile)
    : m_pInfoBlockBody(nullptr), m_pFileBlockBody(nullptr), m_pInfoExBlockBody(nullptr)
{
    const GroupFile::FileHeader* pHeader = static_cast<const GroupFile::FileHeader*>(pGroupFile);
    if (pHeader->signature != GroupFile::SIGNATURE_FILE) {
        return;
    }
    if (pHeader->version - GroupFile::VERSION_MIN > GroupFile::VERSION_MAX - GroupFile::VERSION_MIN) {
        return;
    }
    const void* pInfoBlock = pHeader->GetInfoBlock();
    const void* pFileBlock = pHeader->GetFileBlock();
    const void* pInfoExBlock = pHeader->GetInfoExBlock();
    if (pInfoBlock == nullptr || pFileBlock == nullptr) {
        return;
    }
    m_pInfoBlockBody = reinterpret_cast<const BinaryReferenceTable*>(static_cast<const u8*>(pInfoBlock) + BLOCK_HEADER_SIZE);
    m_pFileBlockBody = static_cast<const u8*>(pFileBlock) + BLOCK_HEADER_SIZE;
    if (pInfoExBlock != nullptr) {
        m_pInfoExBlockBody = reinterpret_cast<const BinaryReferenceTable*>(static_cast<const u8*>(pInfoExBlock) + BLOCK_HEADER_SIZE);
    }
}

// 0x0013D3A8 | nintendogs:callseq-callee [tier A]
u32 nw::snd::internal::GroupFileReader::GetGroupItemExCount() const
{
    if (m_pInfoExBlockBody == nullptr) {
        return 0;
    }
    return m_pInfoExBlockBody->count;
}

// 0x0013D3B8 | nintendogs:bytes [tier A]
bool nw::snd::internal::GroupFileReader::ReadGroupItemInfoEx(GroupFile::GroupItemInfoEx* pInfo, u32 index) const
{
    if (m_pInfoExBlockBody == nullptr) {
        return false;
    }
    const GroupFile::GroupItemInfoEx* pItem = static_cast<const GroupFile::GroupItemInfoEx*>(m_pInfoExBlockBody->GetReferedItem(index));
    if (pItem == nullptr) {
        return false;
    }
    *pInfo = *pItem;
    return true;
}

// 0x0013D3F8 | nintendogs:bytes [tier A]
bool nw::snd::internal::GroupFileReader::ReadGroupItemLocationInfo(GroupItemLocationInfo* pInfo, u32 index) const
{
    if (m_pInfoBlockBody == nullptr) {
        return false;
    }
    const GroupFile::GroupItemInfo* pItem = static_cast<const GroupFile::GroupItemInfo*>(m_pInfoBlockBody->GetReferedItem(index));
    if (pItem == nullptr) {
        return false;
    }
    pInfo->fileId = pItem->fileId;
    if (pItem->embeddedItem.offset == BinaryReference::INVALID_OFFSET) {
        pInfo->address = nullptr;
    } else {
        pInfo->address = GetReferedPointer(m_pFileBlockBody, pItem->embeddedItem);
    }
    return true;
}

} // namespace internal
} // namespace snd
} // namespace nw
