#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
class IWord;
class Insert;
class Phrase;
class IRecept;
class ITalkRecept;

// whether a tag of a word has an attribute (the kinds 0..9 of the tables per tag group; any other kind: always; the
// meaning of the kinds is not known, the name is ours)
bool HasTagAttribute(s32 group, s32 index, u32 kind); // 0x005D608C (name is ours)

struct TextPtr;
// While measuring, the widest of the texts when the target of a grammar tag is not found (the message is the warning
// of the debug build; name is ours)
void SelectWidestText(s32* pIndex, const TextPtr* texts, const u16* lengths, s32 num, const Insert* insert,
                      const char* message); // 0x005F3D08

// The base of the engines that write a message with its inserted words (TalkSeq, MailSeq, UiSeq, ChoiceSeq, the
// measures, StrNicknameEngine): the capital letters at the start of sentences and the frame of tags around inserted
// words. Member names are ours.
// RTTI N6script9SeqEngineE @ 0x008D3E94
// vtable 0x0090BBD8 (vptr 0x0090BBE0), offset_to_top 0, 8 entries
class SeqEngine : public ::script::MsgEngine
{
public:
    // The tags around an inserted word: the color of the word and two tag pairs (group 0xFF, index 2/5 and 3/4), and
    // the inserted words with an attribute (names are ours).
    struct InsertFrame
    {
        // (inline)
        explicit InsertFrame(MsgEngine* pEngine) : pEngine(pEngine), isTag2(0), isTag3(0), color(COLOR_NONE) {}

        // the tags after the word (pushed before the word, so they run after it)
        DECOMP_NOINLINE void PushClose(); // 0x005FDE44
        // the tags before the word; the frame is reset
        DECOMP_NOINLINE void PushOpen(); // 0x005FDEA4
        // a word with the frame; the words of the attributes 1, 2 and 8 are remembered
        DECOMP_NOINLINE void InsertWord(const IWord* word, const Tag* tag); // 0x005FDF88
        // as InsertWord, with the words of the attributes 4, 5 and 6 too
        DECOMP_NOINLINE void InsertWordEx(const IWord* word, const Tag* tag); // 0x005FE03C

        static const u16 COLOR_NONE = 7;

        MsgEngine* pEngine;      // 0x00
        u8 isTag2;               // 0x04 the tag pair 0xFF/2 and 0xFF/5
        u8 isTag3;               // 0x05 the tag pair 0xFF/3 and 0xFF/4
        u16 color;               // 0x06 the color of the word (COLOR_NONE: the default color)
        const IWord* pWords[6];  // 0x08 the last inserted words with the attributes 1, 2, 8, 4, 5 and 6
        const char16* pBegin;    // 0x20 the start of the text (set by Begin of the engines)
        char16 lastChar;     // 0x24 the last character of the text that is no space
    };

    SeqEngine(); // 0x00602690 (name is ours)
    virtual ~SeqEngine();

    // the text and the state are reset
    void Reset(); // 0x00602614
    // the character is capitalized at the start of a sentence; true: *c is the capital letter
    bool ProcessCapital(char16* c, const CharInfo* info); // 0x00602568
    // the text of the tag for the sex of the speaker (pSpeaker; NULL: the player) is inserted
    void InsertSexText(IRecept* pSpeaker, const Tag* tag, const void* unused); // 0x005F39C4

    // The shared handlers of the tags of the engines (the handlers of TalkSeq/MailSeq/UiSeq/ChoiceSeq and the
    // measures pass the phrase, insert or recept of their owner). The grammar tags have their names in the binary
    // (the messages "<NAME> タグの対象が見つからない", "the target of the tag <NAME> was not found"); the other names
    // are ours, after the word of the Insert they insert.

    // the word 0x2B0 instead of the text up to the tag 7/7 (an empty word: the frame tags are dropped)
    void InsertWord2B0Range(Phrase* phrase, const Tag* tag); // 0x005D9004
    // the word 0x238; it counts for the attribute 8 only if it is the name Func56DE5C gives for 4
    void InsertWord238(Phrase* phrase, const Tag* tag, u32 isColor); // 0x005F1E80
    // the article of the next inserted word (kind 1/2: WordInfo::begin/buf, a string of STR_Article)
    void InsertArticle(Insert* insert, u32 kind); // 0x005F1F94
    // a string of STR_Common after the last digit of the number of the word of the attribute 4/5/6
    void InsertNumberText4(const Tag* tag); // 0x005F20BC
    void InsertNumberText5(const Tag* tag); // 0x005F2198
    void InsertNumberText6(const Tag* tag); // 0x005F23C4
    // a string of STR_Common for the gender of the word of the attribute 8 (the recept chooses it; Japan only)
    void InsertGenderText(IRecept* recept, const Tag* tag); // 0x005F2308
    // the words of the Insert by the index in the tag (isColor: in the given color)
    void InsertWord298(Phrase* phrase, const Tag* tag, u32 isColor); // 0x005F22A0
    void InsertTownFruit(Phrase* phrase, const Tag* tag, u32 isColor); // 0x005F24CC
    void InsertWord2B8(Phrase* phrase, const Tag* tag, u32 isColor); // 0x005F2538
    void InsertWord2B4(Phrase* phrase, const Tag* tag, u32 isColor); // 0x005F25A4
    void InsertWord228(Phrase* phrase, const Tag* tag, u32 isColor); // 0x005F2A5C
    void InsertWord2CC(Phrase* phrase, const Tag* tag, u32 isColor); // 0x005F2C7C
    void InsertWord2C0(Phrase* phrase, const Tag* tag, u32 isColor); // 0x005F2CE8
    void InsertWord2BC(Phrase* phrase, const Tag* tag, u32 isColor); // 0x005F2D54
    void InsertWord284(Phrase* phrase, const Tag* tag, u32 isColor); // 0x005F3160
    void InsertWord280(Phrase* phrase, const Tag* tag, u32 isColor); // 0x005F31CC
    void InsertWord2DC(Phrase* phrase, const Tag* tag, u32 isColor); // 0x005F3238
    void InsertWord2D8(Phrase* phrase, const Tag* tag, u32 isColor); // 0x005F3950
    void InsertWord2AC(Phrase* phrase, const Tag* tag, u32 kind, u32 color); // 0x005F352C
    // a word of the Insert by the region of the game (0, Japan: the second one)
    void InsertTag5_24OrWord278(Phrase* phrase, const Tag* tag); // 0x005F2AC4
    void InsertTag5_22OrTag5_23(Phrase* phrase, const Tag* tag); // 0x005F34CC
    // ELISION: the 2nd of two texts if the next inserted word starts with an elision (ElisionEngine)
    void InsertElision(Insert* insert, const Tag* tag); // 0x005F2610
    // the Korean particle for the last character (the 2nd of two texts after a syllable without a final consonant;
    // isRieulFree: the final ㄹ counts as none, as for (으)로; digits by their Korean reading)
    void InsertPatchimForm(Insert* insert, const Tag* tag, u32 isRieulFree); // 0x005F2864
    // one of three texts for the number of the word 4_6 of the tag (0, 1, else)
    void InsertNumberForm(Insert* insert, const Tag* tag); // 0x005F2B5C
    // GENDER_N / GENDER_P: one of two texts for the gender of the word of the attribute 2 / 1
    void InsertGenderN(const Tag* tag, Insert* insert); // 0x005F2DBC
    void InsertGenderP(const Tag* tag, Insert* insert); // 0x005F37B4
    // GENDER: one of three texts for the gender of the inserted word of the index in the tag
    void InsertGender(Insert* insert, const Tag* tag); // 0x005F2F58
    // GENDER_EX: as GENDER for the inserted word of a tag of s_GenderExTags
    void InsertGenderEx(Insert* insert, const Tag* tag); // 0x005F3598
    // ISLAND_GENDER_ENEMY: one of two texts for the gender of the inserted word of the attribute 9
    void InsertIslandGenderEnemy(Insert* insert, const Tag* tag); // 0x005F3B24
    // the Italian conjunction a/e, as ad/ed before a word that starts with the same vowel (EpenthesisEngine)
    void InsertEpenthesisA(Insert* insert, const Tag* tag); // 0x005F32A4
    void InsertEpenthesisE(Insert* insert, const Tag* tag); // 0x005F3310
    void InsertEpenthesis(Insert* insert, const Tag* tag, class EpenthesisEngine* engine); // 0x005F3368

private:
    // (inline) a text in the frame of the inserted words
    void InsertText(const char16* begin, const char16* end)
    {
        m_Frame.PushClose();
        m_Frame.pEngine->PushText(&begin, &end);
        m_Frame.PushOpen();
    }

public:
    // the tags of the words that GENDER_EX looks for (group, index)
    struct TagId
    {
        s32 group;
        s32 index;
    };
    static const TagId s_GenderExTags[38];
    static const u32 GENDER_EX_TAG_NUM = 38;
    // the Italian conjunctions a/e and their forms ad/ed before the same vowel (by EpenthesisEngine::m_IsE)
    static const char16* const s_ConjunctionTexts[2];
    static const char16* const s_EpenthesisTexts[2];

    u8 m_IsCapitalNext;    // 0x60 the next character becomes a capital letter
    u8 m_CapitalState;     // 0x61 0: off, 1: at the start of a sentence, 2: in a sentence
    InsertFrame m_Frame;   // 0x64
};
ASSERT_SIZE(SeqEngine, 0x8C);

} // namespace script
