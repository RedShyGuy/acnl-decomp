#pragma once

#include "imgdb/imgdb_Types.h"

namespace imgdb {

// a record of the DSi picture table: the DCF numbers of an image (100 files per directory)
class LegacyTableRecord
{
public:
    void Initialize();
    void SetIndexInfo(const IndexInfo& index);
    IndexInfo GetIndexInfo() const;

    DateTimeSeconds m_DateTime;  // +0x00
    u32 _08;                     // +0x08
    union
    {
        u32 m_Flags;  // +0x0C
        struct
        {
            u32 m_IsUsed : 1;
            u32 m_DirectoryIndex : 10;
            u32 m_FileIndex : 7;
            u32 : 5;
            u32 _0C_23 : 1;
            u32 : 8;
        };
    };
};
ASSERT_SIZE(LegacyTableRecord, 0x10);

} // namespace imgdb
