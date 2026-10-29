#pragma once

#include "decomp.h"

namespace sead {
class PrintOutput;
template <typename T>
class SafeStringBase;

// Writes a format string to a PrintOutput, one argument per format mark: proceedToFormatMark_ writes
// the text up to the next mark and copies the mark ("%" with flags, width and the conversion), out
// writes the argument with it. "%%" is a percent sign, "%@" takes the argument as it is, "%<" makes
// all following arguments plain (no marks). The class and proceedToFormatMark_/flush/out are the
// names of the reference symbols, the members and the other names are ours (layout from the code).
class PrintFormatter
{
public:
    // the longest format mark
    static const s32 MARK_LENGTH_MAX = 32;

    // a formatter without format for output (inline in StringFormatter; name is ours)
    explicit PrintFormatter(PrintOutput* output)
        : mFormat(nullptr), mOutput(output), mPosition(0), mFormatLength(0), mIsPlain(false)
    {
    }

    // the text up to the next format mark; true with the mark (empty for a plain argument) if there
    // is one
    bool proceedToFormatMark_(char* mark); // 0x00545508 | nintendogs:bytes [tier B]
    // writes the rest of the format
    void flush(); // 0x00545A00 | nintendogs:bytes [tier B]
    // the format string first, then the arguments
    PrintFormatter& append(const char* str); // 0x00545AD4 (name is ours)

    // writes the value with the mark (NULL: "%d" or "%u")
    template <typename T>
    static void out(const T& value, const char* mark, PrintOutput* output);
    static void out(const SafeStringBase<char>& str, const char* mark, PrintOutput* output); // 0x00545A50 (name after out)
    // writes length characters of str (-1: all) with the mark (NULL or "%s": as they are)
    static void outString_(const char* mark, PrintOutput* output, const char* str, s32 length); // 0x00545438 (name is ours)

    const char* mFormat;  // 0x00
    PrintOutput* mOutput; // 0x04
    s32 mPosition;        // 0x08, in the format
    s32 mFormatLength;    // 0x0C
    bool mIsPlain;        // 0x10, after "%<"
};
ASSERT_SIZE(PrintFormatter, 0x14);
} // namespace sead
