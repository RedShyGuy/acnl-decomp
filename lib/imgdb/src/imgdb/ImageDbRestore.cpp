#include "imgdb/ImageDbRecordParam.h"
#include "imgdb/ImageDbRestore.h"
#include "imgdb/ImageFileWriter.h"
#include "imgdb/JpegDecoder.h"
#include "imgdb/MpDecoder.h"
#include "imgdb/PhotoImageInfoReader.h"
#include "imgdb/PictureDatabase.h"
#include "imgdb/imgdb_Api.h"
#include "imgdb/util/util_Api.h"

namespace imgdb {

namespace {

// (only reset here; a restore stops when it is set)
// 0x009826D8 (name is ours)
bool s_IsRestoreCanceled;

// a path of a directory and of an image file
const int DIRECTORY_PATH_SIZE = 30;
const int IMAGE_PATH_SIZE = 44;
// the root of the images with the terminator
const int ROOT_PATH_SIZE = 21;
// a bit per directory index (twice the size the functions of util use)
const int DIRECTORY_FLAGS_SIZE = 226;

inline bool TestBit(const u8* bits, int index)
{
    return (bits[index / 8] & (1 << (index % 8))) != 0;
}

inline void SetBit(u8* bits, int index)
{
    bits[index / 8] |= 1 << (index % 8);
}

} // namespace

// 0x00815D84 | nintendogs:callseq [tier C]
template void imgdb::DynamicArray<imgdb::util::FileIndexInfo, imgdb::XAllocator<imgdb::util::FileIndexInfo> >::Refresh();
// 0x00815E20 | nintendogs:bytes [tier B]
template void imgdb::DynamicArray<imgdb::util::FileIndexInfo, imgdb::XAllocator<imgdb::util::FileIndexInfo> >::Recreate(u32);
// 0x00815F44 | nintendogs:callseq [tier C]
template void imgdb::DynamicArray<int, imgdb::XAllocator<int> >::Refresh();
// 0x00815FE0 | nintendogs:bytes [tier B]
template void imgdb::DynamicArray<int, imgdb::XAllocator<int> >::Recreate(u32);

// 0x005A8028 | nintendogs:bytes [tier B]
void imgdb::ImageDbRestore::SyncPictureDatabase(StorageType storage, PictureDatabase& database)
{
    u8 directoryFlags[DIRECTORY_FLAGS_SIZE] = {};
    for (int i = 0; i < database.m_RecordNum; i++)
    {
        if (s_IsRestoreCanceled)
        {
            return;
        }
        PictureDbRecord* record = database.GetRecord(i);
        DeleteFileIfZeroByte(storage, *record, NULL, NULL);
        PictureTableRecord* pictureRecord = record->GetPictureTableRecord();
        LegacyTableRecord* legacyRecord = record->GetLegacyTableRecord();
        if (legacyRecord->m_IsUsed)
        {
            if (!pictureRecord->m_ImageInfo.GetValidityFlag())
            {
                SyncPictureDatabaseRecordAvailableTwlOnly(storage, *record);
            }
        }
        else if (pictureRecord->m_ImageInfo.GetValidityFlag())
        {
            switch (record->GetPictureTableRecord()->m_ImageInfo.GetImageKind())
            {
            case IMAGE_KIND_JPEG:
                SyncPictureDatabaseRecordAvailableCtrOnlyJpeg(storage, *record);
                break;
            case IMAGE_KIND_MP:
                SyncPictureDatabaseRecordAvailableCtrOnlyMp(storage, *record);
                break;
            default:
                break;
            }
        }
        const ImageInfo& info = record->GetPictureTableRecord()->m_ImageInfo;
        if (info.GetValidityFlag())
        {
            SetBit(directoryFlags, info.GetDirectoryNo() - 100);
        }
    }

    // the longest run of free directory numbers (around the end)
    const int directoryNum = IndexInfo::DIRECTORY_INDEX_NUM - 1;
    int bestStart = 0;
    int bestLength = 0;
    int start = 0;
    while (start < directoryNum)
    {
        if (TestBit(directoryFlags, start))
        {
            start++;
            continue;
        }
        int length = 1;
        while (start + length < directoryNum && !TestBit(directoryFlags, start + length))
        {
            length++;
        }
        int end = start + length;
        int wrapLength = 0;
        if (end == directoryNum)
        {
            while (wrapLength < directoryNum && !TestBit(directoryFlags, wrapLength))
            {
                wrapLength++;
            }
        }
        if (length + wrapLength > bestLength)
        {
            bestStart = start;
            bestLength = length + wrapLength;
        }
        start = end;
    }
    IndexInfo next;
    next.Initialize();
    next.SetDirectoryIndex(bestStart);
    database.SeNextIndexInfo(next);
}

// 0x005A83F8 | nintendogs:bytes [tier B]
bool imgdb::ImageDbRestore::DeleteFileIfZeroByte(StorageType storage, PictureDbRecord& record, bool* isPictureDeleted,
                                                  bool* isLegacyDeleted)
{
    PictureTableRecord* pictureRecord = record.GetPictureTableRecord();
    LegacyTableRecord* legacyRecord = record.GetLegacyTableRecord();
    bool isPictureZero = false;
    bool isLegacyZero = false;
    wchar_t path[IMAGE_PATH_SIZE];
    if (legacyRecord->m_IsUsed)
    {
        util::CreateImagePath(path, IMAGE_PATH_SIZE, storage, IMAGE_KIND_JPEG, legacyRecord->GetIndexInfo(), true);
        bool isExist;
        s64 size = util::GetFileSize(storage, path, &isExist);
        if (isExist && size == 0)
        {
            ImageInfo info;
            info.Initialize();
            info.SetExt(IMAGE_KIND_JPEG);
            info.SetFileName(legacyRecord->GetIndexInfo());
            info.SetDirectoryName(legacyRecord->GetIndexInfo(), true);
            info.SetValidityFlag(true);
            util::DeleteImageTwl(storage, info, true);
            legacyRecord->Initialize();
            isLegacyZero = true;
        }
    }
    if (pictureRecord->m_ImageInfo.GetValidityFlag())
    {
        util::CreateImagePath(path, IMAGE_PATH_SIZE, storage, pictureRecord->m_ImageInfo, false);
        bool isExist;
        s64 size = util::GetFileSize(storage, path, &isExist);
        if (isExist && size == 0)
        {
            util::DeleteImageCtr(storage, pictureRecord->m_ImageInfo, true);
            pictureRecord->Initialize(IMAGE_KIND_NONE);
            isPictureZero = true;
        }
    }
    if (isPictureDeleted != NULL)
    {
        *isPictureDeleted = isPictureZero;
    }
    if (isLegacyDeleted != NULL)
    {
        *isLegacyDeleted = isLegacyZero;
    }
    return isPictureZero | isLegacyZero;
}

// 0x005A8590 | nintendogs:bytes [tier B]
Result imgdb::ImageDbRestore::RestorePictureDatabase(StorageType storage, bool isLegacyValid, bool isPictureValid,
                                                     PictureDatabase& database)
{
    s_IsRestoreCanceled = false;
    if (isLegacyValid && isPictureValid)
    {
        return RESULT_SUCCESS;
    }
    if (!isLegacyValid)
    {
        RestorePictureDatabaseTwl(storage, database);
    }
    if (!isPictureValid)
    {
        RestorePictureDatabaseCtr(storage, database);
    }
    if (database.m_pLegacyData != NULL)
    {
        SyncPictureDatabase(storage, database);
    }
    ExtractPictureDatabaseInfo(storage, database);
    return s_IsRestoreCanceled ? RESULT_DATABASE_BROKEN : RESULT_SUCCESS;
}

// 0x005A8614 | nintendogs:bytes [tier B]
void imgdb::ImageDbRestore::RestorePictureDatabaseCtr(StorageType storage, PictureDatabase& database)
{
    if (s_IsRestoreCanceled)
    {
        return;
    }
    wchar_t rootPath[ROOT_PATH_SIZE];
    util::CreateImageRootDirectoryPath(rootPath, ROOT_PATH_SIZE, storage, false);
    DynamicArray<int, XAllocator<int> > directories;
    util::EnumImageDirectory(directories, storage, rootPath, false);
    for (int* directory = directories.begin(); directory != directories.end(); directory++)
    {
        if (s_IsRestoreCanceled)
        {
            return;
        }
        IndexInfo index;
        index.Initialize();
        index.SetDirectoryIndex(*directory - 100);
        if (s_IsRestoreCanceled)
        {
            continue;
        }
        DynamicArray<util::FileIndexInfo, XAllocator<util::FileIndexInfo> > files;
        wchar_t directoryPath[DIRECTORY_PATH_SIZE];
        util::CreateImageDirectoryPath(directoryPath, DIRECTORY_PATH_SIZE, storage, index, false);
        util::EnumImageFile(files, storage, directoryPath, static_cast<ImageKindBit>(IMAGE_KIND_BIT_JPEG | IMAGE_KIND_BIT_MP),
                            false);
        for (util::FileIndexInfo* file = files.begin(); file != files.end(); file++)
        {
            if (s_IsRestoreCanceled)
            {
                break;
            }
            IndexInfo fileIndex = index;
            fileIndex.SetFileIndex(file->m_FileNo - 1);
            RestorePictureDatabaseFileCtr(storage, database, fileIndex, static_cast<ImageKind>(file->m_ImageKind));
        }
    }
}

// 0x005A88F8 | nintendogs:bytes [tier B]
void imgdb::ImageDbRestore::RestorePictureDatabaseTwl(StorageType storage, PictureDatabase& database)
{
    if (s_IsRestoreCanceled)
    {
        return;
    }
    wchar_t rootPath[ROOT_PATH_SIZE];
    util::CreateImageRootDirectoryPath(rootPath, ROOT_PATH_SIZE, storage, true);
    DynamicArray<int, XAllocator<int> > directories;
    util::EnumImageDirectory(directories, storage, rootPath, true);
    for (int* directory = directories.begin(); directory != directories.end(); directory++)
    {
        if (s_IsRestoreCanceled)
        {
            return;
        }
        IndexInfo index;
        index.Initialize();
        index.SetDirectoryIndex(*directory - 100);
        RestorePictureDatabaseDirTwl(storage, database, index);
    }
}

// 0x005A8A58 | nintendogs:bytes [tier B]
void imgdb::ImageDbRestore::SyncPictureDatabaseRecord(StorageType storage, PictureDbRecord& record)
{
    PictureTableRecord* pictureRecord = record.GetPictureTableRecord();
    LegacyTableRecord* legacyRecord = record.GetLegacyTableRecord();
    if (legacyRecord->m_IsUsed && !pictureRecord->m_ImageInfo.GetValidityFlag())
    {
        SyncPictureDatabaseRecordAvailableTwlOnly(storage, record);
        return;
    }
    if (legacyRecord->m_IsUsed || !pictureRecord->m_ImageInfo.GetValidityFlag())
    {
        return;
    }
    switch (record.GetPictureTableRecord()->m_ImageInfo.GetImageKind())
    {
    case IMAGE_KIND_JPEG:
        SyncPictureDatabaseRecordAvailableCtrOnlyJpeg(storage, record);
        break;
    case IMAGE_KIND_MP:
        SyncPictureDatabaseRecordAvailableCtrOnlyMp(storage, record);
        break;
    default:
        break;
    }
}

// 0x005A8B14 | nintendogs:bytes [tier B]
void imgdb::ImageDbRestore::ExtractPictureDatabaseInfo(StorageType storage, PictureDatabase& database)
{
    for (int i = 0; i < database.m_RecordNum; i++)
    {
        if (s_IsRestoreCanceled)
        {
            return;
        }
        PictureDbRecord* record = database.GetRecord(i);
        PictureTableRecord* pictureRecord = record->GetPictureTableRecord();
        LegacyTableRecord* legacyRecord = record->GetLegacyTableRecord();
        if (!pictureRecord->m_ImageInfo.GetValidityFlag())
        {
            continue;
        }
        ImageKind kind = pictureRecord->m_ImageInfo.GetImageKind();
        // (the path is not used)
        wchar_t path[IMAGE_PATH_SIZE];
        util::CreateImagePath(path, IMAGE_PATH_SIZE, storage, pictureRecord->m_ImageInfo, false);
        PhotoImageInfoReader reader(*GetFileAllocator(), storage, pictureRecord->m_ImageInfo, false);
        if (reader.ReadImage() != RESULT_SUCCESS)
        {
            continue;
        }
        if (kind == IMAGE_KIND_JPEG)
        {
            JpegDecoder decoder(reader.GetData(), reader.GetDataSize());
            if (decoder.ExtractExif())
            {
                util::ExtractPictureInfoFromExif(*pictureRecord, decoder);
                if (legacyRecord != NULL)
                {
                    legacyRecord->m_DateTime = pictureRecord->m_ImageInfo.m_DateTime[0];
                }
            }
        }
        else if (kind == IMAGE_KIND_MP)
        {
            MpDecoder decoder(reader.GetData(), reader.GetDataSize());
            if (decoder.ExtractExif())
            {
                util::ExtractPictureInfoFromExif(*pictureRecord, decoder);
                if (legacyRecord != NULL)
                {
                    legacyRecord->m_DateTime = pictureRecord->m_ImageInfo.m_DateTime[0];
                }
            }
        }
    }
}

// (a photo of the DSi camera gets the record that has its image in the picture table, otherwise an empty one; on
// the CTR NAND it is deleted when there is none)
// 0x005A8CF4 | nintendogs:bytes [tier B]
void imgdb::ImageDbRestore::RestorePictureDatabaseDirTwl(StorageType storage, PictureDatabase& database,
                                                         const IndexInfo& index)
{
    if (s_IsRestoreCanceled)
    {
        return;
    }
    DynamicArray<util::FileIndexInfo, XAllocator<util::FileIndexInfo> > files;
    wchar_t directoryPath[DIRECTORY_PATH_SIZE];
    util::CreateImageDirectoryPath(directoryPath, DIRECTORY_PATH_SIZE, storage, index, true);
    util::EnumImageFile(files, storage, directoryPath, IMAGE_KIND_JPEG, true);
    for (util::FileIndexInfo* file = files.begin(); file != files.end(); file++)
    {
        if (s_IsRestoreCanceled)
        {
            return;
        }
        IndexInfo fileIndex = index;
        fileIndex.SetFileIndex(file->m_FileNo - 1);
        int emptyIndex = -1;
        int foundIndex = -1;
        for (int i = 0; i < database.m_RecordNum; i++)
        {
            PictureDbRecord* record = database.GetRecord(i);
            PictureTableRecord* pictureRecord = record->GetPictureTableRecord();
            LegacyTableRecord* legacyRecord = record->GetLegacyTableRecord();
            if (emptyIndex < 0 && !pictureRecord->m_ImageInfo.GetValidityFlag() && !legacyRecord->m_IsUsed)
            {
                emptyIndex = i;
            }
            if (pictureRecord->m_ImageInfo.GetValidityFlag() && pictureRecord->m_ImageInfo.GetIndexInfo() == fileIndex)
            {
                foundIndex = i;
                break;
            }
        }
        if (foundIndex >= 0)
        {
            PictureDbRecord* record = database.GetRecord(foundIndex);
            PictureTableRecord* pictureRecord = record->GetPictureTableRecord();
            LegacyTableRecord* legacyRecord = record->GetLegacyTableRecord();
            legacyRecord->Initialize();
            legacyRecord->m_IsUsed = true;
            legacyRecord->m_DateTime = pictureRecord->m_ImageInfo.m_DateTime[0];
            legacyRecord->SetIndexInfo(fileIndex);
        }
        else if (emptyIndex >= 0)
        {
            LegacyTableRecord* legacyRecord = database.GetRecord(emptyIndex)->GetLegacyTableRecord();
            legacyRecord->Initialize();
            legacyRecord->m_IsUsed = true;
            legacyRecord->m_DateTime = DateTimeSeconds::GetCurrentDateTime();
            legacyRecord->SetIndexInfo(fileIndex);
        }
        else if (storage == STORAGE_TYPE_CTR_NAND)
        {
            wchar_t path[IMAGE_PATH_SIZE];
            util::CreateImagePath(path, IMAGE_PATH_SIZE, storage, IMAGE_KIND_JPEG, fileIndex, true);
            util::DeleteFile(storage, path);
        }
    }
}

// (an image of the camera gets the record that has it in the DSi table, otherwise an empty one; on the CTR NAND it
// is deleted when there is none)
// 0x005A9000 | nintendogs:bytes [tier B]
void imgdb::ImageDbRestore::RestorePictureDatabaseFileCtr(StorageType storage, PictureDatabase& database,
                                                          const IndexInfo& index, ImageKind kind)
{
    int emptyIndex = -1;
    int recordIndex = -1;
    if (database.m_pLegacyData != NULL)
    {
        for (int i = 0; i < database.m_RecordNum; i++)
        {
            PictureDbRecord* record = database.GetRecord(i);
            PictureTableRecord* pictureRecord = record->GetPictureTableRecord();
            LegacyTableRecord* legacyRecord = record->GetLegacyTableRecord();
            if (emptyIndex < 0 && !pictureRecord->m_ImageInfo.GetValidityFlag() && !legacyRecord->m_IsUsed)
            {
                emptyIndex = i;
            }
            if (legacyRecord->m_IsUsed && legacyRecord->GetIndexInfo() == index)
            {
                recordIndex = i;
                break;
            }
        }
    }
    else
    {
        for (int i = 0; i < database.m_RecordNum; i++)
        {
            if (!database.GetRecord(i)->GetPictureTableRecord()->m_ImageInfo.GetValidityFlag())
            {
                emptyIndex = i;
                break;
            }
        }
    }
    if (recordIndex < 0)
    {
        recordIndex = emptyIndex;
    }
    wchar_t path[IMAGE_PATH_SIZE];
    if (recordIndex < 0)
    {
        if (storage == STORAGE_TYPE_CTR_NAND)
        {
            util::CreateImagePath(path, IMAGE_PATH_SIZE, STORAGE_TYPE_CTR_NAND, IMAGE_KIND_MP, index, false);
            util::DeleteFile(STORAGE_TYPE_CTR_NAND, path);
        }
        return;
    }
    PictureTableRecord* pictureRecord = database.GetRecord(recordIndex)->GetPictureTableRecord();
    ImageInfo& info = pictureRecord->m_ImageInfo;
    pictureRecord->Initialize(kind);
    ImageDbRecordParam param;
    param.Initialize();
    util::StoreImageDbRecordParam2ImageInfo(info, param);
    info.SetValidityFlag(true);
    info.SetExt(kind);
    info.SetFileName(index);
    info.SetDirectoryName(index, false);
    util::CreateImagePath(path, IMAGE_PATH_SIZE, storage, info, false);
    info.m_FileSize = util::GetFileSize(storage, path, NULL);
    util::ExtractPictureInfoFromFile(*pictureRecord, storage, info, false);
}

// 0x005A9210 | nintendogs:bytes [tier B]
void imgdb::ImageDbRestore::SyncPictureDatabaseRecordAvailableTwlOnly(StorageType storage, PictureDbRecord& record)
{
    PictureTableRecord* pictureRecord = record.GetPictureTableRecord();
    LegacyTableRecord* legacyRecord = record.GetLegacyTableRecord();
    ImageInfo& info = pictureRecord->m_ImageInfo;
    info.Initialize();
    ImageDbRecordParam param;
    param.Initialize();
    util::StoreImageDbRecordParam2ImageInfo(info, param);
    info.SetValidityFlag(true);
    info.SetExt(IMAGE_KIND_JPEG);
    info.SetFileName(legacyRecord->GetIndexInfo());
    info.SetDirectoryName(legacyRecord->GetIndexInfo(), false);
    info.m_DateTime[0] = legacyRecord->m_DateTime;
    info.m_DateTime[1] = legacyRecord->m_DateTime;
    ImageInfo legacyInfo;
    util::GenerateImageInfoFromImageInfo(legacyInfo, info, true);
    PhotoImageInfoReader reader(*GetFileAllocator(), storage, legacyInfo, true);
    if (reader.ReadImage() == RESULT_SUCCESS)
    {
        ImageFileWriter writer(*GetFileAllocator(), storage, info, false, reader.GetData(), reader.GetDataSize());
        writer.WriteImage();
        info.m_FileSize = reader.GetFileSize();
    }
}

// 0x005A9358 | nintendogs:callgraph [tier C]
void imgdb::ImageDbRestore::SyncPictureDatabaseRecordAvailableCtrOnlyMp(StorageType storage, PictureDbRecord& record)
{
    PictureTableRecord* pictureRecord = record.GetPictureTableRecord();
    LegacyTableRecord* legacyRecord = record.GetLegacyTableRecord();
    legacyRecord->Initialize();
    legacyRecord->m_IsUsed = true;
    legacyRecord->SetIndexInfo(pictureRecord->m_ImageInfo.GetIndexInfo());
    legacyRecord->m_DateTime = pictureRecord->m_ImageInfo.m_DateTime[0];
    PhotoImageInfoReader reader(*GetFileAllocator(), storage, pictureRecord->m_ImageInfo, false);
    if (reader.ReadImage() != RESULT_SUCCESS)
    {
        return;
    }
    void* twlData = NULL;
    u32 twlSize = 0;
    if (util::CreateTwlJpegFromJpeg(twlData, twlSize, reader.GetData(), reader.GetDataSize()))
    {
        ImageInfo twlInfo;
        util::GenerateImageInfoFromImageInfo(twlInfo, pictureRecord->m_ImageInfo, IMAGE_KIND_JPEG, true);
        ImageFileWriter writer(*GetFileAllocator(), storage, twlInfo, true, twlData, twlSize);
        writer.WriteImage();
    }
    if (twlData != NULL)
    {
        GetAllocator()->Free(twlData);
    }
}

// (the same code as SyncPictureDatabaseRecordAvailableCtrOnlyMp)
// 0x005A9498 | nintendogs:callgraph [tier C]
DECOMP_NOIPA void imgdb::ImageDbRestore::SyncPictureDatabaseRecordAvailableCtrOnlyJpeg(StorageType storage, PictureDbRecord& record)
{
    PictureTableRecord* pictureRecord = record.GetPictureTableRecord();
    LegacyTableRecord* legacyRecord = record.GetLegacyTableRecord();
    legacyRecord->Initialize();
    legacyRecord->m_IsUsed = true;
    legacyRecord->SetIndexInfo(pictureRecord->m_ImageInfo.GetIndexInfo());
    legacyRecord->m_DateTime = pictureRecord->m_ImageInfo.m_DateTime[0];
    PhotoImageInfoReader reader(*GetFileAllocator(), storage, pictureRecord->m_ImageInfo, false);
    if (reader.ReadImage() != RESULT_SUCCESS)
    {
        return;
    }
    void* twlData = NULL;
    u32 twlSize = 0;
    if (util::CreateTwlJpegFromJpeg(twlData, twlSize, reader.GetData(), reader.GetDataSize()))
    {
        ImageInfo twlInfo;
        util::GenerateImageInfoFromImageInfo(twlInfo, pictureRecord->m_ImageInfo, IMAGE_KIND_JPEG, true);
        ImageFileWriter writer(*GetFileAllocator(), storage, twlInfo, true, twlData, twlSize);
        writer.WriteImage();
    }
    if (twlData != NULL)
    {
        GetAllocator()->Free(twlData);
    }
}

} // namespace imgdb
