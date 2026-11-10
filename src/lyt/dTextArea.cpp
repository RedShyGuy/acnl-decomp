#include "lyt/dTextArea.h"
#include "lyt/dTextAreaBoard.h"
#include "lyt/dTextAreaMail.h"
#include "lyt/dTextAreaOneLine.h"

#include <cmath>
#include <cstdio>
#include <cstring>

#include "Other/dPaneVisible.h"
#include "Other/dRegion.h"
#include "lyt/dAnmObject.h"
#include "nn/math/math_ARMv6.h"
#include "nn/math/math_Api.h"
#include "nw/lyt/lyt_TextBox.h"
#include "sead/seadNew.h"
#include "sead/seadWFixedSafeString.h"
#include "script/dWordCPtr.h"
#include "script/dToUpper.h"
#include "script/dScriptExtern.h"
#include "sys/dSwkbd.h"

namespace lyt {
// 0x0095F138
const char* g_DecorPaneName = "N_decor";
// 0x0095F1A4
u32 g_Unknown95F1A4 = 0;
// 0x0095F1A8
nw::ut::Color8 g_LineFeedColor(0, 0, 0, 0);
} // namespace lyt

namespace {
// the play mode the looping cursor animation gets while g_Unknown95F1A4 is set (beyond the play modes)
const s32 CURSOR_PLAY_MODE_UNKNOWN = 5;
// a text area estimates its characters per line with this one
const u16 CHAR_NARROW = 'i';
} // namespace

// 0x0050A7FC slot 0x18
void lyt::TextArea::vf_0x18()
{
    // empty (in the original too)
}

// 0x0050A800 slot 0x30
void lyt::TextArea::CreateDecor(ssys::ma::lyt::ArcResAccReader* arcReader, const char* paneName)
{
    m_pCellPhoneDecor = new (m_pHeap, 4) Object(m_ObjectUnknown, PRIORITY_NO_ANIMATE, m_pHeap);
    m_pCellPhoneDecor->Build(arcReader, "DecorArea_cellphone.bclyt", 0x4000);
    AttachChild(paneName, m_pCellPhoneDecor);
    m_pCellPhoneDecorPane = m_pCellPhoneDecor->GetRootPane()->FindPaneByName("P_decorArea", true);
    for (s32 i = 0; i < GetDecorNum(); i++) {
        m_pRomanDecors[i] = new (m_pHeap, 4) Object(m_ObjectUnknown, PRIORITY_NO_ANIMATE, m_pHeap);
        m_pRomanDecors[i]->Build(arcReader, "DecorArea_roman.bclyt", 0x4000);
        AttachChild(paneName, m_pRomanDecors[i]);
        m_pRomanDecorPanes[i] = m_pRomanDecors[i]->GetRootPane()->FindPaneByName("P_decorArea", true);
    }
    for (s32 i = 0; i < GetDecorNum(); i++) {
        m_pSelectDecors[i] = new (m_pHeap, 4) Object(m_ObjectUnknown, PRIORITY_NO_ANIMATE, m_pHeap);
        m_pSelectDecors[i]->Build(arcReader, "DecorArea_select.bclyt", 0x4000);
        AttachChild(paneName, m_pSelectDecors[i]);
        m_pSelectDecorPanes[i] = m_pSelectDecors[i]->GetRootPane()->FindPaneByName("P_decorArea", true);
    }
    m_pCursor = new (m_pHeap, 4) Object(m_ObjectUnknown, PRIORITY_NO_ANIMATE, m_pHeap);
    m_pCursor->Build(arcReader, "DecorCursor.bclyt", 0x4000);
    AttachChild(paneName, m_pCursor);
    m_pCursorAnim = m_pCursor->CreateAnmObject("DecorCursor_blink.bclan", NULL);
    m_pCursorAnim->SetPlayMode(ut::FrameController::PLAY_MODE_LOOP);
    m_pCursorAnim->Restart();
    m_pCursorPane = m_pCursor->GetRootPane()->FindPaneByName("N_decorCursor", true);
    m_pCursorMSPane = m_pCursor->GetRootPane()->FindPaneByName("P_decorCursorMS", true);
}

// 0x0050ABBC
void lyt::TextArea::LoadInitialText()
{
    const wchar_t* text = NULL;
    s32 length = 0;
    if (sys::g_KeyboardParam.initialTextOffset != -1) {
        // (the offset is in bytes)
        text = reinterpret_cast<const wchar_t*>(reinterpret_cast<const u8*>(sys::g_pInitialText) + sys::g_KeyboardParam.initialTextOffset);
        if (text != NULL) {
            const u16 maxLength = sys::g_KeyboardParam.maxLength;
            const bool isNewlineAllowed = sys::g_KeyboardParam.isNewlineAllowed;
            while (text[length] != 0 && maxLength > length) {
                if (!isNewlineAllowed && text[length] == '\n') {
                    break;
                }
                length++;
            }
        }
    }
    sys::g_pTextManager->SetText(text, length);
}

// 0x0050AC34
bool lyt::TextArea::UpdateLineEnd()
{
    s32 column;
    const sys::TextManager* manager = sys::g_pTextManager;
    const s32 cursor = manager->m_Cursor;
    const bool isLastNewline = manager->m_IsLastNewline;
    const s32 line = sys::GetLineManager()->GetLineOfIndex(cursor, &column);
    bool isLineEnd = m_IsLineEnd;
    if (isLineEnd && !(line > 0 && column == 0 && !isLastNewline)) {
        isLineEnd = false;
    }
    if (m_IsLineEnd != isLineEnd || m_LastCursor != cursor) {
        m_IsLineEnd = isLineEnd;
        m_LastCursor = cursor;
        m_pCursorAnim->Restart();
        m_State = STATE_CHANGED;
    }
    return m_IsLineEnd;
}

// 0x0050ACD8 slot 0x34
void lyt::TextArea::ApplyText(u16 length)
{
    SetText(m_DrawBuffer, m_pTextBox, false, length, 0);
}

// 0x0050AD00
void lyt::TextArea::SetLineEnd(bool isLineEnd)
{
    s32 column;
    const s32 cursor = sys::g_pTextManager->m_Cursor;
    const s32 line = sys::GetLineManager()->GetLineOfIndex(cursor, &column);
    if (isLineEnd && !(line > 0 && column == 0)) {
        isLineEnd = false;
    }
    if (m_IsLineEnd != isLineEnd || m_LastCursor != cursor) {
        m_IsLineEnd = isLineEnd;
        m_LastCursor = cursor;
        m_pCursorAnim->Restart();
        m_State = STATE_CHANGED;
    }
}

// 0x0050AD98
s32 lyt::TextArea::GetCursorLine()
{
    s32 line = sys::GetLineManager()->GetLineOfIndex(sys::g_pTextManager->m_Cursor, NULL);
    if (UpdateLineEnd()) {
        line--;
    }
    return line;
}

// 0x0050ADDC slot 0x14
bool lyt::TextArea::GetIndexAtPosition(s32* index, const nn::math::VEC2* pos)
{
    bool isEnd = false;
    f32 x = 0.0f;
    const nn::math::VEC3 screen(pos->x, pos->y, 0.0f);
    nn::math::VEC3 local;
    nn::math::VEC3Project(&local, &m_InvMtx, &screen);
    ScrollToCursor();

    // the line below the position
    f32 bottom = m_pTextBox->m_FontSize.height;
    s32 line = m_TopLine - GetLineOffset();
    for (s32 i = 0; i < GetLineNum() - 1 && -bottom > local.y; i++) {
        bottom += m_pTextBox->m_FontSize.height + m_pTextBox->m_LineSpace;
        if (sys::GetLineManager()->GetLineStartChecked(line + 1) < 0) {
            break;
        }
        line++;
    }
    if (m_TopLine > line) {
        line = m_TopLine;
    } else if (m_TopLine + GetScrollLineNum() - 1 < line) {
        line = GetScrollLineNum() - 1 + m_TopLine;
    }

    // the character whose middle is right of the position
    const s32 length = sys::GetLineManager()->GetLineLength(line);
    const s32 start = sys::GetLineManager()->GetLineStartChecked(line);
    const wchar_t* text = &sys::g_pTextManager->m_pBuffer[start];
    s32 i = 0;
    for (;;) {
        if (text[i] == '\n' || text[i] == 0) {
            break;
        }
        if (i >= length) {
            isEnd = true;
            break;
        }
        const f32 width = GetCharWidth(sys::g_pTextManager->GetDisplayChar(start + i));
        x += width * 0.5f;
        if (!(local.x > x)) {
            break;
        }
        x += width * 0.5f;
        i++;
    }
    *index = start + i;
    return isEnd;
}

// 0x0050AFDC
void lyt::TextArea::ScrollToCursor()
{
    const s32 line = GetCursorLine();
    if (m_TopLine > line) {
        m_TopLine = line;
    } else if (m_TopLine + GetScrollLineNum() - 1 < line) {
        m_TopLine = line - GetScrollLineNum() + 1;
    }
    // no empty lines at the end while the text can scroll back
    while (m_TopLine > 0 && sys::GetLineManager()->GetLineStartChecked(m_TopLine + GetScrollLineNum() - 1) < 0) {
        m_TopLine--;
    }
}

// 0x0050B0AC slot 0x10
void lyt::TextArea::Setup(ssys::ma::lyt::ArcResAccReader* arcReader, ssys::ma::lyt::ArcResAccReader* decorArcReader)
{
    char name[64];
    std::sprintf(name, "%s.bclyt", GetLayoutName());
    Build(arcReader, name, 0x4000);
    if (decorArcReader != NULL) {
        CreateDecor(decorArcReader, g_DecorPaneName);
    } else {
        CreateDecor(arcReader, g_DecorPaneName);
    }
    m_pTextBox = nw::lyt::DynamicCast<nw::lyt::TextBox>(FindPane("T_textArea"));
    m_pGuideTextBox = nw::lyt::DynamicCast<nw::lyt::TextBox>(FindPane("T_textArea_guide"));
    m_pBaseLines[0] = FindPane("P_baseLine_00");
    m_pBaseLines[1] = FindPane("P_baseLine_01");
    m_pBaseLines[2] = FindPane("P_baseLine_02");
    m_pBaseLines[3] = FindPane("P_baseLine_03");
    m_pBaseLines[4] = FindPane("P_baseLine_04");
    m_pBaseLines[5] = FindPane("P_baseLine_05");

    // the writer measures like the text box; the box gets room for the line feed symbol
    sys::GetLineManager()->Setup(m_pTextBox, 0, false, m_pTextBox->m_Size.width);
    m_Writer.m_pFont = m_pTextBox->m_pFont;
    m_Writer.SetFontSize(m_pTextBox->m_FontSize.width, m_pTextBox->m_FontSize.height);
    m_Writer.m_LineSpace = m_pTextBox->m_LineSpace;
    m_Writer.m_CharSpace = m_pTextBox->m_CharSpace;
    const nw::lyt::Size size = m_pTextBox->m_Size;
    m_pTextBox->m_Size = nw::lyt::Size(size.width + (GetCharWidth(CHAR_LINE_FEED_SYMBOL) + m_Writer.m_CharSpace + 3.0f), size.height);
    m_pTextBox->SetTagProcessor(&m_TagProcessor);
    m_Layout.Animate();

    // from the lower screen into the text box
    nn::math::MTX34 inverse;
    if (nn::math::ARMv6::MTX34InverseAsm(&inverse, &m_pTextBox->m_GlbMtx)) {
        nn::math::MTX44 mtx;
        nn::math::ARMv6::MTX34CopyAsm(reinterpret_cast<nn::math::MTX34*>(&mtx), &inverse);
        mtx.m[3][2] = 0.0f;
        mtx.m[3][1] = 0.0f;
        mtx.m[3][0] = 0.0f;
        mtx.m[3][3] = 1.0f;
        m_InvMtx = mtx;
    } else {
        nn::math::MTX44SetIdentity(&m_InvMtx);
    }

    // the string buffer of the box: the characters that fit (estimated with narrow ones), the line feeds and
    // with line feeds allowed seven more for each line (their symbol and color tags)
    s32 length = static_cast<s32>(std::ceil(sys::GetLineManager()->m_Width / (GetCharWidth(CHAR_NARROW) + m_Writer.m_CharSpace)));
    length *= GetLineNum();
    if (length > sys::g_BufferSize - 1) {
        length = sys::g_BufferSize - 1;
    }
    length += GetLineNum();
    if (sys::g_KeyboardParam.isNewlineAllowed) {
        length += GetLineNum() * 7;
    }
    m_pTextBox->AllocStringBuffer(static_cast<u16>(length), 0);
    m_TextColor = m_pTextBox->m_TextColors[0];
    LoadInitialText();
    SetText(reinterpret_cast<const wchar_t*>(sys::g_KeyboardParam._unknown0x090), m_pGuideTextBox, false, 0, 0);
    sys::g_pTextManager->SetCursor(sys::g_pTextManager->m_Length, true, true);
}

// (inline) the text of the visible lines with the color tags, the cursor and the highlights; into the text box
// (TextArea, TextAreaOneLine and TextAreaBoard have the same code; the board sets m_State at another place)
// (inline) line feeds are allowed until the last line is reached
inline void lyt::TextAreaBoard::UpdateNewlineAllowed(sys::LineManager* lines, s32 cursor)
{
    const bool isLastLine = lines->GetLineOfIndex(cursor, NULL) == lines->m_LineNum - 1;
    sys::g_KeyboardParam.isNewlineAllowed = !(lines->GetLineStartChecked(lines->m_LineNum - 1) >= 0 || isLastLine);
}

// (inline) the header follows the changes of its text before the name; the colors of the name
inline void lyt::TextAreaMail::UpdateHeader(s32 cursor)
{
    if (m_Mode != MODE_HEADER) {
        return;
    }
    UpdateHeaderLength();
    bool isAfterName = false;
    if (m_HeaderCursorPart == HEADER_AFTER_NAME && m_pMail->receiverNameIndent + m_NameLength == cursor) {
        isAfterName = true;
        sys::g_pTextManager->m_Unknown0x90 = cursor;
    }
    sys::g_pTextManager->m_Unknown0x94 = isAfterName;
    // the name has the highlight color
    if (m_pMail->receiverNameIndent != 0) {
        m_ColorBounds[0] = 0;
        m_ColorBounds[1] = m_pMail->receiverNameIndent;
        m_ColorBounds[2] = m_pMail->receiverNameIndent + m_NameLength;
    } else {
        m_ColorBounds[0] = m_pMail->receiverNameIndent + m_NameLength;
    }
}

// (inline) line feeds only in the body, until its last line
inline void lyt::TextAreaMail::UpdateNewlineAllowed(sys::LineManager* lines, s32 cursor)
{
    const bool isLastLine = lines->GetLineOfIndex(cursor, NULL) == lines->m_LineNum - 1;
    const bool isFull = lines->GetLineStartChecked(lines->m_LineNum - 1) >= 0;
    if (m_Mode != MODE_BODY) {
        sys::g_KeyboardParam.isNewlineAllowed = false;
    } else {
        sys::g_KeyboardParam.isNewlineAllowed = !(isFull || isLastLine);
    }
}

// (inline) the x of a decoration: the letter aligns it with its text
DECOMP_ALWAYS_INLINE f32 lyt::TextArea::DecorX(s32 variant, f32 x, f32 maskedX)
{
    if (variant == VARIANT_MAIL) {
        return static_cast<TextAreaMail*>(this)->AdjustX(x, maskedX);
    }
    return x + maskedX;
}

DECOMP_ALWAYS_INLINE void lyt::TextArea::BuildDrawText(s32 cursor, s32 variant)
{
    sys::TextManager* manager = sys::g_pTextManager;
    sys::LineManager* lines = sys::GetLineManager();
    const s32 cursorLine = GetCursorLine();
    const s32 compositionLength = manager->m_CompositionLength;
    const s32 cellPhoneIndex = manager->m_pCellPhoneInput != NULL && manager->m_Cursor > 0 ? manager->m_Cursor - 1 : -1;
    const s32 selectionStart = manager->m_SelectionAnchor <= manager->m_Cursor ? manager->m_SelectionAnchor : manager->m_Cursor;
    const s32 selectionEnd = manager->m_SelectionAnchor <= manager->m_Cursor ? manager->m_Cursor : manager->m_SelectionAnchor;
    s32 selectionLength = 0;
    if (manager->m_IsSelecting) {
        selectionLength = selectionEnd - selectionStart;
    }
    const wchar_t* buffer = manager->m_pBuffer;
    nw::ut::Color8 lastColor = m_TextColor;
    f32 x = 0.0f;
    f32 maskedX = 0.0f;
    s32 column = 0;
    s32 lineLength = 0;
    s32 out = 0;
    s32 row = 0;
    bool isNewLine = true;

    if (variant == VARIANT_BOARD) {
        // the board allows line feeds until its last line and scrolls by lines
        TextAreaBoard* board = static_cast<TextAreaBoard*>(this);
        board->UpdateNewlineAllowed(lines, cursor);
        board->ScrollToCursorPage();
    } else if (variant == VARIANT_MAIL) {
        // the letter follows its header and allows line feeds only in the body
        TextAreaMail* mail = static_cast<TextAreaMail*>(this);
        mail->UpdateHeader(cursor);
        mail->UpdateNewlineAllowed(lines, cursor);
        mail->ScrollToCursorPage();
    } else {
        ScrollToCursor();
    }
    m_pCursor->SetEnabled(false);
    m_pCellPhoneDecor->SetEnabled(false);
    for (s32 i = 0; i < GetDecorNum(); i++) {
        m_pRomanDecors[i]->SetEnabled(false);
        m_pSelectDecors[i]->SetEnabled(false);
    }

    // the lines above the text are empty
    while (m_TopLine - GetLineOffset() + row < 0) {
        m_DrawBuffer[out++] = '\n';
        row++;
    }
    s32 index = lines->GetLineStartChecked(m_TopLine - GetLineOffset() + row);
    while (row < GetLineNum()) {
        if (out >= DRAW_BUFFER_LENGTH - 6) {
            break;
        }
        if (isNewLine) {
            column = 0;
            lineLength = lines->GetLineLength(m_TopLine - GetLineOffset() + row);
            x = 0.0f;
            isNewLine = false;
        }

        // the cursor in the line
        if (index == cursor && !m_pCursor->m_IsEnabled) {
            m_pCursor->SetEnabled(true);
            nw::lyt::Pane* pane = m_pCursorPane;
            const f32 px = std::floor(DecorX(variant, x, maskedX) + 0.5f);
            const f32 py = std::floor(m_LineTop - row * (m_pTextBox->m_FontSize.height + m_pTextBox->m_LineSpace) + 0.5f);
            pane->SetTranslate(nn::math::VEC3(px, py, 0.0f));
            m_pCursorMSPane->SetVisible(false);
        }
        // the romaji being composed
        if (compositionLength > 0) {
            Object* decor = m_pRomanDecors[row];
            const s32 start = manager->m_Cursor - manager->m_CompositionLength;
            if (decor->m_IsEnabled) {
                if (start + compositionLength == index) {
                    nw::lyt::Pane* pane = m_pRomanDecorPanes[row];
                    const f32 height = pane->m_Size.height;
                    pane->m_Size = nw::lyt::Size(std::floor(DecorX(variant, x, maskedX) - m_Writer.m_CharSpace - pane->m_Translate.x + 0.5f), height);
                }
            } else if (start <= index && index < start + compositionLength) {
                decor->SetEnabled(true);
                nw::lyt::Pane* pane = m_pRomanDecorPanes[row];
                const f32 px = std::floor(DecorX(variant, x, maskedX) + 0.5f);
                const f32 py = std::floor(-row * (m_pTextBox->m_FontSize.height + m_pTextBox->m_LineSpace) + 0.5f);
                pane->SetTranslate(nn::math::VEC3(px, py, 0.0f));
                m_pRomanDecorPanes[row]->m_Size = nw::lyt::Size(0.0f, m_pRomanDecorPanes[row]->m_Size.height);
            }
        }
        // the selection
        if (selectionLength > 0) {
            Object* decor = m_pSelectDecors[row];
            if (decor->m_IsEnabled) {
                if (index == selectionEnd) {
                    nw::lyt::Pane* pane = m_pSelectDecorPanes[row];
                    const f32 height = pane->m_Size.height;
                    pane->m_Size = nw::lyt::Size(std::floor(DecorX(variant, x, maskedX) - m_Writer.m_CharSpace - pane->m_Translate.x + 0.5f), height);
                }
            } else if (index >= selectionStart && selectionEnd > index) {
                decor->SetEnabled(true);
                nw::lyt::Pane* pane = m_pSelectDecorPanes[row];
                const f32 px = std::floor(DecorX(variant, x, maskedX) + 0.5f);
                const f32 py = std::floor(-row * (m_pTextBox->m_FontSize.height + m_pTextBox->m_LineSpace) + 0.5f);
                pane->SetTranslate(nn::math::VEC3(px, py, 0.0f));
                m_pSelectDecorPanes[row]->m_Size = nw::lyt::Size(0.0f, m_pSelectDecorPanes[row]->m_Size.height);
            }
        }
        // the character of the multi-tap key
        if (cellPhoneIndex >= 0) {
            if (m_pCellPhoneDecor->m_IsEnabled) {
                if (column > 0 && index == cellPhoneIndex + 1) {
                    nw::lyt::Pane* pane = m_pCellPhoneDecorPane;
                    const f32 height = pane->m_Size.height;
                    pane->m_Size = nw::lyt::Size(std::floor(DecorX(variant, x, maskedX) - m_Writer.m_CharSpace - pane->m_Translate.x + 0.5f), height);
                }
            } else if (index == cellPhoneIndex) {
                m_pCellPhoneDecor->SetEnabled(true);
                nw::lyt::Pane* pane = m_pCellPhoneDecorPane;
                const f32 px = std::floor(DecorX(variant, x, maskedX) + 0.5f);
                const f32 py = std::floor(-row * (m_pTextBox->m_FontSize.height + m_pTextBox->m_LineSpace) + 0.5f);
                pane->SetTranslate(nn::math::VEC3(px, py, 0.0f));
                m_pCellPhoneDecorPane->m_Size.width = 0.0f;
            }
        }

        // the next character of the line
        if (column < lineLength) {
            const u16 c = manager->GetDisplayChar(index);
            if (variant == VARIANT_MAIL) {
                TextAreaMail* mail = static_cast<TextAreaMail*>(this);
                if (mail->m_ColorBounds[mail->m_ColorIndex] == index) {
                    nw::ut::Color8 tagColor = mail->m_ColorIndex == 1 ? mail->m_HighlightColor : m_TextColor;
                    out += WriteColorTag(&m_DrawBuffer[out], DRAW_BUFFER_LENGTH - out, &tagColor);
                    mail->m_ColorIndex++;
                }
            } else {
                nw::ut::Color8 color;
                if (c == '\n') {
                    color = g_LineFeedColor;
                } else {
                    // (the results are unused in the original)
                    if (GetRegionIndex() != 1) {
                        GetRegionIndex();
                    }
                    color = m_TextColor;
                }
                if (color.ToU32() != lastColor.ToU32()) {
                    nw::ut::Color8 tagColor = color;
                    out += WriteColorTag(&m_DrawBuffer[out], DRAW_BUFFER_LENGTH - out, &tagColor);
                    lastColor = color;
                }
            }
            if (c == '\n') {
                m_DrawBuffer[out++] = CHAR_LINE_FEED_SYMBOL;
            } else {
                // a character being composed is not masked
                bool isMaskable = true;
                const s32 composition = manager->m_CompositionLength;
                if (composition > 0) {
                    const s32 start = manager->m_Cursor - composition;
                    if (start <= index && index < start + composition) {
                        isMaskable = false;
                    }
                }
                const s32 extra = manager->m_CompositionExtra;
                if (extra > 0) {
                    const s32 start = manager->m_Cursor - composition - extra;
                    if (start <= index && index < start + extra) {
                        isMaskable = false;
                    }
                }
                m_Writer.m_IsWidthFixed = m_IsMasked && isMaskable;
                const f32 width = GetCharWidth(c);
                m_Writer.m_IsWidthFixed = m_IsMasked;
                m_DrawBuffer[out++] = c;
                if (m_IsMasked && !isMaskable) {
                    maskedX += m_Writer.m_CharSpace + width;
                } else {
                    x += m_Writer.m_CharSpace + width;
                }
            }
            index++;
            column++;
        }

        // the end of the line
        if (column == lineLength) {
            if (!m_IsMasked && manager->m_Length > index) {
                m_DrawBuffer[out++] = '\n';
            }
            Object* romanDecor = m_pRomanDecors[row];
            if (romanDecor->m_IsEnabled) {
                nw::lyt::Pane* pane = m_pRomanDecorPanes[row];
                const f32 height = pane->m_Size.height;
                if (pane->m_Size.width == 0.0f) {
                    pane->m_Size = nw::lyt::Size(std::floor(DecorX(variant, x, maskedX) - m_Writer.m_CharSpace - pane->m_Translate.x + 0.5f), height);
                }
            }
            Object* selectDecor = m_pSelectDecors[row];
            if (selectDecor->m_IsEnabled) {
                nw::lyt::Pane* pane = m_pSelectDecorPanes[row];
                const f32 height = pane->m_Size.height;
                if (pane->m_Size.width == 0.0f) {
                    // a selected line feed covers its symbol
                    const f32 width = DecorX(variant, x, maskedX) - pane->m_Translate.x;
                    f32 w;
                    if (index > 0 && buffer[index - 1] == '\n') {
                        w = GetCharWidth(CHAR_LINE_FEED_SYMBOL) + width;
                    } else {
                        w = width - m_Writer.m_CharSpace;
                    }
                    m_pSelectDecorPanes[row]->m_Size = nw::lyt::Size(std::floor(w + 0.5f), height);
                }
                // (the board and the letter animate while a selection is shown)
                if (variant != VARIANT_TEXT_AREA) {
                    m_State = STATE_CHANGED;
                }
            }
            if (m_pCellPhoneDecor->m_IsEnabled) {
                nw::lyt::Pane* pane = m_pCellPhoneDecorPane;
                const f32 height = pane->m_Size.height;
                if (pane->m_Size.width == 0.0f) {
                    pane->m_Size = nw::lyt::Size(std::floor(DecorX(variant, x, maskedX) - m_Writer.m_CharSpace - pane->m_Translate.x + 0.5f), height);
                }
            }
            // the cursor at the end of the line
            if (m_TopLine - GetLineOffset() + row == cursorLine && !m_pCursor->m_IsEnabled) {
                m_pCursor->SetEnabled(true);
                nw::lyt::Pane* pane = m_pCursorPane;
                const f32 px = std::floor(DecorX(variant, x, maskedX) + 0.5f);
                const f32 py = std::floor(m_LineTop - row * (m_pTextBox->m_FontSize.height + m_pTextBox->m_LineSpace) + 0.5f);
                pane->SetTranslate(nn::math::VEC3(px, py, 0.0f));
                m_pCursorMSPane->SetVisible(false);
            }
            row++;
            if (lines->GetLineStartChecked(m_TopLine - GetLineOffset() + row) < 0) {
                break;
            }
            isNewLine = true;
        }
        if (variant == VARIANT_TEXT_AREA && m_pSelectDecors[row]->m_IsEnabled) {
            m_State = STATE_CHANGED;
        }
    }
    m_DrawBuffer[out] = 0;
    ApplyText(static_cast<u16>(out));

}

// (inline) the cursor blinks again from the start when it moves; the text was set
DECOMP_ALWAYS_INLINE void lyt::TextArea::UpdateCursorAnim(s32 cursor)
{
    if (m_LastCursor != cursor) {
        m_LastCursor = cursor;
        m_pCursorAnim->Restart();
        m_State = STATE_CHANGED;
    }
    if (m_pCursorAnim->m_PlayMode == ut::FrameController::PLAY_MODE_LOOP) {
        if (g_Unknown95F1A4 != 0) {
            m_pCursorAnim->SetPlayMode(CURSOR_PLAY_MODE_UNKNOWN);
        }
    } else if (g_Unknown95F1A4 == 0) {
        m_pCursorAnim->SetPlayMode(ut::FrameController::PLAY_MODE_LOOP);
    }
    if (sys::g_pTextManager->m_IsTextSet == true) {
        m_State = STATE_CHANGED;
        sys::g_pTextManager->m_IsTextSet = false;
    }
}

// 0x0050B56C slot 0x08
void lyt::TextArea::Calc(bool isAnimate)
{
    const s32 cursor = sys::g_pTextManager->m_Cursor;
    BuildDrawText(cursor, VARIANT_TEXT_AREA);
    // the guide while the text is empty
    if (m_pGuideTextBox != NULL && sys::g_KeyboardParam._unknown0x090[0] != 0) {
        const bool isEmpty = sys::g_pTextManager->m_Length == 0;
        m_pTextBox->SetVisible(!isEmpty);
        m_pGuideTextBox->SetVisible(isEmpty);
        m_pCursor->SetEnabled(!isEmpty);
    }
    // the base lines: the last line of a page has its own
    for (s32 i = 0; i < GetLineNum() - 1; i++) {
        if (m_pBaseLines[i * 2] != NULL && m_pBaseLines[i * 2 + 1] != NULL) {
            const s32 line = m_TopLine - GetLineOffset() + i;
            const bool isVisible = line >= 0;
            if (line % GetLineNum() == GetLineNum() - 1) {
                SetFlagVisible(m_pBaseLines[i * 2], isVisible);
                SetFlagVisible(m_pBaseLines[i * 2 + 1], false);
            } else {
                SetFlagVisible(m_pBaseLines[i * 2], false);
                SetFlagVisible(m_pBaseLines[i * 2 + 1], isVisible);
            }
        }
    }
    UpdateCursorAnim(cursor);
    Object::Calc(isAnimate);
}

// 0x00507030 slot 0x08
void lyt::TextAreaOneLine::Calc(bool isAnimate)
{
    const s32 cursor = sys::g_pTextManager->m_Cursor;
    BuildDrawText(cursor, VARIANT_TEXT_AREA);
    UpdateCursorAnim(cursor);
    Object::Calc(isAnimate);
}

// 0x00505D50 slot 0x08
void lyt::TextAreaBoard::Calc(bool isAnimate)
{
    // while the line pane scrolls, only it moves
    if (!m_PageMover.Step()) {
        m_pPagePane->SetTranslate(m_PageMover.m_Position);
        m_State = STATE_CHANGED;
        Object::Calc(isAnimate);
        return;
    }
    if (m_IsScrollingBack == true) {
        SetIndicatorVisible(true);
        vf_0x58(false, true);
        m_IsScrollingBack = false;
    }
    if (m_Page != m_TargetPage) {
        m_Page = m_TargetPage;
        m_IsScrolling = false;
        if (!m_Unknown0x73A) {
            SetIndicatorVisible(true);
            vf_0x58(false, true);
            g_Unknown95F1A4 = 0;
        }
    }
    const s32 cursor = sys::g_pTextManager->m_Cursor;
    BuildDrawText(cursor, VARIANT_BOARD);
    UpdateCursorAnim(cursor);
    Object::Calc(isAnimate);
}

// 0x00503918 slot 0x08
void lyt::TextAreaMail::Calc(bool isAnimate)
{
    sys::LineManager* lines = sys::GetLineManager();
    // while the page pane scrolls, only it moves
    if (!m_PageMover.Step()) {
        m_pPagePane->SetTranslate(m_PageMover.m_Position);
        m_State = STATE_CHANGED;
        Object::Calc(isAnimate);
        return;
    }
    if (m_IsScrollingBack == true) {
        SetIndicatorVisible(true);
        vf_0x58(false, false);
        m_IsScrollingBack = false;
    }
    if (m_Page != m_TargetPage) {
        m_Page = m_TargetPage;
        m_IsScrolling = false;
        if (!m_Unknown0x73A) {
            SetIndicatorVisible(true);
            vf_0x58(false, false);
            g_Unknown95F1A4 = 0;
        }
    }
    m_ColorIndex = 0;
    for (s32 i = 0; i < 4; i++) {
        m_ColorBounds[i] = -1;
    }

    // the edited part follows the page: page 0 is the header, 7 the signature, the others the body
    if (m_Page == 0) {
        if (m_Mode != MODE_HEADER) {
            m_pTextBox->m_TextColors[0] = m_pTextBoxes[0]->m_TextColors[0];
            m_pTextBox->m_TextColors[1] = m_pTextBoxes[0]->m_TextColors[1];
            StoreCurrentPart();
            sys::g_LineNum = 1;
            lines->m_LineNum = 1;
            m_pTextBox->SetTextPositionH(0);
            script::WordPtr header = Func5CCFD4(m_pMail);
            Func759DFC(m_pMail, &m_Name);
            m_NameLength = m_Name.GetLength();
            const u8 indent = m_pMail->receiverNameIndent;
            // (the name of the receiver starts with a capital letter in some regions)
            const u32 region = GetRegionIndex();
            if (region == 1 || (region == 2 && (Func5D51A8() == 4 || indent == 0))) {
                script::ToUpperFirst(&m_Name);
            }
            // the header starts where the name ends
            f32 nameWidth = 0.0f;
            const char16* name = m_Name.GetWord();
            for (u32 i = 0; i < static_cast<u32>(m_NameLength); i++) {
                nameWidth += lines->GetCharWidth(name[i]);
            }
            lines->m_StartX = 76.8f - nameWidth;
            const s32 maxLength = 32 - static_cast<u32>(8.0f - m_NameLength);
            sys::g_KeyboardParam.maxLength = maxLength;
            sys::g_pTextManager->m_MaxLength = maxLength;
            // the text of the header with the name in it
            sead::WFixedSafeString<53> text;
            text.copy(sead::SafeStringBase<wchar_t>(reinterpret_cast<const wchar_t*>(header.GetWord())), m_pMail->receiverNameIndent);
            text.overwriteAt(-1, sead::SafeStringBase<wchar_t>(reinterpret_cast<const wchar_t*>(m_Name.GetWord())), m_NameLength);
            text.overwriteAt(-1, sead::SafeStringBase<wchar_t>(reinterpret_cast<const wchar_t*>(header.GetWord()) + m_pMail->receiverNameIndent), -1);
            script::WordPtr buffer(reinterpret_cast<char16*>(sys::g_pTextBuffer), 53);
            buffer.Copy(script::WordCPtr(reinterpret_cast<const char16*>(text.getCString()), 53));
            sys::g_pTextManager->SetText(sys::g_pTextBuffer, -1);
            m_LastLength = sys::g_pTextManager->m_Length;
            m_TopLine = 0;
            m_LastCursor = 0;
            sys::g_pTextManager->SetCursor(0, true, true);
            m_HeaderCursorPart = HEADER_BEFORE_NAME;
            m_Mode = MODE_HEADER;
            UpdateIndicator();
            sys::g_pTextManager->m_Unknown0x94 = false;
            sys::g_pTextManager->m_Unknown0x98 = 0;
        }
    } else if (m_Page == TEXT_BOX_NUM - 1) {
        if (m_Mode != MODE_FOOTER) {
            m_pTextBox->m_TextColors[0] = m_pTextBoxes[TEXT_BOX_NUM - 1]->m_TextColors[0];
            m_pTextBox->m_TextColors[1] = m_pTextBoxes[TEXT_BOX_NUM - 1]->m_TextColors[1];
            StoreCurrentPart();
            sys::g_KeyboardParam.maxLength = 32;
            sys::g_pTextManager->m_MaxLength = 32;
            sys::g_LineNum = 1;
            lines->m_LineNum = 1;
            // the signature is right-aligned
            m_pTextBox->SetTextPositionH(2);
            lines->m_StartX = 0.0f;
            script::WordPtr footer = Func5CCFB4(m_pMail);
            script::WordPtr buffer(reinterpret_cast<char16*>(sys::g_pTextBuffer), 56);
            buffer.Copy(footer);
            sys::g_pTextManager->SetText(sys::g_pTextBuffer, -1);
            m_TopLine = 0;
            m_LastCursor = 0;
            sys::g_pTextManager->SetCursor(sys::g_pTextManager->m_Length, true, true);
            m_Mode = MODE_FOOTER;
            UpdateIndicator();
            sys::g_pTextManager->m_Unknown0x94 = false;
            sys::g_pTextManager->m_Unknown0x98 = 0;
        }
    } else if (static_cast<u32>(m_Page - 1) < TEXT_BOX_NUM - 2) {
        if (m_Mode != MODE_BODY) {
            m_pTextBox->m_TextColors[0] = m_pTextBoxes[1]->m_TextColors[0];
            m_pTextBox->m_TextColors[1] = m_pTextBoxes[1]->m_TextColors[1];
            StoreCurrentPart();
            sys::g_KeyboardParam.maxLength = 192;
            sys::g_pTextManager->m_MaxLength = 192;
            sys::g_LineNum = 6;
            lines->m_LineNum = 6;
            m_pTextBox->SetTextPositionH(0);
            lines->m_StartX = 0.0f;
            script::WordPtr buffer(reinterpret_cast<char16*>(sys::g_pTextBuffer), 197);
            buffer.Copy(Func5CCF84(m_pMail));
            sys::g_pTextManager->SetText(sys::g_pTextBuffer, -1);
            if (m_Mode != MODE_HEADER) {
                m_TopLine = m_KeptTop;
            } else {
                m_TopLine = 0;
                m_LastCursor = 0;
            }
            sys::g_pTextManager->SetCursor(sys::g_pTextManager->m_Length, true, true);
            m_Mode = MODE_BODY;
            UpdateIndicator();
            sys::g_pTextManager->m_Unknown0x94 = false;
            sys::g_pTextManager->m_Unknown0x98 = 1;
        }
    }
    const s32 cursor = sys::g_pTextManager->m_Cursor;
    BuildDrawText(cursor, VARIANT_MAIL);
    UpdateCursorAnim(cursor);
    Object::Calc(isAnimate);
}

// 0x0050C33C
lyt::TextArea::TextArea(u32 unknown, s32 priority, sead::Heap* heap)
    : Object(unknown, priority, heap), m_pTextBox(NULL), m_pGuideTextBox(NULL), m_pCursor(NULL), m_pCursorAnim(NULL), m_pCursorPane(NULL),
      m_pCursorMSPane(NULL), m_LineTop(0.0f), m_pCellPhoneDecor(NULL), m_pCellPhoneDecorPane(NULL), m_TopLine(0), m_LastCursor(0),
      m_ObjectUnknown(unknown), m_IsMasked(false), m_IsLineEnd(false), m_TextColor(255, 255, 255, 255), m_pTextAreaGroup(NULL)
{
    for (s32 i = 0; i < 4; i++) {
        m_pRomanDecors[i] = NULL;
        m_pRomanDecorPanes[i] = NULL;
    }
    for (s32 i = 0; i < 4; i++) {
        m_pSelectDecors[i] = NULL;
        m_pSelectDecorPanes[i] = NULL;
    }
    std::memset(m_DrawBuffer, 0, sizeof(m_DrawBuffer));
    for (s32 i = 0; i < 8; i++) {
        m_pBaseLines[i] = NULL;
    }
}

// 0x0050C450 slot 0x00
// 0x0050C43C (deleting dtor)
lyt::TextArea::~TextArea()
{
    if (m_pCellPhoneDecor != NULL) {
        delete m_pCellPhoneDecor;
        m_pCellPhoneDecor = NULL;
    }
    for (s32 i = 0; i < 4; i++) {
        if (m_pSelectDecors[i] != NULL) {
            delete m_pSelectDecors[i];
            m_pSelectDecors[i] = NULL;
        }
    }
    for (s32 i = 0; i < 4; i++) {
        if (m_pRomanDecors[i] != NULL) {
            delete m_pRomanDecors[i];
            m_pRomanDecors[i] = NULL;
        }
    }
    if (m_pCursor != NULL) {
        delete m_pCursor;
        m_pCursor = NULL;
    }
}
