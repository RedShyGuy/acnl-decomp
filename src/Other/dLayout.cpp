#include "ssys/ma/lyt/dLayout.h"
#include "Other/dLayout.h"

// 0x005C0214 slot 0x00 | libgarden
Layout::~Layout()
{
}

// 0x00569D98 slot 0x14 (falls into ssys::ma::lyt::Layout::DrawSave)
void Layout::DrawSave()
{
    ssys::ma::lyt::Layout::DrawSave();
}

// 0x005C004C slot 0x18
void Layout::UpdateProj()
{
}

// 0x00126550 | libgarden [tier A]
Layout::Layout()
{
}

