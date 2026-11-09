#include "script/dChoiceStandardWin.h"
#include "script/dRenderRubySet.h"
#include "Audio/bgm/dBgmMgr.h"
#include "nw/lyt/lyt_TextBox.h"
#include "sead/seadNew.h"

namespace script {
// 0x005E7FD8 (name is ours)
void ChoiceStandardWin::UpdateTexts()
{
    for (s32 i = 0; i < RENDER_NUM; i++) {
        SelectWindow::ChoiceText* text = &m_Window.m_Texts[i];
        RenderRubySet* render = m_pRenders[i];
        nw::lyt::TextBox* text0 = text->pText;
        nw::lyt::TextBox* text1 = m_Window.m_Texts2[i].pText;
        nw::lyt::TextBox* ruby1 = m_Window.m_Texts2[i].pRuby;
        nw::lyt::TextBox* ruby0 = text->pRuby;
        nw::lyt::Pane* pane = text->pPane;
        render->UpdateColors();
        ChoiceItem* item = GetItem(i);
        if (item != NULL) {
            render->SetText(&item->m_Word);
            f32 width = render->MeasureWidth();
            if (text0 != NULL) {
                text0->m_Size = nw::lyt::Size(width, text0->m_Size.height);
            }
            if (text1 != NULL) {
                text1->m_Size = nw::lyt::Size(width, text1->m_Size.height);
            }
            if (pane != NULL) {
                pane->m_Size = nw::lyt::Size(width, pane->m_Size.height);
            }
            render->CommitText();
            text->sound = item->m_Kind < 11 ? s_KindSounds[item->m_Kind] : SE_SYS_DECIDE;
        } else {
            if (text0 != NULL) {
                text0->SetString(&sead::SafeStringBase<wchar_t>::s_NullChar, 0);
            }
            if (text1 != NULL) {
                text1->SetString(&sead::SafeStringBase<wchar_t>::s_NullChar, 0);
            }
            if (ruby0 != NULL) {
                ruby0->SetString(&sead::SafeStringBase<wchar_t>::s_NullChar, 0);
            }
            if (ruby1 != NULL) {
                ruby1->SetString(&sead::SafeStringBase<wchar_t>::s_NullChar, 0);
            }
        }
    }
}

// 0x005E8230 (name is ours)
bool ChoiceStandardWin::SetupTextBoxes(u32 arg)
{
    if (!m_IsSetup && m_Window.Setup(arg)) {
        m_IsSetup = 1;
        for (s32 i = 0; i < RENDER_NUM; i++) {
            SelectWindow::ChoiceText* text = &m_Window.m_Texts[i];
            SelectWindow::ChoiceText2* text2 = &m_Window.m_Texts2[i];
            m_pRenders[i]->SetTextBoxes(text->pText, text->pRuby, text2->pText, text2->pRuby);
        }
    }
    return static_cast<s8>(m_IsSetup);
}

// 0x005E82B0 (name is ours)
void ChoiceStandardWin::Run()
{
    StateFunc update = s_UpdateFuncs[m_State];
    if (update != NULL) {
        (this->*update)();
    }
    if (m_NextState != STATE_NUM) {
        u8 state = m_NextState;
        m_State = state;
        m_NextState = STATE_NUM;
        StateFunc enter = s_EnterFuncs[state];
        if (enter != NULL) {
            (this->*enter)();
        }
    }
    m_Window.Update();
    if (m_Window.m_IsTextChanged) {
        UpdateTexts();
    }
    if (m_State != STATE_SELECT || m_IsHold) {
        return;
    }
    if (!m_IsTextSet && m_Window.m_Cursor >= 0) {
        m_IsTextSet = 1;
        if (m_Kind == 1 && m_Window.m_Cursor == 0) {
            audio::bgm::GetStageOpusMgr()->RequestPrologue10();
        }
    }
    if (m_Window.IsClosed()) {
        m_NextState = STATE_DECIDE;
    }
}

// 0x005E83F4 slot 0x30 (name is ours)
void ChoiceStandardWin::OnSetup()
{
    m_IsTextSet = 0;
    UpdateTexts();
    if (m_IsFixed) {
        m_Window.Open(-1);
        return;
    }
    if (m_Kind == 0) {
        m_Window.m_IsNoCancelSound = 1;
    }
    m_Window.OpenWithCancel(m_CancelIndex);
}

// 0x005E8454 (name after the RTTI)
ChoiceStandardWin::ChoiceStandardWin() : ChoiceStandard(ITEM_NUM), m_IsSetup(0), m_IsTextSet(0)
{
    for (s32 i = 0; i < RENDER_NUM; i++) {
        m_pRenders[i] = NULL;
    }
    for (s32 i = 0; i < RENDER_NUM; i++) {
        m_pRenders[i] = new (std::nothrow) RenderRubySet(65, 10);
    }
}

// 0x005E8574 slot 0x00 (name after the RTTI)
// 0x005E8564 (deleting dtor)
ChoiceStandardWin::~ChoiceStandardWin()
{
    for (s32 i = 0; i < RENDER_NUM; i++) {
        if (m_pRenders[i] != NULL) {
            delete m_pRenders[i];
            m_pRenders[i] = NULL;
        }
    }
}

// 0x0020A1E4 slot 0x34 (name is ours)
void ChoiceStandardWin::OnDecide()
{
    if (m_IsEndRequested) {
        m_Window.Close();
    } else if (m_Window.GetState() == &SelectWindow::State_Select) {
        m_Window.Change(&SelectWindow::State_Close);
    }
}

// 0x007131D4 slot 0x10 (name is ours; armlink removed the tail call: falls into SelectWindow::IsClosed)
void ChoiceStandardWin::Update()
{
    m_Window.IsClosed();
}

// 0x0075C178 slot 0x38 (name is ours)
bool ChoiceStandardWin::IsReady() const
{
    return !const_cast<SelectWindow&>(m_Window).IsBusy();
}

// 0x0075C18C slot 0x18 (name is ours)
s32 ChoiceStandardWin::GetCursor() const
{
    if (!m_IsFixed) {
        return m_Window.m_Cursor;
    }
    for (s32 i = 0; i < 7; i++) {
        if (m_Window.m_SliderValue < s_SliderSteps[i]) {
            return i;
        }
    }
    return 7;
}

// 0x0075C1E0 slot 0x20 (name is ours)
bool ChoiceStandardWin::IsCancelPressed() const
{
    return m_Window.m_IsCanceled;
}

// 0x00724208 slot 0x24 (name is ours; armlink removed the tail call: falls into GetSelectedIndex)
s32 ChoiceStandardWin::GetCancelValue() const
{
    return m_Window.m_Control.GetSelectedIndex();
}
} // namespace script
