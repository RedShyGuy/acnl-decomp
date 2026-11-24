#include "Other/dControl2Button.h"

// 0x00268D28
void Control2Button::SetSeOk(SeID a, SeID b)
{
    const SeID values[2] = {a, b};
    for (s32 i = 0; i < m_ButtonNum; i++) {
        m_pButtons[i].SetSeOk(values[i]);
        m_pButtons[i].SetOkSoundEnabled(true);
    }
}

// 0x00268D84
void Control2Button::SetDecide(bool a, bool b)
{
    const u8 values[2] = {a, b};
    for (s32 i = 0; i < m_ButtonNum; i++) {
        m_pButtons[i].SetDecide(values[i]);
    }
}

// 0x00268DD4
void Control2Button::SetSe(SeID a, SeID b)
{
    const SeID values[2] = {a, b};
    for (s32 i = 0; i < m_ButtonNum; i++) {
        m_pButtons[i].SetSe(values[i]);
        m_pButtons[i].SetSoundEnabled(true);
        m_pButtons[i].m_SoundMode = true;
    }
}

// 0x00268E34
void Control2Button::SetInput(u32 a, u32 b)
{
    const u32 values[2] = {a, b};
    for (s32 i = 0; i < m_ButtonNum; i++) {
        m_pButtons[i].m_Input = values[i];
    }
}

// 0x00268E88
void Control2Button::SetText(s32 a, s32 b, const char* label)
{
    const s32 values[2] = {a, b};
    for (s32 i = 0; i < m_ButtonNum; i++) {
        m_pButtons[i].SetText(values[i], label);
    }
}

// 0x00268EE4
void Control2Button::SetPanes(nw::lyt::Pane* a, nw::lyt::Pane* b)
{
    nw::lyt::Pane* const values[2] = {a, b};
    for (s32 i = 0; i < m_ButtonNum; i++) {
        m_pButtons[i].SetPane(values[i]);
    }
}

// 0x00268F80 slot 0x00
// 0x00775FCC (thunk)
// 0x00775F88 (deleting thunk)
// 0x00268F38 (deleting dtor)
Control2Button::~Control2Button()
{
    // (only the members are destroyed)
}
