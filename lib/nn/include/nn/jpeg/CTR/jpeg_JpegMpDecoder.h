#pragma once

#include "decomp.h"

namespace nn {
namespace jpeg {
namespace CTR {
class JpegMpDecoder
{
public:
    void GetMpEntry(nn::jpeg::CTR::MpEntry*, const nn::jpeg::CTR::MpIndex*, unsigned); // 0x0046FF70 | nintendogs:bytes [tier B]
    void GetMpIndex(nn::jpeg::CTR::MpIndex*, const unsigned char*, unsigned); // 0x00470080 | nintendogs:bytes [tier B]
    void Initialize(void*, unsigned); // 0x00470148 | nintendogs:bytes [tier B]
    void ExtractExif(const unsigned char*, unsigned, bool); // 0x0047019C | nintendogs:bytes [tier B]
    void StartJpegDecoder(void*, unsigned, const unsigned char*, unsigned, unsigned, unsigned, nn::jpeg::CTR::PixelFormat, bool); // 0x004702D8 | nintendogs:bytes [tier B]
    void StartJpegDecoderShrink(void*, unsigned, const unsigned char*, unsigned, unsigned, unsigned, nn::jpeg::CTR::PixelFormat, bool, unsigned); // 0x00470380 | nintendogs:bytes [tier B]
    void GetMpRegionsToBuildJpegData(nn::jpeg::CTR::MpRegionsToBuildJpegData*, const unsigned char*, unsigned); // 0x00470490 | nintendogs:bytes [tier B]
    void GetLastDateTime(char*) const; // 0x004731C0 | nintendogs:bytes [tier B]
    void GetLastWidth() const; // 0x007372F4 | nintendogs:bytes [tier B]
    void GetLastHeight() const; // 0x00737324 | nintendogs:bytes [tier B]
    void GetLastMakerNoteSize(unsigned) const; // 0x0073734C | nintendogs:bytes [tier B]
    void GetLastMakerNotePointer(unsigned) const; // 0x007373E0 | nintendogs:bytes [tier B]
    void GetLastUserMakerNoteSize() const; // 0x00737430 | nintendogs:bytes [tier B]
    void GetLastUserMakerNotePointer() const; // 0x00737480 | nintendogs:bytes [tier B]
};
} // namespace CTR
} // namespace jpeg
} // namespace nn
