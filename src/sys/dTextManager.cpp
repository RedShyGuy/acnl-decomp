#include "sys/dTextManager.h"

#include <cwchar>

#include "Other/dRegion.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "event/dEventBase.h"
#include "script/dTalkSystem.h"
#include "sys/sys_Extern.h"

namespace {
// the long vowel mark of the katakana
const u16 CHAR_LONG_VOWEL = 0x30FC;
// ん / ン that a trailing 'n' of the romaji becomes
const u16 CHAR_HIRAGANA_N = 0x3093;
const u16 CHAR_KATAKANA_N = 0x30F3;
// the ideographic space
const u16 CHAR_IDEOGRAPHIC_SPACE = 0x3000;
// a character of the keyboard (private use) that becomes a space when its multi-tap key ends
const u16 CHAR_KEY_SPACE = 0xE057;
// the character whose speech the long vowel mark and some modes play when there is nothing else (purpose unknown)
const u16 SPEECH_CHAR_DEFAULT = 158;
// the kana mode of KeyboardSetting for katakana
const s32 KANA_MODE_KATAKANA = 2;

// the accents of the dead keys (index into AccentLetter::accented and the flags of the regions)
enum Accent
{
    ACCENT_GRAVE,
    ACCENT_ACUTE,
    ACCENT_CIRCUMFLEX,
    ACCENT_TILDE,
    ACCENT_DIAERESIS,
    ACCENT_NUM
};

// a letter and its forms with the accents (0: none)
struct AccentLetter
{
    u16 base;
    u16 accented[ACCENT_NUM];
};
const s32 ACCENT_LETTER_NUM = 14;

// 0x00898274
const AccentLetter s_AccentLetters[ACCENT_LETTER_NUM] = {
    {L'A', {0xC0, 0xC1, 0xC2, 0xC3, 0xC4}}, {L'E', {0xC8, 0xC9, 0xCA, 0x00, 0xCB}}, {L'I', {0xCC, 0xCD, 0xCE, 0x00, 0xCF}},
    {L'N', {0x00, 0x00, 0x00, 0xD1, 0x00}}, {L'O', {0xD2, 0xD3, 0xD4, 0xD5, 0xD6}}, {L'U', {0xD9, 0xDA, 0xDB, 0x00, 0xDC}},
    {L'Y', {0x00, 0xDD, 0x00, 0x00, 0x178}}, {L'a', {0xE0, 0xE1, 0xE2, 0xE3, 0xE4}}, {L'e', {0xE8, 0xE9, 0xEA, 0x00, 0xEB}},
    {L'i', {0xEC, 0xED, 0xEE, 0x00, 0xEF}}, {L'n', {0x00, 0x00, 0x00, 0xF1, 0x00}}, {L'o', {0xF2, 0xF3, 0xF4, 0xF5, 0xF6}},
    {L'u', {0xF9, 0xFA, 0xFB, 0x00, 0xFC}}, {L'y', {0x00, 0xFD, 0x00, 0x00, 0xFF}},
};
// the accents each region allows on the letters of s_AccentLetters
// 0x0089831C
const u8 s_AccentsAmericaFrench[ACCENT_LETTER_NUM][ACCENT_NUM] = {
    {1, 0, 1, 0, 1}, {1, 1, 1, 0, 1}, {0, 0, 1, 0, 1}, {0, 0, 0, 0, 0}, {1, 0, 1, 0, 1}, {0, 0, 1, 0, 1}, {0, 0, 0, 0, 0},
    {1, 0, 1, 0, 1}, {1, 1, 1, 0, 1}, {0, 0, 1, 0, 1}, {0, 0, 0, 0, 0}, {1, 0, 1, 0, 1}, {0, 0, 1, 0, 1}, {0, 0, 0, 0, 0},
};
// 0x00898362
const u8 s_AccentsAmericaSpanish[ACCENT_LETTER_NUM][ACCENT_NUM] = {
    {0, 1, 0, 0, 0}, {0, 1, 0, 0, 0}, {0, 1, 0, 0, 0}, {0, 0, 0, 1, 0}, {0, 1, 0, 0, 0}, {0, 1, 0, 0, 1}, {0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0}, {0, 1, 0, 0, 0}, {0, 1, 0, 0, 0}, {0, 0, 0, 1, 0}, {0, 1, 0, 0, 0}, {0, 1, 0, 0, 1}, {0, 0, 0, 0, 0},
};
// 0x008983A8
const u8 s_AccentsEuropeFrench[ACCENT_LETTER_NUM][ACCENT_NUM] = {
    {1, 1, 1, 0, 1}, {1, 1, 1, 0, 1}, {1, 1, 1, 0, 1}, {0, 0, 0, 0, 0}, {1, 1, 1, 0, 1}, {1, 1, 1, 0, 1}, {0, 1, 0, 0, 1},
    {1, 1, 1, 0, 1}, {1, 1, 1, 0, 1}, {1, 1, 1, 0, 1}, {0, 0, 0, 0, 0}, {1, 1, 1, 0, 1}, {1, 1, 1, 0, 1}, {0, 1, 0, 0, 1},
};
// 0x008983EE
const u8 s_AccentsEuropeGerman[ACCENT_LETTER_NUM][ACCENT_NUM] = {
    {1, 1, 0, 0, 0}, {1, 1, 0, 0, 0}, {1, 1, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 1, 0, 0, 0}, {1, 1, 0, 0, 0}, {0, 1, 0, 0, 0},
    {1, 1, 0, 0, 0}, {1, 1, 0, 0, 0}, {1, 1, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 1, 0, 0, 0}, {1, 1, 0, 0, 0}, {0, 1, 0, 0, 0},
};
// 0x00898434
const u8 s_AccentsEuropeSpanish[ACCENT_LETTER_NUM][ACCENT_NUM] = {
    {0, 1, 0, 0, 1}, {0, 1, 0, 0, 1}, {0, 1, 0, 0, 1}, {0, 0, 0, 0, 0}, {0, 1, 0, 0, 1}, {0, 1, 0, 0, 1}, {0, 1, 0, 0, 1},
    {0, 1, 0, 0, 1}, {0, 1, 0, 0, 1}, {0, 1, 0, 0, 1}, {0, 0, 0, 0, 0}, {0, 1, 0, 0, 1}, {0, 1, 0, 0, 1}, {0, 1, 0, 0, 1},
};
} // namespace

namespace sys {
// (inline)
inline f32 sys::TextManager::GetSoundPan() const
{
    return CalcPaneSoundPan(m_pSoundPane);
}

// (inline)
inline void sys::TextManager::PlaySound(u32 id) const
{
    const f32 pan = GetSoundPan();
    SoundMgr::PlaySound(static_cast<SeID>(id), pan);
}

// (inline)
inline void sys::TextManager::PlaySpeech(u16 c) const
{
    const f32 pan = GetSoundPan();
    SoundMgr::SPlaySpeechChar(script::CharToSpeechSound(c), pan);
}

// (inline)
inline void sys::TextManager::PlaySpeechLong(u16 c) const
{
    const f32 pan = GetSoundPan();
    SoundMgr::SPlaySpeechLongChar(script::CharToSpeechSound(c), pan);
}

// 0x00522DA4
bool sys::TextManager::InputChar(u16 c, u32 romajiMode, bool isDeadKey)
{
    if (c == 0) {
        return false;
    }
    bool result = true;
    bool isInsertPending;
    if (isDeadKey) {
        if (m_PendingAccent == 0) {
            m_PendingAccent = c;
            PlaySpeech(c);
            return result;
        }
        if (m_PendingAccent == c) {
            m_PendingAccent = 0;
            isInsertPending = false;
        } else {
            isInsertPending = true;
        }
    } else if (m_PendingAccent != 0) {
        const u16 composed = ComposeAccent(c, m_PendingAccent);
        if (composed != 0) {
            m_PendingAccent = 0;
            result = InputChar(composed, romajiMode, false);
            PlaySpeech(c);
            return result;
        }
        isInsertPending = true;
    } else {
        isInsertPending = false;
    }
    if (isInsertPending) {
        const u16 pending = m_PendingAccent;
        m_PendingAccent = 0;
        result = InputChar(pending, romajiMode, false);
        if (!result) {
            PlaySpeech(c);
            return result;
        }
    }

    const bool isDakutenKey = c == KEY_DAKUTEN || c == KEY_HANDAKUTEN || c == KEY_SMALL;
    m_Unknown0x18 = -1;
    EndCellPhone(true);
    if (c == ' ' || c == CHAR_IDEOGRAPHIC_SPACE) {
        if (GetRegionIndex() == 1 || GetRegionIndex() == 2) {
            EndCellPhone(true);
            result = Commit(false) & result;
            ClearSelection();
        }
    } else if (c == '\n') {
        if (m_CompositionLength > 0) {
            EndCellPhone(true);
            result = Commit(false) & result;
            ClearSelection();
            PlaySound(result ? SE_SYS_SWK_ENTER : SE_SYS_SWK_ERROR);
            return result;
        }
        if (!g_KeyboardParam.isNewlineAllowed) {
            return false;
        }
    }
    if (romajiMode == 0) {
        result = Commit(false) & result;
    }
    if (!isDakutenKey && m_IsSelecting) {
        DeleteSelection();
    }
    result = InsertChar(c, m_Cursor, romajiMode, false) & result;
    if (!isDakutenKey) {
        ClearSelection();
    }
    return result;
}

// 0x00523018
bool sys::TextManager::Commit(bool)
{
    bool result = true;
    const s32 compositionLength = m_CompositionLength;
    if (compositionLength > 0) {
        const s32 last = m_Cursor - compositionLength - 1 + compositionLength;
        const u16 c = m_pBuffer[last];
        if (c == 'N' || c == 'n') {
            SetChar(last, g_KeyboardSetting.kanaMode == KANA_MODE_KATAKANA ? CHAR_KATAKANA_N : CHAR_HIRAGANA_N);
        }
    }
    const s32 length = m_Length - m_CompositionLength + (m_CompositionLength + m_CompositionExtra);
    if (length > m_MaxLength) {
        const s32 over = length - m_MaxLength;
        const s32 pos = m_Cursor - m_CompositionLength - m_CompositionExtra + (m_CompositionLength + m_CompositionExtra) - over;
        for (s32 i = 0; i < over; i++) {
            Delete(pos, 1);
        }
        result = false;
    }
    m_CompositionLength = 0;
    m_CompositionExtra = 0;
    if ((g_KeyboardParam.isMasked != 0 || m_pLineManager->m_Writer.m_IsWidthFixed) && compositionLength != 0) {
        m_pLineManager->InvalidateFrom(m_Cursor);
    }
    return result;
}

// 0x0052318C
void sys::TextManager::CommitAll(bool isClearSelection)
{
    EndCellPhone(true);
    Commit(false);
    if (isClearSelection) {
        ClearSelection();
    }
}

// 0x005231D0
bool sys::TextManager::InsertChar(u16 c, s32 pos, u32 romajiMode, bool unused)
{
    if (pos < 0) {
        pos = 0;
    }
    if (pos > m_Length) {
        pos = m_Length;
    }
    bool result = true;
    if (c == '\n') {
        romajiMode = 0;
        PlaySound(SE_SYS_SWK_ENTER);
    } else if ((c == KEY_DAKUTEN || c == KEY_HANDAKUTEN || c == KEY_SMALL) && !m_Unknown0x94) {
        // the kana before the position gets the form of the key (or goes back to its plain form)
        if (pos == 0) {
            return result;
        }
        const u16 prev = m_pBuffer[pos - 1];
        u16 variant = prev;
        if (c == KEY_DAKUTEN) {
            const KanaVariant* v = FindKanaVariant(variant);
            if (v != NULL && v->dakuten != 0) {
                variant = v->dakuten;
            }
        } else if (c == KEY_HANDAKUTEN) {
            const KanaVariant* v = FindKanaVariant(variant);
            if (v != NULL && v->handakuten != 0) {
                variant = v->handakuten;
            }
        } else {
            const KanaVariant* v = FindKanaVariant(variant);
            if (v != NULL && v->small != 0) {
                variant = v->small;
            }
        }
        if (variant == prev) {
            const KanaVariant* v = FindKanaVariant(prev);
            if (v != NULL && v->plain != 0) {
                variant = v->plain;
            }
        }
        if (variant != prev) {
            SetChar(pos - 1, variant);
        }
        PlaySpeech(variant);
        return result;
    } else if (romajiMode != 0) {
        // the romaji typed so far with the character, converted to kana
        wchar_t romaji[10];
        wmemcpy(romaji, &m_pBuffer[m_Cursor - m_CompositionLength], m_CompositionLength);
        romaji[m_CompositionLength] = c;
        romaji[m_CompositionLength + 1] = 0;
        wchar_t kana[8];
        const s32 kanaNum = ConvertRomajiToKana(kana, 5, romaji, m_CompositionLength + 1, romajiMode == 1);
        const s32 restNum = wcslen(kana) - kanaNum;
        const bool isAllKana = restNum == 0;
        s32 removeNum = m_CompositionLength - restNum + !isAllKana;
        if (kanaNum > 0) {
            if (isAllKana) {
                removeNum = m_CompositionLength;
            }
            const s32 start = m_Cursor - m_CompositionLength;
            for (s32 i = 0; i < removeNum; i++) {
                Delete(start, 1);
            }
            for (s32 i = 0; i < kanaNum; i++) {
                result = result & InsertChar(kana[i], start + i, 0, unused);
            }
            if (!(result & !isAllKana)) {
                return result;
            }
            pos = m_Cursor;
        }
    }

    const bool canInsert = m_pLineManager->CanInsert(pos - 1, c, false, true);
    if (m_MaxLength <= m_Length - m_CompositionLength || !canInsert) {
        m_IsFull = false;
        if (m_MaxLength <= m_Length - m_CompositionLength && m_Unknown0x98 == 1) {
            m_IsFull = true;
        }
        Commit(false);
        PlaySound(SE_SYS_SWK_ERROR);
        return false;
    }
    wmemmove(&m_pBuffer[pos + 1], &m_pBuffer[pos], m_Length - pos);
    m_Length++;
    m_pBuffer[m_Length] = 0;
    m_pBuffer[pos] = c;
    if (c == '\n') {
        m_IsLastNewline = true;
    } else {
        m_IsLastNewline = false;
        if (c == CHAR_LONG_VOWEL) {
            // the long vowel speaks like the last kana before it
            u16 prev = SPEECH_CHAR_DEFAULT;
            for (s32 n = 1, i = pos - 1; n < pos + 1; n++, i--) {
                const u16 p = m_pBuffer[i];
                if (p != '\n' && p != CHAR_LONG_VOWEL) {
                    prev = p;
                    break;
                }
            }
            PlaySpeechLong(prev);
        } else {
            u16 speech = c;
            if (g_KeyboardSetting.mode == 2 || g_KeyboardSetting.mode == 3) {
                speech = SPEECH_CHAR_DEFAULT;
            }
            PlaySpeech(speech);
        }
    }
    if (romajiMode != 0) {
        m_CompositionLength++;
    }
    if (m_Cursor >= pos) {
        m_Cursor++;
        if (m_Cursor > m_Length) {
            m_Cursor = m_Length;
        }
    }
    m_pLineManager->InvalidateFrom(pos);
    return result;
}

// 0x005236CC
void sys::TextManager::SetCursor(s32 pos, bool isResetUnknown18, bool isClearSelection)
{
    if (isResetUnknown18) {
        m_Unknown0x18 = -1;
    }
    EndCellPhone(true);
    CommitAll(false);
    if (pos < 0) {
        pos = 0;
    }
    if (pos > m_Length) {
        pos = m_Length;
    }
    m_Cursor = pos;
    m_IsSelecting = !isClearSelection;
    if (isClearSelection) {
        m_SelectionAnchor = pos;
        m_Unknown0x90 = -1;
    }
}

// 0x0052373C
sys::TextManager::CellPhoneInput::CellPhoneInput() : m_Num(0), m_Index(0)
{
    for (s32 i = 0; i < 16; i++) {
        m_Chars[i] = 0;
    }
}

// 0x0052377C slot 0x00
// 0x00523778 (deleting dtor)
sys::TextManager::CellPhoneInput::~CellPhoneInput()
{
    // empty (in the original too)
}

// 0x00523780
void sys::TextManager::Delete(s32 pos, s32 count)
{
    if (pos < 0) {
        pos = 0;
    }
    if (pos > m_Length - 1) {
        pos = m_Length - 1;
    }
    if (pos + count > m_Length) {
        count = m_Length - pos;
    }
    const s32 end = pos + count;
    wmemmove(&m_pBuffer[pos], &m_pBuffer[end], m_Length - pos - count);
    m_Length -= count;
    m_pBuffer[m_Length] = 0;

    // the deleted characters of the composition
    s32 removed = 0;
    const s32 compositionStart = m_Cursor - m_CompositionLength;
    if (compositionStart <= end - 1) {
        s32 last = compositionStart - 1 + m_CompositionLength;
        if (pos <= last) {
            if (end - 1 <= last) {
                last = end - 1;
            }
            const s32 first = pos >= compositionStart ? pos : compositionStart;
            removed = last - first + 1;
        }
    }
    m_CompositionLength -= removed;
    if (m_Cursor > pos) {
        m_Cursor -= count;
        if (m_Cursor <= pos) {
            m_Cursor = pos;
        }
    }
    m_pLineManager->InvalidateFrom(pos);
}

// 0x00523870
bool sys::TextManager::InputCellPhone(CellPhoneInput* key)
{
    m_Unknown0x18 = -1;
    if (m_Cursor <= 0) {
        EndCellPhone(true);
    }
    if (key->m_Num <= 0) {
        EndCellPhone(true);
        return true;
    }
    bool result = Commit(g_Unknown95F0E8);
    if (m_IsSelecting) {
        DeleteSelection();
    }
    if (m_pCellPhoneInput == key) {
        // the next character of the key replaces the one before the cursor
        key->m_Index++;
        if (key->m_Index > key->m_Num - 1) {
            key->m_Index = 0;
        }
        u16 c = key->m_Chars[key->m_Index];
        m_pBuffer[ClampIndex(m_Cursor - 1)] = c;
        if (m_pLineManager->CanInsert(m_Cursor - 1, c, true, true)) {
            SetChar(m_Cursor - 1, c);
            if (c == CHAR_LONG_VOWEL) {
                u16 prev = SPEECH_CHAR_DEFAULT;
                for (s32 n = 1, i = m_Length - 2; n < m_Length; n++, i--) {
                    const u16 p = m_pBuffer[i];
                    if (p != '\n' && p != CHAR_LONG_VOWEL) {
                        prev = p;
                        break;
                    }
                }
                PlaySpeechLong(prev);
            } else {
                if (c == 0x152 || c == 0x153) {
                    c = '@';
                }
                PlaySpeech(c);
            }
        } else {
            EndCellPhone(true);
            result = false;
            PlaySound(SE_SYS_SWK_ERROR);
        }
    } else {
        key->m_Index = 0;
        m_pCellPhoneInput = key;
        if (!InsertChar(key->m_Chars[key->m_Index], m_Cursor, 0, g_Unknown95F0E8)) {
            EndCellPhone(true);
            result = false;
            PlaySound(SE_SYS_SWK_ERROR);
        }
    }
    m_SelectionAnchor = m_Cursor;
    m_IsSelecting = false;
    m_Unknown0x90 = -1;
    if (key->m_Num == 1 && result) {
        // a key with one character ends at once
        u16 c = key->m_Chars[key->m_Index];
        EndCellPhone(true);
        if (c == 0x152 || c == 0x153) {
            c = '@';
        }
        PlaySpeech(c);
    }
    return result;
}

// 0x00523B84
void sys::TextManager::EndCellPhone(bool isCheck)
{
    if (m_pCellPhoneInput != NULL) {
        if (isCheck && !m_pLineManager->CanInsert(m_Cursor - 1, m_pCellPhoneInput->m_Chars[m_pCellPhoneInput->m_Index], true, true)) {
            Delete(m_Cursor - 1, 1);
        } else if (m_pCellPhoneInput->m_Chars[m_pCellPhoneInput->m_Index] == CHAR_KEY_SPACE) {
            SetChar(m_Cursor - 1, ' ');
        }
    }
    CellPhoneInput* key = m_pCellPhoneInput;
    m_pCellPhoneInput = NULL;
    if (key != NULL) {
        s32 index = m_Cursor - 1;
        if (index < 0) {
            index = 0;
        }
        m_pLineManager->InvalidateFrom(index);
    }
}

// 0x00523C94
void sys::TextManager::DeleteSelection()
{
    const s32 start = m_SelectionAnchor <= m_Cursor ? m_SelectionAnchor : m_Cursor;
    const s32 end = m_SelectionAnchor <= m_Cursor ? m_Cursor : m_SelectionAnchor;
    if (end - start > 0) {
        Delete(start, end - start);
    }
    ClearSelection();
}

// 0x00523D04
void sys::TextManager::SetText(const wchar_t* text, s32 length)
{
    m_Unknown0x18 = -1;
    EndCellPhone(true);
    if (text != NULL) {
        if (length < 0) {
            length = wcslen(text);
        }
        if (length > m_MaxLength) {
            length = m_MaxLength;
        }
        wmemcpy(m_pBuffer, text, length);
        m_pBuffer[length] = 0;
        m_Length = length;
    } else {
        m_pBuffer[0] = 0;
        m_Length = 0;
    }
    m_CompositionLength = 0;
    m_CompositionExtra = 0;
    m_Cursor = 0;
    m_SelectionAnchor = 0;
    m_pLineManager->InvalidateFrom(0);
    m_IsTextSet = true;
}

// 0x00523DB8
bool sys::TextManager::Backspace(bool isSilent)
{
    if (m_Unknown0x94 == true && !m_IsSelecting) {
        if (!isSilent) {
            PlaySound(SE_SYS_SWK_ERROR);
        }
        return false;
    }
    m_Unknown0x18 = -1;
    EndCellPhone(false);
    const s32 start = m_SelectionAnchor <= m_Cursor ? m_SelectionAnchor : m_Cursor;
    const s32 end = m_SelectionAnchor <= m_Cursor ? m_Cursor : m_SelectionAnchor;
    if (m_IsSelecting && end - start > 0) {
        DeleteSelection();
    } else {
        if (m_Cursor <= 0) {
            ClearSelection();
            if (!isSilent) {
                PlaySound(SE_SYS_SWK_ERROR);
            }
            return false;
        }
        Delete(m_Cursor - 1, 1);
    }
    m_SelectionAnchor = m_Cursor;
    m_Unknown0x90 = -1;
    m_IsSelecting = false;
    PlaySound(SE_SYS_SWK_BACKSPACE);
    m_Unknown0x95 = false;
    return true;
}

// 0x00523F0C slot 0x00
// 0x00523EC8 (deleting dtor)
sys::TextManager::~TextManager()
{
    if (m_pLineManager != NULL) {
        delete m_pLineManager;
        m_pLineManager = NULL;
    }
}

// 0x00748968
u16 sys::TextManager::GetDisplayChar(s32 index)
{
    if (m_Length == index) {
        return m_pBuffer[index];
    }
    if (g_KeyboardParam.isMasked == 0 || !IsOutsideComposition(index)) {
        return m_pBuffer[index];
    }
    // the character of the multi-tap key being pressed stays visible
    const s32 typing = m_pCellPhoneInput != NULL && m_Cursor > 0 ? m_Cursor - 1 : -1;
    if (typing == index) {
        return m_pBuffer[index];
    }
    const u16 c = m_pBuffer[index];
    if (c == '\n') {
        return c;
    }
    if (m_Unknown0x8C > 0 && m_Cursor - 1 == index) {
        return c;
    }
    return g_KeyboardParam.isFullWidthMask ? 0xFF0A : '*';
}

// 0x00748A28
bool sys::TextManager::IsOutsideComposition(s32 index)
{
    bool isOutside = true;
    if (m_CompositionLength > 0) {
        const s32 start = m_Cursor - m_CompositionLength;
        if (start <= index && index < start + m_CompositionLength) {
            isOutside = false;
        }
    }
    if (m_CompositionExtra > 0) {
        const s32 start = m_Cursor - m_CompositionLength - m_CompositionExtra;
        if (start <= index && index < start + m_CompositionExtra) {
            isOutside = false;
        }
    }
    return isOutside;
}

// 0x00748A88
u16 sys::TextManager::ComposeAccent(u16 c, u16 accent)
{
    if (c == '\n') {
        return '\n';
    }
    if (c == ' ') {
        return accent;
    }
    const u8(*accents)[ACCENT_NUM] = NULL;
    switch (GetRegion()) {
    case event::REGION_AMERICA_SPANISH:
        accents = s_AccentsAmericaSpanish;
        break;
    case event::REGION_AMERICA_FRENCH:
        accents = s_AccentsAmericaFrench;
        break;
    case event::REGION_EUROPE_SPANISH:
        accents = s_AccentsEuropeSpanish;
        break;
    case event::REGION_EUROPE_FRENCH:
        accents = s_AccentsEuropeFrench;
        break;
    case event::REGION_EUROPE_GERMAN:
        accents = s_AccentsEuropeGerman;
        break;
    }
    if (accents == NULL) {
        return 0;
    }
    for (s32 i = 0; i < ACCENT_LETTER_NUM; i++) {
        if (s_AccentLetters[i].base != c) {
            continue;
        }
        switch (accent) {
        case '`':
            return accents[i][ACCENT_GRAVE] ? s_AccentLetters[i].accented[ACCENT_GRAVE] : 0;
        case '\'':
        case 0xB4:
            return accents[i][ACCENT_ACUTE] ? s_AccentLetters[i].accented[ACCENT_ACUTE] : 0;
        case '^':
            return accents[i][ACCENT_CIRCUMFLEX] ? s_AccentLetters[i].accented[ACCENT_CIRCUMFLEX] : 0;
        case '~':
            return accents[i][ACCENT_TILDE] ? s_AccentLetters[i].accented[ACCENT_TILDE] : 0;
        case '"':
        case 0xA8:
            return accents[i][ACCENT_DIAERESIS] ? s_AccentLetters[i].accented[ACCENT_DIAERESIS] : 0;
        default:
            return 0;
        }
    }
    return 0;
}

} // namespace sys
