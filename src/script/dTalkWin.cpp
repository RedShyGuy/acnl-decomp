#include "script/dTalkWin.h"
#include "nw/lyt/lyt_Pane.h"
#include "nw/lyt/lyt_TextBox.h"
#include "sead/seadSafeStringBase.h"
#include "script/dRenderMain.h"
#include "script/dRenderRuby.h"
#include "stage/stage_Api.h"

namespace script {
// the shakes: impulse, spring and damping per strength
// 0x00891C2C
const TalkWin::Shake::Param TalkWin::s_ShakeParams[3] = {
    {nn::math::VEC2(2.0f, 4.0f), nn::math::VEC2(0.87f, 2.4f), nn::math::VEC2(0.8f, 0.8f)},
    {nn::math::VEC2(3.0f, 8.0f), nn::math::VEC2(0.87f, 2.36f), nn::math::VEC2(0.8f, 0.8f)},
    {nn::math::VEC2(5.0f, 14.0f), nn::math::VEC2(0.87f, 2.4f), nn::math::VEC2(0.82f, 0.82f)},
};

namespace {
// (inline) the translation of the pane; its matrix is made again
inline void SetPaneTranslate(nw::lyt::Pane* pane, f32 x, f32 y, f32 z)
{
    pane->m_Translate.x = x;
    pane->m_Translate.y = y;
    pane->m_Translate.z = z;
    pane->m_Flag &= ~nw::lyt::Pane::FLAG_MTX_CONDITION_MASK;
}

// (inline) a text box gets a renderer as its tag processor and room for its text
inline void BindTextBox(nw::lyt::TextBox* textBox, RenderBase* render)
{
    textBox->AllocStringBuffer(render->m_Len, 0);
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

// (inline) whether something was written into the renderer
inline bool IsWritten(const RenderBase* render)
{
    return static_cast<s8>(render->m_pBuf2[render->m_Count - 1]) != 0;
}
} // namespace

// 0x005FCA8C
void TalkWin::Shake::Update()
{
    if (state >= STATE_NONE) {
        return;
    }
    const Param& param = s_ShakeParams[state];
    velocity.x += -offset.x * param.spring.x;
    velocity.y += -offset.y * param.spring.y;
    velocity.x = velocity.x * param.damping.x;
    velocity.y = velocity.y * param.damping.y;
    offset.x += velocity.x;
    offset.y += velocity.y;
    if (pPane != NULL) {
        SetPaneTranslate(pPane, base.x + offset.x, base.y + offset.y, base.z);
    }
    if (velocity.x * velocity.x + velocity.y * velocity.y < 0.08f &&
        offset.x * offset.x + offset.y * offset.y < 0.5f) {
        Stop();
    }
}

// 0x005FCBE0
void TalkWin::CommitText(nw::lyt::TextBox* pTextBox)
{
    RenderMain* main = static_cast<RenderMain*>(m_pMain);
    if (!IsWritten(main)) {
        return;
    }
    u16 length = main->GetTextLength();
    main->ResetStore();
    SetTextBoxString(pTextBox, main, length);
}

// 0x005FCCBC
bool TalkWin::Setup()
{
    if (!m_IsReady) {
        m_IsReady = m_Window.Setup();
        if (m_IsReady) {
            nw::lyt::TextBox* textBox = m_Window.m_pTextBox;
            nw::lyt::TextBox* textBox2 = m_Window.m_pTextBox2;
            nw::lyt::TextBox* rubyTextBox = m_Window.m_pRubyTextBox;
            nw::lyt::TextBox* rubyTextBox2 = m_Window.m_pRubyTextBox2;
            if (m_Window.m_pNameTextBox != NULL) {
                m_Window.m_pNameTextBox->AllocStringBuffer(9, 0);
            }
            if (textBox != NULL) {
                BindTextBox(textBox, m_pMain);
                SetupRender(m_pMain, textBox);
            }
            if (textBox2 != NULL) {
                BindTextBox(textBox2, m_pMain);
            }
            if (rubyTextBox != NULL) {
                BindTextBox(rubyTextBox, m_pRuby);
                SetupRender(m_pRuby, rubyTextBox);
            }
            if (rubyTextBox2 != NULL) {
                BindTextBox(rubyTextBox2, m_pRuby);
            }
            s32 stage = stage::GetCurrent();
            if (stage == 101 || stage == 102) {
                m_IsSpecialSetup = 1;
                u16 length = m_pMain->m_Len;
                if (m_Window.m_pSpecialTextBox != NULL) {
                    m_Window.m_pSpecialTextBox->AllocStringBuffer(length, 0);
                    m_Window.m_pSpecialTextBox->SetTagProcessor(m_pMain);
                }
                for (u32 i = 0; m_Window.m_pSpecialTextBoxes[i] != NULL; i++) {
                    m_Window.m_pSpecialTextBoxes[i]->AllocStringBuffer(length, 0);
                    m_Window.m_pSpecialTextBoxes[i]->SetTagProcessor(m_pMain);
                }
            }
            m_Shake.pPane = m_Window.m_pShakePane;
        }
    }
    return m_IsReady;
}

// 0x005FD028
void TalkWin::Update()
{
    if (m_IsSpecial) {
        if (m_Window.m_pSpecialTextBox != NULL) {
            CommitText(m_Window.m_pSpecialTextBox);
        }
        for (u32 i = 0; m_Window.m_pSpecialTextBoxes[i] != NULL; i++) {
            CommitText(m_Window.m_pSpecialTextBoxes[i]);
        }
    } else {
        nw::lyt::TextBox* textBox = m_Window.m_pTextBox;
        nw::lyt::TextBox* textBox2 = m_Window.m_pTextBox2;
        if (textBox != NULL) {
            CommitText(textBox);
        }
        if (textBox2 != NULL) {
            CommitText(textBox2);
        }
        nw::lyt::TextBox* rubyTextBox = m_Window.m_pRubyTextBox;
        nw::lyt::TextBox* rubyTextBox2 = m_Window.m_pRubyTextBox2;
        RenderRuby* ruby = static_cast<RenderRuby*>(m_pRuby);
        if (rubyTextBox != NULL && IsWritten(ruby)) {
            u16 length = ruby->GetTextLength();
            ruby->m_Index = 0;
            SetTextBoxString(rubyTextBox, ruby, length);
        }
        if (rubyTextBox2 != NULL && IsWritten(ruby)) {
            u16 length = ruby->GetTextLength();
            ruby->m_Index = 0;
            SetTextBoxString(rubyTextBox2, ruby, length);
        }
    }
    m_Shake.Update();
    m_Window.Call();
    if (m_State == 1) {
        if (!m_Window.Func770A18()) {
            m_State = 2;
        }
    } else if (m_State == 3) {
        if (!m_Window.Func770A18()) {
            m_State = 0;
        }
    }
}

// 0x005FD22C
TalkWin::TalkWin(RenderBase* pMain, RenderBase* pRuby)
    : m_pMain(pMain), m_pRuby(pRuby), m_IsSpecialSetup(0), m_IsSpecial(0), m_OpenSound(5), m_CloseSound(3), m_State(0),
      m_IsReady(0)
{
    m_Shake.pPane = NULL;
    m_Shake.base.x = 0.0f;
    m_Shake.base.y = 0.0f;
    m_Shake.base.z = 0.0f;
    m_Shake.state = Shake::STATE_NONE;
    m_Shake.offset.x = 0.0f;
    m_Shake.offset.y = 0.0f;
    m_Shake.velocity.x = 0.0f;
    m_Shake.velocity.y = 0.0f;
    m_WindowKind = 11;
}

// 0x005FD37C slot 0x00
// 0x005FD2C4 (deleting dtor)
TalkWin::~TalkWin()
{
    // (only the members are destroyed)
}
} // namespace script
