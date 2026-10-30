#include "imgdb/ArchiveMounter.h"
#include "imgdb/ImageDatabase.h"
#include "imgdb/JpegInMJpegFileReader.h"
#include "imgdb/PhotoFileReader.h"
#include "imgdb/PhotoHeaderFileReader.h"
#include "imgdb/imgdb_Api.h"

namespace imgdb {

namespace {

// 0x0097F0B8
Allocator s_Allocator;
// 0x0097F0BC
Allocator s_FileAllocator;
// 0x0097F0F8
bool s_IsSystemInitialized;
// 0x0097F0F9
bool s_IsDatabaseInitialized;
const u32 MJPEG_LIMIT_FILE_SIZE = 409600;

// a list of records that nothing in this binary creates; Finalize destroys it (names are ours)
struct RecordLinkList
{
    u32 _00[2];                                                        // +0x00
    DynamicArray<ImageDbRecordLink, XAllocator<ImageDbRecordLink> > m_Links;  // +0x08
};

// 0x009826DC
RecordLinkList* s_pRecordLinkList;

} // namespace

// (not in the unnamed namespace: the original reads it although this binary never sets it)
// 0x0097F0FA
bool s_IsUnlimitedCtrNandFreeSpace;

// (the allocators are objects with a destructor)
// 0x0079A5CC (static initializer)

// 0x005B0994 (name is ours)
imgdb::Allocator::~Allocator()
{
    m_pAllocator = NULL;
}

// (falls into FinalizeEx)
// 0x0013979C (name is ours)
void Finalize()
{
    FinalizeEx();
}

// 0x001397A0 (name is ours)
DECOMP_NOINLINE void FinalizeEx()
{
    DestroyRecordLinkList();
    if (s_IsDatabaseInitialized)
    {
        ImageDatabase::GetInstance()->Finalize();
        s_IsDatabaseInitialized = false;
    }
    FinalizeSystem();
}

// (the archives are unmounted, the singletons destroyed)
// 0x0013CAC4 (name is ours)
DECOMP_NOINLINE void FinalizeSystem()
{
    if (!s_IsSystemInitialized)
    {
        return;
    }
    SdArchiveMounter::GetInstance()->Unmount();
    CtrNandArchiveMounter::GetInstance()->Unmount();
    TwlNandArchiveMounter::GetInstance()->Unmount();
    TwlNandArchiveMounter::DestroyInstance();
    CtrNandArchiveMounter::DestroyInstance();
    SdArchiveMounter::DestroyInstance();
    ImageDatabase::DestroyInstance();
    ResetAllocator();
    s_IsSystemInitialized = false;
}

// 0x0013CC40 (name is ours)
DECOMP_NOINLINE void DestroyRecordLinkList()
{
    if (s_pRecordLinkList == NULL)
    {
        return;
    }
    Allocator* allocator = GetAllocator();
    if (s_pRecordLinkList != NULL)
    {
        s_pRecordLinkList->~RecordLinkList();
        allocator->Free(s_pRecordLinkList);
    }
    s_pRecordLinkList = NULL;
}

// 0x0013F8B8 (name is ours)
DECOMP_NOINLINE void ResetAllocator()
{
    s_Allocator.m_pAllocator = NULL;
    s_FileAllocator.m_pAllocator = NULL;
}

// (the NAND is not mounted, the SD card is)
// 0x005A49F0 | nintendogs:bytes [tier B]
bool Initialize(nn::fnd::IAllocator& allocator)
{
    Result nandResult;
    Result sdResult;
    return InitializeEx(allocator, allocator, false, true, MJPEG_LIMIT_FILE_SIZE, nandResult, sdResult);
}

// 0x005A57DC | nintendogs:bytes [tier B]
bool InitializeEx(nn::fnd::IAllocator& allocator, nn::fnd::IAllocator& fileAllocator, bool isMountNand,
                  bool isMountSd, u32 limitFileSize, Result& nandResult, Result& sdResult)
{
    InitializeSystem(allocator, fileAllocator, isMountNand, isMountSd, limitFileSize);
    Result* results[2] = {&nandResult, &sdResult};
    sdResult = RESULT_SUCCESS;
    nandResult = RESULT_SUCCESS;
    if (!s_IsDatabaseInitialized)
    {
        ImageDatabase* database = ImageDatabase::GetInstance();
        database->Initialize(isMountNand, isMountSd);
        for (int i = 0; i < 2; i++)
        {
            StorageType storage = static_cast<StorageType>(i);
            Result* result = results[i];
            Result loadResult = database->LoadDatabase(storage);
            if (loadResult == RESULT_SUCCESS)
            {
                continue;
            }
            if (loadResult == RESULT_NOT_FOUND)
            {
                // no database: the one of the NAND can be rebuilt from the files
                if (storage == STORAGE_TYPE_CTR_NAND)
                {
                    loadResult = database->RestoreDatabase(storage);
                }
                else if (storage == STORAGE_TYPE_SD)
                {
                    loadResult = database->CreateDatabase(storage);
                }
                else
                {
                    *result = loadResult;
                    continue;
                }
                if (loadResult == RESULT_SUCCESS)
                {
                    continue;
                }
            }
            else if (loadResult != RESULT_DATABASE_BROKEN)
            {
                loadResult = database->CreateDatabase(storage);
                if (loadResult == RESULT_SUCCESS)
                {
                    continue;
                }
            }
            // the SD card: is it in the slot and can it be mounted
            if (storage == STORAGE_TYPE_SD && loadResult == RESULT_FS_ERROR)
            {
                SdArchiveMounter* mounter = SdArchiveMounter::GetInstance();
                if (!SdArchiveMounter::IsInsert())
                {
                    loadResult = RESULT_FS_NOT_FOUND;
                }
                else if (SdArchiveMounter::IsInsert() && !mounter->IsMount())
                {
                    loadResult = RESULT_FS_ARCHIVE_ERROR;
                }
            }
            *result = loadResult;
        }
        s_IsDatabaseInitialized = true;
    }
    return nandResult == RESULT_SUCCESS && sdResult == RESULT_SUCCESS;
}

// (each allocator is only set once)
// 0x005A5AB8 | nintendogs:bytes [tier B]
void SetAllocator(nn::fnd::IAllocator* allocator, nn::fnd::IAllocator* fileAllocator)
{
    if (s_Allocator.m_pAllocator == NULL)
    {
        s_Allocator.m_pAllocator = allocator;
    }
    if (s_FileAllocator.m_pAllocator == NULL)
    {
        s_FileAllocator.m_pAllocator = fileAllocator;
    }
}

// 0x005AAC98 | nintendogs:callgraph [tier C]
Allocator* GetFileAllocator()
{
    return &s_FileAllocator;
}

// 0x005AACA4 | nintendogs:bytes [tier B]
void InitializeSystem(nn::fnd::IAllocator& allocator, nn::fnd::IAllocator& fileAllocator, bool isMountNand,
                      bool isMountSd, u32 limitFileSize)
{
    if (s_IsSystemInitialized)
    {
        return;
    }
    SetAllocator(&allocator, &fileAllocator);
    ImageDatabase::CreateInstance();
    SdArchiveMounter::CreateInstance();
    CtrNandArchiveMounter::CreateInstance();
    TwlNandArchiveMounter::CreateInstance();
    if (isMountSd)
    {
        SdArchiveMounter::GetInstance()->Mount();
    }
    if (isMountNand)
    {
        CtrNandArchiveMounter::GetInstance()->Mount();
        TwlNandArchiveMounter::GetInstance()->Mount();
    }
    PhotoFileReader::SetDefaultLimitFileSize(limitFileSize);
    PhotoHeaderFileReader::SetDefaultLimitFileSize(limitFileSize);
    JpegInMJpegFileReader::SetDefaultLimitFileSize(MJPEG_LIMIT_FILE_SIZE);
    s_IsSystemInitialized = true;
}

// 0x005ABF70 | nintendogs:callgraph [tier C]
bool GetUnlimitedCtrNandFreeSpaceFlag()
{
    return s_IsUnlimitedCtrNandFreeSpace;
}

// 0x00141D1C | nintendogs:callgraph [tier C]
Allocator* GetAllocator()
{
    return &s_Allocator;
}

} // namespace imgdb
