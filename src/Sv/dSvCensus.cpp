#include "Sv/dSvCensus.h"

#include "Sv/dSvApi.h"
#include "netgame/netgame_Api.h"

namespace {
// (inline) value + add, kept in [0, 999999999] (the stored value counts as signed)
inline u32 AddCapped(u32 value, s32 add)
{
    const s64 sum = static_cast<s64>(static_cast<s32>(value)) + add;
    if (sum < 0) {
        return 0;
    }
    if (sum > 999999999) {
        return 999999999;
    }
    return static_cast<u32>(sum);
}
} // namespace

// 0x006B6290 (name is ours)
void SvCensusValue::AddTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    totalPlayerStat = AddCapped(totalPlayerStat, value);
}

// 0x006B7ED0 (name is ours)
void SvCensusValue::Add(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(playerStats[SvGetCurrentTownSlot()], value);
    playerStats[SvGetCurrentTownSlot()] = stat;
    AddTotal(value);
}
