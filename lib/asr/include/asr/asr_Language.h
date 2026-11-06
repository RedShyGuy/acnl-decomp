#pragma once

#include "asr/asr_Internal.h"
#include <cstring>

// From the spelling of a word to its phones. The words are looked up in a compressed lexicon of the language; rules
// (a beam search) handle the others. Japanese (1) goes over romaji, Korean (9) over its syllables, Chinese (10) over pinyin.
namespace asr {
// the bits of a word in the lexicon key
struct BitWriter
{
    const u8* table;   // 0x00 eight bytes per character: code, bit count, bits
    u32 tableSize;     // 0x04
    u8 bytes[128];     // 0x08
    s32 bitNum;        // 0x88
    s32 byteIndex;     // 0x8C
    s32 _90;           // 0x90
    s32 _94;           // 0x94
    u8 mask;           // 0x98
};
ASSERT_SIZE(BitWriter, 0x9C);

// reads the Huffman coded phones of the lexicon
struct BitReader
{
    const u16* tree;   // 0x00
    u32 treeSize;      // 0x04
    const u8* bits;    // 0x08
    u32 bitNum;        // 0x0C
    u8 mask;           // 0x10
    s32 bitPos;        // 0x14
    s32 bytePos;       // 0x18
};
ASSERT_SIZE(BitReader, 0x1C);

// the set of rules of a language
struct RuleLetter
{
    u16 letter;        // 0x0
    const u32* rules;  // 0x4
    u32 _8;            // 0x8
};
ASSERT_SIZE(RuleLetter, 0xC);

struct RuleSet
{
    const RuleLetter* letters;    // 0x00
    const s16* const* phones;     // 0x04
    s32 maxPhones;                // 0x08
    s32 _C;                       // 0x0C
    u16 skipPhone;                // 0x10
    s32 _14;                      // 0x14
    const char* const* sets;      // 0x18
    u8 boundary;                  // 0x1C
    s32 _20;                      // 0x20
    u16 letterNum;                // 0x24
    const s16* letterMap;         // 0x28
};
ASSERT_SIZE(RuleSet, 0x2C);

// a hypothesis of the rule engine (the phones follow)
struct RuleHypothesis
{
    s32 score;         // 0x0
    s16 delta;         // 0x4
    u8 phoneNum;       // 0x6
    u8 position;       // 0x7
    u16 phones[1];     // 0x8
};

struct RuleCandidate
{
    s32 score;         // 0x0
    const u32* rule;   // 0x4
    u16 choice;        // 0x8
    u16 parent;        // 0xA
};
ASSERT_SIZE(RuleCandidate, 0xC);

const s32 RULE_BUFFER_SIZE = 0x428;

// the working memory of the rule engine, in the tables of the language
struct RuleMemory
{
    u8 buffers[2][RULE_BUFFER_SIZE]; // 0x000
    RuleCandidate candidates[5];     // 0x850
};
ASSERT_SIZE(RuleMemory, 0x88C);

// the lexicon of the Western languages (the rules follow behind it in the recognizer)
struct WesternInfo
{
    u8 language;              // 0x00
    u8 _1[3];
    const u8* phoneMap;       // 0x04 the phone of the recognizer for each phone of the lexicon
    const RuleSet* rules;     // 0x08
    const u8* writerTable;    // 0x0C
    u32 writerTableBits;      // 0x10
    const u8* readerTree;     // 0x14
    u32 readerTreeBits;       // 0x18
    u32 _1C;                  // 0x1C
    s32 keyNum;               // 0x20
    const u32* keys;          // 0x24
    s32 stringBits;           // 0x28
    const u8* keyStrings;     // 0x2C
};
ASSERT_SIZE(WesternInfo, 0x30);

// a phone written out in a word list
struct WesternPhone
{
    const char* name; // 0x0
    u16 length;       // 0x4
    u16 id;           // 0x6
};
ASSERT_SIZE(WesternPhone, 0x8);

struct WesternTables
{
    const WesternInfo* info;  // 0x000
    u32 _4;                   // 0x004
    s32 isLexicon;            // 0x008
    BitWriter writer;         // 0x00C
    BitReader reader;         // 0x0A8
    u8 _C4[0x914 - 0xC4];     // 0x0C4 of the rule engine
    WesternPhone phones[65];  // 0x914
    s32 phoneNum;             // 0xB1C
};
ASSERT_SIZE(WesternTables, 0xB20);

// what the Western file has besides the tables (behind WesternTables in the recognizer)
struct WesternData
{
    WesternInfo info;           // 0x000
    RuleSet rules;              // 0x030
    RuleMemory memory;          // 0x05C
    RuleLetter letters[45];     // 0x8E8
    const s16* phones[300];     // 0xB04
    const char* sets[10];       // 0xFB4
};
ASSERT_OFFSET(WesternData, letters, 0x8E8);
ASSERT_OFFSET(WesternData, phones, 0xB04);
ASSERT_OFFSET(WesternData, sets, 0xFB4);
ASSERT_SIZE(WesternData, 0xFDC);

// a written out phone of Korean
struct KoreanPhone
{
    const char* name; // 0x0
    u16 length;       // 0x4
    u16 id;           // 0x6
};
ASSERT_SIZE(KoreanPhone, 0x8);

struct KoreanHypothesis
{
    s32 score;        // 0x000
    u8 position;      // 0x004
    u8 phoneNum;      // 0x005
    u8 _6[2];
    u16 phones[130];  // 0x008
};
ASSERT_SIZE(KoreanHypothesis, 0x10C);

struct KoreanCandidate
{
    s32 score;        // 0x0
    const u16* phones; // 0x4
    u16 parent;       // 0x8
};
ASSERT_SIZE(KoreanCandidate, 0xC);

struct KoreanTables
{
    s32 keyNum;                   // 0x0000
    const u8* keys;               // 0x0004 u16 syllable, u16 index in lists
    s32 listNum;                  // 0x0008
    const u16* lists;             // 0x000C
    s32 letterRangeNum;           // 0x0010
    const u8* letterRanges;       // 0x0014
    s32 letterPhoneNum;           // 0x0018
    const u16* letterPhones;      // 0x001C
    s32 pairNum;                  // 0x0020
    const u16* phoneList;         // 0x0024
    s32 phoneNum;                 // 0x0028
    const u16* pairs;             // 0x002C
    KoreanPhone phones[409];      // 0x0030
    KoreanHypothesis buffers[2][4]; // 0x0CF8
    KoreanCandidate candidates[5]; // 0x1558
};
ASSERT_SIZE(KoreanTables, 0x1594);
ASSERT_OFFSET(KoreanTables, buffers, 0xCF8);
ASSERT_OFFSET(KoreanTables, candidates, 0x1558);

struct ChineseTables
{
    s32 rangeNum;       // 0x00
    const u16* ranges;  // 0x04
    s32 entryNum;       // 0x08
    const u8* entries;  // 0x0C
    s32 mapNum;         // 0x10
    const u8* phoneMap; // 0x14
};
ASSERT_SIZE(ChineseTables, 0x18);

// the buffers of Japanese in the recognizer
struct JapaneseTablesData
{
    u8 flags[320];       // 0x000 which vowels are voiceless (the second list of phones lies at +1600)
    char romaji[320];    // 0x140
    char check[1734];    // 0x280 also the variant with voiceless vowels at romaji + 320
    char euc[130];       // 0x946
};
ASSERT_OFFSET(JapaneseTablesData, euc, 0x946);

const s32 REPLACE_ALL = 'g';
const u32 MAX_U16 = 0xFFFF;

s32 ReplaceText(char* text, const char* find, const char* left, const char* right, const char* target, s32 mode); // 0x007C32CC
s32 ApplySoundRules(char* text); // 0x0015B0FC
DECOMP_NOINLINE s32 U16Length(const u16* text); // 0x007C623C
// the number of strings of a list of strings and its size in bytes
DECOMP_NOINLINE s32 U16ListCount(const u16* list); // 0x007B2C94
DECOMP_NOINLINE s32 U16ListSize(const u16* list); // 0x007B2CD0

s32 WesternInitialize(BitWriter* writer, const u8* table, u32 tableBits, BitReader* reader, const u16* tree,
                      u32 treeBytes, s32 value, const u8* bits, u32 bytes); // 0x00159678
s32 KoreanLoad(u8, const File* file, KoreanTables* tables); // 0x0015A500
s32 ChineseLoad(u8, const File* file, ChineseTables* tables); // 0x0015A678

s32 RuleConvert(RuleSet* rules, const u16* text, s32 beam, s32 threshold, u16* out, s32* scores, s32* num, u32* size); // 0x00163F44
s32 WesternConvertWide(WesternTables* tables, const u16* text, s32 variants, u16* out, s32* num, s32* size); // 0x0015A724

// Western languages
const u8* LanguageWesternGetPhoneMap(void* tables); // 0x001592E0
s8 LanguageWesternConvert(void* tables, const char* text, s32 variants, u16* out, s32* num, s32* size); // 0x0015ABD0

// the languages with tables
bool LanguageJapaneseInitialize(void* tables); // 0x001588B0
s8 LanguageJapaneseConvert(void* tables, const char* text, s32 variants, u16* out, s32* num, s32* size); // 0x0015AC30

// Korean
u32 LanguageKoreanFindPhone(const void* tables, const char* name); // 0x00158E20
s8 LanguageKoreanConvert(void* tables, const char* text, s32 variants, u16* out, s32* num, s32* size); // 0x00157C6C

// the languages with tables
const u8* LanguageChineseGetPhoneMap(void* tables); // 0x00158E14
s8 LanguageChineseConvert(void* tables, const char* text, s32 variants, u16* out, s32* num, s32* size); // 0x001597C8

// the written out phones of a word list (inline in the original)
inline bool LanguageWesternHasPhones(void* tables)
{
    return static_cast<WesternTables*>(tables)->phoneNum != 0;
}

inline s32 LanguageWesternFindPhone(void* tables, const char* text, s32 length)
{
    WesternTables* western = static_cast<WesternTables*>(tables);
    s32 num = western->phoneNum;
    s32 i = 0;
    while (i < num && western->phones[i].length > length) {
        i++;
    }
    for (; i < num; i++) {
        if (strncmp(text, western->phones[i].name, western->phones[i].length) == 0) {
            return i;
        }
    }
    return -1;
}

inline u16 LanguageWesternGetPhoneId(void* tables, s32 index)
{
    return static_cast<WesternTables*>(tables)->phones[index].id;
}

inline s32 LanguageWesternGetPhoneLength(void* tables, s32 index)
{
    return static_cast<WesternTables*>(tables)->phones[index].length;
}

inline bool LanguageKoreanHasPhones(const void* tables)
{
    return static_cast<const KoreanTables*>(tables)->phoneNum != 0;
}

inline const u16* LanguageKoreanGetPhones(const void* tables, u32 index)
{
    return &static_cast<const KoreanTables*>(tables)->pairs[index * 2];
}

inline u16 LanguageKoreanGetPhoneLength(const void* tables, u32 index)
{
    return static_cast<const KoreanTables*>(tables)->phones[index].length;
}

} // namespace asr
