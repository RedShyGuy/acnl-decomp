#include "imgdb/ImageDatabase.h"
#include "imgdb/ImageDbRecordParam.h"
#include "imgdb/ImageDbRestore.h"
#include "imgdb/ImageFileWriter.h"
#include "imgdb/PhotoImageInfoReader.h"
#include "imgdb/TwlNandArchiveMounter.h"
#include "imgdb/imgdb_Api.h"
#include "imgdb/util/util_Api.h"

namespace imgdb {

// 0x00983140
template <>
ImageDatabase* Singleton<imgdb::ImageDatabase>::s_pInstance = NULL;
// 0x00983144
template <>
Allocator* Singleton<imgdb::ImageDatabase>::s_pAllocator = NULL;

// 0x0081611C
// 0x0081610C (deleting dtor)
template imgdb::Singleton<imgdb::ImageDatabase>::~Singleton();

// 0x0014013C | nintendogs:bytes [tier B]
template void imgdb::DynamicArray<imgdb::ImageDbRecordLink, imgdb::XAllocator<imgdb::ImageDbRecordLink> >::Refresh();
// 0x00142A9C | nintendogs:bytes [tier B]
template void imgdb::DynamicArray<imgdb::ImageDbRecordLink, imgdb::XAllocator<imgdb::ImageDbRecordLink> >::Recreate(u32);

namespace {

// the length of the file and directory names with the terminator
const int DCF_NAME_SIZE = 9;
// a path of an image file
const int IMAGE_PATH_SIZE = 44;

} // namespace

DECOMP_ALWAYS_INLINE bool imgdb::ImageDatabase::IsLessRecordLink(const ImageDbRecordLink& a, const ImageDbRecordLink& b) const
{
    if (a.GetDays() != b.GetDays())
    {
        return a.GetDays() < b.GetDays();
    }
    const ImageDbRecord* recordA = FindRecord(a);
    const ImageDbRecord* recordB = FindRecord(b);
    DateTimeSeconds dateTimeA = recordA != NULL && recordA->GetImageInfo()->GetValidityFlag()
                                    ? recordA->GetImageInfo()->m_DateTime[0]
                                    : DateTimeSeconds::GetMinSeconds();
    DateTimeSeconds dateTimeB = recordB != NULL && recordB->GetImageInfo()->GetValidityFlag()
                                    ? recordB->GetImageInfo()->m_DateTime[0]
                                    : DateTimeSeconds::GetMinSeconds();
    if (dateTimeA.m_Seconds != dateTimeB.m_Seconds)
    {
        return dateTimeA.m_Seconds < dateTimeB.m_Seconds;
    }
    // (the records are not checked here)
    s64 savedA = recordA->GetImageInfo()->m_DateTime[1].m_Seconds;
    s64 savedB = recordB->GetImageInfo()->m_DateTime[1].m_Seconds;
    if (savedA != savedB)
    {
        return savedA < savedB;
    }
    if (a.GetImageKind() != b.GetImageKind())
    {
        return a.GetImageKind() < b.GetImageKind();
    }
    if (a.GetStorageType() != b.GetStorageType())
    {
        return a.GetStorageType() < b.GetStorageType();
    }
    return recordA < recordB;
}

// the list entries of the valid records of a storage
DECOMP_ALWAYS_INLINE void imgdb::ImageDatabase::AddRecordLinks(StorageType storage)
{
    PictureDatabase& database = m_Databases[storage];
    int recordNum = database.m_RecordNum;
    for (int i = 0; i < recordNum; i++)
    {
        const ImageInfo& info = database.GetRecord(i)->GetPictureTableRecord()->m_ImageInfo;
        if (!info.GetValidityFlag())
        {
            continue;
        }
        ImageDbRecordLink link;
        link.Initialize();
        link.SetStorageType(storage);
        link.SetDbRecordIndex(i);
        link.SetImageKind(info.GetImageKind());
        link.SetIndexInfo(info.GetIndexInfo());
        link.SetDays(info.m_DateTime[0].GetDateDays());
        InsertRecordLinkList(m_RecordLinks, link);
    }
}

// 0x005A5C30 | nintendogs:bytes [tier B]
void imgdb::ImageDatabase::Initialize(bool isMountNand, bool isMountSd)
{
    if (m_IsInitialized)
    {
        return;
    }
    m_RecordLinks.SetMinCapacity(CTR_NAND_RECORD_NUM + SD_RECORD_NUM);
    m_RecordLinks.Refresh();
    if (isMountNand)
    {
        m_Databases[STORAGE_TYPE_CTR_NAND].Initialize(*GetAllocator(), CTR_NAND_RECORD_NUM, STORAGE_TYPE_CTR_NAND);
        ResetTable(STORAGE_TYPE_CTR_NAND);
    }
    if (isMountSd)
    {
        m_Databases[STORAGE_TYPE_SD].Initialize(*GetAllocator(), SD_RECORD_NUM, STORAGE_TYPE_SD);
        ResetTable(STORAGE_TYPE_SD);
    }
    m_IsInitialized = true;
}

// 0x0013CA34 (name is ours)
void imgdb::ImageDatabase::Finalize()
{
    if (!m_IsInitialized)
    {
        return;
    }
    m_RecordLinks.Resize(0, ImageDbRecordLink());
    m_RecordLinks.SetMinCapacity(0);
    m_RecordLinks.Refresh();
    for (int i = 0; i < 2; i++)
    {
        m_Databases[static_cast<StorageType>(i)].Finalize();
    }
    m_IsInitialized = false;
}

// 0x005A5CDC | nintendogs:bytes [tier B]
Result imgdb::ImageDatabase::RegisterMp(StorageType storage, const void* data, u32 size,
                                        const ImageDbRecordParam& param, ImageDbRecordLink* link)
{
    return RegisterImage(storage, IMAGE_KIND_MP, data, size, NULL, 0, param, true, NULL, link);
}

// (the save counter of the picture table is kept)
// 0x005A5D28 | nintendogs:bytes [tier B]
void imgdb::ImageDatabase::ResetTable(StorageType storage)
{
    for (u32 i = 0; i < m_RecordLinks.GetSize(); i++)
    {
        if (m_RecordLinks[i].GetStorageType() == storage)
        {
            m_RecordLinks.Erase(&m_RecordLinks[i]);
            i--;
        }
    }
    if (storage == STORAGE_TYPE_CTR_NAND)
    {
        PictureDatabase& database = m_Databases[STORAGE_TYPE_CTR_NAND];
        if (!database.IsInitialized())
        {
            return;
        }
        database.InitializeValidityStateTable();
        u32 saveCount = database.m_pPictureData != NULL ? database.m_PictureTable.GetHeader()->m_SaveCount : 0;
        database.InitializePictureTable();
        database.m_PictureTable.GetHeader()->m_SaveCount = saveCount;
        if (TwlNandArchiveMounter::GetInstance()->IsMount())
        {
            database.InitializeLegacyTable();
        }
    }
    else if (storage == STORAGE_TYPE_SD)
    {
        PictureDatabase& database = m_Databases[STORAGE_TYPE_SD];
        if (!database.IsInitialized())
        {
            return;
        }
        database.InitializeValidityStateTable();
        u32 saveCount = database.m_pPictureData != NULL ? database.m_PictureTable.GetHeader()->m_SaveCount : 0;
        database.InitializePictureTable();
        database.m_PictureTable.GetHeader()->m_SaveCount = saveCount;
    }
}

// (RESULT_NOT_FOUND: the tables cannot be read but there are images, the database must be restored)
// 0x005A5EAC | nintendogs:callgraph [tier C]
Result imgdb::ImageDatabase::LoadDatabase(StorageType storage)
{
    m_IsCancelRequested = false;
    PictureDatabase& database = m_Databases[storage];
    if (!database.IsInitialized())
    {
        return RESULT_SUCCESS;
    }
    if (database.IsInitialized() && m_IsRestoreRequired[storage])
    {
        return RESULT_NOT_FOUND;
    }
    ResetTable(storage);
    Result result = database.LoadPictureTable(storage, util::GetPictureDbFilePath(storage));
    database.InitializeValidityStateTable();
    if (database.m_pLegacyData != NULL)
    {
        Result legacyResult = database.LoadLegacyTable(storage, util::GetLegacyPictureDbFilePath(storage));
        if (legacyResult != RESULT_SUCCESS)
        {
            result = legacyResult;
        }
    }
    if (result != RESULT_SUCCESS)
    {
        if (util::IsExistImageDataEvidence(storage))
        {
            m_IsRestoreRequired[storage] = true;
            return RESULT_NOT_FOUND;
        }
        return result;
    }
    if (database.m_pLegacyData != NULL)
    {
        result = SyncPictureDatabaseWithTwl(storage);
        if (result != RESULT_SUCCESS)
        {
            if (result == RESULT_DATABASE_BROKEN)
            {
                m_IsRestoreRequired[storage] = true;
            }
            return result;
        }
    }
    AddRecordLinks(storage);
    return RESULT_SUCCESS;
}

// 0x005A60B4 | nintendogs:bytes [tier B]
Result imgdb::ImageDatabase::RegisterJpeg(StorageType storage, const void* data, u32 size,
                                          const ImageDbRecordParam& param, ImageDbRecordLink* link)
{
    return RegisterImage(storage, IMAGE_KIND_JPEG, data, size, NULL, 0, param, true, NULL, link);
}

// 0x005A6100 | nintendogs:bytes [tier B]
DECOMP_NOINLINE Result imgdb::ImageDatabase::SaveDatabase(StorageType storage)
{
    Result result = WritePictureDatabase(storage, false, false);
    if (m_Databases[storage].m_pLegacyData != NULL)
    {
        Result legacyResult = WritePictureDatabase(storage, true, false);
        if (result == RESULT_SUCCESS)
        {
            result = legacyResult;
        }
    }
    return result;
}

// (on a failure the record is removed again)
// 0x005A6154 | nintendogs:bytes [tier B]
Result imgdb::ImageDatabase::RegisterImage(StorageType storage, ImageKind kind, const void* data, u32 size,
                                           const void* subData, u32 subSize, const ImageDbRecordParam& param,
                                           bool isSave, const IndexInfo* index, ImageDbRecordLink* link)
{
    ImageDbRecordLink newLink;
    PictureDatabase& database = m_Databases[storage];
    if (!database.IsInitialized())
    {
        return RESULT_NOT_READY;
    }
    if (database.IsInitialized() && m_IsRestoreRequired[storage])
    {
        return RESULT_NOT_FOUND;
    }
    if (util::IsDbFileReadOnly(storage))
    {
        return RESULT_DATABASE_READ_ONLY;
    }
    Result result;
    if (index != NULL)
    {
        int recordIndex = database.CreateNewRecord(storage, kind, *index);
        if (recordIndex < 0)
        {
            return RESULT_DATABASE_FULL;
        }
        PictureDbRecord* record = database.GetRecord(recordIndex);
        record->GetImageInfo()->m_DateTime[1] = param.m_DateTime;
        record->GetImageInfo()->m_FileSize = size;
        StorePictureDbRecordFromParam(*record, param, kind);
        newLink.SetDbRecordIndex(recordIndex);
        newLink.SetIndexInfo(record->GetImageInfo()->GetIndexInfo());
        newLink.SetStorageType(storage);
        newLink.SetImageKind(kind);
        newLink.SetDays(param.m_DateTime.GetDateDays());
        result = RESULT_SUCCESS;
    }
    else
    {
        result = CreateRecordLink(newLink, storage, param.m_DateTime, kind, param, size);
        if (result != RESULT_SUCCESS)
        {
            return result;
        }
    }
    InsertRecordLinkList(m_RecordLinks, newLink);
    if (isSave)
    {
        result = SaveDatabase(storage);
        if (result != RESULT_SUCCESS)
        {
            UnregisterImage(newLink);
            return result;
        }
    }
    if (data != NULL && size != 0)
    {
        result = util::SaveImage(storage, *FindRecord(newLink)->GetImageInfo(), data, size, subData, subSize);
        if (result != RESULT_SUCCESS)
        {
            UnregisterImage(newLink);
            return result;
        }
    }
    if (link != NULL)
    {
        *link = newLink;
    }
    return result;
}

// 0x005A63B4 | nintendogs:bytes [tier B]
Result imgdb::ImageDatabase::CreateDatabase(StorageType storage)
{
    Result result = RESULT_SUCCESS;
    if (!m_Databases[storage].IsInitialized())
    {
        return result;
    }
    ResetTable(storage);
    if (util::IsDbFileReadOnly(storage))
    {
        return RESULT_DATABASE_READ_ONLY;
    }
    result = SaveDatabase(storage);
    if (result != RESULT_SUCCESS)
    {
        return result;
    }
    m_IsRestoreRequired[storage] = false;
    return result;
}

// (the tables that could not be read are first written with an invalid CRC)
// 0x005A6424 | nintendogs:bytes [tier B]
Result imgdb::ImageDatabase::RestoreDatabase(StorageType storage)
{
    Result result = RESULT_SUCCESS;
    PictureDatabase& database = m_Databases[storage];
    if (!database.IsInitialized())
    {
        return result;
    }
    ResetTable(storage);
    Result legacyResult = RESULT_SUCCESS;
    if (database.m_pLegacyData != NULL)
    {
        legacyResult = database.LoadLegacyTable(storage, util::GetLegacyPictureDbFilePath(storage));
    }
    Result pictureResult = database.LoadPictureTable(storage, util::GetPictureDbFilePath(storage));
    database.InitializeValidityStateTable();
    if (legacyResult != RESULT_SUCCESS || pictureResult != RESULT_SUCCESS)
    {
        if (legacyResult != RESULT_SUCCESS)
        {
            if (pictureResult != RESULT_SUCCESS)
            {
                ResetTable(storage);
            }
            Result writeResult = WritePictureDatabase(storage, true, true);
            if (writeResult != RESULT_SUCCESS)
            {
                result = writeResult;
            }
        }
        if (pictureResult != RESULT_SUCCESS)
        {
            Result writeResult = WritePictureDatabase(storage, false, true);
            if (writeResult != RESULT_SUCCESS)
            {
                return writeResult;
            }
        }
        if (result != RESULT_SUCCESS)
        {
            return result;
        }
        Result restoreResult = ImageDbRestore::RestorePictureDatabase(storage, legacyResult == RESULT_SUCCESS,
                                                                      pictureResult == RESULT_SUCCESS, database);
        if (restoreResult != RESULT_SUCCESS)
        {
            return restoreResult;
        }
    }
    AddRecordLinks(storage);
    result = SaveDatabase(storage);
    if (result != RESULT_SUCCESS)
    {
        return result;
    }
    m_IsRestoreRequired[storage] = false;
    return result;
}

// 0x005A6664 | nintendogs:bytes [tier B]
Result imgdb::ImageDatabase::UnregisterImage(const ImageDbRecordLink& link)
{
    StorageType linkStorage = link.GetStorageType();
    if (m_Databases[linkStorage].IsInitialized() && m_IsRestoreRequired[linkStorage])
    {
        return RESULT_NOT_FOUND;
    }
    if (util::IsDbFileReadOnly(link.GetStorageType()))
    {
        return RESULT_DATABASE_READ_ONLY;
    }
    StorageType storage = link.GetStorageType();
    ImageDbRecord* record = FindRecord(link);
    if (record == NULL)
    {
        return RESULT_SUCCESS;
    }
    Result result = util::DeleteImage(storage, *record->GetImageInfo(), true);
    if (result != RESULT_SUCCESS)
    {
        return result;
    }
    UnregisterRecord(link.GetStorageType(), link.GetDbRecordIndex(), *record);
    SaveDatabase(storage);
    return RESULT_SUCCESS;
}

// (RESULT_NO_FILE_INDEX: there are empty records but no free file number)
// 0x005A6754 | nintendogs:bytes [tier B]
Result imgdb::ImageDatabase::CreateRecordLink(ImageDbRecordLink& link, StorageType storage,
                                              const DateTimeSeconds& dateTime, ImageKind kind,
                                              const ImageDbRecordParam& param, s64 size)
{
    PictureDatabase& database = m_Databases[storage];
    int recordIndex = database.CreateNewRecord(storage, kind);
    if (recordIndex < 0)
    {
        return database.GetEmptyRecordNum() > 0 ? RESULT_NO_FILE_INDEX : RESULT_DATABASE_FULL;
    }
    PictureDbRecord* record = database.GetRecord(recordIndex);
    record->GetImageInfo()->m_DateTime[1] = dateTime;
    record->GetImageInfo()->m_FileSize = size;
    StorePictureDbRecordFromParam(*record, param, kind);
    link.SetDbRecordIndex(recordIndex);
    link.SetIndexInfo(record->GetImageInfo()->GetIndexInfo());
    link.SetStorageType(storage);
    link.SetImageKind(kind);
    link.SetDays(param.m_DateTime.GetDateDays());
    return RESULT_SUCCESS;
}

// 0x005A6870 | nintendogs:bytes [tier B]
Result imgdb::ImageDatabase::UnregisterRecord(StorageType storage, int index, ImageDbRecord& record)
{
    for (ImageDbRecordLink* it = m_RecordLinks.begin(); it != m_RecordLinks.end(); it++)
    {
        if (it->GetDbRecordIndex() == index && it->GetStorageType() == storage)
        {
            m_RecordLinks.Erase(it);
            break;
        }
    }
    record.GetValidityStateTableRecord()->Initialize();
    PictureDbRecord& pictureRecord = static_cast<PictureDbRecord&>(record);
    pictureRecord.GetPictureTableRecord()->Initialize(IMAGE_KIND_NONE);
    LegacyTableRecord* legacyRecord = pictureRecord.GetLegacyTableRecord();
    if (legacyRecord != NULL)
    {
        legacyRecord->Initialize();
    }
    return RESULT_SUCCESS;
}

// (an image that the DSi camera deleted is deleted, one that it saved is taken over with its file)
// 0x005A6984 | nintendogs:bytes [tier B]
Result imgdb::ImageDatabase::SyncPictureDatabaseCore(StorageType storage)
{
    PictureDatabase& database = m_Databases[storage];
    int recordNum = database.m_RecordNum;
    for (int i = 0; i < recordNum; i++)
    {
        if (m_IsCancelRequested)
        {
            return RESULT_DATABASE_BROKEN;
        }
        PictureDbRecord* record = database.GetRecord(i);
        LegacyTableRecord* legacyRecord = record->GetLegacyTableRecord();
        PictureTableRecord* pictureRecord = record->GetPictureTableRecord();
        ImageInfo& info = pictureRecord->m_ImageInfo;
        if (!legacyRecord->m_IsUsed)
        {
            if (info.GetValidityFlag())
            {
                util::DeleteImageCtr(storage, info, true);
                pictureRecord->Initialize(IMAGE_KIND_NONE);
                Result result = database.SavePictureTable(storage, util::GetPictureDbFilePath(storage), false);
                if (result != RESULT_SUCCESS)
                {
                    return result;
                }
            }
            continue;
        }
        if (info.GetValidityFlag() && info.GetIndexInfo() == legacyRecord->GetIndexInfo() &&
            legacyRecord->m_DateTime.m_Seconds == info.m_DateTime[0].m_Seconds)
        {
            continue;
        }
        if (info.GetValidityFlag())
        {
            util::DeleteImageCtr(storage, info, true);
        }
        wchar_t fileName[DCF_NAME_SIZE];
        util::CreateImageFileName(fileName, DCF_NAME_SIZE, legacyRecord->GetIndexInfo(), false);
        wchar_t directoryName[DCF_NAME_SIZE];
        util::CreateImageDirectoryName(directoryName, DCF_NAME_SIZE, legacyRecord->GetIndexInfo(), false);
        info.Initialize();
        ImageDbRecordParam param;
        param.Initialize();
        util::StoreImageDbRecordParam2ImageInfo(info, param);
        info.SetValidityFlag(true);
        info.SetDirectoryName(directoryName);
        info.SetFileName(fileName);
        info.SetExt(IMAGE_KIND_JPEG);
        info.m_DateTime[0] = legacyRecord->m_DateTime;
        info.m_DateTime[1] = legacyRecord->m_DateTime;
        wchar_t path[IMAGE_PATH_SIZE];
        util::CreateImagePath(path, IMAGE_PATH_SIZE, STORAGE_TYPE_CTR_NAND, IMAGE_KIND_JPEG,
                              legacyRecord->GetIndexInfo(), true);
        info.m_FileSize = util::GetFileSize(STORAGE_TYPE_CTR_NAND, path, NULL);
        Result result = database.SavePictureTable(storage, util::GetPictureDbFilePath(storage), false);
        if (result != RESULT_SUCCESS)
        {
            return result;
        }
        if (storage != STORAGE_TYPE_CTR_NAND)
        {
            continue;
        }
        // a copy of the photo in the directories of the camera
        ImageInfo legacyInfo;
        util::GenerateImageInfoFromImageInfo(legacyInfo, info, true);
        PhotoImageInfoReader reader(*GetFileAllocator(), storage, legacyInfo, true);
        if (reader.ReadImage() == RESULT_SUCCESS)
        {
            ImageFileWriter writer(*GetFileAllocator(), storage, info, false, reader.GetData(), reader.GetDataSize());
            writer.WriteImage();
        }
    }
    return RESULT_SUCCESS;
}

// 0x005A6D00 | nintendogs:bytes [tier B]
Result imgdb::ImageDatabase::SyncPictureDatabaseWithTwl(StorageType storage)
{
    PictureDatabase& database = m_Databases[storage];
    if (database.m_pLegacyData == NULL)
    {
        return RESULT_SUCCESS;
    }
    Result result = SortPictureDatabaseCtrAndTwl(storage);
    if (result != RESULT_SUCCESS)
    {
        return result;
    }
    bool isChanged = false;
    int recordNum = database.m_RecordNum;
    for (int i = 0; i < recordNum; i++)
    {
        if (m_IsCancelRequested)
        {
            SaveDatabase(storage);
            return RESULT_DATABASE_BROKEN;
        }
        PictureDbRecord* record = database.GetRecord(i);
        if (ImageDbRestore::DeleteFileIfZeroByte(storage, *record, NULL, NULL))
        {
            ImageDbRestore::SyncPictureDatabaseRecord(storage, *record);
            isChanged = true;
        }
    }
    if (isChanged)
    {
        result = SaveDatabase(storage);
        if (result != RESULT_SUCCESS)
        {
            return result;
        }
    }
    result = SyncPictureDatabaseCore(storage);
    if (result != RESULT_SUCCESS)
    {
        return result;
    }
    return RESULT_SUCCESS;
}

// 0x005A6E38 | nintendogs:bytes [tier B]
Result imgdb::ImageDatabase::SortPictureDatabaseCtrAndTwl(StorageType storage)
{
    PictureDatabase& database = m_Databases[storage];
    bool isChanged = false;
    int recordNum = database.m_RecordNum;
    for (int i = 0; i < recordNum; i++)
    {
        if (m_IsCancelRequested)
        {
            return RESULT_DATABASE_BROKEN;
        }
        PictureDbRecord* record = database.GetRecord(i);
        LegacyTableRecord* legacyRecord = record->GetLegacyTableRecord();
        const ImageInfo& info = record->GetPictureTableRecord()->m_ImageInfo;
        if (!legacyRecord->m_IsUsed)
        {
            continue;
        }
        // (the picture record has no image or the one of another index)
        if (!info.GetValidityFlag() || info.GetIndexInfo() == legacyRecord->GetIndexInfo())
        {
            // (the conditions are tested again)
            if (!legacyRecord->m_IsUsed || info.GetValidityFlag())
            {
                continue;
            }
        }
        for (int j = 0; j < recordNum; j++)
        {
            if (j == i)
            {
                continue;
            }
            PictureDbRecord* other = database.GetRecord(j);
            LegacyTableRecord* otherLegacyRecord = other->GetLegacyTableRecord();
            const ImageInfo& otherInfo = other->GetPictureTableRecord()->m_ImageInfo;
            if (!otherInfo.GetValidityFlag())
            {
                continue;
            }
            if (otherInfo.GetIndexInfo() == legacyRecord->GetIndexInfo())
            {
                LegacyTableRecord temp = *legacyRecord;
                *legacyRecord = *otherLegacyRecord;
                *otherLegacyRecord = temp;
                isChanged = true;
                i--;
                break;
            }
        }
    }
    if (isChanged)
    {
        Result result = SaveDatabase(storage);
        if (result != RESULT_SUCCESS)
        {
            return result;
        }
    }
    return RESULT_SUCCESS;
}

// 0x005A6FFC | nintendogs:bytes [tier B]
void imgdb::ImageDatabase::StorePictureDbRecordFromParam(PictureDbRecord& record, const ImageDbRecordParam& param,
                                                         ImageKind kind)
{
    ImageInfo* info = record.GetImageInfo();
    info->m_DateTime[0] = param.m_DateTime;
    info->m_TitleUniqueId = param.m_TitleUniqueId;
    info->SetHandleTypeBit(static_cast<HandleTypeBit>(param.m_HandleTypeBit));
    info->SetExt(kind);
    info->SetShootingType(static_cast<ShootingType>(param.m_ShootingType));
    info->SetDistinctionTypeBit(static_cast<DistinctionTypeBit>(param.m_DistinctionTypeBit));
    info->SetSaveProcessType(static_cast<SaveProcessType>(param.m_SaveProcessType));
    for (int i = 0; i < BODY_ID_TYPE_NUM; i++)
    {
        BodyIdType type = static_cast<BodyIdType>(i);
        if (param.IsValidBodyId(type))
        {
            info->SetBodyId(type, param.GetBodyId(type));
        }
        else
        {
            info->ResetBodyId(type);
        }
    }
    LegacyTableRecord* legacyRecord = record.GetLegacyTableRecord();
    if (legacyRecord != NULL)
    {
        legacyRecord->m_DateTime = record.GetImageInfo()->m_DateTime[0];
    }
    PictureTableRecord* pictureRecord = record.GetPictureTableRecord();
    for (int i = 0; i < PictureTableRecord::FACE_NUM; i++)
    {
        pictureRecord->m_Faces[i] = param.m_Faces[i];
    }
}

// 0x005A71BC | nintendogs:bytes-fuzzy [tier B]
imgdb::ImageDatabase::ImageDatabase() : m_IsInitialized(false), m_IsCancelRequested(false)
{
    m_IsRestoreRequired[STORAGE_TYPE_CTR_NAND] = false;
    m_IsRestoreRequired[STORAGE_TYPE_SD] = false;
}

// 0x005A72B8
// 0x005A7218 (deleting dtor)
imgdb::ImageDatabase::~ImageDatabase()
{
    // (only the members are destroyed)
}

// 0x005AA904 | nintendogs:bytes [tier B]
DECOMP_NOINLINE Result imgdb::ImageDatabase::WritePictureDatabase(StorageType storage, bool isLegacy, bool isBreakCrc)
{
    PictureDatabase& database = m_Databases[storage];
    if (isLegacy)
    {
        return database.SaveLegacyTable(storage, util::GetLegacyPictureDbFilePath(storage), isBreakCrc);
    }
    return database.SavePictureTable(storage, util::GetPictureDbFilePath(storage), isBreakCrc);
}

// 0x007550BC | nintendogs:bytes [tier B]
ImageDbRecord* imgdb::ImageDatabase::FindRecord(const ImageDbRecordLink& link) const
{
    PictureDbRecord* record = m_Databases[link.GetStorageType()].GetRecord(link.GetDbRecordIndex());
    if (!static_cast<const ImageDbRecord*>(record)->GetImageInfo()->GetValidityFlag())
    {
        return NULL;
    }
    return record;
}

// 0x0075511C | nintendogs:bytes [tier B]
int imgdb::ImageDatabase::GetEmptyImageNum(StorageType storage) const
{
    const PictureDatabase& database = m_Databases[storage];
    if (!database.IsInitialized())
    {
        return 0;
    }
    return database.GetEmptyRecordNum();
}

// 0x00755148 | nintendogs:bytes [tier B]
DECOMP_NOINLINE void imgdb::ImageDatabase::InsertRecordLinkList(DynamicArray<ImageDbRecordLink, XAllocator<ImageDbRecordLink> >& list,
                                                const ImageDbRecordLink& link) const
{
    list.Insert(FindRecordLinkListInsertPos(list, link), link);
}

// (a binary search)
// 0x00755204 | nintendogs:bytes-fuzzy [tier B]
u32 imgdb::ImageDatabase::FindRecordLinkListInsertPos(
    const DynamicArray<ImageDbRecordLink, XAllocator<ImageDbRecordLink> >& list, const ImageDbRecordLink& link) const
{
    if (list.GetSize() == 0)
    {
        return 0;
    }
    u32 low = 0;
    u32 high = list.GetSize();
    while (low < high)
    {
        u32 middle = (low + high) / 2;
        if (IsLessRecordLink(list[middle], link))
        {
            low = middle + 1;
        }
        else
        {
            high = middle;
        }
    }
    return low;
}

} // namespace imgdb
