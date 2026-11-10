#include "lyt/dTextAreaOneLine.h"

#include <cmath>
#include <cstdio>

#include "nn/math/math_ARMv6.h"
#include "nn/math/math_Api.h"
#include "nw/lyt/lyt_TextBox.h"
#include "sys/dSwkbd.h"

namespace {
// the text box is wider than the line by this
const f32 BOX_MARGIN = 16.0f;
} // namespace

// 0x00506C4C slot 0x14
bool lyt::TextAreaOneLine::GetIndexAtPosition(s32* index, const nn::math::VEC2* pos)
{
    bool isEnd = false;
    f32 x = 0.0f;
    const nn::math::VEC3 screen(pos->x, pos->y, 0.0f);
    nn::math::VEC3 local;
    nn::math::VEC3Project(&local, &m_InvMtx, &screen);
    ScrollToCursor();

    // the character whose middle is right of the position
    const s32 length = sys::GetLineManager()->GetLineLength(0);
    const s32 start = sys::GetLineManager()->GetLineStartChecked(0);
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

// 0x00506D50 slot 0x10
void lyt::TextAreaOneLine::Setup(ssys::ma::lyt::ArcResAccReader* arcReader, ssys::ma::lyt::ArcResAccReader* decorArcReader)
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
    const nw::lyt::Size size = m_pTextBox->m_Size;
    m_pTextBox->m_Size = nw::lyt::Size(size.width + BOX_MARGIN, size.height);

    // one line of at most the maximum length
    sys::g_LineNum = 1;
    sys::GetLineManager()->m_LineNum = 1;
    sys::GetLineManager()->Setup(m_pTextBox, sys::g_KeyboardParam.maxLength, false, m_pTextBox->m_Size.width - BOX_MARGIN);
    m_Writer.m_pFont = m_pTextBox->m_pFont;
    m_Writer.SetFontSize(m_pTextBox->m_FontSize.width, m_pTextBox->m_FontSize.height);
    m_Writer.m_LineSpace = m_pTextBox->m_LineSpace;
    m_Writer.m_CharSpace = m_pTextBox->m_CharSpace;
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

    // the string buffer of the box: the characters that fit (estimated with narrow ones)
    s32 length = static_cast<s32>(std::ceil(sys::GetLineManager()->m_Width / (GetCharWidth('i') + m_Writer.m_CharSpace)));
    length *= GetLineNum();
    if (length > sys::g_BufferSize - 1) {
        length = sys::g_BufferSize - 1;
    }
    m_pTextBox->AllocStringBuffer(static_cast<u16>(length), 0);
    m_TextColor = m_pTextBox->m_TextColors[0];
    LoadInitialText();
    sys::g_pTextManager->SetCursor(sys::g_pTextManager->m_Length, true, true);
}

// 0x00507C28
lyt::TextAreaOneLine::TextAreaOneLine(u32 unknown, s32 priority, sead::Heap* heap) : TextArea(unknown, priority, heap)
{
}

// 0x00507C54 slot 0x00
// 0x00507C40 (deleting dtor)
lyt::TextAreaOneLine::~TextAreaOneLine()
{
    // (only the base is destroyed)
}
