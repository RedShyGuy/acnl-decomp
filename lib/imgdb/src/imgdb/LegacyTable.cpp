#include <cstring>

#include "imgdb/Crc16.h"
#include "imgdb/LegacyTable.h"

namespace imgdb {

inline u16 imgdb::LegacyTable::ComputeCrc() const
{
    LegacyTableHeader* header = GetHeader();
    u16 crc = header->m_Crc;
    header->m_Crc = 0;
    Crc16 crc16(m_pData, m_Size);
    header->m_Crc = crc;
    return crc16.GetValue();
}

// (the header, the space the caller asks for and the records, in 32 byte blocks)
// 0x005A55D4 | nintendogs:callgraph [tier C]
u32 imgdb::LegacyTable::ComputeTableSize(int recordNum, u16 extraSize)
{
    return (recordNum * sizeof(LegacyTableRecord) + extraSize + sizeof(LegacyTableHeader) + 31) & ~31;
}

// 0x005A55E4 | nintendogs:callgraph [tier C]
bool imgdb::LegacyTable::SetInitialTableData(void* data, u32 size, int recordNum, u16 extraSize)
{
    m_pData = static_cast<u8*>(data);
    m_Size = size;
    if (data == NULL || size == 0)
    {
        return true;
    }
    if (ComputeTableSize(recordNum, extraSize) > size)
    {
        return false;
    }

    std::memset(data, 0, size);
    LegacyTableHeader* header = GetHeader();
    header->m_RecordNum = 0;
    header->m_TargetArchive = 0;
    header->_0B = 0;
    header->m_IndexInfo[0].Initialize();
    header->m_IndexInfo[1].Initialize();
    header->m_Crc = 0;
    header->m_RecordOffset = 0;
    header->m_Magic[1] = LegacyTableHeader::MAGIC1;
    header->m_Magic[0] = LegacyTableHeader::MAGIC0;
    header->m_RecordNum = recordNum;
    header->m_RecordOffset = extraSize + sizeof(LegacyTableHeader);
    for (int i = 0; i < recordNum; i++)
    {
        GetRecords()[i].Initialize();
    }
    return true;
}

// 0x005A56A0 | nintendogs:bytes [tier B]
bool imgdb::LegacyTable::SetExistingTableData(void* data, u32 size)
{
    m_pData = static_cast<u8*>(data);
    m_Size = size;
    if (data == NULL)
    {
        return false;
    }
    return Validate();
}

// 0x005A56B8 (name is ours)
void imgdb::LegacyTable::UpdateCrc()
{
    GetHeader()->m_Crc = ComputeCrc();
}

// 0x005A5708 (name is ours)
imgdb::LegacyTable::LegacyTable() : m_pData(NULL), m_Size(0)
{
}

// 0x00754D68 | nintendogs:bytes [tier B]
bool imgdb::LegacyTable::Validate() const
{
    if (m_pData == NULL)
    {
        return false;
    }
    const LegacyTableHeader* header = GetHeader();
    if (m_Size < sizeof(LegacyTableHeader) || header->m_RecordOffset < sizeof(LegacyTableHeader) ||
        ComputeTableSize(header->m_RecordNum, header->m_RecordOffset - sizeof(LegacyTableHeader)) > m_Size)
    {
        return false;
    }
    if (header->m_Magic[0] != LegacyTableHeader::MAGIC0 || header->m_Magic[1] != LegacyTableHeader::MAGIC1)
    {
        return false;
    }
    if (header->m_Crc != ComputeCrc())
    {
        return false;
    }
    if (!header->m_IndexInfo[0].IsValidDirectoryIndex() || !header->m_IndexInfo[0].IsValidFileIndex() ||
        !header->m_IndexInfo[1].IsValidDirectoryIndex() || !header->m_IndexInfo[1].IsValidFileIndex())
    {
        return false;
    }
    for (int i = 0; i < header->m_RecordNum; i++)
    {
        const LegacyTableRecord& record = GetRecords()[i];
        if (record.m_IsUsed)
        {
            if (!record.GetIndexInfo().IsValidDirectoryIndex() || !record.GetIndexInfo().IsValidFileIndex())
            {
                return false;
            }
        }
    }
    return true;
}

} // namespace imgdb
