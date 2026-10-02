#include "nn/jpeg/CTR/jpeg_JpegMpEncoder.h"

namespace nn {
namespace jpeg {
namespace CTR {
// 0x0047057C | nintendogs:bytes [tier B]
void nn::jpeg::CTR::JpegMpEncoder::Initialize(void*, unsigned, unsigned)
{
}

// 0x00470688 | nintendogs:bytes [tier B]
void nn::jpeg::CTR::JpegMpEncoder::SetMakerNote(const unsigned char*, unsigned, unsigned)
{
}

// 0x004706C0 | nintendogs:bytes [tier B]
void nn::jpeg::CTR::JpegMpEncoder::GetDateTimeNow(char*)
{
}

// 0x0047075C | nintendogs:bytes [tier B]
void nn::jpeg::CTR::JpegMpEncoder::SetUserMakerNote(const unsigned char*, unsigned)
{
}

// 0x00470788 | nintendogs:bytes [tier B]
void nn::jpeg::CTR::JpegMpEncoder::StartJpegEncoder(unsigned char*, unsigned, const void*, unsigned, unsigned, unsigned, nn::jpeg::CTR::PixelSampling, nn::jpeg::CTR::PixelFormat, bool)
{
}

// 0x0047AC08 | nintendogs:bytes [tier B]
void nn::jpeg::CTR::JpegMpEncoder::ClearTemporarySetting()
{
}

// 0x007374C8 | nintendogs:bytes [tier B]
void nn::jpeg::CTR::JpegMpEncoder::GetLastError() const
{
}

} // namespace CTR
} // namespace jpeg
} // namespace nn
