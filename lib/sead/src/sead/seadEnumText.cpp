#include "sead/seadCriticalSection.h"
#include "sead/seadEnumText.h"

namespace sead {
namespace {
bool isSpace(char c)
{
    return c == ' ' || c == '\t' || c == '\n';
}

// the end of a name or value: the next ',' or '=' (or the end of the string)
char* findSeparator(char* p)
{
    while (*p != ',' && *p != '=' && *p != '\0') {
        p++;
    }
    return p;
}

// the last character of [start, last] that is no space (start at the latest; ARMCC compares the
// difference as a 64-bit value)
char* trimEnd(char* start, char* last)
{
    while (isSpace(*last) && static_cast<s64>(last - start) > 0) {
        last--;
    }
    return last;
}
} // namespace

// 0x00560D84 (name is ours)
void sead::EnumText::parse(char** texts, char* allText, s32 num)
{
    s32 count = 0;
    char* p = allText;
    if (*p == '\0') {
        return;
    }
    do {
        while (*p == ',' || *p == ' ' || *p == '\t' || *p == '\n') {
            p++;
        }
        char* name = p;
        if (*name == '\0') {
            return;
        }
        texts[count++] = name;
        p = findSeparator(name);
        char* last = trimEnd(name, p - 1);
        char c = *p;
        last[1] = '\0';
        if (c == '\0') {
            return;
        }
        if (c == '=') {
            // the value (the original looks for its end but does not keep it)
            p++;
            char* value = p;
            p = findSeparator(value);
            trimEnd(value, p - 1);
            if (*p == '\0') {
                return;
            }
        }
        if (count >= num) {
            return;
        }
    } while (*++p != '\0');
}

// 0x00560ED8 (name is ours)
sead::CriticalSection& sead::EnumText::getParseCS()
{
    // 0x00AE6658 (guard 0x009763C8)
    static CriticalSection s_ParseCS;
    return s_ParseCS;
}
} // namespace sead
