#include "imgdb/ImageDbRestore.h"

namespace imgdb {
// 0x005A8028 | nintendogs:bytes [tier B]
void imgdb::ImageDbRestore::SyncPictureDatabase(imgdb::StorageType, imgdb::PictureDatabase&)
{
}

// 0x005A83F8 | nintendogs:bytes [tier B]
void imgdb::ImageDbRestore::DeleteFileIfZeroByte(imgdb::StorageType, imgdb::PictureDbRecord&, bool*, bool*)
{
}

// 0x005A8590 | nintendogs:bytes [tier B]
void imgdb::ImageDbRestore::RestorePictureDatabase(imgdb::StorageType, bool, bool, imgdb::PictureDatabase&)
{
}

// 0x005A8614 | nintendogs:bytes [tier B]
void imgdb::ImageDbRestore::RestorePictureDatabaseCtr(imgdb::StorageType, imgdb::PictureDatabase&)
{
}

// 0x005A88F8 | nintendogs:bytes [tier B]
void imgdb::ImageDbRestore::RestorePictureDatabaseTwl(imgdb::StorageType, imgdb::PictureDatabase&)
{
}

// 0x005A8A58 | nintendogs:bytes [tier B]
void imgdb::ImageDbRestore::SyncPictureDatabaseRecord(imgdb::StorageType, imgdb::PictureDbRecord&)
{
}

// 0x005A8B14 | nintendogs:bytes [tier B]
void imgdb::ImageDbRestore::ExtractPictureDatabaseInfo(imgdb::StorageType, imgdb::PictureDatabase&)
{
}

// 0x005A8CF4 | nintendogs:bytes [tier B]
void imgdb::ImageDbRestore::RestorePictureDatabaseDirTwl(imgdb::StorageType, imgdb::PictureDatabase&, const imgdb::IndexInfo&)
{
}

// 0x005A9000 | nintendogs:bytes [tier B]
void imgdb::ImageDbRestore::RestorePictureDatabaseFileCtr(imgdb::StorageType, imgdb::PictureDatabase&, const imgdb::IndexInfo&, imgdb::ImageKind)
{
}

// 0x005A9210 | nintendogs:bytes [tier B]
void imgdb::ImageDbRestore::SyncPictureDatabaseRecordAvailableTwlOnly(imgdb::StorageType, imgdb::PictureDbRecord&)
{
}

} // namespace imgdb
