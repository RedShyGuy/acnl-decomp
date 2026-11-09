#include "script/dStrNicknameEngine.h"
#include "Other/dRegion.h"
#include "script/dDetectCode.h"
#include "script/dIWord.h"
#include "script/dWordFix.h"
#include "ssys/ma/ma_Math.h"

namespace script {
// 0x005E880C slot 0x14
void StrNicknameEngine::OnChar(const CharInfo* info)
{
    char16 c;
    if (ProcessCapital(&c, info)) {
        if (m_pDst + 1 > m_pLast) {
            m_IsError = 1;
            return;
        }
        *m_pDst = c;
        m_pDst++;
        CheckLeadingChar(c);
        return;
    }
    if (!PutChar(&m_pDst, &m_pLast, info)) {
        m_IsError = 1;
        return;
    }
    CheckLeadingChar(*info->pText);
}

// 0x005E8894
void StrNicknameEngine::CheckLeadingChar(char16 c)
{
    if (m_IsValid) {
        return;
    }
    // hiragana, katakana, Latin letters
    if (static_cast<u32>(c - 0x3041) <= 85 || static_cast<u32>(c - 0x30A1) <= 85 || (c >= 'A' && c <= 'Z') ||
        (c >= 'a' && c <= 'z')) {
        m_IsValid = 1;
        return;
    }
    switch (GetRegionIndex()) {
    case 3:
        // Hangul syllables, jamo, compatibility jamo
        if (ssys::ma::IsInRange(c, 0xAC00, 0xD7A3) || ssys::ma::IsInRange(c, 0x1100, 0x11F9) ||
            ssys::ma::IsInRange(c, 0x3131, 0x318E)) {
            m_IsValid = 1;
            return;
        }
        break;
    case 1:
    case 2:
        // the Latin-1 letters
        if ((c != 0xD7 && static_cast<u32>(c - 0xC0) <= 30) || (c != 0xF7 && static_cast<u32>(c - 0xE0) <= 30)) {
            m_IsValid = 1;
            return;
        }
        break;
    }
    if (!m_IsValid) {
        m_IsError = 1;
    }
}

inline void StrNicknameEngine::InsertChar(const char16* pChar)
{
    if (pChar == NULL) {
        m_IsError = 1;
        return;
    }
    const char16* begin = pChar;
    const char16* end = pChar + 1;
    m_Frame.PushClose();
    m_Frame.pEngine->PushText(&begin, &end);
    m_Frame.PushOpen();
}

// 0x005E89B0
void StrNicknameEngine::InsertFirstChar()
{
    DetectCode detect;
    InsertChar(detect.Detect(m_pWord, 0));
}

// 0x005E8A94
void StrNicknameEngine::InsertSecondChar()
{
    DetectCode detect;
    InsertChar(detect.Detect(m_pWord, 1));
}

// 0x005E8B7C
void StrNicknameEngine::InsertThirdChar()
{
    DetectCode detect;
    InsertChar(detect.Detect(m_pWord, 2));
}

// 0x005E8C64
void StrNicknameEngine::InsertLastChar()
{
    DetectCode detect;
    InsertChar(detect.DetectFromEnd(m_pWord, 0));
}

// 0x005E8CFC
void StrNicknameEngine::InsertSecondLastChar()
{
    DetectCode detect;
    InsertChar(detect.DetectFromEnd(m_pWord, 1));
}

// 0x005E8D94 slot 0x18
void StrNicknameEngine::OnTag(const Tag* tag)
{
    switch (tag->group) {
    case 6:
        if (static_cast<u32>(tag->index) < 5) {
            (this->*s_InsertFuncs[tag->index])();
        }
        break;
    case 14:
        if (tag->index == 3) {
            m_Frame.isTag2 = 1;
        } else if (tag->index == 6) {
            InsertSexText(NULL, tag, NULL);
        }
        break;
    case 0xFF:
        if (tag->index == 2) {
            m_IsCapitalNext = 1;
        }
        break;
    }
}

// 0x005E8E18
bool StrNicknameEngine::Make(IWord* pDst, const IWord* pMessage)
{
    if (pDst == NULL) {
        m_IsError = 1;
        return !m_IsError;
    }
    Reset();
    pDst->Clear();
    m_pDst = pDst->GetWord();
    m_pLast = m_pDst;
    m_pLast = m_pLast + pDst->GetSize() - 1;
    m_IsError = 0;
    m_IsValid = 0;
    Run(pMessage->GetWord());
    if (m_IsError) {
        return false;
    }
    WordInfo* info = pDst->GetInfo();
    const WordInfo* srcInfo = m_pWord->GetInfo();
    if (info != NULL && srcInfo != NULL) {
        *info = *srcInfo;
    }
    return !m_IsError;
}

// 0x005E8F28 slot 0x08
void StrNicknameEngine::Begin()
{
    m_Frame.pBegin = m_pCur;
}

// 0x005E8F34 slot 0x10
bool StrNicknameEngine::IsContinue()
{
    return !m_IsError;
}

// 0x005E8F58 slot 0x00
// 0x005E8F48 (deleting dtor)
StrNicknameEngine::~StrNicknameEngine()
{
    // empty (in the original too)
}
} // namespace script
