#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"
#include "script/dIWord.h"
#include "script/dToUpper.h"
#include "Other/dRegion.h"

namespace script {
// (inline) two characters that count as equal: the same, ! and & with their full width forms, hiragana and katakana
// (in Japan) and small and capital letters (name is ours)
inline bool IsFuzzyEqual(char16 a, char16 b)
{
    if (a == b) {
        return true;
    }
    if ((a == '!' || a == 0xFF01) && (b == '!' || b == 0xFF01)) {
        return true;
    }
    if ((a == '&' || a == 0xFF06) && (b == '&' || b == 0xFF06)) {
        return true;
    }
    if (GetRegionIndex() == 0) {
        // hiragana as katakana
        char16 kanaA = a - 0x3041 <= 0x55 ? a + 0x60 : a;
        char16 kanaB = b - 0x3041 <= 0x55 ? b + 0x60 : b;
        if (kanaA == kanaB) {
            return true;
        }
    }
    char16 upperA = a;
    char16 upperB = b;
    ToUpper(&upperA);
    ToUpper(&upperB);
    return upperA == upperB;
}

// compares two texts with IsFuzzyEqual: EngineB walks the second text one character for each character of the first
// (member names are ours)
// RTTI N6script14CmpFuzzyEngineE @ 0x008D38B8
// vtable 0x0090A210 (vptr 0x0090A218), offset_to_top 0, 8 entries
class CmpFuzzyEngine : public ::script::MsgEngine
{
public:
    // RTTI N6script14CmpFuzzyEngine7EngineBE @ 0x008D38A0
    // vtable 0x0090A1E8 (vptr 0x0090A1F0), offset_to_top 0, 8 entries
    class EngineB : public ::script::MsgEngine
    {
    public:
        EngineB() : m_pInfo(NULL), m_Unknown0x64(0), m_IsContinue(false), m_IsMatch(false), m_IsEqual(false) {}
        virtual ~EngineB(); // 0x005E5908 slot 0x00
        // 0x005E58F8 slot 0x04 (deleting dtor)
        virtual bool IsContinue(); // 0x005E58F0 slot 0x10
        virtual void OnChar(const CharInfo* info); // 0x005E57D8 slot 0x14
        virtual void OnTag(const Tag* tag); // 0x005E58EC slot 0x18

        const CharInfo* m_pInfo;  // 0x60 the character of the first text
        u32 m_Unknown0x64;        // 0x64
        bool m_IsContinue;        // 0x68
        bool m_IsMatch;           // 0x69
        u8 _pad0x6A[2];
        bool m_IsEqual;           // 0x6C the result
    };
    ASSERT_SIZE(EngineB, 0x70);

    CmpFuzzyEngine() {}
    virtual ~CmpFuzzyEngine(); // 0x005E5934 slot 0x00
    // 0x005E5920 slot 0x04 (deleting dtor)
    virtual bool IsContinue(); // 0x005E5918 slot 0x10
    virtual void OnChar(const CharInfo* info); // 0x005E5790 slot 0x14
    virtual void OnTag(const Tag* tag); // 0x005E57D4 slot 0x18

    EngineB m_B;  // 0x60
};
ASSERT_SIZE(CmpFuzzyEngine, 0xD0);
} // namespace script
