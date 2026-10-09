#pragma once

#include "decomp.h"
#include "nn/ngc/ngc_RegexDfaState.h"
#include "nn/ngc/ngc_UnitList.h"

namespace nn {
namespace ngc {
// Runs the DFA of a regular expression over a text. Member names are ours.
class RegexMatch
{
public:
    // the assertions as the bits that JumpAtomicZeroWidthAssersion takes
    static const int ASSERTION_BIT_BEGIN_LINE = 1 << ASSERTION_BEGIN_LINE;
    static const int ASSERTION_BIT_END_LINE = 1 << ASSERTION_END_LINE;
    static const int ASSERTION_BIT_WORD_BOUNDARY = 1 << ASSERTION_WORD_BOUNDARY;
    static const int ASSERTION_BIT_NOT_WORD_BOUNDARY = 1 << ASSERTION_NOT_WORD_BOUNDARY;
    static const int ASSERTION_BIT_BEGIN_INPUT = 1 << ASSERTION_BEGIN_INPUT;
    static const int ASSERTION_BIT_END_INPUT = 1 << ASSERTION_END_INPUT;
    static const int ASSERTION_BIT_END_INPUT_LINE = 1 << ASSERTION_END_INPUT_LINE;

    RegexMatch() {}

    // follows the assertion links of the current state whose assertion is in assertions
    void JumpAtomicZeroWidthAssersion(int assertions); // 0x003DAD74 | fefates:bytes [tier B]
    // the first match from position start on: *matchStart, *matchLength (the longest match
    // there); with isSkipSpace, spaces inside a match are skipped
    bool Search(const nn::ngc::UnitList<nn::ngc::RegexDfaState>* states, const wchar_t* text, int start, bool isSkipSpace, int* matchStart,
                int* matchLength); // 0x003DADC8 (name is ours)
    // does the whole text match
    bool IsMatch(const nn::ngc::UnitList<nn::ngc::RegexDfaState>* states, const wchar_t* text); // 0x003DB0DC | fefates:bytes [tier B]

    UnitList<RegexDfaState>::ConstIterator m_State; // 0x0
};
ASSERT_SIZE(RegexMatch, 0x4);
} // namespace ngc
} // namespace nn
