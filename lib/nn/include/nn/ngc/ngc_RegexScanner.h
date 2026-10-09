#pragma once

#include "decomp.h"
#include "nn/ngc/ngc_RegexToken.h"
#include "nn/ngc/ngc_UnitList.h"

namespace nn {
namespace ngc {
// Splits a regular expression into tokens. The Scan functions get the new token and read from
// m_Position on; the root ones are chosen by the character (s_RootScanFunctions), the ones of
// an escape by the character behind the backslash (s_BackslashScanFunctions). They return 0 or
// an error (the RESULT_ values are ours). Member names are ours.
class RegexScanner
{
public:
    typedef UnitList<RegexToken>::Iterator TokenIterator;
    typedef int (RegexScanner::*ScanFunction)(TokenIterator& token);

    static const int RESULT_SUCCESS = 0;
    static const int RESULT_OUT_OF_MEMORY = 1;
    static const int RESULT_UNMATCHED_QUOTE_END = 2;    // \E without \Q
    static const int RESULT_INVALID_REPEAT = 3;         // {n,m}
    static const int RESULT_ESCAPE_AT_END = 4;          // a backslash at the end
    static const int RESULT_UNSUPPORTED_ESCAPE = 5;
    static const int RESULT_INVALID_HEXADECIMAL = 6;    // \x \u
    static const int RESULT_INVALID_OCTAL = 7;          // \0
    static const int RESULT_INVALID_CONTROL = 8;        // \c
    static const int RESULT_INVALID_CHAR_CLASS_NAME = 9;    // \p{...}

    // the first character of the scan tables
    static const wchar_t SCAN_TABLE_BEGIN = 32;
    static const u32 SCAN_TABLE_SIZE = 95;

    RegexScanner(); // 0x003DC294 | fefates:bytes [tier B]
    int LexicalAnalysis(nn::ngc::ProfanityFilterTemporaryPool* pool, const wchar_t* pattern, unsigned int length); // 0x003DB3BC | fefates:bytes [tier B]
    int LexicalAnalysis(nn::ngc::ProfanityFilterTemporaryPool* pool, const wchar_t* pattern); // 0x003DB38C (name is ours)

    int ScanRootDot(TokenIterator& token); // 0x003DB27C | fefates:bytes [tier B]
    int ScanRootPlus(TokenIterator& token); // 0x003DB2BC | fefates:bytes [tier B]
    int ScanRootDollar(TokenIterator& token); // 0x003DB2FC | fefates:bytes [tier B]
    int ScanRootHyphen(TokenIterator& token); // 0x003DB34C | fefates:bytes [tier B]
    int ScanBackslash_0(TokenIterator& token); // 0x003DB624 | fefates:bytes [tier B]
    int ScanBackslash_A(TokenIterator& token); // 0x003DB754 | fefates:bytes [tier B]
    int ScanBackslash_B(TokenIterator& token); // 0x003DB780 | fefates:bytes [tier B]
    int ScanBackslash_D(TokenIterator& token); // 0x003DB7AC | fefates:bytes [tier B]
    int ScanBackslash_P(TokenIterator& token); // 0x003DB7D8 | fefates:bytes [tier B]
    int ScanBackslash_S(TokenIterator& token); // 0x003DB86C | fefates:bytes [tier B]
    int ScanBackslash_W(TokenIterator& token); // 0x003DB898 | fefates:bytes [tier B]
    int ScanBackslash_Z(TokenIterator& token); // 0x003DB8C4 | fefates:bytes [tier B]
    int ScanBackslash_a(TokenIterator& token); // 0x003DB8F0 | fefates:bytes [tier B]
    int ScanBackslash_b(TokenIterator& token); // 0x003DB91C | fefates:bytes [tier B]
    int ScanBackslash_c(TokenIterator& token); // 0x003DB948 | fefates:bytes [tier B]
    int ScanBackslash_d(TokenIterator& token); // 0x003DB9B4 | fefates:bytes [tier B]
    int ScanBackslash_e(TokenIterator& token); // 0x003DB9E0 | fefates:bytes [tier B]
    int ScanBackslash_f(TokenIterator& token); // 0x003DBA0C | fefates:bytes [tier B]
    int ScanBackslash_n(TokenIterator& token); // 0x003DBA38 | fefates:bytes [tier B]
    int ScanBackslash_p(TokenIterator& token); // 0x003DBA64 | fefates:bytes [tier B]
    int ScanBackslash_r(TokenIterator& token); // 0x003DBAF8 | fefates:bytes [tier B]
    int ScanBackslash_s(TokenIterator& token); // 0x003DBB24 | fefates:bytes [tier B]
    int ScanBackslash_t(TokenIterator& token); // 0x003DBB50 | fefates:bytes [tier B]
    int ScanBackslash_u(TokenIterator& token); // 0x003DBB7C | fefates:bytes [tier B]
    int ScanBackslash_w(TokenIterator& token); // 0x003DBBA0 | fefates:bytes [tier B]
    int ScanBackslash_x(TokenIterator& token); // 0x003DBBCC | fefates:bytes [tier B]
    int ScanBackslash_z(TokenIterator& token); // 0x003DBBF0 | fefates:bytes [tier B]
    int ScanRootDefault(TokenIterator& token); // 0x003DBC1C | fefates:bytes [tier B]
    int ScanRootAsterisk(TokenIterator& token); // 0x003DBC50 | fefates:bytes [tier B]
    int ScanRootQuestion(TokenIterator& token); // 0x003DBC90 | fefates:bytes [tier B]
    int ScanRootAmpersand(TokenIterator& token); // 0x003DBCD0 | fefates:bytes [tier B]
    int ScanRootBackslash(TokenIterator& token); // 0x003DBD50 | fefates:bytes [tier B]
    int ScanRootOpenBrace(TokenIterator& token); // 0x003DBDBC | fefates:bytes [tier B]
    int ScanRootCircumflex(TokenIterator& token); // 0x003DBFE4 | fefates:bytes [tier B]
    int ScanRootVerticalBar(TokenIterator& token); // 0x003DC024 | fefates:bytes [tier B]
    int ScanBackslashDefault(TokenIterator& token); // 0x003DC064 | fefates:callgraph [tier C]
    int ScanBackslashNotSupported(TokenIterator& token); // 0x003DC098 (name is ours)
    DECOMP_NOINLINE int ScanUnicodeHexadecimal(TokenIterator& token, int digitCount); // 0x003DC0A0 | fefates:bytes [tier B]
    int ScanRootOpenParenthesis(TokenIterator& token); // 0x003DC180 | fefates:bytes [tier B]
    int ScanRootCloseParenthesis(TokenIterator& token); // 0x003DC1C0 | fefates:bytes [tier B]
    int ScanRootOpenSqureBracket(TokenIterator& token); // 0x003DC200 | fefates:bytes [tier B]
    int ScanRootCloseSqureBracket(TokenIterator& token); // 0x003DC268 | fefates:bytes [tier B]

    UnitList<RegexToken> m_Tokens;  // 0x00
    const wchar_t* m_Pattern;       // 0x0C
    u32 m_Position;                 // 0x10
    u32 m_Length;                   // 0x14
    s32 m_CharClassDepth;           // 0x18, inside [...] when > 0
};
ASSERT_SIZE(RegexScanner, 0x1C);
} // namespace ngc
} // namespace nn
