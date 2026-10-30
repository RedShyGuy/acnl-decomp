#include "imgdb/LegacyTableRecord.h"

namespace imgdb {

// 0x005AB208 | nintendogs:bytes [tier B]
void imgdb::LegacyTableRecord::Initialize()
{
    m_DateTime.m_Seconds = 0;
    _08 = 0;
    m_Flags = 0x800000;
}

// 0x005AB224 | nintendogs:bytes [tier B]
void imgdb::LegacyTableRecord::SetIndexInfo(const IndexInfo& index)
{
    m_DirectoryIndex = index.m_DirectoryIndex;
    m_FileIndex = index.m_FileIndex;
}

// 0x00755CA4 | nintendogs:bytes [tier B]
imgdb::IndexInfo imgdb::LegacyTableRecord::GetIndexInfo() const
{
    IndexInfo index;
    index.SetDirectoryIndex(m_DirectoryIndex);
    index.SetFileIndex(m_FileIndex);
    return index;
}

} // namespace imgdb
