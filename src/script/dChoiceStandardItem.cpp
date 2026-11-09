#include "script/dChoiceStandardItem.h"
#include "script/dRenderGarden.h"
#include "nw/lyt/lyt_TextBox.h"
#include "sead/seadNew.h"
#include "sead/seadSafeStringBase.h"
#include "Sound/dSoundLabel.h"

namespace script {
// 0x005E8F68 (name is ours)
void ChoiceStandardItem::SetTitle(const IWord& title)
{
    static_cast<IWord&>(m_Title).Copy(title);
}

// 0x005E8F80 (name is ours)
void ChoiceStandardItem::UpdateTexts()
{
    for (s32 i = 0; i < ITEM_NUM; i++) {
        ItemSelectWindow::ChoiceText* text = &m_Window.m_Texts[i];
        RenderGarden* render = m_pRenders[i];
        nw::lyt::TextBox* textBox = text->pText;
        nw::lyt::Pane* pane = text->pPane;
        ChoiceItem* item = GetItem(i);
        if (item != NULL) {
            render->SetText(&item->m_Word);
            f32 width = render->MeasureWidth();
            if (textBox != NULL) {
                textBox->m_Size = nw::lyt::Size(width, textBox->m_Size.height);
            }
            if (pane != NULL) {
                pane->m_Size = nw::lyt::Size(width, pane->m_Size.height);
            }
            render->CommitText();
            text->sound = item->m_Kind < 11 ? s_KindSounds[item->m_Kind] : SE_SYS_DECIDE;
        } else if (textBox != NULL) {
            textBox->SetString(&sead::SafeStringBase<wchar_t>::s_NullChar, 0);
        }
    }
}

// 0x005E90F8 (name is ours)
bool ChoiceStandardItem::SetupTextBoxes(u32 arg)
{
    if (!m_IsSetup) {
        bool isSetup = true;
        if (arg == 0) {
            isSetup = m_Window.IsSetup();
            if (!isSetup) {
                return static_cast<s8>(m_IsSetup);
            }
        }
        if ((m_Window.Setup(arg) & isSetup) != 0) {
            m_IsSetup = 1;
            for (s32 i = 0; i < ITEM_NUM; i++) {
                m_pRenders[i]->SetTextBoxes(m_Window.m_Texts[i].pText, NULL);
            }
            m_pTitle->SetTextBoxes(m_Window.m_pTitleText, NULL);
        }
    }
    return static_cast<s8>(m_IsSetup);
}

// 0x005E9250 (name is ours)
void ChoiceStandardItem::Run()
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
    if (m_State == STATE_SELECT && !m_IsHold && m_Window.IsClosed()) {
        m_NextState = STATE_DECIDE;
    }
}

// 0x005E933C slot 0x30 (name is ours)
void ChoiceStandardItem::OnSetup()
{
    UpdateTexts();
    RenderGarden* title = m_pTitle;
    nw::lyt::TextBox* titleText = m_Window.m_pTitleText;
    title->SetText(&m_Title);
    titleText->m_Size.width = title->MeasureWidth();
    m_pTitle->CommitText();
    m_Window.Open(m_CancelIndex);
}

// 0x005E9404 (name after the RTTI)
ChoiceStandardItem::ChoiceStandardItem() : ChoiceStandard(ITEM_NUM), m_IsSetup(0), m_pTitle(NULL)
{
    for (s32 i = 0; i < ITEM_NUM; i++) {
        m_pRenders[i] = NULL;
    }
    for (s32 i = 0; i < ITEM_NUM; i++) {
        m_pRenders[i] = new (std::nothrow) RenderGarden(20);
    }
    m_pTitle = new (std::nothrow) RenderGarden(33);
}

// 0x005E954C slot 0x00 (name after the RTTI)
// 0x005E953C (deleting dtor)
ChoiceStandardItem::~ChoiceStandardItem()
{
    if (m_pTitle != NULL) {
        delete m_pTitle;
        m_pTitle = NULL;
    }
    for (s32 i = 0; i < ITEM_NUM; i++) {
        if (m_pRenders[i] != NULL) {
            delete m_pRenders[i];
            m_pRenders[i] = NULL;
        }
    }
}

// 0x0071DD68 slot 0x10 (name is ours; armlink removed the tail call: falls into ItemSelectWindow::IsClosed)
void ChoiceStandardItem::Update()
{
    m_Window.IsClosed();
}

// 0x0075C200 slot 0x18 (name is ours)
s32 ChoiceStandardItem::GetCursor() const
{
    return m_Window.m_Cursor;
}

// 0x0075C210 slot 0x20 (name is ours)
bool ChoiceStandardItem::IsCancelPressed() const
{
    return m_Window.m_IsCanceled;
}

// 0x0075C208 slot 0x24 (name is ours)
s32 ChoiceStandardItem::GetCancelValue() const
{
    return -1;
}

// 0x002BB0AC slot 0x34 (name is ours)
void ChoiceStandardItem::OnDecide()
{
    m_Window.Change(&ItemSelectWindow::State_Close);
}

// 0x0075C1EC slot 0x38 (name is ours)
bool ChoiceStandardItem::IsReady() const
{
    return !const_cast<ItemSelectWindow&>(m_Window).IsBusy();
}
} // namespace script
