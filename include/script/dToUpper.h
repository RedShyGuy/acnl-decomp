#pragma once

#include "decomp.h"

namespace script {
class IWord;

// the small letters without a regular capital (names are ours)
struct CasePair
{
    char16 lower;
    char16 upper;
};
extern const CasePair s_CasePairs[15];

// makes a character a capital letter; false if it is none or already one (names are ours)
bool ToUpper(char16* c); // 0x005EAD64
// makes a character a small letter; false if it is none or already one (names are ours)
bool ToLower(char16* c); // 0x005EAE88
// the first character of a word
void ToUpperFirst(IWord* word); // 0x005EAF9C
} // namespace script
