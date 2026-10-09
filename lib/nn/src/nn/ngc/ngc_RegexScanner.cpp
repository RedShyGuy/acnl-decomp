#include "nn/ngc/ngc_RegexScanner.h"
#include "nn/ngc/ngc_Api.h"
#include "nn/ngc/ngc_RegexStateLink.h"
#include <wchar.h>

namespace nn {
namespace ngc {
namespace {
const wchar_t IDEOGRAPHIC_SPACE = 0x3000;

typedef RegexScanner S;

// by the character (from SCAN_TABLE_BEGIN on)
// 0x0097EA1C
RegexScanner::ScanFunction s_RootScanFunctions[RegexScanner::SCAN_TABLE_SIZE] = {
    &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault,                // ' ' ! " #
    &S::ScanRootDollar, &S::ScanRootDefault, &S::ScanRootAmpersand, &S::ScanRootDefault,               // $ % & '
    &S::ScanRootOpenParenthesis, &S::ScanRootCloseParenthesis, &S::ScanRootAsterisk, &S::ScanRootPlus,  // ( ) * +
    &S::ScanRootDefault, &S::ScanRootHyphen, &S::ScanRootDot, &S::ScanRootDefault,                     // , - . /
    &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault,                // 0 - 3
    &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault,                // 4 - 7
    &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault,                // 8 9 : ;
    &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootQuestion,               // < = > ?
    &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault,                // @ A B C
    &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault,                // D - G
    &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault,                // H - K
    &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault,                // L - O
    &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault,                // P - S
    &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault,                // T - W
    &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootOpenSqureBracket,       // X Y Z [
    &S::ScanRootBackslash, &S::ScanRootCloseSqureBracket, &S::ScanRootCircumflex, &S::ScanRootDefault, // \ ] ^ _
    &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault,                // ` a b c
    &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault,                // d - g
    &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault,                // h - k
    &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault,                // l - o
    &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault,                // p - s
    &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault,                // t - w
    &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootDefault, &S::ScanRootOpenBrace,              // x y z {
    &S::ScanRootVerticalBar, &S::ScanRootDefault, &S::ScanRootDefault,                                 // | } ~
};

// by the character behind the backslash
// 0x0097ED14
RegexScanner::ScanFunction s_BackslashScanFunctions[RegexScanner::SCAN_TABLE_SIZE] = {
    &S::ScanBackslashDefault, &S::ScanBackslashDefault, &S::ScanBackslashDefault, &S::ScanBackslashDefault,     // ' ' ! " #
    &S::ScanBackslashDefault, &S::ScanBackslashDefault, &S::ScanBackslashDefault, &S::ScanBackslashDefault,     // $ % & '
    &S::ScanBackslashDefault, &S::ScanBackslashDefault, &S::ScanBackslashDefault, &S::ScanBackslashDefault,     // ( ) * +
    &S::ScanBackslashDefault, &S::ScanBackslashDefault, &S::ScanBackslashDefault, &S::ScanBackslashDefault,     // , - . /
    &S::ScanBackslash_0, &S::ScanBackslashNotSupported, &S::ScanBackslashNotSupported, &S::ScanBackslashNotSupported,   // 0 - 3
    &S::ScanBackslashNotSupported, &S::ScanBackslashNotSupported, &S::ScanBackslashNotSupported, &S::ScanBackslashNotSupported, // 4 - 7
    &S::ScanBackslashNotSupported, &S::ScanBackslashNotSupported, &S::ScanBackslashDefault, &S::ScanBackslashDefault,  // 8 9 : ;
    &S::ScanBackslashDefault, &S::ScanBackslashDefault, &S::ScanBackslashDefault, &S::ScanBackslashDefault,     // < = > ?
    &S::ScanBackslashDefault, &S::ScanBackslash_A, &S::ScanBackslash_B, &S::ScanBackslashNotSupported,          // @ A B C
    &S::ScanBackslash_D, &S::ScanBackslashNotSupported, &S::ScanBackslashNotSupported, &S::ScanBackslashNotSupported, // D E F G
    &S::ScanBackslashNotSupported, &S::ScanBackslashNotSupported, &S::ScanBackslashNotSupported, &S::ScanBackslashNotSupported, // H - K
    &S::ScanBackslashNotSupported, &S::ScanBackslashNotSupported, &S::ScanBackslashNotSupported, &S::ScanBackslashNotSupported, // L - O
    &S::ScanBackslash_P, &S::ScanBackslashNotSupported, &S::ScanBackslashNotSupported, &S::ScanBackslash_S,     // P Q R S
    &S::ScanBackslashNotSupported, &S::ScanBackslashNotSupported, &S::ScanBackslashNotSupported, &S::ScanBackslash_W, // T U V W
    &S::ScanBackslashNotSupported, &S::ScanBackslashNotSupported, &S::ScanBackslash_Z, &S::ScanBackslashDefault,     // X Y Z [
    &S::ScanBackslashDefault, &S::ScanBackslashDefault, &S::ScanBackslashDefault, &S::ScanBackslashDefault,     // \ ] ^ _
    &S::ScanBackslashDefault, &S::ScanBackslash_a, &S::ScanBackslash_b, &S::ScanBackslash_c,                    // ` a b c
    &S::ScanBackslash_d, &S::ScanBackslash_e, &S::ScanBackslash_f, &S::ScanBackslashNotSupported,               // d e f g
    &S::ScanBackslashNotSupported, &S::ScanBackslashNotSupported, &S::ScanBackslashNotSupported, &S::ScanBackslashNotSupported, // h - k
    &S::ScanBackslashNotSupported, &S::ScanBackslashNotSupported, &S::ScanBackslash_n, &S::ScanBackslashNotSupported, // l m n o
    &S::ScanBackslash_p, &S::ScanBackslashNotSupported, &S::ScanBackslash_r, &S::ScanBackslash_s,               // p q r s
    &S::ScanBackslash_t, &S::ScanBackslash_u, &S::ScanBackslashNotSupported, &S::ScanBackslash_w,               // t u v w
    &S::ScanBackslash_x, &S::ScanBackslashNotSupported, &S::ScanBackslash_z, &S::ScanBackslashDefault,          // x y z {
    &S::ScanBackslashDefault, &S::ScanBackslashDefault, &S::ScanBackslashDefault,                               // | } ~
};

inline bool IsDigit(wchar_t c)
{
    return c >= L'0' && c <= L'9';
}

inline bool IsOctalDigit(wchar_t c)
{
    return c >= L'0' && c <= L'7';
}
} // namespace

// 0x003DB27C | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanRootDot(TokenIterator& token)
{
    if (m_CharClassDepth > 0) {
        token->m_Type = RegexToken::TYPE_CHARACTER;
        token->m_Character = L'.';
    } else {
        token->m_Type = RegexToken::TYPE_BUILT_IN_CHAR_CLASS;
        token->m_CharClassType = BUILT_IN_CHAR_CLASS_ANY;
    }
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DB2BC | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanRootPlus(TokenIterator& token)
{
    if (m_CharClassDepth > 0) {
        token->m_Type = RegexToken::TYPE_CHARACTER;
        token->m_Character = L'+';
    } else {
        token->m_Type = RegexToken::TYPE_MORE_THAN_ONE;
    }
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DB2FC | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanRootDollar(TokenIterator& token)
{
    if (m_CharClassDepth > 0) {
        token->m_Type = RegexToken::TYPE_CHARACTER;
        token->m_Character = L'$';
    } else {
        token->m_Type = RegexToken::TYPE_ATOMIC_ZERO_WIDTH;
        token->m_AssertionType = ASSERTION_END_LINE;
    }
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DB34C | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanRootHyphen(TokenIterator& token)
{
    if (m_CharClassDepth > 0) {
        token->m_Type = RegexToken::TYPE_CHAR_CLASS_RANGE;
    } else {
        token->m_Type = RegexToken::TYPE_CHARACTER;
        token->m_Character = L'-';
    }
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DB38C (name is ours)
int nn::ngc::RegexScanner::LexicalAnalysis(nn::ngc::ProfanityFilterTemporaryPool* pool, const wchar_t* pattern)
{
    return LexicalAnalysis(pool, pattern, wcslen(pattern));
}

// 0x003DB3BC | fefates:bytes [tier B]
int nn::ngc::RegexScanner::LexicalAnalysis(nn::ngc::ProfanityFilterTemporaryPool* pool, const wchar_t* pattern, unsigned int length)
{
    m_Tokens.SetPool(pool);
    m_Pattern = pattern;
    m_Length = length;
    m_CharClassDepth = 0;
    // inside \Q...\E every character stands for itself
    bool isQuoted = false;
    while (m_Position != m_Length) {
        if (isQuoted) {
            if (m_Position != m_Length - 1 && m_Pattern[m_Position] == L'\\' && m_Pattern[m_Position + 1] == L'E') {
                m_Position += 2;
                isQuoted = false;
                continue;
            }
            if (m_Position == m_Length - 1 || m_Pattern[m_Position] != L'\\') {
                if (m_Pattern[m_Position] == L' ' || m_Pattern[m_Position] == IDEOGRAPHIC_SPACE) {
                    m_Position++;
                    continue;
                }
            }
            TokenIterator token = m_Tokens.PushBackNew();
            if (token.IsNull()) {
                return RESULT_OUT_OF_MEMORY;
            }
            token->m_Type = RegexToken::TYPE_CHARACTER;
            token->m_Character = m_Pattern[m_Position];
            m_Position++;
        } else {
            if (m_Position != m_Length - 1 && m_Pattern[m_Position] == L'\\' && m_Pattern[m_Position + 1] == L'Q') {
                m_Position += 2;
                isQuoted = true;
                continue;
            }
            if (m_Position != m_Length - 1 && m_Pattern[m_Position] == L'\\') {
                if (m_Pattern[m_Position + 1] == L'E') {
                    return RESULT_UNMATCHED_QUOTE_END;
                }
            } else if (m_Pattern[m_Position] == L' ' || m_Pattern[m_Position] == IDEOGRAPHIC_SPACE) {
                m_Position++;
                continue;
            }
            TokenIterator token = m_Tokens.PushBackNew();
            if (token.IsNull()) {
                return RESULT_OUT_OF_MEMORY;
            }
            wchar_t c = m_Pattern[m_Position];
            int result;
            if (static_cast<u32>(c - SCAN_TABLE_BEGIN) < SCAN_TABLE_SIZE) {
                result = (this->*s_RootScanFunctions[c - SCAN_TABLE_BEGIN])(token);
            } else {
                result = ScanRootDefault(token);
            }
            if (result != RESULT_SUCCESS) {
                return result;
            }
        }
    }
    TokenIterator end = m_Tokens.PushBackNew();
    if (end.IsNull()) {
        return RESULT_OUT_OF_MEMORY;
    }
    end->m_Type = RegexToken::TYPE_END;
    return RESULT_SUCCESS;
}

// 0x003DB624 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanBackslash_0(TokenIterator& token)
{
    // \0 with up to three octal digits (at most 0377)
    m_Position++;
    if (m_Position == m_Length) {
        return RESULT_INVALID_OCTAL;
    }
    wchar_t c = m_Pattern[m_Position];
    wchar_t value;
    if (c >= L'0' && c <= L'3') {
        value = c - L'0';
        m_Position++;
        if (m_Position != m_Length && IsOctalDigit(m_Pattern[m_Position])) {
            value = static_cast<wchar_t>(value << 3) | (m_Pattern[m_Position] - L'0');
            m_Position++;
            if (m_Position != m_Length && IsOctalDigit(m_Pattern[m_Position])) {
                value = static_cast<wchar_t>(value << 3) | (m_Pattern[m_Position] - L'0');
                m_Position++;
            }
        }
    } else if (c >= L'4' && c <= L'7') {
        value = c - L'0';
        m_Position++;
        if (m_Position != m_Length && IsOctalDigit(m_Pattern[m_Position])) {
            value = static_cast<wchar_t>(value << 3) | (m_Pattern[m_Position] - L'0');
            m_Position++;
        }
    } else {
        return RESULT_INVALID_OCTAL;
    }
    token->m_Type = RegexToken::TYPE_CHARACTER;
    token->m_Character = value;
    return RESULT_SUCCESS;
}

// 0x003DB754 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanBackslash_A(TokenIterator& token)
{
    token->m_Type = RegexToken::TYPE_ATOMIC_ZERO_WIDTH;
    token->m_AssertionType = ASSERTION_BEGIN_INPUT;
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DB780 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanBackslash_B(TokenIterator& token)
{
    token->m_Type = RegexToken::TYPE_ATOMIC_ZERO_WIDTH;
    token->m_AssertionType = ASSERTION_NOT_WORD_BOUNDARY;
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DB7AC | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanBackslash_D(TokenIterator& token)
{
    token->m_Type = RegexToken::TYPE_DENIAL_CHAR_CLASS;
    token->m_CharClassType = BUILT_IN_CHAR_CLASS_DIGIT;
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DB7D8 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanBackslash_P(TokenIterator& token)
{
    m_Position++;
    if (m_Position == m_Length || m_Pattern[m_Position] != L'{') {
        return RESULT_INVALID_CHAR_CLASS_NAME;
    }
    m_Position++;
    if (m_Position == m_Length) {
        return RESULT_INVALID_CHAR_CLASS_NAME;
    }
    unsigned int nameLength;
    BuiltInCharClassType type = GetCharClassTypeFromName(&m_Pattern[m_Position], m_Length - m_Position, &nameLength);
    if (type == BUILT_IN_CHAR_CLASS_INVALID) {
        return RESULT_INVALID_CHAR_CLASS_NAME;
    }
    token->m_Type = RegexToken::TYPE_DENIAL_CHAR_CLASS;
    token->m_CharClassType = type;
    m_Position += nameLength;
    return RESULT_SUCCESS;
}

// 0x003DB86C | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanBackslash_S(TokenIterator& token)
{
    token->m_Type = RegexToken::TYPE_DENIAL_CHAR_CLASS;
    token->m_CharClassType = BUILT_IN_CHAR_CLASS_SPACE;
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DB898 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanBackslash_W(TokenIterator& token)
{
    token->m_Type = RegexToken::TYPE_DENIAL_CHAR_CLASS;
    token->m_CharClassType = BUILT_IN_CHAR_CLASS_WORD;
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DB8C4 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanBackslash_Z(TokenIterator& token)
{
    token->m_Type = RegexToken::TYPE_ATOMIC_ZERO_WIDTH;
    token->m_AssertionType = ASSERTION_END_INPUT_LINE;
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DB8F0 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanBackslash_a(TokenIterator& token)
{
    token->m_Type = RegexToken::TYPE_CHARACTER;
    token->m_Character = L'\a';
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DB91C | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanBackslash_b(TokenIterator& token)
{
    token->m_Type = RegexToken::TYPE_ATOMIC_ZERO_WIDTH;
    token->m_AssertionType = ASSERTION_WORD_BOUNDARY;
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DB948 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanBackslash_c(TokenIterator& token)
{
    // \cA .. \c_: control characters
    m_Position++;
    if (m_Position == m_Length) {
        return RESULT_INVALID_CONTROL;
    }
    if (static_cast<u32>(m_Pattern[m_Position] - L'A') > L'_' - L'A') {
        return RESULT_INVALID_CONTROL;
    }
    token->m_Type = RegexToken::TYPE_CHARACTER;
    token->m_Character = m_Pattern[m_Position] - L'@';
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DB9B4 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanBackslash_d(TokenIterator& token)
{
    token->m_Type = RegexToken::TYPE_BUILT_IN_CHAR_CLASS;
    token->m_CharClassType = BUILT_IN_CHAR_CLASS_DIGIT;
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DB9E0 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanBackslash_e(TokenIterator& token)
{
    token->m_Type = RegexToken::TYPE_CHARACTER;
    token->m_Character = 0x1B;
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DBA0C | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanBackslash_f(TokenIterator& token)
{
    token->m_Type = RegexToken::TYPE_CHARACTER;
    token->m_Character = L'\f';
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DBA38 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanBackslash_n(TokenIterator& token)
{
    token->m_Type = RegexToken::TYPE_CHARACTER;
    token->m_Character = L'\n';
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DBA64 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanBackslash_p(TokenIterator& token)
{
    m_Position++;
    if (m_Position == m_Length || m_Pattern[m_Position] != L'{') {
        return RESULT_INVALID_CHAR_CLASS_NAME;
    }
    m_Position++;
    if (m_Position == m_Length) {
        return RESULT_INVALID_CHAR_CLASS_NAME;
    }
    unsigned int nameLength;
    BuiltInCharClassType type = GetCharClassTypeFromName(&m_Pattern[m_Position], m_Length - m_Position, &nameLength);
    if (type == BUILT_IN_CHAR_CLASS_INVALID) {
        return RESULT_INVALID_CHAR_CLASS_NAME;
    }
    token->m_Type = RegexToken::TYPE_BUILT_IN_CHAR_CLASS;
    token->m_CharClassType = type;
    m_Position += nameLength;
    return RESULT_SUCCESS;
}

// 0x003DBAF8 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanBackslash_r(TokenIterator& token)
{
    token->m_Type = RegexToken::TYPE_CHARACTER;
    token->m_Character = L'\r';
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DBB24 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanBackslash_s(TokenIterator& token)
{
    token->m_Type = RegexToken::TYPE_BUILT_IN_CHAR_CLASS;
    token->m_CharClassType = BUILT_IN_CHAR_CLASS_SPACE;
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DBB50 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanBackslash_t(TokenIterator& token)
{
    token->m_Type = RegexToken::TYPE_CHARACTER;
    token->m_Character = L'\t';
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DBB7C | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanBackslash_u(TokenIterator& token)
{
    m_Position++;
    if (m_Position == m_Length) {
        return RESULT_INVALID_HEXADECIMAL;
    }
    return ScanUnicodeHexadecimal(token, 4);
}

// 0x003DBBA0 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanBackslash_w(TokenIterator& token)
{
    token->m_Type = RegexToken::TYPE_BUILT_IN_CHAR_CLASS;
    token->m_CharClassType = BUILT_IN_CHAR_CLASS_WORD;
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DBBCC | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanBackslash_x(TokenIterator& token)
{
    m_Position++;
    if (m_Position == m_Length) {
        return RESULT_INVALID_HEXADECIMAL;
    }
    return ScanUnicodeHexadecimal(token, 2);
}

// 0x003DBBF0 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanBackslash_z(TokenIterator& token)
{
    token->m_Type = RegexToken::TYPE_ATOMIC_ZERO_WIDTH;
    token->m_AssertionType = ASSERTION_END_INPUT;
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DBC1C | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanRootDefault(TokenIterator& token)
{
    token->m_Type = RegexToken::TYPE_CHARACTER;
    token->m_Character = m_Pattern[m_Position];
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DBC50 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanRootAsterisk(TokenIterator& token)
{
    if (m_CharClassDepth > 0) {
        token->m_Type = RegexToken::TYPE_CHARACTER;
        token->m_Character = L'*';
    } else {
        token->m_Type = RegexToken::TYPE_MORE_THAN_ZERO;
    }
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DBC90 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanRootQuestion(TokenIterator& token)
{
    if (m_CharClassDepth > 0) {
        token->m_Type = RegexToken::TYPE_CHARACTER;
        token->m_Character = L'?';
    } else {
        token->m_Type = RegexToken::TYPE_ONE_OR_ZERO;
    }
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DBCD0 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanRootAmpersand(TokenIterator& token)
{
    if (m_CharClassDepth > 0 && m_Position != m_Length - 1 && m_Pattern[m_Position + 1] == L'&') {
        token->m_Type = RegexToken::TYPE_CHAR_CLASS_AND;
        m_Position += 2;
    } else {
        token->m_Type = RegexToken::TYPE_CHARACTER;
        token->m_Character = L'&';
        m_Position++;
    }
    return RESULT_SUCCESS;
}

// 0x003DBD50 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanRootBackslash(TokenIterator& token)
{
    m_Position++;
    if (m_Position == m_Length) {
        return RESULT_ESCAPE_AT_END;
    }
    wchar_t c = m_Pattern[m_Position];
    if (static_cast<u32>(c - SCAN_TABLE_BEGIN) < SCAN_TABLE_SIZE) {
        return (this->*s_BackslashScanFunctions[c - SCAN_TABLE_BEGIN])(token);
    }
    return ScanBackslashDefault(token);
}

namespace {
// a decimal number of a repeat from the position on; -1 if there is none or it is too big
// (name is ours)
DECOMP_ALWAYS_INLINE s32 ScanNumber(RegexScanner* scanner)
{
    if (scanner->m_Position == scanner->m_Length || !IsDigit(scanner->m_Pattern[scanner->m_Position])) {
        return -1;
    }
    s32 value = 0;
    while (scanner->m_Position != scanner->m_Length && IsDigit(scanner->m_Pattern[scanner->m_Position])) {
        value = value * 10 + (scanner->m_Pattern[scanner->m_Position] - L'0');
        if (value >= 0x10000) {
            return -1;
        }
        scanner->m_Position++;
    }
    return value;
}
} // namespace

// 0x003DBDBC | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanRootOpenBrace(TokenIterator& token)
{
    if (m_CharClassDepth > 0) {
        token->m_Type = RegexToken::TYPE_CHARACTER;
        token->m_Character = L'{';
        m_Position++;
        return RESULT_SUCCESS;
    }
    m_Position++;
    s32 min = ScanNumber(this);
    if (min == -1 || m_Position == m_Length) {
        return RESULT_INVALID_REPEAT;
    }
    if (m_Pattern[m_Position] == L'}') {
        token->m_Min = min;
        token->m_Type = RegexToken::TYPE_REPEAT_EQUALS;
        m_Position++;
        return RESULT_SUCCESS;
    }
    if (m_Pattern[m_Position] != L',') {
        return RESULT_INVALID_REPEAT;
    }
    m_Position++;
    s32 max = ScanNumber(this);
    if (m_Position == m_Length) {
        return RESULT_INVALID_REPEAT;
    }
    if (max == -1) {
        if (m_Pattern[m_Position] != L'}') {
            return RESULT_INVALID_REPEAT;
        }
        token->m_Min = min;
        token->m_Type = RegexToken::TYPE_MORE_THAN;
        m_Position++;
        return RESULT_SUCCESS;
    }
    if (m_Pattern[m_Position] != L'}') {
        return RESULT_INVALID_REPEAT;
    }
    if (min == max) {
        token->m_Min = min;
        token->m_Type = RegexToken::TYPE_REPEAT_EQUALS;
        m_Position++;
        return RESULT_SUCCESS;
    }
    if (min >= max) {
        return RESULT_INVALID_REPEAT;
    }
    token->m_Min = min;
    token->m_Max = max;
    token->m_Type = RegexToken::TYPE_RANGE;
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DBFE4 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanRootCircumflex(TokenIterator& token)
{
    if (m_CharClassDepth > 0) {
        token->m_Type = RegexToken::TYPE_CHARACTER;
        token->m_Character = L'^';
    } else {
        token->m_Type = RegexToken::TYPE_ATOMIC_ZERO_WIDTH;
        token->m_AssertionType = ASSERTION_BEGIN_LINE;
    }
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DC024 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanRootVerticalBar(TokenIterator& token)
{
    if (m_CharClassDepth > 0) {
        token->m_Type = RegexToken::TYPE_CHARACTER;
        token->m_Character = L'|';
    } else {
        token->m_Type = RegexToken::TYPE_ALTERNATION;
    }
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DC064 | fefates:callgraph [tier C]
int nn::ngc::RegexScanner::ScanBackslashDefault(TokenIterator& token)
{
    token->m_Type = RegexToken::TYPE_CHARACTER;
    token->m_Character = m_Pattern[m_Position];
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DC098 (name is ours)
int nn::ngc::RegexScanner::ScanBackslashNotSupported(TokenIterator& token)
{
    return RESULT_UNSUPPORTED_ESCAPE;
}

// 0x003DC0A0 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanUnicodeHexadecimal(TokenIterator& token, int digitCount)
{
    token->m_Type = RegexToken::TYPE_CHARACTER;
    token->m_Character = 0;
    for (int i = 0; i < digitCount; i++) {
        token->m_Character <<= 4;
        if (m_Position == m_Length) {
            return RESULT_INVALID_HEXADECIMAL;
        }
        wchar_t c = m_Pattern[m_Position];
        if (c >= L'0' && c <= L'9') {
            token->m_Character |= c - L'0';
        } else if (c >= L'a' && c <= L'f') {
            token->m_Character |= c - L'a' + 10;
        } else if (c >= L'A' && c <= L'F') {
            token->m_Character |= c - L'A' + 10;
        } else {
            return RESULT_INVALID_HEXADECIMAL;
        }
        m_Position++;
    }
    return RESULT_SUCCESS;
}

// 0x003DC180 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanRootOpenParenthesis(TokenIterator& token)
{
    if (m_CharClassDepth > 0) {
        token->m_Type = RegexToken::TYPE_CHARACTER;
        token->m_Character = L'(';
    } else {
        token->m_Type = RegexToken::TYPE_BEGIN_GROUP;
    }
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DC1C0 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanRootCloseParenthesis(TokenIterator& token)
{
    if (m_CharClassDepth > 0) {
        token->m_Type = RegexToken::TYPE_CHARACTER;
        token->m_Character = L')';
    } else {
        token->m_Type = RegexToken::TYPE_END_GROUP;
    }
    m_Position++;
    return RESULT_SUCCESS;
}

// 0x003DC200 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanRootOpenSqureBracket(TokenIterator& token)
{
    if (m_Position != m_Length - 1 && m_Pattern[m_Position + 1] == L'^') {
        token->m_Type = RegexToken::TYPE_BEGIN_DENIAL_CHAR_CLASS;
        m_Position += 2;
    } else {
        token->m_Type = RegexToken::TYPE_BEGIN_CHAR_CLASS;
        m_Position++;
    }
    m_CharClassDepth++;
    return RESULT_SUCCESS;
}

// 0x003DC268 | fefates:bytes [tier B]
int nn::ngc::RegexScanner::ScanRootCloseSqureBracket(TokenIterator& token)
{
    token->m_Type = RegexToken::TYPE_END_CHAR_CLASS;
    m_Position++;
    m_CharClassDepth--;
    return RESULT_SUCCESS;
}

// 0x003DC294 | fefates:bytes [tier B]
nn::ngc::RegexScanner::RegexScanner() : m_Pattern(NULL), m_Position(0), m_Length(0), m_CharClassDepth(0)
{
}

} // namespace ngc
} // namespace nn
