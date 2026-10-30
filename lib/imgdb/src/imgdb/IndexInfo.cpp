#include "imgdb/IndexInfo.h"

namespace imgdb {

// 0x005B10E8 (name is ours)
void imgdb::IndexInfo::Initialize()
{
    m_DirectoryIndex = 0;
    m_FileIndex = 0;
}

// 0x005B10F4 | nintendogs:bytes [tier B]
bool imgdb::IndexInfo::SetFileIndex(int index)
{
    if (index > FILE_INDEX_MAX)
    {
        return false;
    }
    m_FileIndex = index;
    return true;
}

// 0x005B1110 | nintendogs:bytes [tier B]
bool imgdb::IndexInfo::IsValidFileIndex(int index)
{
    return index <= FILE_INDEX_MAX;
}

// 0x005B1128 | nintendogs:bytes [tier B]
bool imgdb::IndexInfo::SetDirectoryIndex(int index)
{
    if (index >= DIRECTORY_INDEX_NUM)
    {
        return false;
    }
    m_DirectoryIndex = index;
    return true;
}

// 0x005B113C (name is ours)
bool imgdb::IndexInfo::IsValidDirectoryIndex(int index)
{
    return index < DIRECTORY_INDEX_NUM;
}

// the next index the database writes: 100 files per directory
// 0x005B114C | nintendogs:bytes [tier B]
void imgdb::IndexInfo::UpdateNextIndexByOfficial()
{
    m_FileIndex++;
    if (m_FileIndex >= OFFICIAL_FILE_NUM)
    {
        UpdateNextDirectoryIndex();
    }
}

// 0x005B1188 (name is ours)
void imgdb::IndexInfo::UpdateNextDirectoryIndex()
{
    m_FileIndex = 0;
    m_DirectoryIndex++;
    if (m_DirectoryIndex >= DIRECTORY_INDEX_NUM)
    {
        m_DirectoryIndex = 0;
    }
}

// 0x00756718 | nintendogs:bytes [tier B]
bool imgdb::IndexInfo::IsValidFileIndex() const
{
    return m_FileIndex <= FILE_INDEX_MAX;
}

// 0x00756734 | nintendogs:bytes [tier B]
bool imgdb::IndexInfo::IsValidDirectoryIndex() const
{
    return m_DirectoryIndex < DIRECTORY_INDEX_NUM;
}

// 0x00756748 | nintendogs:bytes [tier B]
bool imgdb::IndexInfo::operator==(const IndexInfo& other) const
{
    return m_DirectoryIndex == other.m_DirectoryIndex && m_FileIndex == other.m_FileIndex;
}

// 0x00756774 | nintendogs:bytes [tier B]
bool imgdb::IndexInfo::operator!=(const IndexInfo& other) const
{
    return !(*this == other);
}

} // namespace imgdb
