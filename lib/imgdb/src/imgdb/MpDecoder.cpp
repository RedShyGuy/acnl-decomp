#include "nn/util/ADLFireWall/util_NonCopyable.h"
#include "imgdb/JpegMpBaseDecoder.h"
#include "imgdb/MpDecoder.h"

namespace imgdb {
// ctor candidate(s) 0x005B19E4 (unverified)
imgdb::MpDecoder::MpDecoder()
{
}

// 0x005B1B54 slot 0x00 | nintendogs:bytes
imgdb::MpDecoder::~MpDecoder()
{
}

// 0x005B1ABC slot 0x04 | virtual slot, introduced by imgdb::MpDecoder
void imgdb::MpDecoder::vf_0x04()
{
}

// 0x005B165C | nintendogs:bytes [tier B]
void imgdb::MpDecoder::ExtractExif(bool)
{
}

// 0x005B177C | nintendogs:bytes [tier B]
void imgdb::MpDecoder::ExtractJpeg(int, void*, unsigned, unsigned&)
{
}

// 0x005B1864 | nintendogs:bytes [tier B]
void imgdb::MpDecoder::ExtractJpegL(void*, unsigned, unsigned&)
{
}

// 0x005B18D4 | nintendogs:bytes-fuzzy [tier B]
void imgdb::MpDecoder::ComputeExtractJpegSize()
{
}

// 0x005B19E4 | nintendogs:bytes [tier B]
imgdb::MpDecoder::MpDecoder(const void*, unsigned)
{
}

} // namespace imgdb
