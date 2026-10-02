#include "nn/util/ADLFireWall/util_NonCopyable.h"
#include "imgdb/JpegMpBaseDecoder.h"
#include "imgdb/JpegDecoder.h"

namespace imgdb {
// ctor candidate(s) 0x005A4F00 (unverified)
imgdb::JpegDecoder::JpegDecoder()
{
}

// 0x005A5044 slot 0x00 | nintendogs:bytes
imgdb::JpegDecoder::~JpegDecoder()
{
}

// 0x005A4FD4 slot 0x04 | virtual slot, introduced by imgdb::JpegDecoder
void imgdb::JpegDecoder::vf_0x04()
{
}

// 0x005A4A20 | nintendogs:bytes [tier B]
void imgdb::JpegDecoder::ExtractExif(bool)
{
}

// 0x005A4B18 | nintendogs:bytes [tier B]
void imgdb::JpegDecoder::ExtractExif()
{
}

// 0x005A4C04 | nintendogs:bytes [tier B]
void imgdb::JpegDecoder::Decode(nn::jpeg::CTR::PixelFormat, bool)
{
}

// 0x005A4F00 | nintendogs:bytes [tier B]
imgdb::JpegDecoder::JpegDecoder(const void*, unsigned)
{
}

} // namespace imgdb
