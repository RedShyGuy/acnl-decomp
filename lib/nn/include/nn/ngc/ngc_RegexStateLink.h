#pragma once

#include "decomp.h"
#include "nn/ngc/ngc_CharacterRangeList.h"
#include "nn/ngc/ngc_UnitList.h"

namespace nn {
namespace ngc {
// the zero width assertions of a regular expression (values are ours; RegexMatch tests them as
// bits 1 << type)
enum AtomicZeroWidthAssersionType : u8
{
    ASSERTION_BEGIN_LINE = 0,           // ^
    ASSERTION_END_LINE = 1,             // $
    ASSERTION_WORD_BOUNDARY = 2,        // \b
    ASSERTION_NOT_WORD_BOUNDARY = 3,    // \B
    ASSERTION_BEGIN_INPUT = 4,          // \A
    ASSERTION_END_INPUT = 5,            // \z
    ASSERTION_END_INPUT_LINE = 6        // \Z
};

// A transition of an NFA or DFA state to m_Target. Member names are ours.
template <typename T>
class RegexStateLink
{
public:
    enum Type : u8
    {
        TYPE_EPSILON = 1,
        TYPE_CHARACTER = 2,
        TYPE_CHAR_CLASS = 3,
        TYPE_ATOMIC_ZERO_WIDTH = 4
    };

    typedef typename UnitList<T>::Iterator TargetIterator;

    RegexStateLink();
    ~RegexStateLink();
    void LinkTo(TargetIterator target);
    void LinkTo(TargetIterator target, AtomicZeroWidthAssersionType assertionType);
    void LinkTo(TargetIterator target, ProfanityFilterTemporaryPool* pool);
    void LinkTo(TargetIterator target, wchar_t character);
    bool IsMatch(wchar_t character) const;
    bool DeepCopyFrom(const RegexStateLink& other, TargetIterator target, ProfanityFilterTemporaryPool* pool);
    bool DeepCopyFrom(const RegexStateLink& other, ProfanityFilterTemporaryPool* pool);

    TargetIterator m_Target;    // 0x0
    u8 m_Type;                  // 0x4, Type
    union
    {
        wchar_t m_Character;
        AtomicZeroWidthAssersionType m_AssertionType;
    };                          // 0x6
    CharacterRangeList m_CharClass; // 0x8
};
} // namespace ngc
} // namespace nn
