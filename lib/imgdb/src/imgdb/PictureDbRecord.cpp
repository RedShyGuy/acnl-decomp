#include "imgdb/PictureDatabase.h"
#include "imgdb/PictureDbRecord.h"

namespace imgdb {

// 0x00755A68 (name is ours)
const ImageInfo* imgdb::PictureDbRecord::GetImageInfo() const
{
    return &GetPictureTableRecord()->m_ImageInfo;
}

// 0x005AAC80 (name is ours)
ImageInfo* imgdb::PictureDbRecord::GetImageInfo()
{
    return const_cast<ImageInfo*>(static_cast<const ImageDbRecord*>(this)->GetImageInfo());
}

// 0x00755ADC | nintendogs:bytes
const ValidityStateTableRecord* imgdb::PictureDbRecord::GetValidityStateTableRecord() const
{
    return &m_pDatabase->m_ValidityStateTable.GetRecords()[m_Index];
}

// 0x005AAC8C (name is ours)
ValidityStateTableRecord* imgdb::PictureDbRecord::GetValidityStateTableRecord()
{
    return const_cast<ValidityStateTableRecord*>(static_cast<const ImageDbRecord*>(this)->GetValidityStateTableRecord());
}

// 0x00755A8C | nintendogs:bytes [tier B]
LegacyTableRecord* imgdb::PictureDbRecord::GetLegacyTableRecord() const
{
    if (m_pDatabase->m_pLegacyData == NULL)
    {
        return NULL;
    }
    return &m_pDatabase->m_LegacyTable.GetRecords()[m_Index];
}

// 0x00755AB8 | nintendogs:bytes [tier B]
PictureTableRecord* imgdb::PictureDbRecord::GetPictureTableRecord() const
{
    return &m_pDatabase->m_PictureTable.GetRecords()[m_Index];
}

} // namespace imgdb
