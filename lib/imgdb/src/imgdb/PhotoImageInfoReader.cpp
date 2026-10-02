#include "imgdb/PhotoFileReader.h"
#include "imgdb/PhotoImageInfoReader.h"

namespace imgdb {
// ctor candidate(s) 0x005AB458 (unverified)
imgdb::PhotoImageInfoReader::PhotoImageInfoReader()
{
}

// 0x005AB504 slot 0x00 | virtual slot, introduced by imgdb::FileReader
void imgdb::PhotoImageInfoReader::vf_0x00()
{
}

// 0x005AB4AC slot 0x04 | virtual slot, introduced by imgdb::FileReader
void imgdb::PhotoImageInfoReader::vf_0x04()
{
}

// 0x005AB3FC | nintendogs:bytes [tier B]
void imgdb::PhotoImageInfoReader::ReadImage()
{
}

// 0x005AB458 | nintendogs:bytes [tier B]
imgdb::PhotoImageInfoReader::PhotoImageInfoReader(imgdb::Allocator&, imgdb::StorageType, const imgdb::ImageInfo&, bool)
{
}

} // namespace imgdb
