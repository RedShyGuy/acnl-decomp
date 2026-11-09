#pragma once

#include "decomp.h"
#include "script/dToUpper.h"
#include "script/dWordCPtr.h"
#include "script/dWordPtr.h"

struct SvMail;

namespace script {
class StrMgr;

// (inline; name is ours) a character as the letter checks compare it: hiragana and a-z stay, katakana become hiragana,
// the rest lower case
inline char16 NormalizeMailChar(char16 c)
{
    char16 normalized = c;
    if (static_cast<u32>(c - 0x3041) > 85 && !(c >= 'a' && c <= 'z')) {
        if (static_cast<u32>(c - 0x30A1) <= 85) {
            normalized = c - 0x60;
        } else {
            ToLower(&normalized);
        }
    }
    return normalized;
}

// (name is ours) true: the word starts with the keyword (its first three characters at most; a line of the list ends
// with a line feed)
bool IsKeywordHead(const char16* keyword, const char16* word); // 0x0052522C

// A constant text with a position in it: the lines of a list (names are ours).
struct WordCursor
{
    // (inline)
    WordCursor() : m_pPos(NULL) {}
    // (copy in the binary)
    explicit WordCursor(const WordCPtr& text); // 0x005E9C2C

    // (inline) the first line; false: the text is empty
    bool Begin()
    {
        m_pPos = static_cast<const IWord&>(m_Text).GetWord();
        return m_pPos != NULL && *m_pPos != 0;
    }
    // (inline) the next line (the first one at the start); false: no more lines
    bool Next()
    {
        if (m_pPos == NULL) {
            return Begin();
        }
        bool isNewLine = false;
        for (;;) {
            m_pPos++;
            char16 c = *m_pPos;
            if (c == '\n') {
                isNewLine = true;
                continue;
            }
            if (c == 0) {
                return false;
            }
            if (isNewLine) {
                return true;
            }
        }
    }

    WordCPtr m_Text;       // 0x0
    const char16* m_pPos;  // 0xC
};
ASSERT_SIZE(WordCursor, 0x10);

// A counter of the characters of a text that are no spaces or line feeds (names are ours).
struct WordCounter
{
    // (inline)
    explicit WordCounter(const WordCPtr& text) : m_Text(text), m_Count(0) {}

    void Reset(); // 0x005E9C20
    void Count(); // 0x005E9B9C

    WordCPtr m_Text;  // 0x0
    u32 m_Count;      // 0xC
};
ASSERT_SIZE(WordCounter, 0x10);

// Checks letters of the player (names are ours): the text is split into words, which are compared with the keyword
// lists STR_Mailchk (the matcher: by the first character, the list of the index of that character) and with the lists of
// a kind of letter (birthday, moving, winter) for a score.
class MailChecker
{
public:
    static const u32 BODY_SIZE = 193;
    static const u32 HEADER_SIZE = 17;
    static const u32 WORD_NUM = 96;
    static const u32 FIRST_CHAR_NUM = 80;

    // the words of the text: the starts after spaces and punctuation
    struct Splitter
    {
        // (inline)
        explicit Splitter(const WordCPtr& text);

        // the starts of the words (at most WORD_NUM)
        void Split(); // 0x005E9C54
        // (inline) before the first word
        void Begin() { m_pCursor = m_Words - 1; }
        // (inline) the next word (NULL: none)
        const char16* Next()
        {
            m_pCursor++;
            if (*m_pCursor == NULL || m_pCursor >= m_Words + WORD_NUM) {
                return NULL;
            }
            return *m_pCursor;
        }

        WordCPtr m_Text;                  // 0x000
        u8 m_IsJapanese;                  // 0x00C (the separators 、。！？を instead of ,.!?)
        const char16** m_pCursor;         // 0x010
        const char16* m_Words[WORD_NUM];  // 0x014
    };

    // the keywords of STR_Mailchk found in the words
    struct Matcher
    {
        // (inline in MailChecker)
        explicit Matcher(Splitter* pSplitter); // 0x005E76C8

        void Reset(); // 0x005E767C
        // the words are compared with the lists of their first characters
        void Search(); // 0x005E7580
        // the word is compared with the keyword list index (its first character matched)
        void Match(const char16* word, s32 index); // 0x005E7354

        Splitter* m_pSplitter;                  // 0x000
        StrMgr* m_pStrMgr;                      // 0x004
        s32 m_FoundNum;                         // 0x008
        WordCursor m_Keyword;                   // 0x00C
        s32 m_FirstCharNum;                     // 0x01C
        char16 m_FirstChars[FIRST_CHAR_NUM];    // 0x020 the first characters of the lists
        const char16* m_Found[WORD_NUM];        // 0x0C0 the keywords found
    };

    // whether the text contains a word
    struct Comparer
    {
        // (inline)
        explicit Comparer(const WordCPtr& text)
            : m_Text(text), m_IsFound(0), m_pWord(NULL), m_WordLength(0), m_pText(NULL), m_TextLength(0)
        {
        }

        void Set(const IWord& word); // 0x005E41E8
        // (inline) m_IsFound: the word is in the text
        void Find()
        {
            const char16* text = m_pText;
            for (s32 i = 0; i < m_TextLength - m_WordLength; i++, text++) {
                s32 j;
                for (j = 0; j < m_WordLength; j++) {
                    if (m_pWord[j] != text[j]) {
                        break;
                    }
                }
                if (j == m_WordLength) {
                    m_IsFound = 1;
                    return;
                }
            }
        }

        WordCPtr m_Text;          // 0x00
        u8 m_IsFound;             // 0x0C
        const char16* m_pWord;    // 0x10
        s32 m_WordLength;         // 0x14
        const char16* m_pText;    // 0x18
        s32 m_TextLength;         // 0x1C
    };

    // the score of a letter of a kind: the checks run until one ends the evaluation
    struct Evaluator
    {
        typedef void (Evaluator::*CheckFunc)();
        static const u8 KIND_NONE = 3;
        static const u32 CHECK_NUM = 5;

        // (inline in MailChecker)
        Evaluator(const WordCPtr& header, Splitter* pSplitter, const WordCPtr& text)
            : m_Header(header), m_pSplitter(pSplitter), m_Kind(KIND_NONE), m_Score(0), m_IsDone(0), m_Compare(text)
        {
        }

        // (inline) the evaluation of a letter of the kind (0 birthday, 1 moving, 2 winter)
        DECOMP_EMIT_COPY void Start(u8 kind) // 0x00311F58
        {
            m_Kind = kind;
            m_Score = 0;
            m_IsDone = 0;
        }
        // (inline) all checks
        void Evaluate(); // 0x00311F6C
        DECOMP_EMIT_COPY s32 GetScore() const { return m_Score; } // 0x00311FC8

        // one word of the first list of the kind is needed (+50), else the score is 0
        void CheckRequired(); // 0x005E775C
        // +10 for each word of the second list (5 at most: done), none: the score is 0
        void CheckMain(); // 0x005E78A4
        // +5 for each word of the third list (2 at most)
        void CheckExtra(); // 0x005E7A5C
        // +5 when the text contains the header
        void CheckHeader(); // 0x005E7BF8
        // +5 when the text contains the name of the town
        void CheckTownName(); // 0x005E7CB8

        WordCPtr m_Header;        // 0x00
        Splitter* m_pSplitter;    // 0x0C
        u8 m_Kind;                // 0x10
        s32 m_Score;              // 0x14
        u8 m_IsDone;              // 0x18
        WordCursor m_List;        // 0x1C
        Comparer m_Compare;       // 0x2C

        static const CheckFunc s_Checks[CHECK_NUM];
        // the indices of the lists per check and kind, the lists of the kinds
        static const s32 s_ListIndices[3][3];
        static const char* const s_ListNames[3];
    };

    MailChecker(); // 0x005E4358

    // the letter: its body is split into words
    void Set(const SvMail* mail); // 0x005E430C
    DECOMP_EMIT_COPY Matcher* GetMatcher() { return &m_Matcher; } // 0x005E42FC
    DECOMP_EMIT_COPY WordCounter* GetCounter() { return &m_Counter; } // 0x005E4304
    DECOMP_EMIT_COPY Evaluator* GetEvaluator() { return &m_Evaluator; } // 0x00311F50

    WordPtr m_Body;                         // 0x000 (m_BodyBuffer)
    WordCPtr m_BodyText;                    // 0x00C
    WordPtr m_Header;                       // 0x018 (m_HeaderBuffer)
    WordCPtr m_HeaderText;                  // 0x024
    Evaluator m_Evaluator;                  // 0x030
    Matcher m_Matcher;                      // 0x07C
    WordCounter m_Counter;                  // 0x2BC
    Splitter m_Splitter;                    // 0x2CC
    char16 m_BodyBuffer[BODY_SIZE];         // 0x460
    char16 m_HeaderBuffer[HEADER_SIZE];     // 0x5E2
    const SvMail* m_pMail;                  // 0x604
};
ASSERT_SIZE(MailChecker::Splitter, 0x194);
ASSERT_SIZE(MailChecker::Matcher, 0x240);
ASSERT_SIZE(MailChecker::Comparer, 0x20);
ASSERT_SIZE(MailChecker::Evaluator, 0x4C);
ASSERT_SIZE(MailChecker, 0x608);

} // namespace script
