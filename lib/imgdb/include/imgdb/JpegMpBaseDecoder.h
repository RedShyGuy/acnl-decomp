#pragma once

#include "decomp.h"

namespace imgdb {
// RTTI N5imgdb17JpegMpBaseDecoderE @ 0x008D2B6C
class JpegMpBaseDecoder
{
public:
    JpegMpBaseDecoder(); // ctor address unknown
    void ComputeShrinkLevel(int&, int&, int, int); // 0x005AAFF4 | nintendogs:bytes [tier B]
    void GetDateTime(nn::fnd::DateTime&) const; // 0x00755B94 | nintendogs:bytes [tier B]
    void GetSysMakerNote(imgdb::SysMakerNote&) const; // 0x00755BEC | nintendogs:bytes [tier B]
    void GetSysMakerNote() const; // 0x00755C5C | nintendogs:bytes [tier B]
};
} // namespace imgdb
