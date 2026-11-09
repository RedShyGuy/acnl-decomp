#include "script/dRenderGarden.h"
#include "nw/lyt/lyt_TextBox.h"
#include "sead/seadSafeStringBase.h"
#include "script/dChip.h"
#include "script/dIWord.h"

namespace script {
// 0x005E3768 | libgarden [tier A]
void RenderGarden::SetTextBoxes(nw::lyt::TextBox* pTextBox, nw::lyt::TextBox* pTextBox2)
{
    if (m_IsTextBoxSet) {
        return;
    }
    if (pTextBox != NULL) {
        m_pTextBox = pTextBox;
        u16 length = m_Main.m_Len;
        pTextBox->AllocStringBuffer(length, 0);
        pTextBox->SetTagProcessor(&m_Main);
        m_Main.SetDefaultColors(pTextBox->m_TextColors[0], pTextBox->m_TextColors[1]);
        const nw::font::Font* font = pTextBox->m_pFont;
        m_Main.m_FontSize.x = pTextBox->m_FontSize.width / font->GetWidth();
        m_Main.m_FontSize.y = pTextBox->m_FontSize.height / font->GetHeight();
        if (pTextBox2 != NULL) {
            m_pTextBox2 = pTextBox2;
            pTextBox2->AllocStringBuffer(length, 0);
            pTextBox2->SetTagProcessor(&m_Main);
        }
    }
    m_IsTextBoxSet = true;
}

// 0x005E38D4 | libgarden [tier A]
void RenderGarden::Clear()
{
    m_IsTextBoxSet = false;
    if (m_pTextBox != NULL) {
        m_pTextBox->FreeStringBuffer();
        m_pTextBox = NULL;
    }
    if (m_pTextBox2 != NULL) {
        m_pTextBox2->FreeStringBuffer();
        m_pTextBox2 = NULL;
    }
}

// 0x005E3920 (name after libgarden)
void RenderGarden::SetText(const IWord* word)
{
    m_Main.ClearBuf2();
    m_Trace.m_pTextBox = m_pTextBox;
    m_Trace.m_pTextBoxSub = NULL;
    m_Trace.Run(word->GetWord());
    m_Trace.m_pTextBox = NULL;
    m_Trace.m_pTextBoxSub = NULL;
}

// 0x005E3984 (name is ours)
void RenderGardenFit::SetTextFit(const IWord* word)
{
    nw::lyt::TextBox* textBox = m_pTextBox;
    textBox->SetFontSize(m_FontSize);
    f32 scale = Chip::CalcFitScale(textBox, word);
    textBox->SetFontSize(nw::lyt::Size(m_FontSize.width * scale, m_FontSize.height));
    SetText(word);
    CommitText();
}

namespace {
// (inline) the written text into a text box
inline void CommitTextBox(nw::lyt::TextBox* textBox, RenderMain* render)
{
    u16 length = render->GetTextLength();
    render->ResetStore();
    if (length != 0) {
        textBox->SetString(reinterpret_cast<const wchar_t*>(render->m_pText), 0, length);
    } else {
        textBox->SetString(&sead::SafeStringBase<wchar_t>::s_NullChar, 0);
    }
}
} // namespace

// 0x005E3A64 | libgarden [tier A]
void RenderGarden::CommitText()
{
    if (m_pTextBox != NULL) {
        CommitTextBox(m_pTextBox, &m_Main);
    }
    if (m_pTextBox2 != NULL) {
        CommitTextBox(m_pTextBox2, &m_Main);
    }
}

// 0x005E3BF8 | libgarden [tier A]
RenderGarden::RenderGarden(u32 length)
    : m_Main(length, 0, 0), m_Trace(&m_Main, NULL), m_pTextBox(NULL), m_pTextBox2(NULL), m_IsTextBoxSet(false)
{
}

// 0x005E3C64 | libgarden [tier A]
RenderGarden::~RenderGarden()
{
    Clear();
}
} // namespace script
