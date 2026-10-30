#include "imgdb/PhotoHeaderFileReader.h"
#include "imgdb/util/util_Api.h"

namespace imgdb {

// 0x009826C9
bool imgdb::PhotoHeaderFileReader::s_IsDefaultLimitFileSizeSet;
// 0x009826D0
u32 imgdb::PhotoHeaderFileReader::s_DefaultLimitFileSize;

// 0x005AB640 | nintendogs:bytes [tier B]
void imgdb::PhotoHeaderFileReader::SetDefaultLimitFileSize(u32 size)
{
    s_DefaultLimitFileSize = size;
    s_IsDefaultLimitFileSizeSet = true;
}

// 0x005AB658
bool imgdb::PhotoHeaderFileReader::Read(StorageType storage, const wchar_t* path)
{
    return FileReader::Read(storage, path, 0, 0);
}

// 0x005AB67C | nintendogs:bytes [tier B]
bool imgdb::PhotoHeaderFileReader::Read(StorageType storage, const wchar_t* path, s64 offset, u32 size)
{
    if (util::GetFileSize(storage, path, NULL) > m_LimitFileSize)
    {
        return false;
    }
    return FileReader::Read(storage, path, offset, size);
}

// 0x005AB744 | nintendogs:callgraph [tier C]
// 0x005AB6EC (deleting dtor)
imgdb::PhotoHeaderFileReader::~PhotoHeaderFileReader()
{
    // (only the base is destroyed)
}

} // namespace imgdb
