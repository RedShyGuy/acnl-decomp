#include "Other/dControl3Button.h"

// 0x00268FC4
void Control3Button::SetSeOk(SeID a, SeID b, SeID c)
{
    const SeID values[3] = {a, b, c};
    for (s32 i = 0; i < m_ButtonNum; i++) {
        m_pButtons[i].SetSeOk(values[i]);
        m_pButtons[i].SetOkSoundEnabled(true);
    }
}

// 0x00269020
void Control3Button::SetDecide(bool a, bool b, bool c)
{
    const u8 values[3] = {a, b, c};
    for (s32 i = 0; i < m_ButtonNum; i++) {
        m_pButtons[i].SetDecide(values[i]);
    }
}

// 0x00269074
void Control3Button::SetInput(u32 a, u32 b, u32 c)
{
    const u32 values[3] = {a, b, c};
    for (s32 i = 0; i < m_ButtonNum; i++) {
        m_pButtons[i].m_Input = values[i];
    }
}

// 0x002690C8
void Control3Button::SetText(s32 a, s32 b, s32 c, const char* label)
{
    const s32 values[3] = {a, b, c};
    for (s32 i = 0; i < m_ButtonNum; i++) {
        m_pButtons[i].SetText(values[i], label);
    }
}

// 0x00269124
void Control3Button::SetPanes(nw::lyt::Pane* a, nw::lyt::Pane* b, nw::lyt::Pane* c)
{
    nw::lyt::Pane* const values[3] = {a, b, c};
    for (s32 i = 0; i < m_ButtonNum; i++) {
        m_pButtons[i].SetPane(values[i]);
    }
}

// 0x002691C0 slot 0x00
// 0x00776050 (thunk)
// 0x0077600C (deleting thunk)
// 0x00269178 (deleting dtor)
Control3Button::~Control3Button()
{
    // (only the members are destroyed)
}
