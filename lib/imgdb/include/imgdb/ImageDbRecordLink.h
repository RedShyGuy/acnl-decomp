#pragma once

#include <cstring>

#include "imgdb/imgdb_Types.h"

namespace imgdb {

// a reference to a record of the database, sortable by the days of the image
class ImageDbRecordLink
{
public:
    DECOMP_EMIT_COPY ImageDbRecordLink() { std::memset(this, 0, sizeof(*this)); } // 0x0013F9A0 | nintendogs:callgraph [tier C]

    void Initialize();
    void SetImageKind(ImageKind kind);
    void SetIndexInfo(const IndexInfo& index);
    void SetStorageType(StorageType type);
    void SetDbRecordIndex(int index);
    void SetDays(int days);
    StorageType GetStorageType() const;
    int GetDbRecordIndex() const;
    int GetDays() const;
    ImageKind GetImageKind() const;

    s32 m_Days : 18;               // +0x00
    u32 m_DbRecordIndex : 14;
    u32 m_StorageType : 2;         // +0x04
    u32 m_ImageKind : 3;
    u32 m_DirectoryIndex : 10;
    u32 m_FileIndex : 14;
    u32 : 3;
};
ASSERT_SIZE(ImageDbRecordLink, 0x8);

} // namespace imgdb
