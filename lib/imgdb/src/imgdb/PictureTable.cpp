#include <cstring>

#include "imgdb/Crc16.h"
#include "imgdb/PictureTable.h"

namespace imgdb {

inline u16 imgdb::PictureTable::ComputeCrc() const
{
    PictureTableHeader* header = GetHeader();
    u16 crc = header->m_Crc;
    header->m_Crc = 0;
    Crc16 crc16(m_pData, m_Size);
    header->m_Crc = crc;
    return crc16.GetValue();
}

// (the header, the space the caller asks for and the records)
// 0x005A5944 | nintendogs:bytes [tier B]
u32 imgdb::PictureTable::ComputeTableSize(int recordNum, u16 extraSize)
{
    return recordNum * sizeof(PictureTableRecord) + extraSize + sizeof(PictureTableHeader);
}

// 0x005A5958 | nintendogs:bytes [tier B]
bool imgdb::PictureTable::SetInitialTableData(void* data, u32 size, int recordNum, u16 extraSize)
{
    m_pData = static_cast<u8*>(data);
    m_Size = size;
    if (data == NULL)
    {
        return true;
    }
    if (ComputeTableSize(recordNum, extraSize) > size)
    {
        return false;
    }

    std::memset(data, 0, size);
    PictureTableHeader* header = GetHeader();
    header->m_SaveCount = 0;
    header->m_RecordNum = 0;
    header->m_TargetArchive = 0;
    header->_0F = 0;
    header->m_IndexInfo.Initialize();
    header->m_Crc = 0;
    header->m_RecordOffset = 0;
    header->m_Magic[1] = PictureTableHeader::MAGIC1;
    header->m_Magic[0] = PictureTableHeader::MAGIC0;
    header->m_RecordNum = recordNum;
    header->m_RecordOffset = extraSize + sizeof(PictureTableHeader);
    for (int i = 0; i < recordNum; i++)
    {
        GetRecords()[i].Initialize(IMAGE_KIND_NONE);
    }
    return true;
}

// 0x005A5A40 | nintendogs:bytes [tier B]
bool imgdb::PictureTable::SetExistingTableData(void* data, u32 size)
{
    m_pData = static_cast<u8*>(data);
    m_Size = size;
    if (data == NULL)
    {
        return true;
    }
    return Validate();
}

// 0x005A5A58 (name is ours)
void imgdb::PictureTable::UpdateCrc()
{
    GetHeader()->m_Crc = ComputeCrc();
}

// 0x005A5AA8 (name is ours)
imgdb::PictureTable::PictureTable() : m_pData(NULL), m_Size(0)
{
}

// 0x00754F30 | nintendogs:bytes [tier B]
bool imgdb::PictureTable::Validate() const
{
    if (m_pData == NULL)
    {
        return false;
    }
    const PictureTableHeader* header = GetHeader();
    if (m_Size < sizeof(PictureTableHeader) || header->m_RecordOffset < sizeof(PictureTableHeader) ||
        ComputeTableSize(header->m_RecordNum, header->m_RecordOffset - sizeof(PictureTableHeader)) > m_Size)
    {
        return false;
    }
    if (header->m_Magic[0] != PictureTableHeader::MAGIC0 || header->m_Magic[1] != PictureTableHeader::MAGIC1)
    {
        return false;
    }
    if (header->m_TargetArchive >= STORAGE_TYPE_TWL_NAND)
    {
        return false;
    }
    if (header->m_Crc != ComputeCrc())
    {
        return false;
    }
    if (!header->m_IndexInfo.IsValidDirectoryIndex() || !header->m_IndexInfo.IsValidFileIndex())
    {
        return false;
    }
    for (int i = 0; i < header->m_RecordNum; i++)
    {
        const PictureTableRecord& record = GetRecords()[i];
        if (!record.m_ImageInfo.Validate())
        {
            return false;
        }
        for (int j = 0; j < PictureTableRecord::FACE_NUM; j++)
        {
            if (!record.m_Faces[j].Validate())
            {
                return false;
            }
        }
    }
    return true;
}

} // namespace imgdb
