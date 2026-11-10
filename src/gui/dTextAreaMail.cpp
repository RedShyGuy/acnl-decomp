#include "gui/dTextAreaMail.h"

#include "gui/dRepeatButtonEx2.h"
#include "gui/dTchOnButtonSp.h"
#include "script/dIWord.h"

namespace gui {
// 0x004FACFC
TextAreaMail::TextAreaMail()
{
    // (only the bases are constructed)
}

// 0x004F764C slot 0x00
// 0x004FAD3C slot 0x04 (deleting dtor)
TextAreaMail::~TextAreaMail()
{
    // (only the bases are destroyed)
}

// 0x004FA898 slot 0x10
void TextAreaMail::OnEvent(Widget* sender, s32 event)
{
    if (event != EVENT_DECIDE) {
        return;
    }
    if (!(sender != NULL && sender == m_pArrowButtons[BUTTON_TOP]) && sender != m_pArrowButtons[BUTTON_BOTTOM] &&
        sender != m_pTouchButtons[BUTTON_TOP] && sender != m_pTouchButtons[BUTTON_BOTTOM]) {
        return;
    }
    sys::TextManager* text = sys::g_pTextManager;
    sys::LineManager* lines = sys::GetLineManager();
    s32 column;
    const s32 line = GetCursorLine(text, lines, &column);
    s32* page = &GetTextAreaMail()->m_Page;
    lyt::TextAreaMail* mail = GetTextAreaMail();
    const s32 top = mail->m_TopLine;
    s32 relLine = line - top;
    bool isNextPage = false;
    if (sender == m_pArrowButtons[BUTTON_TOP] || (sender == m_pTouchButtons[BUTTON_TOP] && *page != PAGE_HEADER)) {
        if (*page == PAGE_SIGNATURE) {
            // back to the kept page and first line of the body, with its text
            const s32 keptTop = mail->m_KeptTop;
            mail->ScrollToPage(mail->m_Unknown0x730);
            GetTextAreaMail()->vf_0x58(true, true);
            lyt::TextAreaMail* textArea = GetTextAreaMail();
            script::WordPtr buffer(reinterpret_cast<char16*>(sys::g_pTextBuffer), 197);
            static_cast<script::IWord&>(buffer).Copy(lyt::Func5CCF84(textArea->m_pMail));
            // the length of the text (the count starts at the character before the buffer)
            s32 length = -1;
            const wchar_t* textBuffer = sys::g_pTextBuffer;
            if (textBuffer != NULL) {
                while (textBuffer[length] != 0) {
                    length++;
                }
            }
            sys::g_pTextManager->SetText(sys::g_pTextBuffer, length);
            GetTextAreaMail()->m_TopLine = keptTop;
        } else if (static_cast<u32>(*page - 1) < PAGE_BODY_LAST) {
            // the body: a line up, the previous page at its first line
            bool isScrolled = true;
            if (top > 0) {
                mail->m_TopLine = top - 1;
                GetTextAreaMail()->ScrollToPage(*page - 1);
            } else if (relLine != 0) {
                relLine = 0;
                isScrolled = false;
            } else {
                mail->ScrollToPage(*page - 1);
            }
            GetTextAreaMail()->vf_0x58(isScrolled, isScrolled);
        }
    } else if (sender == m_pArrowButtons[BUTTON_BOTTOM]) {
        if (*page == PAGE_HEADER || *page == PAGE_BODY_LAST) {
            isNextPage = true;
        } else if (GetPageNum() - 1 > static_cast<u32>(*page)) {
            // the body: the page and the first line are kept for the signature
            bool isScrolled = true;
            GetTextAreaMail()->m_Unknown0x730 = GetTextAreaMail()->m_Page;
            GetTextAreaMail()->m_KeptTop = GetTextAreaMail()->m_TopLine;
            const sys::TextManager* lineText = lines->m_pText;
            if (lines->GetLineEnd(line) == lineText->m_Length) {
                // the cursor is in the last line: to the signature
                GetTextAreaEx()->ScrollToPage(GetPageNum() - 1);
            } else {
                const s32 topLine = GetTextAreaMail()->m_TopLine;
                if (lines->GetLineStartChecked(GetTextAreaMail()->GetScrollLineNum() + topLine) >= 0) {
                    GetTextAreaMail()->m_TopLine = topLine + 1;
                    GetTextAreaMail()->ScrollToPage(*page + 1);
                } else {
                    relLine = GetTextAreaMail()->GetScrollLineNum() - 1;
                    isScrolled = false;
                }
            }
            GetTextAreaMail()->vf_0x58(isScrolled, isScrolled);
        }
    } else if (sender == m_pTouchButtons[BUTTON_BOTTOM] && *page == PAGE_HEADER) {
        isNextPage = true;
    }
    if (isNextPage) {
        mail->ScrollToPage(*page + 1);
        GetTextAreaMail()->vf_0x58(true, true);
    }
    MoveCursorToLine(text, lines, relLine, column);
}
} // namespace gui
