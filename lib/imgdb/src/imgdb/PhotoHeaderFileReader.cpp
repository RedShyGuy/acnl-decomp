#include "imgdb/FileReader.h"
#include "imgdb/PhotoHeaderFileReader.h"

namespace imgdb {
// ctor address unknown
imgdb::PhotoHeaderFileReader::PhotoHeaderFileReader()
{
}

// 0x005AB744 slot 0x00 | nintendogs:callgraph
void imgdb::PhotoHeaderFileReader::vf_0x00()
{
}

// 0x005AB6EC slot 0x04 | virtual slot, introduced by imgdb::FileReader
void imgdb::PhotoHeaderFileReader::vf_0x04()
{
}

// 0x005AB658 slot 0x08 | slot vf_0x08 of imgdb::FileReader
void imgdb::PhotoHeaderFileReader::Read(imgdb::StorageType, const wchar_t*)
{
}

// 0x005AB67C slot 0x0C | nintendogs:bytes
void imgdb::PhotoHeaderFileReader::Read(imgdb::StorageType, const wchar_t*, long long, unsigned)
{
}

// 0x005AB640 | nintendogs:bytes [tier B]
void imgdb::PhotoHeaderFileReader::SetDefaultLimitFileSize(unsigned)
{
}

} // namespace imgdb
