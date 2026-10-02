#pragma once

#include "decomp.h"
#include "imgdb/PhotoFileReader.h"

namespace imgdb {
// RTTI N5imgdb20PhotoImageInfoReaderE @ 0x008D2B7C
// vtable 0x00908FC0 (vptr 0x00908FC8), offset_to_top 0, 4 entries
class PhotoImageInfoReader : public ::imgdb::PhotoFileReader
{
public:
    PhotoImageInfoReader(); // ctor candidate(s) 0x005AB458 (unverified)
    virtual void vf_0x00(); // 0x005AB504 slot 0x00 | virtual slot, introduced by imgdb::FileReader
    virtual void vf_0x04(); // 0x005AB4AC slot 0x04 | virtual slot, introduced by imgdb::FileReader
    void ReadImage(); // 0x005AB3FC | nintendogs:bytes [tier B]
    PhotoImageInfoReader(imgdb::Allocator&, imgdb::StorageType, const imgdb::ImageInfo&, bool); // 0x005AB458 | nintendogs:bytes [tier B]
};
} // namespace imgdb
