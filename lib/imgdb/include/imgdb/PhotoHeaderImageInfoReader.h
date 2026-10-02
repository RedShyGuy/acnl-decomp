#pragma once

#include "decomp.h"
#include "imgdb/PhotoHeaderFileReader.h"

namespace imgdb {
// RTTI N5imgdb26PhotoHeaderImageInfoReaderE @ 0x008D2BF0
// vtable 0x00909070 (vptr 0x00909078), offset_to_top 0, 4 entries
class PhotoHeaderImageInfoReader : public ::imgdb::PhotoHeaderFileReader
{
public:
    PhotoHeaderImageInfoReader(); // ctor candidate(s) 0x005ABE70 (unverified)
    virtual void vf_0x00(); // 0x005ABF1C slot 0x00 | virtual slot, introduced by imgdb::FileReader
    virtual void vf_0x04(); // 0x005ABEC4 slot 0x04 | virtual slot, introduced by imgdb::FileReader
    void ReadImage(); // 0x005ABE1C | nintendogs:bytes [tier B]
    PhotoHeaderImageInfoReader(imgdb::Allocator&, imgdb::StorageType, const imgdb::ImageInfo&, bool); // 0x005ABE70 | nintendogs:bytes [tier B]
};
} // namespace imgdb
