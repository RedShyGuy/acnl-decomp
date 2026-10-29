#pragma once

#include "decomp.h"

namespace sead {
class StreamFormat;
class StreamSrc;
class BinaryStreamFormat;
class TextStreamFormat;
template <typename T>
class SafeStringBase;

// A stream: values in a format (binary or text) on a source. Only writeDecorationText is in the
// binary (the heap dumps write through it), the rest of the class is unknown; the class and member
// names are ours.
class Stream
{
public:
    // writes text that only the text format keeps (a line of a dump)
    void writeDecorationText(const SafeStringBase<char>& text); // 0x00540C08

    u32 mField0;           // 0x0 (unknown)
    StreamFormat* mFormat; // 0x4
    StreamSrc* mSrc;       // 0x8

    // the formats of the modes (names are ours)
    static BinaryStreamFormat s_BinaryStreamFormat;
    static TextStreamFormat s_TextStreamFormat;
};
} // namespace sead
