#include "imgdb/PhotoHeaderFileReader.h"
#include "imgdb/PhotoHeaderImageInfoReader.h"

namespace imgdb {
// ctor candidate(s) 0x005ABE70 (unverified)
imgdb::PhotoHeaderImageInfoReader::PhotoHeaderImageInfoReader()
{
}

// 0x005ABF1C slot 0x00 | virtual slot, introduced by imgdb::FileReader
void imgdb::PhotoHeaderImageInfoReader::vf_0x00()
{
}

// 0x005ABEC4 slot 0x04 | virtual slot, introduced by imgdb::FileReader
void imgdb::PhotoHeaderImageInfoReader::vf_0x04()
{
}

// 0x005ABE1C | nintendogs:bytes [tier B]
void imgdb::PhotoHeaderImageInfoReader::ReadImage()
{
}

// 0x005ABE70 | nintendogs:bytes [tier B]
imgdb::PhotoHeaderImageInfoReader::PhotoHeaderImageInfoReader(imgdb::Allocator&, imgdb::StorageType, const imgdb::ImageInfo&, bool)
{
}

} // namespace imgdb
