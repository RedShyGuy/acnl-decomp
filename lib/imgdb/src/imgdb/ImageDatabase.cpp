#include "imgdb/Singleton.h"
#include "imgdb/ImageDatabase.h"

namespace imgdb {
// 0x005A72B8 slot 0x00 | virtual slot, introduced by imgdb::Singleton<imgdb::ImageDatabase>
void imgdb::ImageDatabase::vf_0x00()
{
}

// 0x005A7218 slot 0x04 | virtual slot, introduced by imgdb::Singleton<imgdb::ImageDatabase>
void imgdb::ImageDatabase::vf_0x04()
{
}

// 0x005A5C30 | nintendogs:bytes [tier B]
void imgdb::ImageDatabase::Initialize(bool, bool)
{
}

// 0x005A5CDC | nintendogs:bytes [tier B]
void imgdb::ImageDatabase::RegisterMp(imgdb::StorageType, const void*, unsigned, const imgdb::ImageDbRecordParam&, imgdb::ImageDbRecordLink*)
{
}

// 0x005A5D28 | nintendogs:bytes [tier B]
void imgdb::ImageDatabase::ResetTable(imgdb::StorageType)
{
}

// 0x005A60B4 | nintendogs:bytes [tier B]
void imgdb::ImageDatabase::RegisterJpeg(imgdb::StorageType, const void*, unsigned, const imgdb::ImageDbRecordParam&, imgdb::ImageDbRecordLink*)
{
}

// 0x005A6100 | nintendogs:bytes [tier B]
void imgdb::ImageDatabase::SaveDatabase(imgdb::StorageType)
{
}

// 0x005A6154 | nintendogs:bytes [tier B]
void imgdb::ImageDatabase::RegisterImage(imgdb::StorageType, imgdb::ImageKind, const void*, unsigned, const void*, unsigned, const imgdb::ImageDbRecordParam&, bool, const imgdb::IndexInfo*, imgdb::ImageDbRecordLink*)
{
}

// 0x005A63B4 | nintendogs:bytes [tier B]
void imgdb::ImageDatabase::CreateDatabase(imgdb::StorageType)
{
}

// 0x005A6424 | nintendogs:bytes [tier B]
void imgdb::ImageDatabase::RestoreDatabase(imgdb::StorageType)
{
}

// 0x005A6664 | nintendogs:bytes [tier B]
void imgdb::ImageDatabase::UnregisterImage(const imgdb::ImageDbRecordLink&)
{
}

// 0x005A6754 | nintendogs:bytes [tier B]
void imgdb::ImageDatabase::CreateRecordLink(imgdb::ImageDbRecordLink&, imgdb::StorageType, const imgdb::DateTimeSeconds&, imgdb::ImageKind, const imgdb::ImageDbRecordParam&, long long)
{
}

// 0x005A6870 | nintendogs:bytes [tier B]
void imgdb::ImageDatabase::UnregisterRecord(imgdb::StorageType, int, imgdb::ImageDbRecord&)
{
}

// 0x005A6984 | nintendogs:bytes [tier B]
void imgdb::ImageDatabase::SyncPictureDatabaseCore(imgdb::StorageType)
{
}

// 0x005A6D00 | nintendogs:bytes [tier B]
void imgdb::ImageDatabase::SyncPictureDatabaseWithTwl(imgdb::StorageType)
{
}

// 0x005A6E38 | nintendogs:bytes [tier B]
void imgdb::ImageDatabase::SortPictureDatabaseCtrAndTwl(imgdb::StorageType)
{
}

// 0x005A71BC | nintendogs:bytes-fuzzy [tier B]
imgdb::ImageDatabase::ImageDatabase()
{
}

// 0x005AA904 | nintendogs:bytes [tier B]
void imgdb::ImageDatabase::WritePictureDatabase(imgdb::StorageType, bool, bool)
{
}

// 0x007550BC | nintendogs:bytes [tier B]
void imgdb::ImageDatabase::FindRecord(const imgdb::ImageDbRecordLink&) const
{
}

// 0x0075511C | nintendogs:bytes [tier B]
void imgdb::ImageDatabase::GetEmptyImageNum(imgdb::StorageType) const
{
}

// 0x00755148 | nintendogs:bytes [tier B]
void imgdb::ImageDatabase::InsertRecordLinkList(imgdb::DynamicArray<imgdb::ImageDbRecordLink, imgdb::XAllocator<imgdb::ImageDbRecordLink>>&, const imgdb::ImageDbRecordLink&) const
{
}

// 0x00755204 | nintendogs:bytes-fuzzy [tier B]
void imgdb::ImageDatabase::FindRecordLinkListInsertPos(const imgdb::DynamicArray<imgdb::ImageDbRecordLink, imgdb::XAllocator<imgdb::ImageDbRecordLink>>&, const imgdb::ImageDbRecordLink&) const
{
}

} // namespace imgdb
