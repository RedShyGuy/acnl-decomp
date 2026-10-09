#pragma once

#include "decomp.h"

namespace nn {
namespace ngc {
namespace CTR {
// The word lists of the shared contents (ngword:/<n>.txt), for the regions and languages (names
// are ours).
enum ProfanityFilterPatternList : u8
{
    PATTERNLIST_JAPAN_JAPANESE = 0,
    PATTERNLIST_AMERICA_ENGLISH = 1,
    PATTERNLIST_AMERICA_FRENCH = 2,
    PATTERNLIST_AMERICA_SPANISH = 3,
    PATTERNLIST_EUROPE_ENGLISH = 4,
    PATTERNLIST_EUROPE_FRENCH = 5,
    PATTERNLIST_EUROPE_GERMAN = 6,
    PATTERNLIST_EUROPE_ITALIAN = 7,
    PATTERNLIST_EUROPE_SPANISH = 8,
    PATTERNLIST_EUROPE_DUTCH = 9,
    PATTERNLIST_KOREA_KOREAN = 10,
    PATTERNLIST_CHINA_SIMPLIFIED_CHINESE = 11,
    PATTERNLIST_EUROPE_PORTUGUESE = 12,
    PATTERNLIST_EUROPE_RUSSIAN = 13,
    PATTERNLIST_AMERICA_PORTUGUESE = 14,
    PATTERNLIST_TAIWAN_TRADITIONAL_CHINESE = 15,
    PATTERNLIST_TAIWAN_ENGLISH = 16,
    PATTERNLIST_MAX = 17
};

// RTTI N2nn3ngc3CTR19ProfanityFilterBaseE @ 0x008CF7BC
// The helpers of the filter: the user input in the form of the word lists (full width letters and
// digits as ASCII, hiragana and half width katakana as katakana). No data, no virtual functions.
class ProfanityFilterBase
{
public:
    static const wchar_t WORD_LENGTH_MAX = 64;

    bool IsIncludesAtSign(const wchar_t* text, int length); // 0x003E1640 | fefates:bytes [tier B]
    // the input for the masking functions: textMap gets how many characters of text each
    // character of converted stands for (name is ours)
    static void ConvertUserInputForText(wchar_t* converted, u8* textMap, int size, const wchar_t* text); // 0x003E168C (name is ours)
    // the input for a word to check, without spaces
    static void ConvertUserInputForWord(wchar_t* converted, int size, const wchar_t* word); // 0x003E192C | fefates:bytes [tier B]
    // the word lists for the region and language of the system (flag is not used)
    void GetPatternListsFromRegion(nn::ngc::CTR::ProfanityFilterPatternList* lists, int* count, bool flag); // 0x003E1BA8 | fefates:bytes-fuzzy [tier B]
    // a line of a word list as a regular expression (* and ^ $ at the ends to \b); false if it
    // does not fit (name is ours)
    static bool ConvertPatternToRegularExpression(wchar_t* regularExpression, const wchar_t* pattern, u32 length); // 0x003E1E10 (name is ours)
};
} // namespace CTR
} // namespace ngc
} // namespace nn
