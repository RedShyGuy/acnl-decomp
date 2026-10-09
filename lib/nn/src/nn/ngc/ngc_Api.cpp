#include "nn/ngc/ngc_Api.h"
#include "nn/ngc/ngc_CharacterRangeList.h"
#include <wchar.h>

namespace nn {
namespace ngc {
// the ranges of a built-in character class in s_CharClassRanges (name is ours)
struct BuiltInCharClass
{
    s32 m_First;
    s32 m_Count;
};

namespace {
// the name of a \p{...} class, with the closing brace (names are ours)
struct CharClassName
{
    const wchar_t* m_Name;
    u32 m_Length;
};

const s32 CASE_FOLDING_PAIR_COUNT = 1398;
} // namespace

// the characters that have others of another case, sorted by m_Character (names are ours)
// 0x0097FA78
extern CaseFoldingPair s_CaseFoldingPairs[];
// the character classes of BuiltInCharClassType (their ranges in s_CharClassRanges)
// 0x008C5C28
extern const BuiltInCharClass s_BuiltInCharClasses[];
// 0x008C33F0
extern const CharacterRange s_CharClassRanges[];

namespace {
// 0x008C6048
const CharClassName s_CharClassNames[BUILT_IN_CHAR_CLASS_NAMED_COUNT] = {
    {NULL, 0},
    {NULL, 0},
    {NULL, 0},
    {NULL, 0},
    {L"Lower}", 6},
    {L"Upper}", 6},
    {L"ASCII}", 6},
    {L"Alpha}", 6},
    {L"Alnum}", 6},
    {L"Punct}", 6},
    {L"Graph}", 6},
    {L"Print}", 6},
    {L"Blank}", 6},
    {L"Cntrl}", 6},
    {L"XDigit}", 7},
    {L"InLatin-1Supplement}", 20},
    {L"InLatinExtended-A}", 18},
    {L"InIPAExtensions}", 16},
    {L"InSpacingModifierLetters}", 25},
    {L"InCombiningDiacriticalMarks}", 28},
    {L"InGreek}", 8},
    {L"InCyrillic}", 11},
    {L"InArmenian}", 11},
    {L"InHebrew}", 9},
    {L"InArabic}", 9},
    {L"InSyriac}", 9},
    {L"InThaana}", 9},
    {L"InDevanagari}", 13},
    {L"InBengali}", 10},
    {L"InGurmukhi}", 11},
    {L"InGujarati}", 11},
    {L"InOriya}", 8},
    {L"InTamil}", 8},
    {L"InTelugu}", 9},
    {L"InKannada}", 10},
    {L"InMalayalam}", 12},
    {L"InSinhala}", 10},
    {L"InThai}", 7},
    {L"InLao}", 6},
    {L"InTibetan}", 10},
    {L"InMyanmar}", 10},
    {L"InGeorgian}", 11},
    {L"InHangulJamo}", 13},
    {L"InEthiopic}", 11},
    {L"InCherokee}", 11},
    {L"InUnifiedCanadianAboriginalSyllabics}", 37},
    {L"InOgham}", 8},
    {L"InRunic}", 8},
    {L"InKhmer}", 8},
    {L"InMongolian}", 12},
    {L"InLatinExtendedAdditional}", 26},
    {L"InGreekExtended}", 16},
    {L"InGeneralPunctuation}", 21},
    {L"InSuperscriptsandSubscripts}", 28},
    {L"InCurrencySymbols}", 18},
    {L"InLetterlikeSymbols}", 20},
    {L"InNumberForms}", 14},
    {L"InArrows}", 9},
    {L"InMathematicalOperators}", 24},
    {L"InMiscellaneousTechnical}", 25},
    {L"InControlPictures}", 18},
    {L"InOpticalCharacterRecognition}", 30},
    {L"InEnclosedAlphanumerics}", 24},
    {L"InBoxDrawing}", 13},
    {L"InBlockElements}", 16},
    {L"InGeometricShapes}", 18},
    {L"InMiscellaneousSymbols}", 23},
    {L"InDingbats}", 11},
    {L"InBraillePatterns}", 18},
    {L"InCJKRadicalsSupplement}", 24},
    {L"InKangxiRadicals}", 17},
    {L"InIdeographicDescriptionCharacters}", 35},
    {L"InCJKSymbolsandPunctuation}", 27},
    {L"InHiragana}", 11},
    {L"InKatakana}", 11},
    {L"InBopomofo}", 11},
    {L"InHangulCompatibilityJamo}", 26},
    {L"InKanbun}", 9},
    {L"InBopomofoExtended}", 19},
    {L"InEnclosedCJKLettersandMonths}", 30},
    {L"InCJKCompatibility}", 19},
    {L"InCJKUnifiedIdeographsExtensionA}", 33},
    {L"InCJKUnifiedIdeographs}", 23},
    {L"InYiSyllables}", 14},
    {L"InYiRadicals}", 13},
    {L"InHangulSyllables}", 18},
    {L"InHighSurrogates}", 17},
    {L"InHighPrivateUseSurrogates}", 27},
    {L"InLowSurrogates}", 16},
    {L"InCJKCompatibilityIdeographs}", 29},
    {L"InAlphabeticPresentationForms}", 30},
    {L"InArabicPresentationForms-A}", 28},
    {L"InCombiningHalfMarks}", 21},
    {L"InCJKCompatibilityForms}", 24},
    {L"InSmallFormVariants}", 20},
    {L"InHalfwidthandFullwidthForms}", 29},
    {L"InSpecials}", 11},
    {L"IsL}", 4},
    {L"IsLl}", 5},
    {L"IsLu}", 5},
    {L"IsLt}", 5},
    {L"IsLm}", 5},
    {L"IsLo}", 5},
    {L"IsM}", 4},
    {L"IsMn}", 5},
    {L"IsMc}", 5},
    {L"IsMe}", 5},
    {L"IsZ}", 4},
    {L"IsZs}", 5},
    {L"IsZl}", 5},
    {L"IsZp}", 5},
    {L"IsS}", 4},
    {L"IsSm}", 5},
    {L"IsSc}", 5},
    {L"IsSk}", 5},
    {L"IsSo}", 5},
    {L"IsN}", 4},
    {L"IsNd}", 5},
    {L"IsNl}", 5},
    {L"IsNo}", 5},
    {L"IsP}", 4},
    {L"IsPd}", 5},
    {L"IsPs}", 5},
    {L"IsPe}", 5},
    {L"IsPc}", 5},
    {L"IsPo}", 5},
    {L"IsC}", 4},
    {L"IsCc}", 5},
    {L"IsCf}", 5},
    {L"IsCo}", 5},
    {L"IsCn}", 5},
    {L"IsCs}", 5},
};
} // namespace

// 0x003DF98C | fefates:bytes [tier B]
const CaseFoldingPair* GetCaseFoldingPair(wchar_t character)
{
    s32 low = 0;
    s32 high = CASE_FOLDING_PAIR_COUNT - 1;
    while (low <= high) {
        s32 middle = low + (high - low) / 2;
        if (s_CaseFoldingPairs[middle].m_Character == character) {
            return &s_CaseFoldingPairs[middle];
        }
        if (s_CaseFoldingPairs[middle].m_Character > character) {
            high = middle - 1;
        } else {
            low = middle + 1;
        }
    }
    return NULL;
}

// 0x003DFC78 | fefates:bytes [tier B]
bool CombineBuiltInCharClass(nn::ngc::CharacterRangeList* list, nn::ngc::BuiltInCharClassType type)
{
    const BuiltInCharClass& charClass = s_BuiltInCharClasses[type];
    for (s32 i = charClass.m_First + charClass.m_Count - 1; i >= charClass.m_First; i--) {
        if (!list->AddCharacterRange(s_CharClassRanges[i].m_Begin, s_CharClassRanges[i].m_End)) {
            return false;
        }
    }
    return true;
}

// 0x003DFCE0 | fefates:bytes [tier B]
nn::ngc::BuiltInCharClassType GetCharClassTypeFromName(const wchar_t* name, unsigned int length, unsigned int* nameLength)
{
    for (s32 i = 0; i < BUILT_IN_CHAR_CLASS_NAMED_COUNT; i++) {
        const CharClassName& entry = s_CharClassNames[i];
        if (entry.m_Name != NULL && entry.m_Length <= length && wcsncmp(name, entry.m_Name, entry.m_Length) == 0) {
            *nameLength = entry.m_Length;
            return static_cast<BuiltInCharClassType>(i);
        }
    }
    return BUILT_IN_CHAR_CLASS_INVALID;
}

} // namespace ngc
} // namespace nn
