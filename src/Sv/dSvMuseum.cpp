#include "Sv/dSvMuseum.h"

// 0x006BCED8 (name is ours)
SvMuseumExhibit* SvMuseum::GetExhibit(u32 index)
{
    if (index >= 4) {
        return &exhibits[0];
    }
    return &exhibits[index];
}
