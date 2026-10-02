#pragma once

#include "decomp.h"
#include "imgdb/FileWriter.h"

namespace imgdb {
// RTTI N5imgdb15ImageFileWriterE @ 0x008D2B0C
// vtable 0x00908F1C (vptr 0x00908F24), offset_to_top 0, 3 entries
class ImageFileWriter : public ::imgdb::FileWriter
{
public:
    ImageFileWriter(); // ctor candidate(s) 0x005A9B3C (unverified)
    virtual void vf_0x00(); // 0x005A9B94 slot 0x00 | virtual slot, introduced by imgdb::FileWriter
    virtual void vf_0x04(); // 0x005A9B90 slot 0x04 | virtual slot, introduced by imgdb::FileWriter
    void WriteImage(); // 0x005A9AB8 | nintendogs:bytes [tier B]
    ImageFileWriter(imgdb::Allocator&, imgdb::StorageType, const imgdb::ImageInfo&, bool, const void*, unsigned); // 0x005A9B3C | nintendogs:bytes [tier B]
};
} // namespace imgdb
