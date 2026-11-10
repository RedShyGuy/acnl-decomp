#pragma once

#include "decomp.h"
#include "lyt/dTextArea.h"
#include "sys/dSwkbd.h"
#include "sys/dTextManager.h"

namespace sead {
class Heap;
}

namespace gui {
class EditArea;
class RepeatButtonEx;
class Widget;

// The controls of a lyt::TextArea: the field (EditArea on the group G_textArea) and the two scroll buttons
// (G_scrollUp/G_scrollDown, enabled while the text has more lines than the area scrolls through). A decided scroll
// button moves the first line and keeps the cursor in its column. Only the class name is from the binary (RTTI);
// member and function names are ours.
// RTTI N3gui8TextAreaE @ 0x008D0EE4
// vtable 0x009041A4 (vptr 0x009041AC), offset_to_top 0, 5 entries
class TextArea
{
public:
    static const s32 SCROLL_BUTTON_NUM = 2;

    // (inline, the copy is called by the game)
    DECOMP_EMIT_COPY TextArea() // 0x00500758
        : m_pTextArea(NULL), m_pEditArea(NULL), m_pHeap(NULL)
    {
        m_pScrollButtons[0] = NULL;
        m_pScrollButtons[1] = NULL;
        m_IsScrollEnabled[0] = false;
        m_IsScrollEnabled[1] = false;
    }
    virtual ~TextArea(); // 0x005007F8 slot 0x00
    // 0x00500788 slot 0x04 (deleting dtor)
    // the controls of the text area (the layout of the text area has the scroll buttons)
    virtual void Initialize(lyt::TextArea* textArea); // 0x005001C4 slot 0x08
    // the scroll buttons follow the lines
    virtual void Update(); // 0x00500420 slot 0x0C
    // the events of the buttons (gui::Callback)
    virtual void OnEvent(Widget* sender, s32 event); // 0x00500590 slot 0x10

    // (inline) the line of the cursor (the previous line while the cursor is shown at its end) and the column
    s32 GetCursorLine(sys::TextManager* text, sys::LineManager* lines, s32* column);
    // (inline) the cursor goes to the line relLine below the first line, in the column it keeps in the text manager
    // (the column of the cursor at the first move) or at the end of the line
    void MoveCursorToLine(sys::TextManager* text, sys::LineManager* lines, s32 relLine, s32 column);

    static const char* s_GroupName;
    static const char* s_ScrollGroupNames[SCROLL_BUTTON_NUM];

    lyt::TextArea* m_pTextArea;                              // 0x04
    EditArea* m_pEditArea;                                   // 0x08
    RepeatButtonEx* m_pScrollButtons[SCROLL_BUTTON_NUM];     // 0x0C
    bool m_IsScrollEnabled[SCROLL_BUTTON_NUM];               // 0x14 (the arrow buttons in TextAreaEx)
    sead::Heap* m_pHeap;                                     // 0x18
};
ASSERT_SIZE(TextArea, 0x1C);

inline s32 TextArea::GetCursorLine(sys::TextManager* text, sys::LineManager* lines, s32* column)
{
    s32 line = lines->GetLineOfIndex(text->m_Cursor, column);
    if (m_pTextArea->UpdateLineEnd()) {
        line--;
        *column = sys::GetLineManager()->GetLineLength(line);
    }
    return line;
}

inline void TextArea::MoveCursorToLine(sys::TextManager* text, sys::LineManager* lines, s32 relLine, s32 column)
{
    const s32 line = relLine + m_pTextArea->m_TopLine;
    s32 length = lines->GetLineLength(line);
    const sys::TextManager* lineText = lines->m_pText;
    // the cursor stays before the line feed
    if (lines->GetLineEnd(line) != lineText->m_Length) {
        length--;
    }
    s32 keptColumn = text->m_Unknown0x18;
    if (keptColumn < 0) {
        keptColumn = column - m_pTextArea->UpdateLineEnd();
        text->m_Unknown0x18 = keptColumn;
    }
    if (keptColumn < length) {
        length = keptColumn;
    }
    text->SetCursor(lines->GetLineStartChecked(line) + length, false, !text->m_IsSelecting);
}
} // namespace gui
