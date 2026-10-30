#pragma once

#include "imgdb/imgdb_Types.h"

namespace imgdb {

// the header of the picture table (24 bytes; the records follow at m_RecordOffset)
class PictureTableHeader
{
public:
    static const u32 MAGIC0 = 0x514A5531;  // "1UJQ"
    static const u32 MAGIC1 = 0x315F3030;  // "00_1"

    void SetTargetArchive(StorageType type);
    StorageType GetTargetArchive() const;

    u32 m_Magic[2];         // +0x00
    u32 m_SaveCount;        // +0x08 (counts the saves)
    u16 m_RecordNum;        // +0x0C
    u8 m_TargetArchive;     // +0x0E
    u8 _0F;                 // +0x0F
    IndexInfo m_IndexInfo;  // +0x10
    u16 m_Crc;              // +0x14 CRC-16 of the table with this field 0
    u16 m_RecordOffset;     // +0x16
};
ASSERT_SIZE(PictureTableHeader, 0x18);

} // namespace imgdb
