#include "nn/ngc/ngc_RegexStateLink.h"
#include "nn/ngc/ngc_RegexDfaState.h"
#include "nn/ngc/ngc_RegexNfaState.h"

// The template functions of RegexStateLink. The original has them out of line in its section of
// template instances (0x7E4D2C...); IsMatch lies elsewhere (0x827CC8).

namespace nn {
namespace ngc {
template <typename T>
RegexStateLink<T>::RegexStateLink() : m_Type(TYPE_EPSILON)
{
}

template <typename T>
RegexStateLink<T>::~RegexStateLink()
{
}

template <typename T>
void RegexStateLink<T>::LinkTo(TargetIterator target)
{
    m_Type = TYPE_EPSILON;
    m_Target = target;
}

template <typename T>
void RegexStateLink<T>::LinkTo(TargetIterator target, AtomicZeroWidthAssersionType assertionType)
{
    m_Type = TYPE_ATOMIC_ZERO_WIDTH;
    m_AssertionType = assertionType;
    m_Target = target;
}

template <typename T>
void RegexStateLink<T>::LinkTo(TargetIterator target, ProfanityFilterTemporaryPool* pool)
{
    m_Type = TYPE_CHAR_CLASS;
    m_CharClass.SetPool(pool);
    m_Target = target;
}

template <typename T>
void RegexStateLink<T>::LinkTo(TargetIterator target, wchar_t character)
{
    m_Type = TYPE_CHARACTER;
    m_Character = character;
    m_Target = target;
}

template <typename T>
bool RegexStateLink<T>::IsMatch(wchar_t character) const
{
    switch (m_Type) {
    case TYPE_CHARACTER:
        return m_Character == character;
    case TYPE_CHAR_CLASS:
        for (CharacterRangeList::ConstIterator it = m_CharClass.Begin(); it != m_CharClass.End(); ++it) {
            if (it->m_Begin <= character && character <= it->m_End) {
                return true;
            }
        }
        return false;
    default:
        return false;
    }
}

template <typename T>
bool RegexStateLink<T>::DeepCopyFrom(const RegexStateLink& other, TargetIterator target, ProfanityFilterTemporaryPool* pool)
{
    switch (other.m_Type) {
    case TYPE_EPSILON:
        LinkTo(target);
        break;
    case TYPE_CHARACTER:
        LinkTo(target, other.m_Character);
        break;
    case TYPE_CHAR_CLASS:
        LinkTo(target, pool);
        if (!m_CharClass.CopyFrom(other.m_CharClass)) {
            return false;
        }
        break;
    case TYPE_ATOMIC_ZERO_WIDTH:
        LinkTo(target, other.m_AssertionType);
        break;
    }
    return true;
}

template <typename T>
bool RegexStateLink<T>::DeepCopyFrom(const RegexStateLink& other, ProfanityFilterTemporaryPool* pool)
{
    return DeepCopyFrom(other, other.m_Target, pool);
}

// 0x007E4D2C | fefates:callgraph [tier C]
template void RegexStateLink<RegexDfaState>::LinkTo(UnitList<RegexDfaState>::Iterator, AtomicZeroWidthAssersionType);
// 0x007E4D44 | fefates:callgraph [tier C]
template void RegexStateLink<RegexDfaState>::LinkTo(UnitList<RegexDfaState>::Iterator, ProfanityFilterTemporaryPool*);
// 0x007E4D5C | fefates:callgraph [tier C]
template void RegexStateLink<RegexDfaState>::LinkTo(UnitList<RegexDfaState>::Iterator, wchar_t);
// 0x007E4D74 | fefates:callgraph [tier C]
template RegexStateLink<RegexDfaState>::RegexStateLink();
// 0x00827CC8 | fefates:callgraph [tier C]
template bool RegexStateLink<RegexDfaState>::IsMatch(wchar_t) const;

// 0x007E4D98 | fefates:bytes [tier B]
template bool nn::ngc::RegexStateLink<nn::ngc::RegexNfaState>::DeepCopyFrom(const nn::ngc::RegexStateLink<nn::ngc::RegexNfaState>&,
                                                                            nn::ngc::UnitList<nn::ngc::RegexNfaState>::Iterator,
                                                                            nn::ngc::ProfanityFilterTemporaryPool*);
// 0x007E4EC8 | fefates:bytes [tier B]
template bool nn::ngc::RegexStateLink<nn::ngc::RegexNfaState>::DeepCopyFrom(const nn::ngc::RegexStateLink<nn::ngc::RegexNfaState>&,
                                                                            nn::ngc::ProfanityFilterTemporaryPool*);
// 0x007E4EE4 | fefates:bytes [tier B]
template void RegexStateLink<RegexNfaState>::LinkTo(UnitList<RegexNfaState>::Iterator);
// 0x007E4EF8 | fefates:callgraph [tier C]
template void RegexStateLink<RegexNfaState>::LinkTo(UnitList<RegexNfaState>::Iterator, AtomicZeroWidthAssersionType);
// 0x007E4F10 | fefates:callgraph [tier C]
template void RegexStateLink<RegexNfaState>::LinkTo(UnitList<RegexNfaState>::Iterator, ProfanityFilterTemporaryPool*);
// 0x007E4F28 | fefates:callgraph [tier C]
template void RegexStateLink<RegexNfaState>::LinkTo(UnitList<RegexNfaState>::Iterator, wchar_t);
// 0x007E4F40 | fefates:callgraph [tier C]
template RegexStateLink<RegexNfaState>::RegexStateLink();
// 0x007E4F64 | fefates:bytes [tier B]
template RegexStateLink<RegexNfaState>::~RegexStateLink();
// 0x00827D34 | fefates:callgraph [tier C]
template bool RegexStateLink<RegexNfaState>::IsMatch(wchar_t) const;

// 0x007E4FC0 | fefates:bytes [tier B]
template <>
UnitList<RegexStateLink<RegexNfaState> >::Iterator UnitList<RegexStateLink<RegexNfaState> >::PushBackNew()
{
    return Insert(End());
}

} // namespace ngc
} // namespace nn
