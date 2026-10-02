#pragma once

#include "decomp.h"

namespace imgdb {
class PictureDatabase
{
public:
    void CreateNewRecord(imgdb::StorageType, imgdb::ImageKind); // 0x005A9E90 | nintendogs:bytes-fuzzy [tier B]
    void CreateNewRecord(imgdb::StorageType, imgdb::ImageKind, const imgdb::IndexInfo&); // 0x005AA1D4 | nintendogs:bytes [tier B]
    void SeNextIndexInfo(const imgdb::IndexInfo&); // 0x005AA6FC | nintendogs:bytes [tier B]
    void SavePictureTable(imgdb::StorageType, const wchar_t*, bool); // 0x005AA964 | nintendogs:bytes-fuzzy [tier B]
    void InitializeLegacyTable(); // 0x005AAAA0 | nintendogs:bytes [tier B]
    void InitializePictureTable(); // 0x005AAB20 | nintendogs:bytes [tier B]
    void InitializeValidityStateTable(); // 0x005AABA0 | nintendogs:bytes [tier B]
    PictureDatabase(); // 0x005AAC08 | nintendogs:bytes [tier B]
    void GetEmptyRecordNum() const; // 0x007559A4 | nintendogs:bytes [tier B]
};
} // namespace imgdb
