#pragma once

#include "imgdb/LegacyTable.h"
#include "imgdb/LegacyTableHeader.h"
#include "imgdb/LegacyTableRecord.h"
#include "imgdb/PictureDbRecord.h"
#include "imgdb/PictureTable.h"
#include "imgdb/PictureTableHeader.h"
#include "imgdb/PictureTableRecord.h"
#include "imgdb/ValidityStateTable.h"

namespace imgdb {

// the database of a storage: a record per image in the picture table (and in the table of the DSi camera on the
// CTR NAND) with a validity state each (member names are ours)
class PictureDatabase
{
public:
    PictureDatabase();
    ~PictureDatabase();  // 0x005AAC6C

    void Initialize(Allocator& allocator, int recordNum, StorageType storage);
    // the index of a new record for an image at the next free index of the storage, -1 when it is full
    int CreateNewRecord(StorageType storage, ImageKind kind);
    // a new record at the given index, -1 when it is used
    int CreateNewRecord(StorageType storage, ImageKind kind, const IndexInfo& index);
    Result LoadLegacyTable(StorageType storage, const wchar_t* path);
    // isBreakCrc: the CRC is made invalid
    Result SaveLegacyTable(StorageType storage, const wchar_t* path, bool isBreakCrc);
    void SeNextIndexInfo(const IndexInfo& index);
    Result LoadPictureTable(StorageType storage, const wchar_t* path);
    Result SavePictureTable(StorageType storage, const wchar_t* path, bool isBreakCrc);
    void InitializeLegacyTable();
    void InitializePictureTable();
    void InitializeValidityStateTable();
    int GetEmptyRecordNum() const;

    DECOMP_EMIT_COPY bool IsInitialized() const { return m_pAllocator != NULL; } // 0x00755994
    DECOMP_EMIT_COPY PictureDbRecord* GetRecord(int index) const { return &m_pRecords[index]; } // 0x00755A58
    // the buffers are freed
    DECOMP_EMIT_COPY void Finalize() // 0x0013F8D4 (name is ours)
    {
        if (m_pAllocator == NULL)
        {
            return;
        }
        if (m_pPictureData != NULL)
        {
            m_pAllocator->Free(m_pPictureData);
            m_pPictureData = NULL;
            m_PictureSize = 0;
            m_PictureCapacity = 0;
        }
        if (m_pLegacyData != NULL)
        {
            m_pAllocator->Free(m_pLegacyData);
            m_pLegacyData = NULL;
            m_LegacySize = 0;
            m_LegacyCapacity = 0;
        }
        if (m_pValidityData != NULL)
        {
            m_pAllocator->Free(m_pValidityData);
            m_pValidityData = NULL;
            m_ValiditySize = 0;
        }
        if (m_pRecords != NULL)
        {
            m_pAllocator->Free(m_pRecords);
            m_pRecords = NULL;
        }
        m_pAllocator = NULL;
        m_Storage = STORAGE_TYPE_TWL_NAND;
        m_RecordNum = 0;
    }

private:
    // the first record that neither table uses, -1 without one
    int FindEmptyRecord() const
    {
        bool hasPictureTable = m_pPictureData != NULL;
        bool hasLegacyTable = m_pLegacyData != NULL;
        if (!hasPictureTable && !hasLegacyTable)
        {
            return -1;
        }
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
            return i;
        }
        return -1;
    }
    // the next free index from index on (on the CTR NAND also free for the DSi camera)
    bool FindAvailableIndexInfo(IndexInfo& index, StorageType storage) const;
    void InitializeRecord(int recordIndex, ImageKind kind, const IndexInfo& index);

public:
    // (the records read the tables)
    Allocator* m_pAllocator;                // +0x00
    StorageType m_Storage;                  // +0x04
    int m_RecordNum;                        // +0x08
    PictureDbRecord* m_pRecords;            // +0x0C
    void* m_pValidityData;                  // +0x10
    u32 m_ValiditySize;                     // +0x14
    ValidityStateTable m_ValidityStateTable;  // +0x18
    void* m_pPictureData;                   // +0x20
    u32 m_PictureSize;                      // +0x24
    u32 m_PictureCapacity;                  // +0x28
    PictureTable m_PictureTable;            // +0x2C
    void* m_pLegacyData;                    // +0x34
    u32 m_LegacySize;                       // +0x38
    u32 m_LegacyCapacity;                   // +0x3C
    LegacyTable m_LegacyTable;              // +0x40
};
ASSERT_SIZE(PictureDatabase, 0x48);

} // namespace imgdb
