#include "imgdb/PictureDatabase.h"

namespace imgdb {
// 0x005A9E90 | nintendogs:bytes-fuzzy [tier B]
void imgdb::PictureDatabase::CreateNewRecord(imgdb::StorageType, imgdb::ImageKind)
{
}

// 0x005AA1D4 | nintendogs:bytes [tier B]
void imgdb::PictureDatabase::CreateNewRecord(imgdb::StorageType, imgdb::ImageKind, const imgdb::IndexInfo&)
{
}

// 0x005AA6FC | nintendogs:bytes [tier B]
void imgdb::PictureDatabase::SeNextIndexInfo(const imgdb::IndexInfo&)
{
}

// 0x005AA964 | nintendogs:bytes-fuzzy [tier B]
void imgdb::PictureDatabase::SavePictureTable(imgdb::StorageType, const wchar_t*, bool)
{
}

// 0x005AAAA0 | nintendogs:bytes [tier B]
void imgdb::PictureDatabase::InitializeLegacyTable()
{
}

// 0x005AAB20 | nintendogs:bytes [tier B]
void imgdb::PictureDatabase::InitializePictureTable()
{
}

// 0x005AABA0 | nintendogs:bytes [tier B]
void imgdb::PictureDatabase::InitializeValidityStateTable()
{
}

// 0x005AAC08 | nintendogs:bytes [tier B]
imgdb::PictureDatabase::PictureDatabase()
{
}

// 0x007559A4 | nintendogs:bytes [tier B]
void imgdb::PictureDatabase::GetEmptyRecordNum() const
{
}

} // namespace imgdb
