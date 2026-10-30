#include "imgdb/PhotoFileReader.h"

namespace imgdb {

// 0x009826C8
bool imgdb::PhotoFileReader::s_IsDefaultLimitFileSizeSet;
// 0x009826CC
u32 imgdb::PhotoFileReader::s_DefaultLimitFileSize;

// 0x005A9CC8 | nintendogs:bytes [tier B]
void imgdb::PhotoFileReader::SetDefaultLimitFileSize(u32 size)
{
    s_DefaultLimitFileSize = size;
    s_IsDefaultLimitFileSizeSet = true;
}

// 0x005A9D38 | nintendogs:callgraph [tier C]
// 0x005A9CE0 (deleting dtor)
imgdb::PhotoFileReader::~PhotoFileReader()
{
    // (only the base is destroyed)
}

} // namespace imgdb
