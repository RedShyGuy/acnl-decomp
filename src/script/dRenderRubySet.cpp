#include "script/dRenderRubySet.h"
#include "nw/lyt/lyt_TextBox.h"
#include "sead/seadSafeStringBase.h"
#include "script/dIWord.h"

namespace script {
namespace {
// (inline) a text box gets the text of a renderer (as its tag processor) and room for it
inline void BindTextBox(nw::lyt::TextBox* textBox, RenderBase* render, u16 length)
{
    textBox->AllocStringBuffer(length, 0);
    textBox->SetTagProcessor(render);
}

// (inline) the default colors and the font scale of a renderer from its text box
inline void SetupRender(RenderBase* render, const nw::lyt::TextBox* textBox)
{
    render->SetDefaultColors(textBox->m_TextColors[0], textBox->m_TextColors[1]);
    const nw::font::Font* font = textBox->m_pFont;
    render->m_FontSize.x = textBox->m_FontSize.width / font->GetWidth();
    render->m_FontSize.y = textBox->m_FontSize.height / font->GetHeight();
}

// (inline) the written text into a text box
inline void SetTextBoxString(nw::lyt::TextBox* textBox, const RenderBase* render, u16 length)
{
    if (length != 0) {
        textBox->SetString(reinterpret_cast<const wchar_t*>(render->m_pText), 0, length);
    } else {
        textBox->SetString(&sead::SafeStringBase<wchar_t>::s_NullChar, 0);
    }
}
} // namespace

// 0x005D748C | libgarden [tier A]
void RenderRubySet::SetTextBoxes(nw::lyt::TextBox* pText, nw::lyt::TextBox* pTextRuby, nw::lyt::TextBox* pText2,
                                 nw::lyt::TextBox* pText2Ruby)
{
    if (m_IsTextBoxSet) {
        return;
    }
    if (pText != NULL) {
        m_pText = pText;
        u16 length = m_Main.m_Len;
        BindTextBox(pText, &m_Main, length);
        SetupRender(&m_Main, pText);
        if (pText2 != NULL) {
            m_pText2 = pText2;
            BindTextBox(pText2, &m_Main, length);
        }
    }
    if (pTextRuby != NULL) {
        m_pTextRuby = pTextRuby;
        u16 length = m_Ruby.m_Len;
        BindTextBox(pTextRuby, &m_Ruby, length);
        SetupRender(&m_Ruby, pTextRuby);
        if (pText2Ruby != NULL) {
            m_pText2Ruby = pText2Ruby;
            BindTextBox(pText2Ruby, &m_Ruby, length);
        }
    }
    m_IsTextBoxSet = true;
}

// 0x005D76CC | libgarden [tier A]
void RenderRubySet::Clear()
{
    m_IsTextBoxSet = false;
    if (m_pText != NULL) {
        m_pText->FreeStringBuffer();
        m_pText = NULL;
    }
    if (m_pText2 != NULL) {
        m_pText2->FreeStringBuffer();
        m_pText2 = NULL;
    }
    if (m_pTextRuby != NULL) {
        m_pTextRuby->FreeStringBuffer();
        m_pTextRuby = NULL;
    }
    if (m_pText2Ruby != NULL) {
        m_pText2Ruby->FreeStringBuffer();
        m_pText2Ruby = NULL;
    }
}

// 0x005D7750 (name is ours)
void RenderRubySet::UpdateColors()
{
    if (m_pText != NULL) {
        m_Main.SetDefaultColors(m_pText->m_TextColors[0], m_pText->m_TextColors[1]);
    }
    if (m_pTextRuby != NULL) {
        m_Ruby.SetDefaultColors(m_pTextRuby->m_TextColors[0], m_pTextRuby->m_TextColors[1]);
    }
}

// 0x005D7790 (name after libgarden)
void RenderRubySet::SetText(const IWord* word)
{
    m_Main.ClearBuf2();
    m_Ruby.ClearBuf2();
    m_Trace.m_pTextBox = m_pText;
    m_Trace.m_pTextBoxSub = m_pTextRuby;
    m_Trace.Run(word->GetWord());
    m_Trace.m_pTextBox = NULL;
    m_Trace.m_pTextBoxSub = NULL;
}

// 0x005D77F8 | libgarden [tier A]
void RenderRubySet::CommitText()
{
    if (m_pText != NULL) {
        u16 length = m_Main.GetTextLength();
        m_Main.ResetStore();
        SetTextBoxString(m_pText, &m_Main, length);
    }
    if (m_pText2 != NULL) {
        u16 length = m_Main.GetTextLength();
        m_Main.ResetStore();
        SetTextBoxString(m_pText2, &m_Main, length);
    }
    if (m_pTextRuby != NULL) {
        u16 length = m_Ruby.GetTextLength();
        m_Ruby.m_Index = 0;
        SetTextBoxString(m_pTextRuby, &m_Ruby, length);
    }
    if (m_pText2Ruby != NULL) {
        u16 length = m_Ruby.GetTextLength();
        m_Ruby.m_Index = 0;
        SetTextBoxString(m_pText2Ruby, &m_Ruby, length);
    }
}

// 0x005D7A7C | libgarden [tier A]
RenderRubySet::RenderRubySet(u32 length, u32 storeNum)
    : m_Main(length, storeNum, 0), m_Ruby(length, 0, &m_Main), m_Trace(&m_Main, &m_Ruby), m_pText(NULL),
      m_pTextRuby(NULL), m_pText2(NULL), m_pText2Ruby(NULL), m_IsTextBoxSet(false)
{
}

// 0x005D7B0C (name after libgarden)
RenderRubySet::~RenderRubySet()
{
    Clear();
}
} // namespace script
