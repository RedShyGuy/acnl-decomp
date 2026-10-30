#include "imgdb/Directory.h"
#include "imgdb/util/util_Api.h"

namespace imgdb {

// 0x005B09A0 | nintendogs:bytes [tier B]
void imgdb::Directory::Initialize(Allocator& allocator, StorageType storage, const wchar_t* path, int maxEntryNum)
{
    m_MaxEntryNum = maxEntryNum;
    m_pAllocator = &allocator;
    m_Storage = storage;
    m_pEntries = static_cast<nn::fs::DirectoryEntry*>(
        m_pAllocator->Allocate(READ_ENTRY_NUM * sizeof(nn::fs::DirectoryEntry), 4));
    nn::Result result = m_Directory.TryInitialize(path);
    m_IsOpen = result.IsSuccess();
    if (!m_IsOpen && storage == STORAGE_TYPE_CTR_NAND)
    {
        util::ThrowFatalErrByNandOperation(result);
    }
}

// 0x005B0A0C | nintendogs:bytes [tier B]
bool imgdb::Directory::Read()
{
    if (!m_IsOpen || m_MaxEntryNum <= m_ReadNum)
    {
        return false;
    }
    m_ReadNum++;
    m_EntryIndex++;
    if (m_EntryIndex >= m_EntryNum)
    {
        m_EntryIndex = 0;
        s32 readNum;
        nn::Result result = m_Directory.TryRead(&readNum, m_pEntries, READ_ENTRY_NUM);
        if (result.IsFailure())
        {
            m_Directory.Finalize();
            m_IsOpen = false;
            if (m_Storage == STORAGE_TYPE_CTR_NAND)
            {
                util::ThrowFatalErrByNandOperation(result);
            }
            return false;
        }
        m_EntryNum = readNum;
    }
    return m_EntryIndex < m_EntryNum;
}

// 0x005B0AC0 | nintendogs:bytes [tier B]
imgdb::Directory::Directory(Allocator& allocator, StorageType storage, const wchar_t* path, int maxEntryNum)
    : m_IsOpen(false), m_pEntries(NULL), m_EntryNum(0), m_EntryIndex(0), m_ReadNum(0)
{
    Initialize(allocator, storage, path, maxEntryNum);
}

// 0x005B0B54 | nintendogs:bytes [tier B]
imgdb::Directory::Directory()
    : m_pAllocator(NULL),
      m_IsOpen(false),
      m_Storage(STORAGE_TYPE_TWL_NAND),
      m_pEntries(NULL),
      m_EntryNum(0),
      m_EntryIndex(0),
      m_MaxEntryNum(0),
      m_ReadNum(0)
{
}

// 0x005B0B90 | nintendogs:bytes [tier B]
imgdb::Directory::~Directory()
{
    if (m_pAllocator != NULL)
    {
        if (m_IsOpen)
        {
            m_Directory.Finalize();
            m_IsOpen = false;
        }
        if (m_pEntries != NULL)
        {
            m_pAllocator->Free(m_pEntries);
            m_pEntries = NULL;
        }
        m_MaxEntryNum = 0;
        m_pAllocator = NULL;
    }
}

// 0x00756258 | nintendogs:bytes [tier B]
bool imgdb::Directory::IsDirectory() const
{
    const nn::fs::DirectoryEntry* entry = GetEntry();
    return entry != NULL && entry->attributes.isDirectory;
}

// 0x007562AC | nintendogs:bytes [tier B]
bool imgdb::Directory::IsFile() const
{
    const nn::fs::DirectoryEntry* entry = GetEntry();
    return entry != NULL && !entry->attributes.isDirectory;
}

// 0x00756304 | nintendogs:bytes [tier B]
const nn::fs::DirectoryEntry* imgdb::Directory::GetEntry() const
{
    if (!m_IsOpen || m_ReadNum > m_MaxEntryNum || m_EntryIndex >= m_EntryNum)
    {
        return NULL;
    }
    return &m_pEntries[m_EntryIndex];
}

} // namespace imgdb
