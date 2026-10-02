#pragma once

#include "decomp.h"

namespace imgdb {
class ImageDbRecordLink
{
public:
    void SetImageKind(imgdb::ImageKind); // 0x005AAF48 | nintendogs:bytes [tier B]
    void SetIndexInfo(const imgdb::IndexInfo&); // 0x005AAF64 | nintendogs:bytes [tier B]
    void SetStorageType(imgdb::StorageType); // 0x005AAFA4 | nintendogs:bytes [tier B]
    void SetDbRecordIndex(int); // 0x005AAFBC | nintendogs:bytes [tier B]
    void SetDays(int); // 0x005AAFD4 | nintendogs:bytes [tier B]
};
} // namespace imgdb
