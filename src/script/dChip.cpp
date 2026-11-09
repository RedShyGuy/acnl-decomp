#include "script/dChip.h"
#include "font/dMgr.h"
#include "nw/font/font_TextWriterBase.h"
#include "nw/lyt/lyt_TextBox.h"
#include "script/dChip_MeasureProcessor.h"
#include "script/dIWord.h"
#include <float.h>

namespace script {
// 0x005EA938
f32 Chip::CalcFitScale(const nw::lyt::TextBox* pTextBox, const IWord* word)
{
    const char16* text = word->GetWord();
    u32 size = word->GetSize();
    f32 scale = 1.0f;
    if (text != NULL && size != 0) {
        const char16* end = text + size - 1;
        f32 width = MeasureWidth(pTextBox, &text, &end);
        if (width > 0.0f) {
            width += 1.0f;
            f32 limit = pTextBox->m_Size.width * pTextBox->m_Scale.x;
            if (width > limit) {
                scale = limit / width;
            }
        }
    }
    return scale;
}

// 0x005EAAAC
f32 Chip::MeasureWidth(const IWord* word)
{
    nw::font::TextWriterBase<wchar_t> writer;
    writer.m_pFont = font::Mgr::s_pInstance->GetFont(font::GARDEN_MSG_16);
    writer.m_ScaleH = 1.0f;
    writer.m_ScaleV = 1.0f;
    writer.m_LineSpace = 0.0f;
    writer.m_CharSpace = 0.0f;
    writer.m_WidthLimit = FLT_MAX;
    writer.m_DrawFlag = 0;
    return writer.CalcStringWidth(reinterpret_cast<const wchar_t*>(word->GetWord()), word->GetSize() - 1);
}

// 0x005EAB78
f32 Chip::MeasureWidth(const nw::lyt::TextBox* pTextBox, const char16* const* begin, const char16* const* end)
{
    nw::font::TextWriterBase<wchar_t> writer;
    writer.m_pFont = pTextBox->m_pFont;
    writer.SetFontSize(pTextBox->m_FontSize.width, pTextBox->m_FontSize.height);
    writer.m_LineSpace = pTextBox->m_LineSpace;
    writer.m_CharSpace = pTextBox->m_CharSpace;
    writer.m_WidthLimit = FLT_MAX;
    MeasureProcessor processor(nn::math::VEC2(writer.m_ScaleH, writer.m_ScaleV));
    writer.m_pTagProcessor = &processor;
    writer.m_DrawFlag = 0;
    return writer.CalcStringWidth(reinterpret_cast<const wchar_t*>(*begin), *end - *begin);
}
} // namespace script
