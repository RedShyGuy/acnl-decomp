#include "script/dMailChecker.h"
#include "Other/dRegion.h"
#include "script/dScriptExtern.h"
#include "script/dStrMgr.h"
#include "script/dWordCPtrSv.h"
#include "script/dWordFix.h"
#include "script/dWordRes.h"

#include <cstring>

namespace script {
// 0x0052522C (name is ours)
bool IsKeywordHead(const char16* keyword, const char16* word)
{
    bool isMatch = true;
    for (s32 i = 0; i < 3; i++) {
        char16 c = keyword[i];
        if (c == '\n' || c == 0) {
            if (i == 0) {
                isMatch = false;
            }
            return isMatch;
        }
        if (c != NormalizeMailChar(word[i])) {
            return false;
        }
    }
    return isMatch;
}

// 0x005E9C2C (name is ours)
WordCursor::WordCursor(const WordCPtr& text) : m_Text(text), m_pPos(NULL)
{
}

// 0x005E9C20 (name is ours)
void WordCounter::Reset()
{
    m_Count = 0;
}

// 0x005E9B9C (name is ours)
void WordCounter::Count()
{
    const IWord& word = m_Text;
    u32 size = word.GetSize();
    const char16* text = word.GetWord();
    for (u32 i = 0; i < size; i++, text++) {
        char16 c = *text;
        if (c == 0) {
            return;
        }
        if (c != ' ' && c != 0x3000 && c != '\n') {
            m_Count++;
        }
    }
}

// (inline)
inline MailChecker::Splitter::Splitter(const WordCPtr& text) : m_Text(text), m_IsJapanese(0), m_pCursor(NULL), m_Words()
{
    m_IsJapanese = GetRegion() == 0 ? 1 : 0;
    for (u32 i = 0; i < WORD_NUM; i++) {
        m_Words[i] = NULL;
    }
}

// 0x005E9C54 (name is ours)
void MailChecker::Splitter::Split()
{
    for (u32 i = 0; i < WORD_NUM; i++) {
        m_Words[i] = NULL;
    }
    const IWord& string = m_Text;
    const char16* text = string.GetWord();
    u32 size = string.GetSize();
    const char16** word = m_Words;
    const char16* end = text + size - 3;
    if (*text == 0) {
        return;
    }
    bool isStart = true;
    for (;;) {
        char16 c = *text;
        if (c == 0 || end <= text) {
            return;
        }
        bool isSeparator;
        if (c == ' ' || c == 0x3000 || c == '\n') {
            isSeparator = true;
        } else if (m_IsJapanese) {
            // 、。！？を
            isSeparator = c == 0x3001 || c == 0x3002 || c == 0xFF01 || c == 0xFF1F || c == 0x3092;
        } else {
            isSeparator = c == ',' || c == '.' || c == '!' || c == '?';
        }
        if (isSeparator) {
            isStart = true;
        } else if (isStart) {
            if (word >= m_Words + WORD_NUM) {
                return;
            }
            isStart = false;
            *word++ = text;
        }
        text++;
    }
}

// 0x005E76C8 (name is ours)
MailChecker::Matcher::Matcher(Splitter* pSplitter)
    : m_pSplitter(pSplitter), m_pStrMgr(NULL), m_FoundNum(0), m_FirstCharNum(0), m_FirstChars(), m_Found()
{
    for (u32 i = 0; i < FIRST_CHAR_NUM; i++) {
        m_FirstChars[i] = 0;
    }
    for (u32 i = 0; i < WORD_NUM; i++) {
        m_Found[i] = NULL;
    }
}

// 0x005E767C (name is ours)
void MailChecker::Matcher::Reset()
{
    m_FoundNum = 0;
    for (u32 i = 0; i < WORD_NUM; i++) {
        m_Found[i] = NULL;
    }
}

// 0x005E7580 (name is ours)
void MailChecker::Matcher::Search()
{
    m_pSplitter->Begin();
    while (const char16* word = m_pSplitter->Next()) {
        char16 c = word[0];
        if (c == 0) {
            continue;
        }
        s32 num = m_FirstCharNum;
        for (s32 i = 0; i < num; i++) {
            char16 first = m_FirstChars[i];
            if (first == 0 || first != NormalizeMailChar(c)) {
                continue;
            }
            if (i >= 0) {
                Match(word, i);
            }
            break;
        }
    }
}

// 0x005E7354 (name is ours)
void MailChecker::Matcher::Match(const char16* word, s32 index)
{
    WordRes res;
    m_pStrMgr->Find(&res, "STR_Mailchk", index);
    m_Keyword.m_Text.m_pWord = res.m_pWord;
    m_Keyword.m_Text.m_Size = res.m_Size;
    m_Keyword.m_pPos = NULL;
    char16 second = word[1];
    char16 third = word[2];
    if (!m_Keyword.Begin()) {
        return;
    }
    do {
        const char16* line = m_Keyword.m_pPos;
        char16 c = line[1];
        if (c != '\n' && c != 0) {
            if (c != NormalizeMailChar(second)) {
                continue;
            }
            c = line[2];
            if (c != '\n' && c != 0 && c != NormalizeMailChar(third)) {
                continue;
            }
        }
        for (u32 i = 0; i < WORD_NUM; i++) {
            if (m_Found[i] == NULL) {
                m_Found[i] = line;
                m_FoundNum++;
                return;
            }
            if (m_Found[i] == line) {
                break;
            }
        }
    } while (m_Keyword.Next());
}

// 0x005E41E8 (name is ours)
void MailChecker::Comparer::Set(const IWord& word)
{
    m_IsFound = 0;
    m_pWord = word.GetWord();
    u32 size = word.GetSize();
    const char16* text = word.GetWord();
    s32 length = 0;
    for (u32 i = 1; i < size; i++, text++) {
        if (*text == 0) {
            break;
        }
        length++;
    }
    m_WordLength = length;
    const IWord& string = m_Text;
    m_pText = string.GetWord();
    size = string.GetSize();
    text = string.GetWord();
    length = 0;
    for (u32 i = 1; i < size; i++, text++) {
        if (*text == 0) {
            break;
        }
        length++;
    }
    m_TextLength = length;
}

// 0x00311F6C (name is ours)
void MailChecker::Evaluator::Evaluate()
{
    for (u32 i = 0; i < CHECK_NUM; i++) {
        (this->*s_Checks[i])();
        if (m_IsDone) {
            return;
        }
    }
}

// 0x005E775C (name is ours)
void MailChecker::Evaluator::CheckRequired()
{
    WordRes res;
    StrMgr::s_pInstance->Find(&res, s_ListNames[m_Kind], s_ListIndices[0][m_Kind]);
    m_List.m_Text.m_pWord = res.m_pWord;
    m_List.m_Text.m_Size = res.m_Size;
    m_List.m_pPos = NULL;
    while (m_List.Next()) {
        const char16* line = m_List.m_pPos;
        if (line == NULL) {
            continue;
        }
        m_pSplitter->Begin();
        while (const char16* word = m_pSplitter->Next()) {
            if (IsKeywordHead(line, word)) {
                m_Score += 50;
                return;
            }
        }
    }
    m_IsDone = 1;
    m_Score = 0;
}

// 0x005E78A4 (name is ours)
void MailChecker::Evaluator::CheckMain()
{
    WordRes res;
    StrMgr::s_pInstance->Find(&res, s_ListNames[m_Kind], s_ListIndices[1][m_Kind]);
    m_List.m_Text.m_pWord = res.m_pWord;
    m_List.m_Text.m_Size = res.m_Size;
    m_List.m_pPos = NULL;
    s32 count = 0;
    if (m_List.Begin()) {
        do {
            const char16* line = m_List.m_pPos;
            if (line == NULL) {
                continue;
            }
            m_pSplitter->Begin();
            while (const char16* word = m_pSplitter->Next()) {
                if (IsKeywordHead(line, word)) {
                    m_Score += 10;
                    if (++count >= 5) {
                        m_IsDone = 1;
                        return;
                    }
                    break;
                }
            }
        } while (m_List.Next());
        if (count != 0) {
            return;
        }
    }
    m_IsDone = 1;
    m_Score = 0;
}

// 0x005E7A5C (name is ours)
void MailChecker::Evaluator::CheckExtra()
{
    WordRes res;
    StrMgr::s_pInstance->Find(&res, s_ListNames[m_Kind], s_ListIndices[2][m_Kind]);
    m_List.m_Text.m_pWord = res.m_pWord;
    m_List.m_Text.m_Size = res.m_Size;
    m_List.m_pPos = NULL;
    s32 count = 0;
    if (!m_List.Begin()) {
        return;
    }
    do {
        const char16* line = m_List.m_pPos;
        if (line == NULL) {
            continue;
        }
        m_pSplitter->Begin();
        while (const char16* word = m_pSplitter->Next()) {
            if (IsKeywordHead(line, word)) {
                m_Score += 5;
                if (++count >= 2) {
                    return;
                }
                break;
            }
        }
    } while (m_List.Next());
}

// 0x005E7BF8 (name is ours)
void MailChecker::Evaluator::CheckHeader()
{
    m_Compare.Set(m_Header);
    m_Compare.Find();
    if (m_Compare.m_IsFound) {
        m_Score += 5;
    }
}

// 0x005E7CB8 (name is ours)
void MailChecker::Evaluator::CheckTownName()
{
    WordFix<9> name;
    GetTownName(&name);
    m_Compare.Set(name);
    m_Compare.Find();
    if (m_Compare.m_IsFound) {
        m_Score += 5;
    }
}

// 0x005E4358 (name is ours)
MailChecker::MailChecker()
    : m_Body(m_BodyBuffer, BODY_SIZE), m_BodyText(m_BodyBuffer, BODY_SIZE), m_Header(m_HeaderBuffer, HEADER_SIZE),
      m_HeaderText(m_HeaderBuffer, HEADER_SIZE), m_Evaluator(m_HeaderText, &m_Splitter, m_BodyText),
      m_Matcher(&m_Splitter), m_Counter(m_BodyText), m_Splitter(m_BodyText)
{
    std::memset(m_BodyBuffer, 0, sizeof(m_BodyBuffer));
    std::memset(m_HeaderBuffer, 0, sizeof(m_HeaderBuffer));
    m_pMail = NULL;
}

// 0x005E430C (name is ours)
void MailChecker::Set(const SvMail* mail)
{
    static_cast<IWord&>(m_Body).Copy(GetMailBody(mail));
    Func759DFC(mail, &m_Header);
    m_Splitter.Split();
    m_pMail = mail;
}
} // namespace script
