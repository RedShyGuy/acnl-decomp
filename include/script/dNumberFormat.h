#pragma once

#include "decomp.h"
#include "sead/seadBufferedSafeStringBase.h"
#include "script/dWordPtr.h"

namespace script {
class IWord;

// Numbers as text for inserted words: right or left aligned with spaces or zeros, thousands separators by region,
// decimal places, hexadecimal groups. All formatters share one buffer of 19 characters. Class, function and member
// names are ours (the class has no RTTI).
class NumberFormat
{
public:
    static const u32 BUFFER_SIZE = 19;

    // the alignment and padding of Format; the odd modes add thousands separators
    enum PadMode
    {
        PAD_NONE = 0,
        PAD_NONE_SEPARATED = 1,
        PAD_SPACE_RIGHT = 2,      // spaces after the number
        PAD_SPACE_RIGHT_SEPARATED = 3,
        PAD_SPACE_LEFT = 4,       // spaces before the number
        PAD_SPACE_LEFT_SEPARATED = 5,
        PAD_ZERO_LEFT = 6,        // zeros before the number
        PAD_ZERO_LEFT_SEPARATED = 7,
        PAD_ZERO_RIGHT = 8,       // zeros after the number (decimal places)
        PAD_BY_REGION = 10        // separated in the Americas and Europe, otherwise none
    };

    // the thousands separator of Format
    enum Separator
    {
        SEPARATOR_COMMA = 0,
        SEPARATOR_PERIOD = 1,
        SEPARATOR_SPACE = 2,
        SEPARATOR_BY_REGION = 4
    };

    NumberFormat(); // 0x005F7D0C

    // the number with the separators of the region (period in German, otherwise by region)
    void Format(IWord* pDst, s32 value, s32 width); // 0x005F7614
    // the number with decimal places
    void FormatDecimal(IWord* pDst, f32 value, s32 decimals); // 0x005F7664
    // the low 48 bits as three groups of four hexadecimal digits
    void FormatHex(IWord* pDst, u64 value); // 0x005F77EC
    // the number with a width of 1..10 (otherwise the word is emptied), padding and separators
    void Format(IWord* pDst, s32 value, s32 width, u8 padMode, u8 separator); // 0x005F7980

private:
    // the number in the buffer is moved right to the width and the front filled with c
    DECOMP_NOINLINE static void PadLeft(s32 width, char16 c); // 0x005F789C
    // (inline) the length of the number in the buffer (at most BUFFER_SIZE - 1)
    static s32 GetLength();

public:
    sead::BufferedSafeStringBase<wchar_t> m_String; // 0x00 on the buffer
    WordPtr m_Word;                                 // 0x0C on the buffer

    static char16 s_Buffer[BUFFER_SIZE];
};
ASSERT_SIZE(NumberFormat, 0x18);
} // namespace script
