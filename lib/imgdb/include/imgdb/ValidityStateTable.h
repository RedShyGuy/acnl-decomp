#pragma once

#include "imgdb/imgdb_Types.h"

namespace imgdb {

// the state of a record of the database (1 byte)
class ValidityStateTableRecord
{
public:
    void Initialize();

    u8 m_State;  // +0x00
};
ASSERT_SIZE(ValidityStateTableRecord, 0x1);

// a state per record: a 4 byte header (the number of records) and the records
class ValidityStateTable
{
public:
    ValidityStateTable();

    static u32 ComputeTableSize(int recordNum);
    bool SetInitialTableData(void* data, u32 size, int recordNum);

    u16 GetRecordNum() const { return *reinterpret_cast<u16*>(m_pData); }
    ValidityStateTableRecord* GetRecords() const { return reinterpret_cast<ValidityStateTableRecord*>(m_pData + 4); }

private:
    u8* m_pData;  // +0x00
    u32 m_Size;   // +0x04
};
ASSERT_SIZE(ValidityStateTable, 0x8);

} // namespace imgdb
