#include <cwchar>
#include <new>

#include "imgdb/FileReader.h"
#include "imgdb/FileWriter.h"
#include "imgdb/PictureDatabase.h"
#include "imgdb/util/util_Api.h"

namespace imgdb {

namespace {

// the capacity of a table buffer: as many extra bytes as the header allows
const u16 MAX_EXTRA_SIZE = 0xFFFF;
// the length of "nnnNIN03" with the terminator
const int DIRECTORY_NAME_SIZE = 9;

} // namespace

inline bool imgdb::PictureDatabase::FindAvailableIndexInfo(IndexInfo& index, StorageType storage) const
{
    if (storage == STORAGE_TYPE_CTR_NAND && m_pLegacyData != NULL)
    {
        return util::FindAvailableIndexInfoInNand(index, index);
    }
    return util::FindAvailableIndexInfo(index, storage, false, index);
}

inline void imgdb::PictureDatabase::InitializeRecord(int recordIndex, ImageKind kind, const IndexInfo& index)
{
    PictureDbRecord& record = m_pRecords[recordIndex];
    record.GetValidityStateTableRecord()->Initialize();
    PictureTableRecord* pictureRecord = record.GetPictureTableRecord();
    pictureRecord->Initialize(kind);
    pictureRecord->m_ImageInfo.SetValidityFlag(true);
    pictureRecord->m_ImageInfo.SetExt(kind);
    pictureRecord->m_ImageInfo.SetFileName(index);
    pictureRecord->m_ImageInfo.SetDirectoryName(index, false);
    LegacyTableRecord* legacyRecord = record.GetLegacyTableRecord();
    if (legacyRecord != NULL)
    {
        legacyRecord->Initialize();
        legacyRecord->m_IsUsed = true;
        legacyRecord->SetIndexInfo(index);
    }
}

// 0x005A9D8C | nintendogs:bytes [tier B]
void imgdb::PictureDatabase::Initialize(Allocator& allocator, int recordNum, StorageType storage)
{
    if (m_pAllocator != NULL)
    {
        return;
    }
    m_pAllocator = &allocator;
    m_RecordNum = recordNum;
    m_Storage = storage;
    if (m_RecordNum <= 0)
    {
        return;
    }
    InitializeValidityStateTable();
    PictureDbRecord* records =
        static_cast<PictureDbRecord*>(m_pAllocator->Allocate(recordNum * sizeof(PictureDbRecord), 4));
    if (records != NULL)
    {
        for (u32 i = 0; i < static_cast<u32>(recordNum); i++)
        {
            new (&records[i]) PictureDbRecord();
        }
    }
    m_pRecords = records;
    for (int i = 0; i < recordNum; i++)
    {
        m_pRecords[i].m_pDatabase = this;
        m_pRecords[i].m_Index = i;
    }
}

// (the index must not be used by a record of the same directory)
// 0x005A9E90 | nintendogs:bytes-fuzzy [tier B]
int imgdb::PictureDatabase::CreateNewRecord(StorageType storage, ImageKind kind)
{
    int recordIndex = FindEmptyRecord();
    if (recordIndex < 0)
    {
        return -1;
    }
    IndexInfo index = m_PictureTable.GetHeader()->m_IndexInfo;
    IndexInfo first;
    bool isFirst = true;
    while (true)
    {
        if (!FindAvailableIndexInfo(index, storage))
        {
            return -1;
        }
        if (isFirst)
        {
            first = index;
            isFirst = false;
        }
        else if (first == index)
        {
            return -1;
        }
        wchar_t directoryName[DIRECTORY_NAME_SIZE];
        util::CreateImageDirectoryName(directoryName, DIRECTORY_NAME_SIZE, index, false);
        bool isUsed = false;
        for (int i = 0; i < m_PictureTable.GetHeader()->m_RecordNum; i++)
        {
            const ImageInfo& info = m_PictureTable.GetRecords()[i].m_ImageInfo;
            if (!info.GetValidityFlag())
            {
                continue;
            }
            wchar_t name[DIRECTORY_NAME_SIZE];
            util::CreateImageDirectoryName(name, DIRECTORY_NAME_SIZE, info);
            if (std::wcscmp(directoryName, name) != 0)
            {
                continue;
            }
            if (index.m_FileIndex == info.GetIndexInfo().m_FileIndex)
            {
                isUsed = true;
                break;
            }
        }
        if (!isUsed)
        {
            break;
        }
        index.UpdateNextIndexByOfficial();
    }
    InitializeRecord(recordIndex, kind, index);
    IndexInfo next = index;
    next.UpdateNextIndexByOfficial();
    FindAvailableIndexInfo(next, storage);
    m_PictureTable.GetHeader()->m_IndexInfo = next;
    if (m_pLegacyData != NULL)
    {
        m_LegacyTable.GetHeader()->m_IndexInfo[0] = next;
    }
    return recordIndex;
}

// (on the CTR NAND the next free index moves on)
// 0x005AA1D4 | nintendogs:bytes [tier B]
int imgdb::PictureDatabase::CreateNewRecord(StorageType storage, ImageKind kind, const IndexInfo& index)
{
    int recordIndex = FindEmptyRecord();
    if (recordIndex < 0)
    {
        return -1;
    }
    if (!util::IsAvailableIndexInfo(storage, index, false))
    {
        return -1;
    }
    if (m_pLegacyData != NULL && !util::IsAvailableIndexInfo(storage, index, true))
    {
        return -1;
    }
    InitializeRecord(recordIndex, kind, index);
    if (storage == STORAGE_TYPE_CTR_NAND)
    {
        IndexInfo next = m_PictureTable.GetHeader()->m_IndexInfo;
        FindAvailableIndexInfo(next, storage);
        m_PictureTable.GetHeader()->m_IndexInfo = next;
        if (m_pLegacyData != NULL)
        {
            m_LegacyTable.GetHeader()->m_IndexInfo[0] = next;
        }
    }
    return recordIndex;
}

// (a table that cannot be read or does not fit is made new)
// 0x005AA414 | nintendogs:callgraph [tier C]
Result imgdb::PictureDatabase::LoadLegacyTable(StorageType storage, const wchar_t* path)
{
    Result result = RESULT_SUCCESS;
    if (m_pLegacyData == NULL)
    {
        InitializeLegacyTable();
    }
    FileReader reader(m_pLegacyData, m_LegacyCapacity, false);
    if (reader.Read(storage, path))
    {
        u32 size = reader.GetDataSize();
        if (m_LegacyTable.SetExistingTableData(const_cast<void*>(reader.GetData()), size) &&
            m_LegacyTable.GetHeader()->m_RecordNum == m_RecordNum &&
            m_LegacyTable.GetHeader()->GetTargetArchive() == m_Storage)
        {
            m_LegacySize = reader.GetDataSize();
            return result;
        }
        result = RESULT_INVALID_DATA;
    }
    else
    {
        result = reader.GetResult();
        if (result == RESULT_SUCCESS)
        {
            return result;
        }
    }
    InitializeLegacyTable();
    return result;
}

// 0x005AA5E8 | nintendogs:bytes [tier B]
Result imgdb::PictureDatabase::SaveLegacyTable(StorageType storage, const wchar_t* path, bool isBreakCrc)
{
    Result result = RESULT_SUCCESS;
    if (m_pLegacyData == NULL)
    {
        return result;
    }
    m_LegacyTable.GetHeader()->SetTargetArchive(m_Storage);
    m_LegacyTable.UpdateCrc();
    if (isBreakCrc)
    {
        m_LegacyTable.GetHeader()->m_Crc++;
    }
    int size = std::wcslen(path) + 1;
    wchar_t* directoryPath = static_cast<wchar_t*>(m_pAllocator->Allocate(size * sizeof(wchar_t), 32));
    util::CreateDirectoryPathFromFilePath(directoryPath, size, path);
    result = util::CreateDirectory(storage, directoryPath, NULL);
    if (result == RESULT_SUCCESS)
    {
        FileWriter writer(*m_pAllocator);
        writer.Write(storage, path, m_pLegacyData, m_LegacySize);
        result = writer.GetResult();
    }
    if (directoryPath != NULL)
    {
        m_pAllocator->Free(directoryPath);
    }
    return result;
}

// 0x005AA6FC | nintendogs:bytes [tier B]
void imgdb::PictureDatabase::SeNextIndexInfo(const IndexInfo& index)
{
    if (m_pPictureData != NULL)
    {
        m_PictureTable.GetHeader()->m_IndexInfo = index;
    }
    if (m_pLegacyData != NULL)
    {
        m_LegacyTable.GetHeader()->m_IndexInfo[0] = index;
    }
}

// 0x005AA730 | nintendogs:callgraph [tier C]
Result imgdb::PictureDatabase::LoadPictureTable(StorageType storage, const wchar_t* path)
{
    Result result = RESULT_SUCCESS;
    if (m_pPictureData == NULL)
    {
        InitializePictureTable();
    }
    FileReader reader(m_pPictureData, m_PictureCapacity, false);
    if (reader.Read(storage, path))
    {
        u32 size = reader.GetDataSize();
        if (m_PictureTable.SetExistingTableData(const_cast<void*>(reader.GetData()), size) &&
            m_PictureTable.GetHeader()->m_RecordNum == m_RecordNum &&
            m_PictureTable.GetHeader()->GetTargetArchive() == m_Storage)
        {
            m_PictureSize = reader.GetDataSize();
            return result;
        }
        result = RESULT_INVALID_DATA;
    }
    else
    {
        result = reader.GetResult();
        if (result == RESULT_SUCCESS)
        {
            return result;
        }
    }
    InitializePictureTable();
    return result;
}

// (the header counts the saves; a failed save is not counted)
// 0x005AA964 | nintendogs:bytes-fuzzy [tier B]
Result imgdb::PictureDatabase::SavePictureTable(StorageType storage, const wchar_t* path, bool isBreakCrc)
{
    Result result = RESULT_SUCCESS;
    if (m_pPictureData == NULL)
    {
        return result;
    }
    m_PictureTable.GetHeader()->m_SaveCount++;
    m_PictureTable.GetHeader()->SetTargetArchive(m_Storage);
    m_PictureTable.UpdateCrc();
    if (isBreakCrc)
    {
        m_PictureTable.GetHeader()->m_Crc++;
    }
    int size = std::wcslen(path) + 1;
    wchar_t* directoryPath = static_cast<wchar_t*>(m_pAllocator->Allocate(size * sizeof(wchar_t), 32));
    util::CreateDirectoryPathFromFilePath(directoryPath, size, path);
    result = util::CreateDirectory(storage, directoryPath, NULL);
    if (result == RESULT_SUCCESS)
    {
        FileWriter writer(*m_pAllocator);
        writer.Write(storage, path, m_pPictureData, m_PictureSize);
        result = writer.GetResult();
    }
    if (directoryPath != NULL)
    {
        m_pAllocator->Free(directoryPath);
    }
    if (result != RESULT_SUCCESS)
    {
        m_PictureTable.GetHeader()->m_SaveCount--;
    }
    return result;
}

// (the buffer is only as large as the table)
// 0x005AAAA0 | nintendogs:bytes [tier B]
void imgdb::PictureDatabase::InitializeLegacyTable()
{
    if (m_RecordNum <= 0)
    {
        return;
    }
    m_LegacySize = LegacyTable::ComputeTableSize(m_RecordNum, 0);
    if (m_pLegacyData == NULL)
    {
        m_LegacyCapacity = LegacyTable::ComputeTableSize(m_RecordNum, MAX_EXTRA_SIZE);
        m_pLegacyData = m_pAllocator->Allocate(m_LegacySize, 32);
    }
    m_LegacyTable.SetInitialTableData(m_pLegacyData, m_LegacySize, m_RecordNum, 0);
}

// 0x005AAB20 | nintendogs:bytes [tier B]
void imgdb::PictureDatabase::InitializePictureTable()
{
    if (m_RecordNum <= 0)
    {
        return;
    }
    m_PictureSize = PictureTable::ComputeTableSize(m_RecordNum, 0);
    if (m_pPictureData == NULL)
    {
        m_PictureCapacity = PictureTable::ComputeTableSize(m_RecordNum, MAX_EXTRA_SIZE);
        m_pPictureData = m_pAllocator->Allocate(m_PictureCapacity, 32);
    }
    m_PictureTable.SetInitialTableData(m_pPictureData, m_PictureSize, m_RecordNum, 0);
}

// 0x005AABA0 | nintendogs:bytes [tier B]
void imgdb::PictureDatabase::InitializeValidityStateTable()
{
    if (m_RecordNum <= 0)
    {
        return;
    }
    m_ValiditySize = ValidityStateTable::ComputeTableSize(m_RecordNum);
    if (m_pValidityData == NULL)
    {
        m_pValidityData = m_pAllocator->Allocate(m_ValiditySize, 32);
    }
    m_ValidityStateTable.SetInitialTableData(m_pValidityData, m_ValiditySize, m_RecordNum);
}

// 0x005AAC08 | nintendogs:bytes [tier B]
imgdb::PictureDatabase::PictureDatabase()
    : m_pAllocator(NULL), m_Storage(STORAGE_TYPE_TWL_NAND), m_RecordNum(0), m_pRecords(NULL), m_pValidityData(NULL),
      m_ValiditySize(0), m_pPictureData(NULL), m_PictureSize(0), m_PictureCapacity(0), m_pLegacyData(NULL),
      m_LegacySize(0), m_LegacyCapacity(0)
{
}

// 0x005AAC6C (name is ours)
imgdb::PictureDatabase::~PictureDatabase()
{
    Finalize();
}

// 0x007559A4 | nintendogs:bytes [tier B]
int imgdb::PictureDatabase::GetEmptyRecordNum() const
{
    bool hasPictureTable = m_pPictureData != NULL;
    bool hasLegacyTable = m_pLegacyData != NULL;
    if (!hasPictureTable && !hasLegacyTable)
    {
        return 0;
    }
    int num = 0;
    for (int i = 0; i < m_RecordNum; i++)
    {
        if (hasPictureTable && m_PictureTable.GetRecords()[i].m_ImageInfo.GetValidityFlag())
        {
            continue;
        }
        if (hasLegacyTable && m_LegacyTable.GetRecords()[i].m_IsUsed)
        {
            continue;
        }
        num++;
    }
    return num;
}

} // namespace imgdb
