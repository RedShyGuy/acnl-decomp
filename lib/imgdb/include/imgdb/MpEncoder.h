#pragma once

#include "decomp.h"
#include "imgdb/JpegMpBaseEncoder.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace imgdb {
// RTTI N5imgdb9MpEncoderE @ 0x008D2C68
// vtable 0x009090B8 (vptr 0x009090C0), offset_to_top 0, 2 entries
class MpEncoder : public ::imgdb::JpegMpBaseEncoder, public ::nn::util::ADLFireWall::NonCopyable<imgdb::MpEncoder>
{
public:
    MpEncoder(); // ctor candidate(s) 0x005B200C (unverified)
    virtual ~MpEncoder(); // 0x005B212C slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x005B20E8 slot 0x04 | virtual slot, introduced by imgdb::JpegMpBaseEncoder
    void Encode(); // 0x005B1BE8 | nintendogs:bytes-fuzzy [tier B]
    MpEncoder(const void*, const void*, nn::jpeg::CTR::PixelFormat, int, int); // 0x005B200C | nintendogs:bytes [tier B]
};
} // namespace imgdb
