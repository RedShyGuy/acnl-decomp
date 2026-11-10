#include "gui/dTextAreaBoard.h"

#include "gui/dEditAreaEx.h"
#include "gui/dRepeatButtonEx2.h"
#include "gui/dTchOnButtonSp.h"

namespace gui {
// 0x004FB514
TextAreaBoard::TextAreaBoard()
{
    // (only the bases are constructed)
}

// 0x004FB564 slot 0x00
// 0x004FB554 slot 0x04 (deleting dtor)
TextAreaBoard::~TextAreaBoard()
{
    // (only the bases are destroyed)
}

// 0x004FB248 slot 0x10
void TextAreaBoard::OnEvent(Widget* sender, s32 event)
{
    if (event == EVENT_DECIDE) {
        if (!(sender != NULL && sender == m_pArrowButtons[BUTTON_TOP]) && sender != m_pArrowButtons[BUTTON_BOTTOM] &&
            sender != m_pTouchButtons[BUTTON_TOP]) {
            return;
        }
        sys::TextManager* text = sys::g_pTextManager;
        sys::LineManager* lines = sys::GetLineManager();
        s32 column;
        const s32 line = GetCursorLine(text, lines, &column);
        s32* page = &GetTextAreaEx()->m_Page;
        lyt::TextAreaEx* textArea = GetTextAreaEx();
        const s32 top = textArea->m_TopLine;
        s32 relLine = line - top;
        if (sender == m_pArrowButtons[BUTTON_TOP] || (sender == m_pTouchButtons[BUTTON_TOP] && *page != 0)) {
            if (top > 0) {
                textArea->m_TopLine = top - 1;
                GetTextAreaEx()->ScrollToPage(*page - 1);
            } else {
                relLine = 0;
            }
        } else if (sender == m_pArrowButtons[BUTTON_BOTTOM]) {
            if (lines->GetLineStartChecked(textArea->GetScrollLineNum() + top) >= 0) {
                GetTextAreaEx()->m_TopLine = top + 1;
                GetTextAreaEx()->ScrollToPage(*page + 1);
            } else {
                relLine = GetTextAreaEx()->GetScrollLineNum() - 1;
            }
        }
        MoveCursorToLine(text, lines, relLine, column);
    } else if (event == EditAreaEx::EVENT_PRESS) {
        // the touch areas are inactive while the field is pressed
        if (sender != NULL && sender == m_pEditAreaEx) {
            for (s32 i = 0; i < BUTTON_NUM; i++) {
                if (m_pTouchButtons[i] != NULL) {
                    m_pTouchButtons[i]->SetActive(false);
                }
            }
        }
    } else if (event == EditAreaEx::EVENT_RELEASE) {
        if (sender != NULL && sender == m_pEditAreaEx) {
            for (s32 i = 0; i < BUTTON_NUM; i++) {
                if (m_pTouchButtons[i] != NULL) {
                    m_pTouchButtons[i]->SetActive(true);
                }
            }
        }
    }
}
} // namespace gui
