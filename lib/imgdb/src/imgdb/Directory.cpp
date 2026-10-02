#include "imgdb/Directory.h"

namespace imgdb {
// 0x005B09A0 | nintendogs:bytes [tier B]
void imgdb::Directory::Initialize(imgdb::Allocator&, imgdb::StorageType, const wchar_t*, int)
{
}

// 0x005B0A0C | nintendogs:bytes [tier B]
void imgdb::Directory::Read()
{
}

// 0x005B0AC0 | nintendogs:bytes [tier B]
imgdb::Directory::Directory(imgdb::Allocator&, imgdb::StorageType, const wchar_t*, int)
{
}

// 0x005B0B54 | nintendogs:bytes [tier B]
imgdb::Directory::Directory()
{
}

// 0x005B0B90 | nintendogs:bytes [tier B]
imgdb::Directory::~Directory()
{
}

// 0x00756258 | nintendogs:bytes [tier B]
void imgdb::Directory::IsDirectory() const
{
}

// 0x007562AC | nintendogs:bytes [tier B]
void imgdb::Directory::IsFile() const
{
}

// 0x00756304 | nintendogs:bytes [tier B]
void imgdb::Directory::GetEntry() const
{
}

} // namespace imgdb
