#pragma once

#include "decomp.h"

namespace nn {
namespace ngc {
class CharacterRangeList;

// The built-in character classes: . \d \s \w, then the \p{...} names of s_CharClassNames
// (Lower, Upper, ...); the values index s_BuiltInCharClasses. Names are ours.
enum BuiltInCharClassType : u8
{
    BUILT_IN_CHAR_CLASS_ANY = 0,        // .
    BUILT_IN_CHAR_CLASS_DIGIT = 1,      // \d
    BUILT_IN_CHAR_CLASS_SPACE = 2,      // \s
    BUILT_IN_CHAR_CLASS_WORD = 3,       // \w
    BUILT_IN_CHAR_CLASS_NAMED_COUNT = 132,
    BUILT_IN_CHAR_CLASS_INVALID = 133
};

// a character and up to three characters that are the same without case (0 = none); names are
// ours
struct CaseFoldingPair
{
    wchar_t m_Character;    // 0x0
    wchar_t m_Pairs[3];     // 0x2
};
ASSERT_SIZE(CaseFoldingPair, 0x8);

const CaseFoldingPair* GetCaseFoldingPair(wchar_t character); // 0x003DF98C | fefates:bytes [tier B]
bool CombineBuiltInCharClass(nn::ngc::CharacterRangeList* list, nn::ngc::BuiltInCharClassType type); // 0x003DFC78 | fefates:bytes [tier B]
// the class whose name (with the closing brace) starts name; *nameLength gets its length
nn::ngc::BuiltInCharClassType GetCharClassTypeFromName(const wchar_t* name, unsigned int length, unsigned int* nameLength); // 0x003DFCE0 | fefates:bytes [tier B]
} // namespace ngc
} // namespace nn
