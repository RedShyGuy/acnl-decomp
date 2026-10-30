#pragma once

#include "imgdb/imgdb_Types.h"

namespace imgdb {

// the header of the DSi picture table (24 bytes; the records follow at m_RecordOffset)
class LegacyTableHeader
{
public:
    static const u32 MAGIC0 = 0x50495430;  // "0TIP"
    static const u32 MAGIC1 = 0x315F3030;  // "00_1"

    void SetTargetArchive(StorageType type);
    StorageType GetTargetArchive() const;

    u32 m_Magic[2];            // +0x00
    u16 m_RecordNum;           // +0x08
    u8 m_TargetArchive;        // +0x0A
    u8 _0B;                    // +0x0B
    IndexInfo m_IndexInfo[2];  // +0x0C
    u16 m_Crc;                 // +0x14 CRC-16 of the table with this field 0
    u16 m_RecordOffset;        // +0x16
};
ASSERT_SIZE(LegacyTableHeader, 0x18);

} // namespace imgdb
