#include "gui/dNrmKey.h"

#include "sead/seadNew.h"
#include "ssys/ma/dAllocator.h"
#include "ssys/ma/lyt/lyt_Api.h"

namespace gui {
// 0x004FE8BC slot 0x14
void NrmKey::UpdateLabel()
{
    SetLabelText(m_Desc2.pButton);
}

// 0x004FE908 slot 0x10
ButtonEx* NrmKey::GetButton()
{
    return m_Desc2.pButton;
}

// 0x004FE910 slot 0x08
void NrmKey::Update()
{
    // empty (in the original too)
}

// 0x004FE914
NrmKey::NrmKey(const Description& desc) : Key(desc), m_Desc2(desc), m_pChars(NULL)
{
    m_pChars = new (ssys::ma::lyt::GetAllocator()->m_pHeap, 4) u16[m_Desc2.labelNum];
    for (s32 i = 0; i < m_Desc2.labelNum; i++) {
        m_pChars[i] = m_Desc2.chars[i];
    }
}

// 0x004FEA28 slot 0x00
// 0x004FE9E4 (deleting dtor)
NrmKey::~NrmKey()
{
    if (m_pChars != NULL) {
        delete[] m_pChars;
        m_pChars = NULL;
    }
}
} // namespace gui
