#include "Other/dControlButtonBase.h"

// 0x002CD748 slot 0x08 | slot vf_0x08 of ButtonActionControl
void ControlButtonBase::Update()
{
    for (s32 i = m_FirstVisible; i < m_ButtonNum; i++) {
        m_pButtons[i].FUN_002b72e0();
    }
    Call();
}

// 0x002CD810 (name is ours)
void ControlButtonBase::AnimateButtons()
{
    for (s32 i = m_FirstVisible; i < m_ButtonNum; i++) {
        m_pButtons[i].AnimateLayout();
    }
}

// 0x002CD8AC (name is ours)
void ControlButtonBase::SetButtonOkSound(SeID se, s32 index)
{
    m_pButtons[index].m_SeOk = se;
    m_pButtons[index].m_OkSoundEnabled = true;
}

// 0x002CD944 (name is ours)
void ControlButtonBase::SetDisabled(bool disabled)
{
    SetFlags(DISABLED, disabled);
    for (s32 i = 0; i < m_ButtonNum; i++) {
        m_pButtons[i].SetDisabled(disabled);
    }
}

// 0x002CD99C (name is ours)
void ControlButtonBase::SetVisibleNum(s32 num)
{
    m_FirstVisible = m_ButtonNum - num;
    for (s32 i = 0; i < m_ButtonNum; i++) {
        m_pButtons[i].SetVisible(m_FirstVisible <= i);
        m_pButtons[i].m_IsActive = m_FirstVisible <= i;
    }
}

// 0x002CDA10 (name after the RTTI)
ControlButtonBase::ControlButtonBase(CommonButtonSingle* buttons, s32 buttonNum)
    : m_pButtons(buttons), m_ButtonNum(buttonNum), m_FirstVisible(0), m_Unknown0x114(0)
{
}

// 0x002F8184 slot 0x00 | slot vf_0x00 of ssys::st::List
// 0x002CDA78 (deleting dtor)
// 0x007760A4 (thunk)
// 0x00776090 (deleting thunk)
ControlButtonBase::~ControlButtonBase()
{
    // empty (in the original too)
}
