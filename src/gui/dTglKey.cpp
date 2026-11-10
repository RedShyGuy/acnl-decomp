#include "gui/dTglKey.h"

namespace gui {
// 0x004FEA60 slot 0x14
void TglKey::UpdateLabel()
{
    SetLabelText(m_Desc2.pButton);
}

// 0x004FEAAC slot 0x10
ButtonEx* TglKey::GetButton()
{
    return m_Desc2.pButton;
}

// 0x004FEAB4 slot 0x08
void TglKey::Update()
{
    // empty (in the original too)
}

// 0x004FE658 slot 0x00
// 0x004FEAB8 (deleting dtor)
TglKey::~TglKey()
{
    // (only the base is destroyed)
}
} // namespace gui
