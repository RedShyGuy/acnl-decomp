#pragma once

#include "decomp.h"
#include "imgdb/JpegMpBaseEncoder.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace imgdb {
// RTTI N5imgdb11JpegEncoderE @ 0x008D2AC4
// vtable 0x00908ED0 (vptr 0x00908ED8), offset_to_top 0, 2 entries
class JpegEncoder : public ::imgdb::JpegMpBaseEncoder, public ::nn::util::ADLFireWall::NonCopyable<imgdb::JpegEncoder>
{
public:
    JpegEncoder(); // ctor candidate(s) 0x005A5478 (unverified)
    virtual ~JpegEncoder(); // 0x005A5594 slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x005A5550 slot 0x04 | virtual slot, introduced by imgdb::JpegMpBaseEncoder
    void Encode(bool); // 0x005A50B0 | nintendogs:bytes-fuzzy [tier B]
    JpegEncoder(const void*, nn::jpeg::CTR::PixelFormat, int, int); // 0x005A5478 | nintendogs:bytes [tier B]
};
} // namespace imgdb
