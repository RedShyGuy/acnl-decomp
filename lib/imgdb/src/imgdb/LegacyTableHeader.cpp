#include "imgdb/LegacyTableHeader.h"

namespace imgdb {

// 0x005AB1F8 (name is ours)
void imgdb::LegacyTableHeader::SetTargetArchive(StorageType type)
{
    if (type == STORAGE_TYPE_CTR_NAND || type == STORAGE_TYPE_SD)
    {
        m_TargetArchive = type;
    }
}

// 0x00755C8C | nintendogs:bytes [tier B]
imgdb::StorageType imgdb::LegacyTableHeader::GetTargetArchive() const
{
    if (m_TargetArchive == STORAGE_TYPE_CTR_NAND || m_TargetArchive == STORAGE_TYPE_SD)
    {
        return static_cast<StorageType>(m_TargetArchive);
    }
    return STORAGE_TYPE_TWL_NAND;
}

} // namespace imgdb
