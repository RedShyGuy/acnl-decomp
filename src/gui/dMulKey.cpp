#include "gui/dMulKey.h"

namespace gui {
// 0x004FE7D8 slot 0x14
void MulKey::UpdateLabel()
{
    SetLabelText(m_Desc2.pButton);
}

// 0x004FE824 slot 0x10
ButtonEx* MulKey::GetButton()
{
    return m_Desc2.pButton;
}

// 0x004FE82C slot 0x08
void MulKey::Update()
{
    // empty (in the original too)
}

// 0x004FE87C slot 0x00
// 0x004FE830 (deleting dtor)
MulKey::~MulKey()
{
    if (m_pInputs != NULL) {
        delete[] m_pInputs;
        m_pInputs = NULL;
    }
}
} // namespace gui
