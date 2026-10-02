#pragma once

#include "decomp.h"
#include "imgdb/JpegMpBaseDecoder.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace imgdb {
// RTTI N5imgdb9MpDecoderE @ 0x008D2C48
// vtable 0x009090A8 (vptr 0x009090B0), offset_to_top 0, 2 entries
class MpDecoder : public ::imgdb::JpegMpBaseDecoder, public ::nn::util::ADLFireWall::NonCopyable<imgdb::MpDecoder>
{
public:
    MpDecoder(); // ctor candidate(s) 0x005B19E4 (unverified)
    virtual ~MpDecoder(); // 0x005B1B54 slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x005B1ABC slot 0x04 | virtual slot, introduced by imgdb::MpDecoder
    void ExtractExif(bool); // 0x005B165C | nintendogs:bytes [tier B]
    void ExtractJpeg(int, void*, unsigned, unsigned&); // 0x005B177C | nintendogs:bytes [tier B]
    void ExtractJpegL(void*, unsigned, unsigned&); // 0x005B1864 | nintendogs:bytes [tier B]
    void ComputeExtractJpegSize(); // 0x005B18D4 | nintendogs:bytes-fuzzy [tier B]
    MpDecoder(const void*, unsigned); // 0x005B19E4 | nintendogs:bytes [tier B]
};
} // namespace imgdb
