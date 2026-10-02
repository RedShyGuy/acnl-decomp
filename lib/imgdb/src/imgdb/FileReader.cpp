#include "nn/util/ADLFireWall/util_NonCopyable.h"
#include "imgdb/FileReader.h"

namespace imgdb {
// ctor candidate(s) 0x005A45B8, 0x005A45F8 (unverified)
imgdb::FileReader::FileReader()
{
}

// 0x005A4698 slot 0x00 | virtual slot, introduced by imgdb::FileReader
void imgdb::FileReader::vf_0x00()
{
}

// 0x005A4640 slot 0x04 | virtual slot, introduced by imgdb::FileReader
void imgdb::FileReader::vf_0x04()
{
}

// 0x005A4280 slot 0x08 | nintendogs:bytes
void imgdb::FileReader::Read(imgdb::StorageType, const wchar_t*)
{
}

// 0x005A42AC slot 0x0C | nintendogs:callgraph
void imgdb::FileReader::Read(imgdb::StorageType, const wchar_t*, long long, unsigned)
{
}

// 0x005A45B8 | nintendogs:bytes [tier B]
imgdb::FileReader::FileReader(void*, unsigned, bool)
{
}

// 0x005A45F8 | nintendogs:bytes [tier B]
imgdb::FileReader::FileReader(imgdb::Allocator&, unsigned, bool)
{
}

} // namespace imgdb
