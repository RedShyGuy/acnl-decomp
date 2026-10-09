#include "nn/ngc/ngc_RegexMatch.h"

namespace nn {
namespace ngc {
namespace {
const wchar_t NEXT_LINE = 0x85;
const wchar_t LINE_SEPARATOR = 0x2028;
const wchar_t PARAGRAPH_SEPARATOR = 0x2029;
const wchar_t IDEOGRAPHIC_SPACE = 0x3000;

// the characters that end a word (\b \B); names are ours
inline bool IsSpace(wchar_t c)
{
    return c == L' ' || (c >= L'\t' && c <= L'\r') || c == NEXT_LINE || c == LINE_SEPARATOR || c == PARAGRAPH_SEPARATOR ||
           c == IDEOGRAPHIC_SPACE;
}

// the length of the line break at text (CR LF: 2), -1 if there is none
inline int GetLineBreakLength(const wchar_t* text)
{
    switch (text[0]) {
    case L'\n':
    case NEXT_LINE:
    case LINE_SEPARATOR:
    case PARAGRAPH_SEPARATOR:
        return 1;
    case L'\r':
        if (text[1] == L'\n') {
            return 2;
        }
        return 1;
    default:
        return -1;
    }
}

const int ASSERTIONS_BEGIN = RegexMatch::ASSERTION_BIT_BEGIN_LINE | RegexMatch::ASSERTION_BIT_BEGIN_INPUT;
const int ASSERTIONS_END = RegexMatch::ASSERTION_BIT_END_LINE | RegexMatch::ASSERTION_BIT_END_INPUT | RegexMatch::ASSERTION_BIT_END_INPUT_LINE;
} // namespace

// 0x003DAD74 | fefates:bytes [tier B]
void nn::ngc::RegexMatch::JumpAtomicZeroWidthAssersion(int assertions)
{
    UnitList<RegexStateLink<RegexDfaState> >::ConstIterator link = m_State->m_Links.Begin();
    while (link != m_State->m_Links.End()) {
        if (link->m_Type == RegexStateLink<RegexDfaState>::TYPE_ATOMIC_ZERO_WIDTH && (assertions & (1 << link->m_AssertionType))) {
            // on from the new state
            m_State = link->m_Target;
            link = m_State->m_Links.Begin();
        } else {
            ++link;
        }
    }
}

// 0x003DADC8 (name is ours)
bool nn::ngc::RegexMatch::Search(const nn::ngc::UnitList<nn::ngc::RegexDfaState>* states, const wchar_t* text, int start, bool isSkipSpace,
                                 int* matchStart, int* matchLength)
{
    for (; text[start] != 0; start++) {
        bool isPreviousSpace = start == 0 || IsSpace(text[start - 1]);
        bool isMatched = false;
        m_State = states->Begin();
        JumpAtomicZeroWidthAssersion(ASSERTIONS_BEGIN);
        int length = 0;
        int lineBreak = 0;
        for (; text[start + length] != 0; length++) {
            if (lineBreak <= 0) {
                lineBreak = GetLineBreakLength(&text[start + length]);
                if (lineBreak > 0) {
                    JumpAtomicZeroWidthAssersion(ASSERTION_BIT_END_LINE);
                }
            }
            bool isSpace = IsSpace(text[start + length]);
            JumpAtomicZeroWidthAssersion(isSpace != isPreviousSpace ? ASSERTION_BIT_WORD_BOUNDARY : ASSERTION_BIT_NOT_WORD_BOUNDARY);
            isPreviousSpace = isSpace;
            if (m_State->m_IsAccept) {
                isMatched = true;
                *matchStart = start;
                *matchLength = length;
            }
            wchar_t c = text[start + length];
            bool isFound = false;
            for (UnitList<RegexStateLink<RegexDfaState> >::ConstIterator link = m_State->m_Links.Begin(); link != m_State->m_Links.End(); ++link) {
                if (link->IsMatch(c)) {
                    m_State = link->m_Target;
                    isFound = true;
                    break;
                }
            }
            if (!isFound) {
                if (isMatched) {
                    return true;
                }
                // spaces inside a word are skipped
                if (!(isSkipSpace && isSpace && length != 0)) {
                    goto next;
                }
            }
            if (lineBreak != -1) {
                lineBreak--;
                if (lineBreak == 0) {
                    JumpAtomicZeroWidthAssersion(ASSERTION_BIT_BEGIN_LINE);
                }
            }
            if (m_State->m_IsAccept) {
                isMatched = true;
                *matchStart = start;
                *matchLength = length + 1;
            }
        }
        JumpAtomicZeroWidthAssersion((isPreviousSpace ? ASSERTION_BIT_NOT_WORD_BOUNDARY : ASSERTION_BIT_WORD_BOUNDARY) | ASSERTIONS_END);
        if (m_State->m_IsAccept) {
            *matchStart = start;
            *matchLength = length;
            return true;
        }
        if (isMatched) {
            return true;
        }
    next:;
    }
    return false;
}

// 0x003DB0DC | fefates:bytes [tier B]
bool nn::ngc::RegexMatch::IsMatch(const nn::ngc::UnitList<nn::ngc::RegexDfaState>* states, const wchar_t* text)
{
    m_State = states->Begin();
    JumpAtomicZeroWidthAssersion(ASSERTIONS_BEGIN);
    bool isPreviousSpace = true;
    int lineBreak = 0;
    for (; *text != 0; text++) {
        if (lineBreak <= 0) {
            lineBreak = GetLineBreakLength(text);
            if (lineBreak > 0) {
                JumpAtomicZeroWidthAssersion(ASSERTION_BIT_END_LINE);
            }
        }
        bool isSpace = IsSpace(*text);
        JumpAtomicZeroWidthAssersion(isSpace != isPreviousSpace ? ASSERTION_BIT_WORD_BOUNDARY : ASSERTION_BIT_NOT_WORD_BOUNDARY);
        isPreviousSpace = isSpace;
        wchar_t c = *text;
        bool isFound = false;
        for (UnitList<RegexStateLink<RegexDfaState> >::ConstIterator link = m_State->m_Links.Begin(); link != m_State->m_Links.End(); ++link) {
            if (link->IsMatch(c)) {
                m_State = link->m_Target;
                isFound = true;
                break;
            }
        }
        if (!isFound) {
            return false;
        }
        if (lineBreak != -1) {
            lineBreak--;
            if (lineBreak == 0) {
                JumpAtomicZeroWidthAssersion(ASSERTION_BIT_BEGIN_LINE);
            }
        }
    }
    JumpAtomicZeroWidthAssersion((isPreviousSpace ? ASSERTION_BIT_NOT_WORD_BOUNDARY : ASSERTION_BIT_WORD_BOUNDARY) | ASSERTIONS_END);
    return m_State->m_IsAccept;
}

} // namespace ngc
} // namespace nn
