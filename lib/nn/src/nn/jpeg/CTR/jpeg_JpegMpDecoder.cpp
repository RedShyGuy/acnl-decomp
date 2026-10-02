#include "nn/jpeg/CTR/jpeg_JpegMpDecoder.h"

namespace nn {
namespace jpeg {
namespace CTR {
// 0x0046FF70 | nintendogs:bytes [tier B]
void nn::jpeg::CTR::JpegMpDecoder::GetMpEntry(nn::jpeg::CTR::MpEntry*, const nn::jpeg::CTR::MpIndex*, unsigned)
{
}

// 0x00470080 | nintendogs:bytes [tier B]
void nn::jpeg::CTR::JpegMpDecoder::GetMpIndex(nn::jpeg::CTR::MpIndex*, const unsigned char*, unsigned)
{
}

// 0x00470148 | nintendogs:bytes [tier B]
void nn::jpeg::CTR::JpegMpDecoder::Initialize(void*, unsigned)
{
}

// 0x0047019C | nintendogs:bytes [tier B]
void nn::jpeg::CTR::JpegMpDecoder::ExtractExif(const unsigned char*, unsigned, bool)
{
}

// 0x004702D8 | nintendogs:bytes [tier B]
void nn::jpeg::CTR::JpegMpDecoder::StartJpegDecoder(void*, unsigned, const unsigned char*, unsigned, unsigned, unsigned, nn::jpeg::CTR::PixelFormat, bool)
{
}

// 0x00470380 | nintendogs:bytes [tier B]
void nn::jpeg::CTR::JpegMpDecoder::StartJpegDecoderShrink(void*, unsigned, const unsigned char*, unsigned, unsigned, unsigned, nn::jpeg::CTR::PixelFormat, bool, unsigned)
{
}

// 0x00470490 | nintendogs:bytes [tier B]
void nn::jpeg::CTR::JpegMpDecoder::GetMpRegionsToBuildJpegData(nn::jpeg::CTR::MpRegionsToBuildJpegData*, const unsigned char*, unsigned)
{
}

// 0x004731C0 | nintendogs:bytes [tier B]
void nn::jpeg::CTR::JpegMpDecoder::GetLastDateTime(char*) const
{
}

// 0x007372F4 | nintendogs:bytes [tier B]
void nn::jpeg::CTR::JpegMpDecoder::GetLastWidth() const
{
}

// 0x00737324 | nintendogs:bytes [tier B]
void nn::jpeg::CTR::JpegMpDecoder::GetLastHeight() const
{
}

// 0x0073734C | nintendogs:bytes [tier B]
void nn::jpeg::CTR::JpegMpDecoder::GetLastMakerNoteSize(unsigned) const
{
}

// 0x007373E0 | nintendogs:bytes [tier B]
void nn::jpeg::CTR::JpegMpDecoder::GetLastMakerNotePointer(unsigned) const
{
}

// 0x00737430 | nintendogs:bytes [tier B]
void nn::jpeg::CTR::JpegMpDecoder::GetLastUserMakerNoteSize() const
{
}

// 0x00737480 | nintendogs:bytes [tier B]
void nn::jpeg::CTR::JpegMpDecoder::GetLastUserMakerNotePointer() const
{
}

} // namespace CTR
} // namespace jpeg
} // namespace nn
