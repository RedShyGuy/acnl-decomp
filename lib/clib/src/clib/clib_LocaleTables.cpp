#include "clib/clib_Locale.h"
#include <cstddef>

// The tables of the "C" locale (ASCII only), in a source file of their own like in the C library.

#define CTYPE_ROW_8(x) x, x, x, x, x, x, x, x

namespace clib {
// (name is ours)
// 0x00945418
extern const LocaleBlock<CtypeData> s_CtypeLocale = {
    sizeof(LocaleBlock<CtypeData>),
    "C",
    -8,
    {{
        0,
        // 0x00-0x1F: control characters, 0x09-0x0D also white space
        CTYPE_ROW_8(CTYPE_CNTRL), CTYPE_CNTRL, CTYPE_CNTRL | CTYPE_SPACE, CTYPE_CNTRL | CTYPE_SPACE, CTYPE_CNTRL | CTYPE_SPACE,
        CTYPE_CNTRL | CTYPE_SPACE, CTYPE_CNTRL | CTYPE_SPACE, CTYPE_CNTRL, CTYPE_CNTRL, CTYPE_ROW_8(CTYPE_CNTRL), CTYPE_ROW_8(CTYPE_CNTRL),
        // 0x20: the space, 0x21-0x2F punctuation
        CTYPE_BLANK | CTYPE_SPACE, CTYPE_PUNCT, CTYPE_PUNCT, CTYPE_PUNCT, CTYPE_PUNCT, CTYPE_PUNCT, CTYPE_PUNCT, CTYPE_PUNCT,
        CTYPE_ROW_8(CTYPE_PUNCT),
        // 0x30-0x39 digits, 0x3A-0x40 punctuation
        CTYPE_ROW_8(CTYPE_DIGIT), CTYPE_DIGIT, CTYPE_DIGIT, CTYPE_PUNCT, CTYPE_PUNCT, CTYPE_PUNCT, CTYPE_PUNCT, CTYPE_PUNCT, CTYPE_PUNCT,
        CTYPE_PUNCT,
        // 0x41-0x5A upper case letters (A-F hexadecimal), 0x5B-0x60 punctuation
        CTYPE_UPPER | CTYPE_XDIGIT, CTYPE_UPPER | CTYPE_XDIGIT, CTYPE_UPPER | CTYPE_XDIGIT, CTYPE_UPPER | CTYPE_XDIGIT,
        CTYPE_UPPER | CTYPE_XDIGIT, CTYPE_UPPER | CTYPE_XDIGIT, CTYPE_UPPER, CTYPE_UPPER, CTYPE_ROW_8(CTYPE_UPPER), CTYPE_ROW_8(CTYPE_UPPER),
        CTYPE_UPPER, CTYPE_UPPER, CTYPE_PUNCT, CTYPE_PUNCT, CTYPE_PUNCT, CTYPE_PUNCT, CTYPE_PUNCT,
        CTYPE_PUNCT,
        // 0x61-0x7A lower case letters (a-f hexadecimal), 0x7B-0x7E punctuation, 0x7F control
        CTYPE_LOWER | CTYPE_XDIGIT, CTYPE_LOWER | CTYPE_XDIGIT, CTYPE_LOWER | CTYPE_XDIGIT, CTYPE_LOWER | CTYPE_XDIGIT,
        CTYPE_LOWER | CTYPE_XDIGIT, CTYPE_LOWER | CTYPE_XDIGIT, CTYPE_LOWER, CTYPE_LOWER, CTYPE_ROW_8(CTYPE_LOWER), CTYPE_ROW_8(CTYPE_LOWER),
        CTYPE_LOWER, CTYPE_LOWER, CTYPE_PUNCT, CTYPE_PUNCT, CTYPE_PUNCT, CTYPE_PUNCT, CTYPE_CNTRL,
        // 0x80-0xFF: no class
    }, {}},
};

// (name is ours)
// 0x00945528
extern const LocaleBlock<NumericData> s_NumericLocale = {
    sizeof(LocaleBlock<NumericData>),
    "C",
    -8,
    {offsetof(NumericData, m_Strings), offsetof(NumericData, m_Strings) + 2, offsetof(NumericData, m_Strings) + 3, {'.', '\0', '\0', '\0'}},
};

// (name is ours)
// 0x00945544
extern const LocaleBlock<CollateData> s_CollateLocale = {
    sizeof(LocaleBlock<CollateData>),
    "C",
    -8,
    {0},
};
} // namespace clib
