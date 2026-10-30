#pragma once

#include "imgdb/imgdb_Types.h"

namespace imgdb {

class PictureDatabase;
class PictureDbRecord;

// rebuilds the tables of a database from the image files of the camera and of the DSi camera (static functions,
// names are ours except the ones of the functions)
class ImageDbRestore
{
public:
    // the records of both tables are brought together, the next index starts the longest run of free directories
    static void SyncPictureDatabase(StorageType storage, PictureDatabase& database);
    // files of 0 bytes and their records are deleted; whether one of the record was
    static bool DeleteFileIfZeroByte(StorageType storage, PictureDbRecord& record, bool* isPictureDeleted,
                                     bool* isLegacyDeleted);
    // isLegacyValid / isPictureValid: the table could be read
    static Result RestorePictureDatabase(StorageType storage, bool isLegacyValid, bool isPictureValid,
                                         PictureDatabase& database);
    static void RestorePictureDatabaseCtr(StorageType storage, PictureDatabase& database);
    static void RestorePictureDatabaseTwl(StorageType storage, PictureDatabase& database);
    static void SyncPictureDatabaseRecord(StorageType storage, PictureDbRecord& record);
    // the attributes of the images from their Exif data
    static void ExtractPictureDatabaseInfo(StorageType storage, PictureDatabase& database);
    static void RestorePictureDatabaseDirTwl(StorageType storage, PictureDatabase& database, const IndexInfo& index);
    static void RestorePictureDatabaseFileCtr(StorageType storage, PictureDatabase& database, const IndexInfo& index,
                                              ImageKind kind);
    // an image only the DSi camera knows: a record and a copy in the directories of the camera
    static void SyncPictureDatabaseRecordAvailableTwlOnly(StorageType storage, PictureDbRecord& record);
    // an image only the camera knows: a record and a copy for the DSi camera
    static void SyncPictureDatabaseRecordAvailableCtrOnlyMp(StorageType storage, PictureDbRecord& record);
    static void SyncPictureDatabaseRecordAvailableCtrOnlyJpeg(StorageType storage, PictureDbRecord& record);
};

} // namespace imgdb
