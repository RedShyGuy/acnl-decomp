#pragma once

#include "decomp.h"

namespace pead {
class BinaryStreamFormat;
class TextStreamFormat;

// pia's copy of our sead::Stream: only the two global formats of the modes are in the binary (no function of
// the stream itself); the class and member names are ours.
class Stream
{
public:
    static BinaryStreamFormat s_BinaryStreamFormat;
    static TextStreamFormat s_TextStreamFormat;
};
} // namespace pead
