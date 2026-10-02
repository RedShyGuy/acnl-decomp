#include "nn/util/ADLFireWall/util_NonCopyable.h"
#include "imgdb/JpegMpBaseEncoder.h"
#include "imgdb/MpEncoder.h"

namespace imgdb {
// ctor candidate(s) 0x005B200C (unverified)
imgdb::MpEncoder::MpEncoder()
{
}

// 0x005B212C slot 0x00 | nintendogs:bytes
imgdb::MpEncoder::~MpEncoder()
{
}

// 0x005B20E8 slot 0x04 | virtual slot, introduced by imgdb::JpegMpBaseEncoder
void imgdb::MpEncoder::vf_0x04()
{
}

// 0x005B1BE8 | nintendogs:bytes-fuzzy [tier B]
void imgdb::MpEncoder::Encode()
{
}

// 0x005B200C | nintendogs:bytes [tier B]
imgdb::MpEncoder::MpEncoder(const void*, const void*, nn::jpeg::CTR::PixelFormat, int, int)
{
}

} // namespace imgdb
