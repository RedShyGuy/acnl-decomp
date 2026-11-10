#include "lyt/dTextAreaMail.h"

#include <cstdio>
#include <cstring>

#include "Sv/dSvMail.h"
#include "lyt/dAnmObject.h"
#include "nn/gx/gx_Api.h"
#include "nn/math/math_ARMv6.h"
#include "nn/math/math_Api.h"
#include "nw/lyt/lyt_Bounding.h"
#include "nw/lyt/lyt_Material.h"
#include "nw/lyt/lyt_Picture.h"
#include "nw/lyt/lyt_TextBox.h"
#include "script/dWordCPtr.h"
#include "sead/seadWFixedSafeString.h"
#include "ssys/ma/dAllocator.h"
#include "ssys/ma/lyt/lyt_Api.h"
#include "sys/dSwkbd.h"

namespace lyt {
// 0x00950BD0
u8* g_pUnknown950BD0 = NULL;
} // namespace lyt

// 0x0095F1E0
const char* lyt::TextAreaMail::s_TextBoxNames[TEXT_BOX_NUM] = {
    "T_Hedder_00", "T_Hnbn_00", "T_Hnbn_01", "T_Hnbn_02", "T_Hnbn_03", "T_Hnbn_04", "T_Hnbn_05", "T_Footer_00",
};

namespace {
// the text box of the edited part is wider than its lines by this
const f32 EDIT_MARGIN = 30.0f;
// the body lines are wider than the formatted lines by this
const f32 LINE_MARGIN = 16.0f;
// the longest body; the lengths of the words of the parts
const s32 BODY_LENGTH_MAX = 192;
const u32 HEADER_WORD_SIZE = 53;
const u32 BODY_WORD_SIZE = 197;
const u32 FOOTER_WORD_SIZE = 56;
// the text position of a text box: horizontal part (NW lyt: 0 left, 1 center, 2 right)
const u8 TEXT_POSITION_H_RIGHT = 2;

// (inline) the input of the keyboard ends (the composition is committed)
inline void EndInput(sys::TextManager* manager)
{
    manager->EndCellPhone(true);
    manager->Commit(false);
}
} // namespace

// 0x00501F04 slot 0x44
void lyt::TextAreaMail::vf_0x44()
{
    const s32 page = m_TopLine + 1;
    m_Page = page;
    m_TargetPage = page;
    nn::math::VEC3 pos = m_pPagePane->m_Translate;
    pos.y = m_PageTop + m_PageHeight * m_Page;
    m_pPagePane->SetTranslate(pos);
}

// 0x00501F6C
f32 lyt::TextAreaMail::AdjustX(f32 x, f32 offset)
{
    const f32 pos = x + offset;
    if (m_pTextBox->m_TextPosition % 3 != TEXT_POSITION_H_RIGHT) {
        return pos;
    }
    f32 width = 0.0f;
    const wchar_t* text = sys::g_pTextManager->m_pBuffer;
    for (s32 i = 0; text[i] != '\n' && text[i] != 0; i++) {
        width += GetCharWidth(sys::g_pTextManager->GetDisplayChar(i));
    }
    return m_pTextBox->m_Size.width - width + pos;
}

// 0x00502018
void lyt::TextAreaMail::StoreText(bool isEditing)
{
    sys::TextManager* manager = sys::g_pTextManager;
    switch (m_Mode) {
    case MODE_HEADER: {
        EndInput(manager);
        UpdateHeaderLength();
        // the header without the name
        sead::WFixedSafeString<HEADER_WORD_SIZE> header;
        header.copy(sead::SafeStringBase<wchar_t>(sys::g_pTextBuffer), m_pMail->receiverNameIndent);
        header.overwriteAt(-1, sead::SafeStringBase<wchar_t>(sys::g_pTextBuffer + m_pMail->receiverNameIndent + m_NameLength), -1);
        const script::WordCPtr word(reinterpret_cast<const char16*>(header.getCString()), HEADER_WORD_SIZE);
        Func5CCFD4(m_pMail).Copy(word);
        break;
    }
    case MODE_BODY: {
        if (manager->m_Length > BODY_LENGTH_MAX) {
            EndInput(manager);
            manager->m_IsSelecting = false;
            manager->m_SelectionAnchor = manager->m_Cursor;
            manager->m_Unknown0x90 = -1;
        }
        const script::WordPtr text(reinterpret_cast<char16*>(sys::g_pTextBuffer), BODY_WORD_SIZE);
        Func5CCF84(m_pMail).Copy(text);
        break;
    }
    case MODE_FOOTER: {
        EndInput(manager);
        const script::WordPtr text(reinterpret_cast<char16*>(sys::g_pTextBuffer), FOOTER_WORD_SIZE);
        Func5CCFB4(m_pMail).Copy(text);
        break;
    }
    default:
        break;
    }
    FormatLines(isEditing);
}

// 0x00502350
void lyt::TextAreaMail::FormatLines(bool isEditing)
{
    for (s32 i = 1; i < TEXT_BOX_NUM - 1; i++) {
        m_pTextBoxes[i]->m_Size = nw::lyt::Size(m_LineWidth, m_pTextBoxes[i]->m_Size.height);
    }
    m_Text.Format(m_pMail, m_pTextBoxes, 1, isEditing);
    for (s32 i = 0; i < TEXT_BOX_NUM; i++) {
        m_Lines[i].SetText(m_Text.GetLine(i));
        m_Lines[i].CommitText();
    }
    for (s32 i = 1; i < TEXT_BOX_NUM - 1; i++) {
        m_pTextBoxes[i]->m_Size = nw::lyt::Size(m_LineWidth + LINE_MARGIN, m_pTextBoxes[i]->m_Size.height);
    }
}

// 0x00502538 slot 0x48
void lyt::TextAreaMail::vf_0x48()
{
    const s32 page = m_TopLine + 1;
    m_Page = page;
    m_TargetPage = page;
    nn::math::VEC3 pos = m_pPagePane->m_Translate;
    pos.y = m_PageTop + m_PageHeight * m_Page;
    m_pPagePane->SetTranslate(pos);
}

// 0x005025A0 slot 0x58
void lyt::TextAreaMail::vf_0x58(bool isShown, bool isEditing)
{
    if (isShown) {
        StoreText(isEditing);
        SetPartsVisible(isShown, isShown, isShown);
        return;
    }
    SetPartsVisible(m_Mode != MODE_HEADER, m_Mode != MODE_BODY, m_Mode != MODE_FOOTER);
}

// 0x0050260C
void lyt::TextAreaMail::SetPartsVisible(bool isHeader, bool isBody, bool isFooter)
{
    m_pTextBoxes[0]->m_Flag = (m_pTextBoxes[0]->m_Flag & ~nw::lyt::Pane::FLAG_VISIBLE) | isHeader;
    for (s32 i = 1; i < TEXT_BOX_NUM - 1; i++) {
        m_pTextBoxes[i]->m_Flag = (m_pTextBoxes[i]->m_Flag & ~nw::lyt::Pane::FLAG_VISIBLE) | isBody;
    }
    m_pTextBoxes[TEXT_BOX_NUM - 1]->m_Flag = (m_pTextBoxes[TEXT_BOX_NUM - 1]->m_Flag & ~nw::lyt::Pane::FLAG_VISIBLE) | isFooter;
}

// 0x005026B8
bool lyt::TextAreaMail::CheckText()
{
    if (!m_TextCheck.IsDone()) {
        return false;
    }
    if (m_TextCheck.m_Result >= 3) {
        Func5CCF74(m_pMail);
    } else {
        Func5CCFA4(m_pMail);
    }
    return true;
}

// 0x00502704
void lyt::TextAreaMail::StartTextCheck()
{
    static_cast<script::IWord&>(m_Body).Copy(Func5CCF84(m_pMail));
    m_TextCheck.Start(&m_Body);
}

// 0x00502754
void lyt::TextAreaMail::StoreCurrentPart()
{
    switch (m_Mode) {
    case MODE_HEADER: {
        sead::WFixedSafeString<HEADER_WORD_SIZE> header;
        header.copy(sead::SafeStringBase<wchar_t>(sys::g_pTextBuffer), m_pMail->receiverNameIndent);
        header.overwriteAt(-1, sead::SafeStringBase<wchar_t>(sys::g_pTextBuffer + m_pMail->receiverNameIndent + m_NameLength), -1);
        const script::WordCPtr word(reinterpret_cast<const char16*>(header.getCString()), HEADER_WORD_SIZE);
        Func5CCFD4(m_pMail).Copy(word);
        break;
    }
    case MODE_BODY: {
        const script::WordPtr text(reinterpret_cast<char16*>(sys::g_pTextBuffer), BODY_WORD_SIZE);
        Func5CCF84(m_pMail).Copy(text);
        break;
    }
    default:
        break;
    }
    FormatLines(true);
}

// 0x00502968
void lyt::TextAreaMail::UpdateIndicator()
{
    nn::math::VEC3 pos = m_pIndicatorPane->m_Translate;
    pos.x = 0.0f;
    pos.y = m_PageHeight;
    switch (m_Mode) {
    case MODE_HEADER:
        pos.y += 3.0f;
        SetPartsVisible(false, true, true);
        break;
    case MODE_BODY:
        SetPartsVisible(true, false, true);
        break;
    case MODE_FOOTER:
        pos.y -= 3.0f;
        pos.x = -33.0f;
        SetPartsVisible(true, true, false);
        break;
    default:
        break;
    }
    m_pIndicatorPane->SetTranslate(pos);
}

// 0x00502A50
void lyt::TextAreaMail::SetupTextBoxes()
{
    SetupPaper();
    for (s32 i = 0; i < TEXT_BOX_NUM; i++) {
        m_pTextBoxes[i] = m_Layout.FindTextBox(s_TextBoxNames[i]);
        m_pTextBoxes[i]->m_Flag &= ~nw::lyt::Pane::FLAG_VISIBLE;
    }
    for (s32 i = 0; i < TEXT_BOX_NUM; i++) {
        m_Lines[i].SetTextBoxes(m_pTextBoxes[i], NULL);
    }
    m_LineWidth = m_pTextBoxes[0]->m_Size.width;
    FormatLines(true);
    m_pTextBoxes[0]->m_Flag = (m_pTextBoxes[0]->m_Flag & ~nw::lyt::Pane::FLAG_VISIBLE) | nw::lyt::Pane::FLAG_VISIBLE;
    m_pTextBoxes[TEXT_BOX_NUM - 1]->m_Flag =
        (m_pTextBoxes[TEXT_BOX_NUM - 1]->m_Flag & ~nw::lyt::Pane::FLAG_VISIBLE) | nw::lyt::Pane::FLAG_VISIBLE;
}

namespace {
// (inline) the image of the texture
inline nw::gfx::res::ResImageData* GetImage(nw::gfx::res::ResImageTexture& texture)
{
    return texture.ptr()->image.To<nw::gfx::res::ResImageData>();
}

// (inline) the picture shows the texture of the letter paper
inline void SetPaperTexture(nw::lyt::Picture* picture, nw::gfx::res::ResImageTexture& texture, u32 format)
{
    nw::lyt::Material* material = picture->GetMaterial(0);
    nw::lyt::TexMap texMap = *material->GetTexMapAry();
    texMap.m_Size = nw::lyt::TexSize(texture.ptr()->width, texture.ptr()->height);
    texMap.Update();
    texMap.m_RealSize = nw::lyt::TexSize(texture.ptr()->width, texture.ptr()->height);
    texMap.Update();
    texMap.m_Bits.format = format;
    texMap.Update();
    const nw::gfx::res::ResImageData* image = GetImage(texture);
    const void* address = image->pLocationAddress != NULL ? image->pLocationAddress : image->data.ToPtr();
    texMap.m_PhysicalAddress = nngxGetPhysicalAddr(reinterpret_cast<uptr>(address));
    *material->GetTexMapAry() = texMap;
    material->SetCacheDirty();
}
} // namespace

// 0x00502B84
void lyt::TextAreaMail::SetupPaper()
{
    m_pMail = reinterpret_cast<SvMail*>(g_pUnknown950BD0 + 0x118);
    m_PaperKind = Func75910C() ? 42 : 41;
    const u8 paperId = m_pMail->paperId;
    ssys::ma::Allocator* allocator = ssys::ma::lyt::GetDeviceAllocator();
    sead::Heap* heap = ssys::ma::lyt::GetDeviceAllocator()->m_pHeap;
    SvFgName paper = Func6BC8A4(m_PaperKind, paperId);
    m_PaperLoader.Load(&paper, heap, 0, 0, 0, true, 0);
    const void* image = m_PaperLoader.GetTextureImage(0);
    const u8 format = m_PaperLoader.GetTextureFormat(0);

    // the paper in a texture of 256 x 256 of its own
    nw::gfx::res::ResImageTexture::DynamicDescription description;
    description.height = 256;
    description.width = 256;
    description.mipmapLevels = 1;
    description.locationFlag = 0x2020000;
    description.pLocationAddress = NULL;
    description.format = format;
    description.isCleared = false;
    description.isAllocated = true;
    const u32 texMapFormat = format == 2 ? 7 : 8;
    m_PaperTexture = nw::gfx::res::ResImageTexture::CreateDynamic(description, allocator);
    std::memmove(GetImage(m_PaperTexture)->data.ToPtr(), image, 0x20000);
    SetPaperTexture(m_Layout.FindPicture("P_ltrBase_00"), m_PaperTexture, texMapFormat);
    SetPaperTexture(m_Layout.FindPicture("P_ltrBaseSdw_00"), m_PaperTexture, texMapFormat);

    // the color of the lines after the paper
    const f32 frame = static_cast<f32>(Func76BF8C(&paper));
    m_pPaperAnim = CreateAnmObject("ltr_base_color.bclan", "G_color");
    m_pPaperAnim->ApplyFrame(frame - 1.0f);
}

// 0x00502F38 slot 0x38
void lyt::TextAreaMail::ClampCursor(s32* index)
{
    if (m_Mode != MODE_HEADER) {
        return;
    }
    // the cursor stays off the name of the receiver
    s32 pos = *index;
    const s32 anchor = sys::g_pTextManager->m_SelectionAnchor;
    if (m_pMail->receiverNameIndent + m_NameLength > anchor) {
        if (pos > m_pMail->receiverNameIndent) {
            pos = m_pMail->receiverNameIndent;
            m_HeaderCursorPart = HEADER_BEFORE_NAME;
        }
    } else if (m_pMail->receiverNameIndent + m_NameLength > pos) {
        m_HeaderCursorPart = HEADER_AFTER_NAME;
        pos = m_pMail->receiverNameIndent + m_NameLength;
    }
    *index = pos;
}

// 0x00502FE0 slot 0x14
bool lyt::TextAreaMail::GetIndexAtPosition(s32* index, const nn::math::VEC2* pos)
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
    if (m_Mode == MODE_HEADER) {
        EndInput(sys::g_pTextManager);
        UpdateHeaderLength();
    }

    // the character whose middle is right of the position (right-aligned text starts where it ends)
    const s32 length = sys::GetLineManager()->GetLineLength(line);
    const s32 start = sys::GetLineManager()->GetLineStartChecked(line);
    const wchar_t* text = &sys::g_pTextManager->m_pBuffer[start];
    f32 left = 0.0f;
    if (m_pTextBox->m_TextPosition % 3 == TEXT_POSITION_H_RIGHT) {
        f32 width = 0.0f;
        for (s32 i = 0; text[i] != '\n' && text[i] != 0; i++) {
            width += GetCharWidth(sys::g_pTextManager->GetDisplayChar(start + i));
        }
        left = m_pTextBox->m_Size.width - EDIT_MARGIN - width;
    }
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
        if (!(local.x > left + x)) {
            break;
        }
        x += width * 0.5f;
        i++;
    }
    s32 result = start + i;

    // in the header the position is never inside the name
    if (m_Mode == MODE_HEADER) {
        m_HeaderCursorPart = HEADER_BEFORE_NAME;
        const s32 indent = m_pMail->receiverNameIndent;
        if (indent >= result) {
            m_HeaderCursorPart = HEADER_BEFORE_NAME;
        }
        if (indent < result && indent + m_NameLength / 2 >= result) {
            result = indent;
            m_HeaderCursorPart = HEADER_BEFORE_NAME;
        }
        if (m_pMail->receiverNameIndent + m_NameLength <= result) {
            m_HeaderCursorPart = HEADER_AFTER_NAME;
        }
        if (m_pMail->receiverNameIndent + m_NameLength / 2 < result && result < m_pMail->receiverNameIndent + m_NameLength) {
            m_HeaderCursorPart = HEADER_AFTER_NAME;
            result = m_pMail->receiverNameIndent + m_NameLength;
        }
    }
    *index = result;
    return isEnd;
}

// 0x005033BC
void lyt::TextAreaMail::ScrollToCursorPage()
{
    if (m_IsScrolling == true) {
        return;
    }
    const s32 line = GetCursorLine();
    s32 page = m_Page;
    if (m_TopLine > line) {
        page = line + 1;
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
    if (page != 0 && m_Page != page) {
        ScrollToPage(page);
    }
}

// 0x005034DC slot 0x10
void lyt::TextAreaMail::Setup(ssys::ma::lyt::ArcResAccReader* arcReader, ssys::ma::lyt::ArcResAccReader* decorArcReader)
{
    char name[64];
    std::sprintf(name, "%s.bclyt", GetLayoutName());
    Build(arcReader, name, 0x7000);
    FindPane("P_black")->m_Flag &= ~nw::lyt::Pane::FLAG_VISIBLE;
    m_pPagePane = FindPane("N_set");
    m_pIndicatorPane = FindPane("N_ltr_edit");
    SetupTextBoxes();
    m_HighlightColor = m_pTextBoxes[0]->m_TextColors[0];
    m_TextColor = m_pTextBoxes[1]->m_TextColors[0];
    m_pGuideTextBox = NULL;
    if (decorArcReader != NULL) {
        CreateDecor(decorArcReader, "N_decor_E");
    } else {
        CreateDecor(arcReader, "N_decor_E");
    }
    m_pTextBox = nw::lyt::DynamicCast<nw::lyt::TextBox>(FindPane("T_edit_00"));
    nw::lyt::Bounding* bounding = nw::lyt::DynamicCast<nw::lyt::Bounding>(FindPane("B_edit_00"));

    // the edited part
    const nw::lyt::Size size = m_pTextBox->m_Size;
    m_pTextBox->m_Size = nw::lyt::Size(size.width + EDIT_MARGIN, size.height);
    bounding->m_Size.width += 45.0f;
    sys::GetLineManager()->Setup(m_pTextBox, BODY_WORD_SIZE - 1, false, m_pTextBox->m_Size.width - EDIT_MARGIN);
    m_Writer.m_pFont = m_pTextBox->m_pFont;
    m_Writer.SetFontSize(m_pTextBox->m_FontSize.width, m_pTextBox->m_FontSize.height);
    m_Writer.m_LineSpace = m_pTextBox->m_LineSpace;
    m_Writer.m_CharSpace = m_pTextBox->m_CharSpace;
    // a page is a line; page 0 is the header
    m_PageHeight = m_pTextBox->m_FontSize.height + m_pTextBox->m_LineSpace;
    m_PageTop = 0.0f;
    m_Page = 1;
    m_TargetPage = 1;
    SetPagePosition(1);
    m_pIndicatorPane->SetTranslate(nn::math::VEC3(m_pIndicatorPane->m_Translate.x, m_PageHeight + m_PageTop, m_pIndicatorPane->m_Translate.z));
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
    LoadInitialText();
    sys::g_pTextManager->SetCursor(0, true, true);
    m_PageMover.Stop();
    sys::g_pTextManager->m_Unknown0x98 = 1;
}

// 0x00504FEC slot 0x00
// 0x00504F20 (deleting dtor)
lyt::TextAreaMail::~TextAreaMail()
{
    m_PaperLoader.Release();
    m_PaperTexture.DestroyDynamic();
}
