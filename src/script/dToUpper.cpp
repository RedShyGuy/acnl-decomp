#include "script/dToUpper.h"
#include "script/dIWord.h"

// The capital letters of the scripts (the name of the file is ours).

namespace script {
// 0x005EAD64 (name is ours)
bool ToUpper(char16* c)
{
    u32 code = *c;
    char16 upper = 0;
    if ((code >= 'a' && code <= 'z') || code - 0xFF41 <= 25 || (code != 0xF7 && code - 0xE0 <= 30)) {
        // ASCII, full width, Latin-1
        upper = code - 0x20;
    } else if (code - 0x3B1 <= 24 ? code != 0x3C2 : code - 0x430 <= 31) {
        // Greek (not the final sigma), Cyrillic
        upper = code - 0x20;
    } else if (code - 0x100 <= 119) {
        // Latin Extended-A: the odd ones are small
        if ((code & 1) == 0) {
            return false;
        }
        upper = code - 1;
    } else {
        for (const CasePair* pair = s_CasePairs; pair < s_CasePairs + 15; pair++) {
            if (pair->lower == code) {
                upper = pair->upper;
                break;
            }
            if (pair->upper == code) {
                break;
            }
        }
        if (upper == 0 && code - 0x179 <= 5 && (code & 1) == 0) {
            upper = code - 1;
        }
    }
    if (upper == 0) {
        return false;
    }
    *c = upper;
    return true;
}

// 0x005EAE88 (name is ours)
bool ToLower(char16* c)
{
    u32 code = *c;
    char16 lower = 0;
    if ((code >= 'A' && code <= 'Z') || code - 0xFF21 <= 25 || (code != 0xD7 && code - 0xC0 <= 30)) {
        // ASCII, full width, Latin-1
        lower = code + 0x20;
    } else if (code - 0x391 <= 24 || code - 0x410 <= 31) {
        // Greek, Cyrillic
        lower = code + 0x20;
    } else if (code - 0x100 <= 119) {
        // Latin Extended-A: the even ones are capitals
        if ((code & 1) != 0) {
            return false;
        }
        lower = code + 1;
    } else {
        for (const CasePair* pair = s_CasePairs; pair < s_CasePairs + 15; pair++) {
            if (pair->upper == code) {
                lower = pair->lower;
                break;
            }
            if (pair->lower == code) {
                break;
            }
        }
        if (lower == 0 && code - 0x179 <= 5 && (code & 1) != 0) {
            lower = code + 1;
        }
    }
    if (lower == 0) {
        return false;
    }
    *c = lower;
    return true;
}

// 0x005EAF9C (name is ours)
void ToUpperFirst(IWord* word)
{
    char16* text = word->GetWord();
    u32 size = word->GetSize();
    if (text != NULL && size != 0 && *text != 0) {
        ToUpper(text);
    }
}
} // namespace script
