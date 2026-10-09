#include "nn/ngc/ngc_RegexFastMatch.h"
#include "nn/ngc/ngc_Api.h"
#include "nn/ngc/ngc_RegexStateLink.h"
#include <wchar.h>

namespace nn {
namespace ngc {
// 0x003DC384 | fefates:bytes [tier B]
bool nn::ngc::RegexFastMatch::IsMatchString(TokenIterator token, unsigned int start, unsigned int length)
{
    for (u32 i = start; i < start + length; i++, ++token) {
        if (m_Text[i] == token->m_Character) {
            continue;
        }
        // the same character in another case
        const CaseFoldingPair* pair = GetCaseFoldingPair(token->m_Character);
        if (pair == NULL) {
            return false;
        }
        bool isMatch = false;
        for (s32 k = 0; k < 3; k++) {
            if (pair->m_Pairs[k] == 0) {
                break;
            }
            if (m_Text[i] == pair->m_Pairs[k]) {
                isMatch = true;
                break;
            }
        }
        if (!isMatch) {
            return false;
        }
    }
    return true;
}

// 0x003DC43C | fefates:bytes [tier B]
int nn::ngc::RegexFastMatch::MatchFast(const nn::ngc::UnitList<nn::ngc::RegexToken>* tokens, const wchar_t* text)
{
    m_Text = text;
    m_Length = wcslen(text);
    TokenIterator first = tokens->Begin();
    if (first->m_Type == RegexToken::TYPE_CHARACTER) {
        // "abc", "abc.*", "abc$"
        TokenIterator it = first;
        u32 count = 0;
        while (it->m_Type == RegexToken::TYPE_CHARACTER) {
            ++it;
            count++;
        }
        if (it->m_Type == RegexToken::TYPE_BUILT_IN_CHAR_CLASS) {
            if (it->m_CharClassType != BUILT_IN_CHAR_CLASS_ANY) {
                return RESULT_NOT_SIMPLE;
            }
            ++it;
            if (it->m_Type != RegexToken::TYPE_MORE_THAN_ZERO) {
                return RESULT_NOT_SIMPLE;
            }
            ++it;
            if (it->m_Type != RegexToken::TYPE_END) {
                return RESULT_NOT_SIMPLE;
            }
            if (m_Length < count) {
                return RESULT_NO_MATCH;
            }
            return IsMatchString(first, 0, count) ? RESULT_MATCH : RESULT_NO_MATCH;
        }
        if (it->m_Type == RegexToken::TYPE_ATOMIC_ZERO_WIDTH) {
            if (it->m_AssertionType != ASSERTION_END_LINE) {
                return RESULT_NOT_SIMPLE;
            }
            ++it;
            if (it->m_Type != RegexToken::TYPE_END) {
                return RESULT_NOT_SIMPLE;
            }
            if (m_Length != count) {
                return RESULT_NO_MATCH;
            }
            return IsMatchString(first, 0, count) ? RESULT_MATCH : RESULT_NO_MATCH;
        }
        if (it->m_Type == RegexToken::TYPE_END) {
            if (m_Length != count) {
                return RESULT_NO_MATCH;
            }
            return IsMatchString(first, 0, count) ? RESULT_MATCH : RESULT_NO_MATCH;
        }
        return RESULT_NOT_SIMPLE;
    }
    if (first->m_Type == RegexToken::TYPE_BUILT_IN_CHAR_CLASS) {
        // ".*abc.*", ".*abc"
        if (first->m_CharClassType != BUILT_IN_CHAR_CLASS_ANY) {
            return RESULT_NOT_SIMPLE;
        }
        TokenIterator it = first;
        ++it;
        if (it->m_Type != RegexToken::TYPE_MORE_THAN_ZERO) {
            return RESULT_NOT_SIMPLE;
        }
        ++it;
        TokenIterator characters = it;
        u32 count = 0;
        while (it->m_Type == RegexToken::TYPE_CHARACTER) {
            ++it;
            count++;
        }
        if (it->m_Type == RegexToken::TYPE_BUILT_IN_CHAR_CLASS) {
            if (count == 0 || it->m_CharClassType != BUILT_IN_CHAR_CLASS_ANY) {
                return RESULT_NOT_SIMPLE;
            }
            ++it;
            if (it->m_Type != RegexToken::TYPE_MORE_THAN_ZERO) {
                return RESULT_NOT_SIMPLE;
            }
            ++it;
            if (it->m_Type != RegexToken::TYPE_END) {
                return RESULT_NOT_SIMPLE;
            }
            if (count > m_Length) {
                return RESULT_NO_MATCH;
            }
            for (u32 position = 0; position + count <= m_Length; position++) {
                if (IsMatchString(characters, position, count)) {
                    return RESULT_MATCH;
                }
            }
            return RESULT_NO_MATCH;
        }
        if (it->m_Type == RegexToken::TYPE_END) {
            if (count == 0) {
                return RESULT_NOT_SIMPLE;
            }
            if (m_Length < count) {
                return RESULT_NO_MATCH;
            }
            return IsMatchString(characters, m_Length - count, count) ? RESULT_MATCH : RESULT_NO_MATCH;
        }
        return RESULT_NOT_SIMPLE;
    }
    if (first->m_Type == RegexToken::TYPE_ATOMIC_ZERO_WIDTH && first->m_AssertionType == ASSERTION_BEGIN_LINE) {
        // "^abc$", "^abc" (both compared with the whole text)
        TokenIterator it = first;
        ++it;
        TokenIterator characters = it;
        u32 count = 0;
        while (it->m_Type == RegexToken::TYPE_CHARACTER) {
            ++it;
            count++;
        }
        if (it->m_Type == RegexToken::TYPE_ATOMIC_ZERO_WIDTH) {
            if (count == 0 || it->m_AssertionType != ASSERTION_END_LINE) {
                return RESULT_NOT_SIMPLE;
            }
            ++it;
            if (it->m_Type != RegexToken::TYPE_END) {
                return RESULT_NOT_SIMPLE;
            }
            if (m_Length != count) {
                return RESULT_NO_MATCH;
            }
            return IsMatchString(characters, 0, count) ? RESULT_MATCH : RESULT_NO_MATCH;
        }
        if (it->m_Type == RegexToken::TYPE_END) {
            if (count == 0) {
                return RESULT_NOT_SIMPLE;
            }
            if (m_Length != count) {
                return RESULT_NO_MATCH;
            }
            return IsMatchString(characters, 0, count) ? RESULT_MATCH : RESULT_NO_MATCH;
        }
        return RESULT_NOT_SIMPLE;
    }
    return RESULT_NOT_SIMPLE;
}

// 0x003DC75C | fefates:callgraph [tier C]
nn::ngc::RegexFastMatch::RegexFastMatch() : m_Text(NULL), m_Length(0)
{
}

} // namespace ngc
} // namespace nn
