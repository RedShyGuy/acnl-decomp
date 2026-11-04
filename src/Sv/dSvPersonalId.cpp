#include "Sv/dSvPersonalId.h"

// 0x0076D6F0 (name is ours)
DECOMP_NOINLINE bool SvTownId::IsValid() const
{
    return tid != 0;
}

// 0x002FBE08 (name is ours)
bool SvPersonalId::IsValid() const
{
    if (playerData.pid != 0 && townData.IsValid()) {
        return true;
    }
    return false;
}
