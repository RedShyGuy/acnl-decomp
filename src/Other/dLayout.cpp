#include "ssys/ma/lyt/dLayout.h"
#include "Other/dLayout.h"

// 0x005C0214 slot 0x00 (name after libgarden)
// 0x005C01E4 (deleting dtor)
Layout::~Layout()
{
    // (only the base is destroyed)
}

// 0x00569D98 slot 0x14 (falls into ssys::ma::lyt::Layout::DrawSave)
void Layout::DrawSave()
{
    ssys::ma::lyt::Layout::DrawSave();
}

// 0x005C004C slot 0x18 (name after libgarden)
void Layout::UpdateProj()
{
    // (the same as the one of the base)
    nw::ut::Rect rect = m_Layout.GetLayoutRect();
    ssys::ma::lyt::detail::MakeProjection(&m_DrawInfo, rect, m_IsPerspective == true);
}

// 0x00126550 (name after libgarden)
Layout::Layout()
{
    // (only the base is constructed)
}

