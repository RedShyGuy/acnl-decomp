#include "nn/util/ADLFireWall/util_NonCopyable.h"
#include "imgdb/JpegMpBaseEncoder.h"
#include "imgdb/JpegEncoder.h"

namespace imgdb {
// ctor candidate(s) 0x005A5478 (unverified)
imgdb::JpegEncoder::JpegEncoder()
{
}

// 0x005A5594 slot 0x00 | nintendogs:bytes
imgdb::JpegEncoder::~JpegEncoder()
{
}

// 0x005A5550 slot 0x04 | virtual slot, introduced by imgdb::JpegMpBaseEncoder
void imgdb::JpegEncoder::vf_0x04()
{
}

// 0x005A50B0 | nintendogs:bytes-fuzzy [tier B]
void imgdb::JpegEncoder::Encode(bool)
{
}

// 0x005A5478 | nintendogs:bytes [tier B]
imgdb::JpegEncoder::JpegEncoder(const void*, nn::jpeg::CTR::PixelFormat, int, int)
{
}

} // namespace imgdb
