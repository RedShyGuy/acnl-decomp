#include "lyt/dTextAreaBoard.h"

#include <cstdio>

#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "nn/math/math_ARMv6.h"
#include "nn/math/math_Api.h"
#include "nw/lyt/lyt_TextBox.h"
#include "sys/dSwkbd.h"

namespace lyt {
// 0x009506D4
u8* g_pUnknown9506D4 = NULL;
} // namespace lyt

// 0x0095F200
const char* lyt::TextAreaBoard::s_TextBoxNames[TEXT_BOX_NUM] = {
    "T_Hnbn_00", "T_Hnbn_01", "T_Hnbn_02", "T_Hnbn_03", "T_Hnbn_04", "T_Hnbn_05", "T_Footer_00", "T_date_00", "T_page_L_00", "T_page_R_00",
};

namespace {
// the line boxes are wider than the formatted lines by this
const f32 LINE_MARGIN = 30.0f;
// the most messages counted for the page number
const s32 MESSAGE_NUM_MAX = 14;
// the longest text kept while the keyboard is open
const s32 TEXT_LENGTH_MAX = 192;
} // namespace

// 0x005050BC
void lyt::TextAreaBoard::SetupTextBoxes()
{
    for (s32 i = 0; i < TEXT_BOX_NUM; i++) {
        m_pTextBoxes[i] = m_Layout.FindTextBox(s_TextBoxNames[i]);
        m_pTextBoxes[i]->m_Flag &= ~nw::lyt::Pane::FLAG_VISIBLE;
    }
    m_LineWidth = m_pTextBoxes[0]->m_Size.width;
    for (s32 i = 0; i < TEXT_BOX_NUM; i++) {
        m_Lines[i].SetTextBoxes(m_pTextBoxes[i], NULL);
    }
    m_pMessage = g_pUnknown9506D4 + 0x434;
    FormatLines(true);
    for (s32 i = LINE_NUM; i < TEXT_BOX_NUM; i++) {
        m_pTextBoxes[i]->m_Flag = (m_pTextBoxes[i]->m_Flag & ~nw::lyt::Pane::FLAG_VISIBLE) | nw::lyt::Pane::FLAG_VISIBLE;
    }
    m_PageMover.Stop();
}

// 0x0050526C slot 0x44
void lyt::TextAreaBoard::vf_0x44()
{
    s32 top = m_TopLine;
    if (top <= 0) {
        top = 0;
    }
    m_TopLine = top;
    m_Page = top;
    m_TargetPage = top;
    nn::math::VEC3 pos = m_pPagePane->m_Translate;
    pos.y = m_PageTop + m_PageHeight * m_Page;
    m_pPagePane->SetTranslate(pos);
}

// 0x005052DC
void lyt::TextAreaBoard::FormatLines(bool isEditing)
{
    s32 messageNum = Func118B88(GetGardenData()->townData.unk_0x1AE94.bBoardMessages);
    if (messageNum > MESSAGE_NUM_MAX) {
        messageNum = MESSAGE_NUM_MAX;
    }
    for (s32 i = 0; i < LINE_NUM; i++) {
        m_pTextBoxes[i]->m_Size = nw::lyt::Size(m_LineWidth, m_pTextBoxes[i]->m_Size.height);
    }
    m_Text.Format(m_pMessage, m_pTextBoxes, messageNum + 1, messageNum + 1, isEditing);
    for (s32 i = 0; i < TEXT_BOX_NUM; i++) {
        m_Lines[i].SetText(m_Text.GetLine(i));
        m_Lines[i].CommitText();
    }
    for (s32 i = 0; i < LINE_NUM; i++) {
        m_pTextBoxes[i]->m_Size = nw::lyt::Size(m_LineWidth + LINE_MARGIN, m_pTextBoxes[i]->m_Size.height);
    }
}

// 0x0050554C slot 0x48
void lyt::TextAreaBoard::vf_0x48()
{
    s32 top = m_TopLine;
    if (top > 4) {
        top = 4;
    }
    m_TopLine = top;
    m_Page = top;
    m_TargetPage = top;
    nn::math::VEC3 pos = m_pPagePane->m_Translate;
    pos.y = m_PageTop + m_PageHeight * m_Page;
    m_pPagePane->SetTranslate(pos);
}

// 0x005055BC slot 0x58
void lyt::TextAreaBoard::vf_0x58(bool isShown, bool isEditing)
{
    if (isShown) {
        StoreText();
        FormatLines(isEditing);
    }
    for (s32 i = 0; i < LINE_NUM; i++) {
        m_pTextBoxes[i]->m_Flag = (m_pTextBoxes[i]->m_Flag & ~nw::lyt::Pane::FLAG_VISIBLE) | isShown;
    }
}

// 0x0050569C slot 0x14
bool lyt::TextAreaBoard::GetIndexAtPosition(s32* index, const nn::math::VEC2* pos)
{
    bool isEnd = false;
    f32 x = 0.0f;
    const nn::math::VEC3 screen(pos->x, pos->y, 0.0f);
    nn::math::VEC3 local;
    nn::math::VEC3Project(&local, &m_InvMtx, &screen);
    ScrollToCursorPage();

    // the line below the position
    f32 bottom = m_pTextBox->m_FontSize.height;
    s32 line = m_TopLine - GetLineOffset();
    for (s32 i = 0; i < GetLineNum() && -bottom > local.y; i++) {
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

// 0x00505894
void lyt::TextAreaBoard::ScrollToCursorPage()
{
    if (m_IsScrolling == true) {
        return;
    }
    const s32 line = GetCursorLine();
    s32 page = m_Page;
    if (m_TopLine > line) {
        page = line;
        m_TopLine = line;
    } else {
        const s32 top = m_TopLine;
        if (top + GetScrollLineNum() - 1 < line) {
            m_TopLine = line - GetScrollLineNum() + 1;
            page = m_Page + (m_TopLine - top);
        }
    }
    while (m_TopLine > 0 && sys::GetLineManager()->GetLineStartChecked(m_TopLine + GetScrollLineNum() - 1) < 0) {
        page--;
        m_TopLine--;
    }
    if (m_Page != page) {
        ScrollToPage(page);
    }
}

// 0x005059B0 slot 0x10
void lyt::TextAreaBoard::Setup(ssys::ma::lyt::ArcResAccReader* arcReader, ssys::ma::lyt::ArcResAccReader* decorArcReader)
{
    char name[64];
    std::sprintf(name, "%s.bclyt", GetLayoutName());
    Build(arcReader, name, 0x7000);
    if (decorArcReader != NULL) {
        CreateDecor(decorArcReader, "N_decor_E");
    } else {
        CreateDecor(arcReader, "N_decor_E");
    }
    m_pPagePane = FindPane("N_set");
    m_pIndicatorPane = FindPane("N_ltr_edit");
    m_pTextBox = nw::lyt::DynamicCast<nw::lyt::TextBox>(FindPane("T_edit_00"));
    m_pGuideTextBox = NULL;
    SetupTextBoxes();

    // the edited line
    const nw::lyt::Size size = m_pTextBox->m_Size;
    m_pTextBox->m_Size = nw::lyt::Size(size.width + LINE_MARGIN, size.height);
    sys::GetLineManager()->Setup(m_pTextBox, TEXT_LENGTH_MAX, false, m_pTextBox->m_Size.width - LINE_MARGIN);
    m_Writer.m_pFont = m_pTextBox->m_pFont;
    m_Writer.SetFontSize(m_pTextBox->m_FontSize.width, m_pTextBox->m_FontSize.height);
    m_Writer.m_LineSpace = m_pTextBox->m_LineSpace;
    m_Writer.m_CharSpace = m_pTextBox->m_CharSpace;
    // a page is a line
    const f32 lineHeight = m_pTextBox->m_FontSize.height + m_pTextBox->m_LineSpace;
    m_PageHeight = lineHeight;
    m_PageTop = lineHeight;
    m_Page = 0;
    m_TargetPage = 0;
    SetPagePosition(0);
    m_pIndicatorPane->SetTranslate(nn::math::VEC3(m_pIndicatorPane->m_Translate.x, m_PageTop, m_pIndicatorPane->m_Translate.z));
    m_pTextBox->m_Size.width += 3.0f;
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
    // (the string buffer of the box is not set up here; the original still measures for it)
    GetCharWidth('i');
    GetLineNum();
    m_TextColor = m_pTextBox->m_TextColors[0];
    LoadInitialText();
    if (m_pGuideTextBox != NULL) {
        SetText(reinterpret_cast<const wchar_t*>(sys::g_KeyboardParam._unknown0x090), m_pGuideTextBox, false, 0, 0);
    }
    sys::g_pTextManager->SetCursor(0, true, true);
    sys::g_pTextManager->m_Unknown0x98 = 1;
}

// 0x00506AA8
void lyt::TextAreaBoard::StoreText()
{
    sys::TextManager* manager = sys::g_pTextManager;
    if (manager->m_Length > TEXT_LENGTH_MAX) {
        // the composition ends; the selection is gone
        manager->EndCellPhone(true);
        manager->Commit(false);
        manager->m_IsSelecting = false;
        manager->m_SelectionAnchor = manager->m_Cursor;
        manager->m_Unknown0x90 = -1;
    }
    script::WordPtr text(reinterpret_cast<char16*>(sys::g_pTextBuffer), 201);
    script::WordPtr message = Func1187AA(m_pMessage);
    message.Copy(text);
}

// 0x00506BCC slot 0x00
// 0x00506B48 (deleting dtor)
lyt::TextAreaBoard::~TextAreaBoard()
{
    // (only the members are destroyed)
}

