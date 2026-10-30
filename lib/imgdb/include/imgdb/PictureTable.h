#pragma once

#include "imgdb/PictureTableHeader.h"
#include "imgdb/PictureTableRecord.h"

namespace imgdb {

// the picture table of the database: a header and 152 byte records in a buffer of the caller
class PictureTable
{
public:
    PictureTable();

    static u32 ComputeTableSize(int recordNum, u16 extraSize);
    bool SetInitialTableData(void* data, u32 size, int recordNum, u16 extraSize);
    bool SetExistingTableData(void* data, u32 size);
    void UpdateCrc();
    bool Validate() const;

    PictureTableHeader* GetHeader() const { return reinterpret_cast<PictureTableHeader*>(m_pData); }
    PictureTableRecord* GetRecords() const
    {
        return reinterpret_cast<PictureTableRecord*>(m_pData + GetHeader()->m_RecordOffset);
    }
    // the CRC of the table with the CRC field 0
    u16 ComputeCrc() const;

private:
    u8* m_pData;  // +0x00
    u32 m_Size;   // +0x04
};
ASSERT_SIZE(PictureTable, 0x8);

} // namespace imgdb
