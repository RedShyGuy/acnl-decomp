#pragma once

#include "decomp.h"
#include "imgdb/JpegMpBaseDecoder.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace imgdb {
// RTTI N5imgdb11JpegDecoderE @ 0x008D2AA4
// vtable 0x00908EC0 (vptr 0x00908EC8), offset_to_top 0, 2 entries
class JpegDecoder : public ::imgdb::JpegMpBaseDecoder, public ::nn::util::ADLFireWall::NonCopyable<imgdb::JpegDecoder>
{
public:
    JpegDecoder(); // ctor candidate(s) 0x005A4F00 (unverified)
    virtual ~JpegDecoder(); // 0x005A5044 slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x005A4FD4 slot 0x04 | virtual slot, introduced by imgdb::JpegDecoder
    void ExtractExif(bool); // 0x005A4A20 | nintendogs:bytes [tier B]
    void ExtractExif(); // 0x005A4B18 | nintendogs:bytes [tier B]
    void Decode(nn::jpeg::CTR::PixelFormat, bool); // 0x005A4C04 | nintendogs:bytes [tier B]
    JpegDecoder(const void*, unsigned); // 0x005A4F00 | nintendogs:bytes [tier B]
};
} // namespace imgdb
