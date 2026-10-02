#pragma once

#include "decomp.h"

namespace nn {
namespace jpeg {
namespace CTR {
class JpegMpEncoder
{
public:
    void Initialize(void*, unsigned, unsigned); // 0x0047057C | nintendogs:bytes [tier B]
    void SetMakerNote(const unsigned char*, unsigned, unsigned); // 0x00470688 | nintendogs:bytes [tier B]
    void GetDateTimeNow(char*); // 0x004706C0 | nintendogs:bytes [tier B]
    void SetUserMakerNote(const unsigned char*, unsigned); // 0x0047075C | nintendogs:bytes [tier B]
    void StartJpegEncoder(unsigned char*, unsigned, const void*, unsigned, unsigned, unsigned, nn::jpeg::CTR::PixelSampling, nn::jpeg::CTR::PixelFormat, bool); // 0x00470788 | nintendogs:bytes [tier B]
    void ClearTemporarySetting(); // 0x0047AC08 | nintendogs:bytes [tier B]
    void GetLastError() const; // 0x007374C8 | nintendogs:bytes [tier B]
};
} // namespace CTR
} // namespace jpeg
} // namespace nn
