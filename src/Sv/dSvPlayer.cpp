#include "Sv/dSvPlayer.h"

// 0x002FB9EC (name is ours)
// 0x00302030 (copy)
SvPlayer* SvPlayerData::Get(u32 index)
{
    if (index >= 4) {
        return NULL;
    }
    return &players[index];
}
