#pragma once

#include "decomp.h"

namespace sead {
class CriticalSection;

// The texts of an enum from one string ("a, b = 1, c"): the game splits the string on the first use (under
// the lock of getParseCS) and indexes the texts by the value. No RTTI; the class and all names are ours.
class EnumText
{
public:
    // splits allText in place into up to num names (separated by ',', a name may be followed by "=value",
    // which is skipped; spaces, tabs and newlines around the names are dropped)
    static void parse(char** texts, char* allText, s32 num); // 0x00560D84 (name is ours)
    // the lock of the first parse
    static CriticalSection& getParseCS(); // 0x00560ED8 (name is ours)
};
} // namespace sead
