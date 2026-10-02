#include "imgdb/JpegMpBaseDecoder.h"

namespace imgdb {
// ctor address unknown
imgdb::JpegMpBaseDecoder::JpegMpBaseDecoder()
{
}

// 0x005AAFF4 | nintendogs:bytes [tier B]
void imgdb::JpegMpBaseDecoder::ComputeShrinkLevel(int&, int&, int, int)
{
}

// 0x00755B94 | nintendogs:bytes [tier B]
void imgdb::JpegMpBaseDecoder::GetDateTime(nn::fnd::DateTime&) const
{
}

// 0x00755BEC | nintendogs:bytes [tier B]
void imgdb::JpegMpBaseDecoder::GetSysMakerNote(imgdb::SysMakerNote&) const
{
}

// 0x00755C5C | nintendogs:bytes [tier B]
void imgdb::JpegMpBaseDecoder::GetSysMakerNote() const
{
}

} // namespace imgdb
