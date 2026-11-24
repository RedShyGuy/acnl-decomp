#include "Other/dControl1Button.h"

// 0x00268C7C
void Control1Button::SetSeOk(SeID value)
{
    m_pButtons->SetSeOk(value);
    m_pButtons->SetOkSoundEnabled(true);
}

// 0x002E080C (a nop entry that falls into CommonButtonSingle::SetDecide)
void Control1Button::SetDecide(bool value)
{
    m_pButtons->SetDecide(value);
}

// 0x00268C90
void Control1Button::SetInput(u32 value)
{
    m_pButtons->m_Input = value;
}

// 0x002E08CC (a nop entry that falls into CommonButtonSingle::SetText)
void Control1Button::SetText(s32 value, const char* label)
{
    m_pButtons->SetText(value, label);
}

// 0x002E0940 (a nop entry that falls into CommonButtonSingle::SetPane)
void Control1Button::SetPane(nw::lyt::Pane* value)
{
    m_pButtons->SetPane(value);
}

// 0x00268CE4 slot 0x00
// 0x00775F48 (thunk)
// 0x00775F04 (deleting thunk)
// 0x00268C9C (deleting dtor)
Control1Button::~Control1Button()
{
    // (only the members are destroyed)
}
