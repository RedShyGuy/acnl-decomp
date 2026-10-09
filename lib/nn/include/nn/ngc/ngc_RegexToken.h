#pragma once

#include "decomp.h"

namespace nn {
namespace ngc {
// A token of a regular expression (RegexScanner -> RegexNfaParser). Names are ours.
struct RegexToken
{
    enum Type : u8
    {
        TYPE_CHARACTER = 0,             // m_Character
        TYPE_ALTERNATION = 1,           // |
        TYPE_BEGIN_CHAR_CLASS = 2,      // [
        TYPE_BEGIN_DENIAL_CHAR_CLASS = 3,   // [^
        TYPE_END_CHAR_CLASS = 4,        // ]
        TYPE_CHAR_CLASS_RANGE = 5,      // - in a class
        TYPE_CHAR_CLASS_AND = 6,        // && in a class
        TYPE_BUILT_IN_CHAR_CLASS = 7,   // m_CharClassType: . \d \s \w \p{...}
        TYPE_DENIAL_CHAR_CLASS = 8,     // m_CharClassType: \D \S \W \P{...}
        TYPE_BEGIN_GROUP = 9,           // (
        TYPE_END_GROUP = 10,            // )
        TYPE_ONE_OR_ZERO = 11,          // ?
        TYPE_MORE_THAN_ZERO = 12,       // *
        TYPE_MORE_THAN_ONE = 13,        // +
        TYPE_REPEAT_EQUALS = 14,        // {n}: m_Min
        TYPE_MORE_THAN = 15,            // {n,}: m_Min
        TYPE_RANGE = 16,                // {n,m}: m_Min, m_Max
        TYPE_ATOMIC_ZERO_WIDTH = 17,    // m_AssertionType: ^ $ \b \B \A \z \Z
        TYPE_END = 18,
        TYPE_COUNT = 19
    };

    u8 m_Type;  // 0x0, Type
    union
    {
        wchar_t m_Character;
        u8 m_CharClassType;     // BuiltInCharClassType
        u8 m_AssertionType;     // AtomicZeroWidthAssersionType
        u16 m_Min;
    };          // 0x2
    u16 m_Max;  // 0x4
};
ASSERT_SIZE(RegexToken, 0x6);
} // namespace ngc
} // namespace nn
