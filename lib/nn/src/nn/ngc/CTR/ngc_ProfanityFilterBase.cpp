#include "nn/ngc/CTR/ngc_ProfanityFilterBase.h"
#include "nn/cfg/CTR/CTR_Api.h"

namespace nn {
namespace ngc {
namespace CTR {
// a character and what it becomes (name is ours)
struct KanaConversion
{
    wchar_t m_From;
    wchar_t m_To;
};

// the small katakana to the large ones
// 0x008B4346
extern const KanaConversion s_SmallKanaConversions[];
// the half width katakana to the full width ones
// 0x008B4376
extern const KanaConversion s_HalfwidthKanaConversions[];
// the half width katakana with the half width dakuten (voiced) to the full width ones
// 0x008B445E
extern const KanaConversion s_HalfwidthVoicedKanaConversions[];
// with the half width handakuten (semi-voiced)
// 0x008B44B2
extern const KanaConversion s_HalfwidthSemiVoicedKanaConversions[];

namespace {
const s32 SMALL_KANA_CONVERSION_COUNT = 12;
const s32 HALFWIDTH_KANA_CONVERSION_COUNT = 58;
const s32 HALFWIDTH_VOICED_KANA_CONVERSION_COUNT = 21;
const s32 HALFWIDTH_SEMI_VOICED_KANA_CONVERSION_COUNT = 5;

const wchar_t IDEOGRAPHIC_SPACE = 0x3000;
const wchar_t FULLWIDTH_AT_SIGN = 0xFF20;
const wchar_t HALFWIDTH_DAKUTEN = 0xFF9E;
const wchar_t HALFWIDTH_HANDAKUTEN = 0xFF9F;
// full width ASCII - this = ASCII
const wchar_t FULLWIDTH_OFFSET = 0xFEE0;
// katakana - hiragana
const wchar_t KATAKANA_OFFSET = 0x60;

// the longest regular expression of a line of a word list
const u32 REGULAR_EXPRESSION_LENGTH_MAX = 256;

inline bool IsFullwidthAlphanumeric(wchar_t c)
{
    return (c >= 0xFF41 && c <= 0xFF5A) || (c >= 0xFF21 && c <= 0xFF3A) || (c >= 0xFF10 && c <= 0xFF19);
}

inline bool IsHiragana(wchar_t c)
{
    return c >= 0x3041 && c <= 0x3096;
}

inline bool IsKatakana(wchar_t c)
{
    return c >= 0x30A1 && c <= 0x30FA;
}

inline bool IsHalfwidthKatakana(wchar_t c)
{
    return c >= 0xFF66 && c <= 0xFF9F;
}

inline void ConvertSmallKana(wchar_t* c)
{
    for (s32 i = 0; i < SMALL_KANA_CONVERSION_COUNT; i++) {
        if (s_SmallKanaConversions[i].m_From == *c) {
            *c = s_SmallKanaConversions[i].m_To;
            break;
        }
    }
}

// the conversion of c in table to *converted; false if there is none
inline bool ConvertKana(wchar_t* converted, wchar_t c, const KanaConversion* table, s32 count)
{
    for (s32 i = 0; i < count; i++) {
        if (table[i].m_From == c) {
            *converted = table[i].m_To;
            return true;
        }
    }
    return false;
}

inline void CopyCharacters(wchar_t* destination, const wchar_t* source, u32 count)
{
    for (u32 i = 0; i < count; i++) {
        destination[i] = source[i];
    }
}

inline void SetWordBoundary(wchar_t* destination)
{
    destination[0] = L'\\';
    destination[1] = L'b';
}
} // namespace

// 0x003E1640 | fefates:bytes [tier B]
bool nn::ngc::CTR::ProfanityFilterBase::IsIncludesAtSign(const wchar_t* text, int length)
{
    for (int i = 0; i < length; i++) {
        if (text[i] == 0) {
            break;
        }
        if (text[i] == L'@' || text[i] == FULLWIDTH_AT_SIGN) {
            return true;
        }
    }
    return false;
}

// 0x003E168C (name is ours)
void nn::ngc::CTR::ProfanityFilterBase::ConvertUserInputForText(wchar_t* converted, u8* textMap, int size, const wchar_t* text)
{
    int out = 0;
    int in = 0;
    for (;;) {
        if (out == size - 1) {
            converted[out] = 0;
            return;
        }
        wchar_t c = text[in];
        if (IsFullwidthAlphanumeric(c)) {
            converted[out] = c - FULLWIDTH_OFFSET;
            in++;
            textMap[out] = 1;
        } else if (IsHiragana(c)) {
            converted[out] = c + KATAKANA_OFFSET;
            ConvertSmallKana(&converted[out]);
            in++;
            textMap[out] = 1;
        } else if (IsKatakana(c)) {
            converted[out] = c;
            ConvertSmallKana(&converted[out]);
            textMap[out] = 1;
            in++;
        } else if (IsHalfwidthKatakana(c)) {
            if ((text[in + 1] == HALFWIDTH_DAKUTEN &&
                 ConvertKana(&converted[out], c, s_HalfwidthVoicedKanaConversions, HALFWIDTH_VOICED_KANA_CONVERSION_COUNT)) ||
                (text[in + 1] == HALFWIDTH_HANDAKUTEN &&
                 ConvertKana(&converted[out], c, s_HalfwidthSemiVoicedKanaConversions, HALFWIDTH_SEMI_VOICED_KANA_CONVERSION_COUNT))) {
                // two characters of the text
                in += 2;
                textMap[out] = 2;
            } else {
                ConvertKana(&converted[out], c, s_HalfwidthKanaConversions, HALFWIDTH_KANA_CONVERSION_COUNT);
                ConvertSmallKana(&converted[out]);
                in++;
                textMap[out] = 1;
            }
        } else {
            converted[out] = c;
            if (c == 0) {
                return;
            }
            in++;
            textMap[out] = 1;
        }
        out++;
    }
}

// 0x003E192C | fefates:bytes [tier B]
void nn::ngc::CTR::ProfanityFilterBase::ConvertUserInputForWord(wchar_t* converted, int size, const wchar_t* word)
{
    int out = 0;
    int in = 0;
    for (;;) {
        if (out == size - 1) {
            converted[out] = 0;
            return;
        }
        wchar_t c = word[in];
        if (c == L' ' || c == IDEOGRAPHIC_SPACE) {
            in++;
            continue;
        }
        if (IsFullwidthAlphanumeric(c)) {
            converted[out] = c - FULLWIDTH_OFFSET;
            in++;
        } else if (IsHiragana(c)) {
            converted[out] = c + KATAKANA_OFFSET;
            ConvertSmallKana(&converted[out]);
            in++;
        } else if (IsKatakana(c)) {
            converted[out] = c;
            ConvertSmallKana(&converted[out]);
            in++;
        } else if (IsHalfwidthKatakana(c)) {
            if ((word[in + 1] == HALFWIDTH_DAKUTEN &&
                 ConvertKana(&converted[out], c, s_HalfwidthVoicedKanaConversions, HALFWIDTH_VOICED_KANA_CONVERSION_COUNT)) ||
                (word[in + 1] == HALFWIDTH_HANDAKUTEN &&
                 ConvertKana(&converted[out], c, s_HalfwidthSemiVoicedKanaConversions, HALFWIDTH_SEMI_VOICED_KANA_CONVERSION_COUNT))) {
                in += 2;
            } else {
                ConvertKana(&converted[out], c, s_HalfwidthKanaConversions, HALFWIDTH_KANA_CONVERSION_COUNT);
                ConvertSmallKana(&converted[out]);
                in++;
            }
        } else {
            converted[out] = c;
            if (c == 0) {
                return;
            }
            in++;
        }
        out++;
    }
}

// 0x003E1BA8 | fefates:bytes-fuzzy [tier B]
void nn::ngc::CTR::ProfanityFilterBase::GetPatternListsFromRegion(nn::ngc::CTR::ProfanityFilterPatternList* lists, int* count, bool flag)
{
    *count = 1;
    switch (nn::cfg::CTR::GetRegion()) {
    case nn::cfg::CTR::CFG_REGION_JPN:
        if (nn::cfg::CTR::GetLanguage() == nn::cfg::CTR::CFG_LANGUAGE_JA) {
            lists[0] = PATTERNLIST_JAPAN_JAPANESE;
            return;
        }
        break;
    case nn::cfg::CTR::CFG_REGION_USA:
        switch (nn::cfg::CTR::GetLanguage()) {
        case nn::cfg::CTR::CFG_LANGUAGE_EN:
            lists[0] = PATTERNLIST_AMERICA_ENGLISH;
            return;
        case nn::cfg::CTR::CFG_LANGUAGE_FR:
            lists[0] = PATTERNLIST_AMERICA_FRENCH;
            lists[1] = PATTERNLIST_AMERICA_ENGLISH;
            (*count)++;
            return;
        case nn::cfg::CTR::CFG_LANGUAGE_ES:
            lists[0] = PATTERNLIST_AMERICA_SPANISH;
            lists[1] = PATTERNLIST_AMERICA_ENGLISH;
            (*count)++;
            return;
        case nn::cfg::CTR::CFG_LANGUAGE_PT:
            lists[0] = PATTERNLIST_AMERICA_PORTUGUESE;
            lists[1] = PATTERNLIST_AMERICA_ENGLISH;
            (*count)++;
            return;
        default:
            break;
        }
        break;
    case nn::cfg::CTR::CFG_REGION_EUR:
        switch (nn::cfg::CTR::GetLanguage()) {
        case nn::cfg::CTR::CFG_LANGUAGE_EN:
            lists[0] = PATTERNLIST_EUROPE_ENGLISH;
            return;
        case nn::cfg::CTR::CFG_LANGUAGE_FR:
            lists[0] = PATTERNLIST_EUROPE_FRENCH;
            lists[1] = PATTERNLIST_EUROPE_ENGLISH;
            (*count)++;
            return;
        case nn::cfg::CTR::CFG_LANGUAGE_DE:
            lists[0] = PATTERNLIST_EUROPE_GERMAN;
            lists[1] = PATTERNLIST_EUROPE_ENGLISH;
            (*count)++;
            return;
        case nn::cfg::CTR::CFG_LANGUAGE_IT:
            lists[0] = PATTERNLIST_EUROPE_ITALIAN;
            lists[1] = PATTERNLIST_EUROPE_ENGLISH;
            (*count)++;
            return;
        case nn::cfg::CTR::CFG_LANGUAGE_ES:
            lists[0] = PATTERNLIST_EUROPE_SPANISH;
            lists[1] = PATTERNLIST_EUROPE_ENGLISH;
            (*count)++;
            return;
        case nn::cfg::CTR::CFG_LANGUAGE_NL:
            lists[0] = PATTERNLIST_EUROPE_DUTCH;
            lists[1] = PATTERNLIST_EUROPE_ENGLISH;
            (*count)++;
            return;
        case nn::cfg::CTR::CFG_LANGUAGE_PT:
            lists[0] = PATTERNLIST_EUROPE_PORTUGUESE;
            lists[1] = PATTERNLIST_EUROPE_ENGLISH;
            (*count)++;
            return;
        case nn::cfg::CTR::CFG_LANGUAGE_RU:
            lists[0] = PATTERNLIST_EUROPE_RUSSIAN;
            lists[1] = PATTERNLIST_EUROPE_ENGLISH;
            (*count)++;
            return;
        default:
            break;
        }
        break;
    case nn::cfg::CTR::CFG_REGION_CHN:
        if (nn::cfg::CTR::GetLanguage() == nn::cfg::CTR::CFG_LANGUAGE_ZH) {
            lists[0] = PATTERNLIST_CHINA_SIMPLIFIED_CHINESE;
            return;
        }
        break;
    case nn::cfg::CTR::CFG_REGION_KOR:
        if (nn::cfg::CTR::GetLanguage() == nn::cfg::CTR::CFG_LANGUAGE_KO) {
            lists[0] = PATTERNLIST_KOREA_KOREAN;
            return;
        }
        break;
    case nn::cfg::CTR::CFG_REGION_TWN:
        switch (nn::cfg::CTR::GetLanguage()) {
        case nn::cfg::CTR::CFG_LANGUAGE_EN:
            lists[0] = PATTERNLIST_TAIWAN_ENGLISH;
            return;
        case nn::cfg::CTR::CFG_LANGUAGE_TW:
            lists[0] = PATTERNLIST_TAIWAN_TRADITIONAL_CHINESE;
            return;
        default:
            break;
        }
        break;
    default:
        break;
    }
    // no word list (also for AUS)
    *count = 0;
}

// 0x003E1E10 (name is ours)
bool nn::ngc::CTR::ProfanityFilterBase::ConvertPatternToRegularExpression(wchar_t* regularExpression, const wchar_t* pattern, u32 length)
{
    // ".*abc.*" -> "abc" (anywhere)
    if (length >= 5 && length - 4 < REGULAR_EXPRESSION_LENGTH_MAX && pattern[0] == L'.' && pattern[1] == L'*' &&
        pattern[length - 2] == L'.' && pattern[length - 1] == L'*') {
        CopyCharacters(regularExpression, &pattern[2], length - 4);
        regularExpression[length - 4] = 0;
        return true;
    }
    if (length >= 4 && length - 1 < REGULAR_EXPRESSION_LENGTH_MAX) {
        if (pattern[0] == L'.') {
            // ".*abc$" -> "abc\b"
            if (pattern[1] == L'*' && pattern[length - 1] == L'$') {
                CopyCharacters(regularExpression, &pattern[2], length - 3);
                SetWordBoundary(&regularExpression[length - 3]);
                regularExpression[length - 1] = 0;
                return true;
            }
        } else if (pattern[0] == L'^') {
            // "^abc.*" -> "\babc"
            if (pattern[length - 2] == L'.' && pattern[length - 1] == L'*') {
                SetWordBoundary(regularExpression);
                CopyCharacters(&regularExpression[2], &pattern[1], length - 3);
                regularExpression[length - 1] = 0;
                return true;
            }
        }
    }
    // "^abc$" -> "\babc\b"
    if (length >= 3 && length + 2 < REGULAR_EXPRESSION_LENGTH_MAX && pattern[0] == L'^' && pattern[length - 1] == L'$') {
        SetWordBoundary(regularExpression);
        CopyCharacters(&regularExpression[2], &pattern[1], length - 2);
        SetWordBoundary(&regularExpression[length]);
        regularExpression[length + 2] = 0;
        return true;
    }
    if (length >= 2 && length + 3 < REGULAR_EXPRESSION_LENGTH_MAX) {
        // "^abc" -> "\babc\b"
        if (pattern[0] == L'^') {
            SetWordBoundary(regularExpression);
            CopyCharacters(&regularExpression[2], &pattern[1], length - 1);
            SetWordBoundary(&regularExpression[length + 1]);
            regularExpression[length + 3] = 0;
            return true;
        }
        // "abc$" -> "\babc\b"
        if (pattern[length - 1] == L'$') {
            SetWordBoundary(regularExpression);
            CopyCharacters(&regularExpression[2], pattern, length - 1);
            SetWordBoundary(&regularExpression[length + 1]);
            regularExpression[length + 3] = 0;
            return true;
        }
    }
    if (length >= 3 && length < REGULAR_EXPRESSION_LENGTH_MAX) {
        // "abc.*" -> "\babc"
        if (pattern[length - 2] == L'.' && pattern[length - 1] == L'*') {
            SetWordBoundary(regularExpression);
            CopyCharacters(&regularExpression[2], pattern, length - 2);
            regularExpression[length] = 0;
            return true;
        }
        // ".*abc" -> "abc\b"
        if (pattern[0] == L'.' && pattern[1] == L'*') {
            CopyCharacters(regularExpression, &pattern[2], length - 2);
            SetWordBoundary(&regularExpression[length - 2]);
            regularExpression[length] = 0;
            return true;
        }
    }
    // "abc" -> "\babc\b"
    if (length + 4 >= REGULAR_EXPRESSION_LENGTH_MAX) {
        return false;
    }
    SetWordBoundary(regularExpression);
    CopyCharacters(&regularExpression[2], pattern, length);
    SetWordBoundary(&regularExpression[length + 2]);
    regularExpression[length + 4] = 0;
    return true;
}

} // namespace CTR
} // namespace ngc
} // namespace nn
