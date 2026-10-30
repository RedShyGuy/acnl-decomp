#pragma once

#include "imgdb/LegacyTableHeader.h"
#include "imgdb/LegacyTableRecord.h"

namespace imgdb {

// the picture table of the DSi camera ("pit.bin"): a header and 16 byte records in a buffer of the caller
class LegacyTable
{
public:
    LegacyTable();

    static u32 ComputeTableSize(int recordNum, u16 extraSize);
    bool SetInitialTableData(void* data, u32 size, int recordNum, u16 extraSize);
    bool SetExistingTableData(void* data, u32 size);
    void UpdateCrc();
    bool Validate() const;

    LegacyTableHeader* GetHeader() const { return reinterpret_cast<LegacyTableHeader*>(m_pData); }
    LegacyTableRecord* GetRecords() const
    {
        return reinterpret_cast<LegacyTableRecord*>(m_pData + GetHeader()->m_RecordOffset);
    }
    // the CRC of the table with the CRC field 0
    u16 ComputeCrc() const;

private:
    u8* m_pData;  // +0x00
    u32 m_Size;   // +0x04
};
ASSERT_SIZE(LegacyTable, 0x8);

} // namespace imgdb
