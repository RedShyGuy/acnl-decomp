#pragma once

#include <cstring>

#include "decomp.h"
#include "sys/dLineManager.h"

namespace nw {
namespace lyt {
class Pane;
}
} // namespace nw

namespace sys {
// The text being typed on the software keyboard: the buffer with the cursor and a selection, romaji that is being
// composed into kana, multi-tap keys (CellPhoneInput), dakuten keys (the characters 1-3), dead keys for the accents of
// the European regions, the typing sounds and the masking of passwords. Only the class names are from the binary
// (RTTI); member and function names are ours.
// RTTI N3sys11TextManagerE @ 0x008D1040
// vtable 0x009047D0 (vptr 0x009047D8), offset_to_top 0, 2 entries
class TextManager
{
public:
    class CellPhoneInput;

    // the keys 1-3 change the kana before the cursor (dakuten, handakuten, small form)
    static const u16 KEY_DAKUTEN = 1;
    static const u16 KEY_HANDAKUTEN = 2;
    static const u16 KEY_SMALL = 3;

    // (inline)
    TextManager(s32 maxLength, wchar_t* buffer)
        : m_pLineManager(NULL), m_Length(0), m_MaxLength(maxLength), m_pBuffer(buffer), m_Cursor(0), m_Unknown0x18(-1), m_SelectionAnchor(0),
          m_IsSelecting(false), m_CompositionLength(0), m_CompositionExtra(0), m_Unknown0x2C(0), m_Unknown0x80(0), m_pCellPhoneInput(NULL),
          m_PendingAccent(0), m_Unknown0x8C(0), m_Unknown0x90(-1), m_Unknown0x94(false), m_Unknown0x95(false), m_IsTextSet(false), m_IsFull(false),
          m_Unknown0x98(0), m_IsLastNewline(false), m_pSoundPane(NULL)
    {
        std::memset(m_Unknown0x2E, 0, sizeof(m_Unknown0x2E));
    }
    virtual ~TextManager(); // 0x00523F0C slot 0x00
    // 0x00523EC8 slot 0x04 (deleting dtor)

    // a typed character (romajiMode: 0 directly, 1 as romaji for hiragana, else for katakana; isDeadKey: an accent that
    // waits for the next letter); false: it could not be put
    bool InputChar(u16 c, u32 romajiMode, bool isDeadKey); // 0x00522DA4
    // the romaji being composed is ended (a trailing 'n' becomes ん/ン); false: the text was too long and was cut
    bool Commit(bool unused); // 0x00523018
    // the multi-tap key and the composition are ended (and the selection, with isClearSelection)
    void CommitAll(bool isClearSelection); // 0x0052318C
    // the character is put at pos (see InputChar)
    bool InsertChar(u16 c, s32 pos, u32 romajiMode, bool unused); // 0x005231D0
    // the cursor goes to pos; the selection is cleared or extended from the anchor
    void SetCursor(s32 pos, bool isResetUnknown18, bool isClearSelection); // 0x005236CC
    // count characters at pos are deleted
    void Delete(s32 pos, s32 count); // 0x00523780
    // a press of a multi-tap key: the next of its characters, or its first one at the cursor
    bool InputCellPhone(CellPhoneInput* key); // 0x00523870
    // the multi-tap key is ended (isCheck: its character is removed again if it does not fit)
    void EndCellPhone(bool isCheck); // 0x00523B84
    // the selected characters are deleted
    DECOMP_NOINLINE void DeleteSelection(); // 0x00523C94
    // the text (length < 0: up to the end) replaces the buffer; the cursor goes to the start
    void SetText(const wchar_t* text, s32 length); // 0x00523D04
    // the character before the cursor (or the selection) is deleted; false: nothing was deleted (with an error sound
    // unless isSilent)
    bool Backspace(bool isSilent); // 0x00523DB8

    // the character as it is shown ('*' for a masked character that is not being typed)
    DECOMP_NOINLINE u16 GetDisplayChar(s32 index); // 0x00748968
    // the character is not in the romaji being composed
    DECOMP_NOINLINE bool IsOutsideComposition(s32 index); // 0x00748A28
    // the letter with the accent (a dead key) if the region has it (0: none; a line feed and a space stay)
    DECOMP_NOINLINE u16 ComposeAccent(u16 c, u16 accent); // 0x00748A88

private:
    // (inline) the index within the text
    s32 ClampIndex(s32 index) const
    {
        if (index < 0) {
            index = 0;
        }
        if (index > m_Length - 1) {
            index = m_Length - 1;
        }
        return index;
    }
    // (inline) a character of the text is replaced
    void SetChar(s32 index, u16 c)
    {
        const s32 i = ClampIndex(index);
        m_pBuffer[i] = c;
        m_pLineManager->InvalidateFrom(i);
    }
    // (inline) the selection ends at the cursor
    void ClearSelection()
    {
        m_IsSelecting = false;
        m_SelectionAnchor = m_Cursor;
        m_Unknown0x90 = -1;
    }
    // (inline) the sounds of the keyboard at the pane of the text
    f32 GetSoundPan() const;
    void PlaySound(u32 id) const;
    void PlaySpeech(u16 c) const;
    void PlaySpeechLong(u16 c) const;

public:
    LineManager* m_pLineManager;        // 0x04
    s32 m_Length;                       // 0x08
    s32 m_MaxLength;                    // 0x0C
    wchar_t* m_pBuffer;                 // 0x10
    s32 m_Cursor;                       // 0x14
    s32 m_Unknown0x18;                  // 0x18 (set to -1 by most inputs)
    s32 m_SelectionAnchor;              // 0x1C
    bool m_IsSelecting;                 // 0x20
    s32 m_CompositionLength;            // 0x24 the romaji before the cursor that is being composed
    s32 m_CompositionExtra;             // 0x28 (counted before the composition)
    u8 m_Unknown0x2C;                   // 0x2C
    wchar_t m_Unknown0x2E[41];          // 0x2E
    s32 m_Unknown0x80;                  // 0x80
    CellPhoneInput* m_pCellPhoneInput;  // 0x84 the multi-tap key being pressed
    u16 m_PendingAccent;                // 0x88 the dead key waiting for its letter
    s32 m_Unknown0x8C;                  // 0x8C (> 0: the character before the cursor is not masked)
    s32 m_Unknown0x90;                  // 0x90
    bool m_Unknown0x94;                 // 0x94 the keys 1-3 are no dakuten keys; Backspace only deletes a selection
    bool m_Unknown0x95;                 // 0x95 (cleared by Backspace)
    bool m_IsTextSet;                   // 0x96 set by SetText
    bool m_IsFull;                      // 0x97 a character did not fit because the text is full (with m_Unknown0x98)
    u8 m_Unknown0x98;                   // 0x98
    bool m_IsLastNewline;               // 0x99 the last character put was a line feed
    nw::lyt::Pane* m_pSoundPane;        // 0x9C the sounds are panned to it
};
ASSERT_OFFSET(TextManager, m_Unknown0x2E, 0x2E);
ASSERT_OFFSET(TextManager, m_pCellPhoneInput, 0x84);
ASSERT_OFFSET(TextManager, m_pSoundPane, 0x9C);
ASSERT_SIZE(TextManager, 0xA0);

// A multi-tap key (like the keys of a cell phone): pressing it again goes to its next character.
// RTTI N3sys11TextManager14CellPhoneInputE @ 0x008D1038
// vtable 0x009047C0 (vptr 0x009047C8), offset_to_top 0, 2 entries
class TextManager::CellPhoneInput
{
public:
    CellPhoneInput(); // 0x0052373C
    virtual ~CellPhoneInput(); // 0x0052377C slot 0x00
    // 0x00523778 slot 0x04 (deleting dtor)

    u16 m_Chars[16];  // 0x04
    s32 m_Num;        // 0x24
    s32 m_Index;      // 0x28 the character shown
};
ASSERT_SIZE(TextManager::CellPhoneInput, 0x2C);

inline s32 LineManager::GetLineEnd(s32 line)
{
    const s32 next = GetLineStartChecked(line + 1);
    if (next >= 0) {
        return next - 1;
    }
    if (GetLineStartChecked(line) >= 0) {
        return m_pText->m_Length;
    }
    return -1;
}
} // namespace sys
