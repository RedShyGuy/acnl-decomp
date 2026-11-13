#pragma once

#include "decomp.h"
#include "Sv/dSvCommon.h"
#include "Item/item_Api.h"
#include "Sv/dSvFgCategory.h"
#include "nn/fnd/fnd_DateTime.h"

// Game functions that the shop of the Nooklings (dSvShopZakka.cpp) calls, not decompiled yet (names after the
// addresses while the purpose is unknown).
struct SvFgName;
struct SvShopZakka;
struct SvTownDataUnk1E368;

// a time goes on by days / a part of the time is set (the arguments are not understood yet)
void Func5CE8B8(SvTime* time, s32 days); // 0x005CE8B8
void Func5CE934(SvTime* time, s32 a, u8 b, s32 c, s32 d); // 0x005CE934
// the days between a time and a date (the last two arguments are always 1)
s32 Func303850(const SvTime* time, const nn::fnd::DateTimeParameters* date, s32 a, s32 b); // 0x00303850
bool Func75A6C8(const SvTime* time, s32 a); // 0x0075A6C8
// an entry of the town for a day (the ids 9..12 are the renewals of the Nooklings to the levels 1..4)
void Func1B6680(SvTownDataUnk1E368* data, u32 id, const SvDate* date); // 0x001B6680
// the level of the shop at town+0x132C0 (the shop of the Nooklings sets 0, 3 and 4)
void Func30607C(s32* shop, u32 level); // 0x0030607C (Thumb)
// the item hangs on a wall (it becomes the sold-out sign for walls) / a check before the item is replaced on the field
bool Func303BD4(const SvFgName* item); // 0x00303BD4
bool Func30B658(const SvFgName* item); // 0x0030B658
// the item at a position is replaced (kinds 1..4) / the position is updated
void Func4EA100(s32 x, s32 z, u32 kind, s32 unknown); // 0x004EA100
void Func59F144(s32 x, s32 z, u32 value); // 0x0059F144

// (the restocking, Func108C40) the item sets: the group item::GROUP_WALL_FURNITURE (SetupFgNameSet with 51; the group
// is ParamItemEntry::group) / the items of a category with a group (special groups 1..8) / a value
// / the items are picked at random into the places / the number of items of the set
struct SvFgNameSet;
void Func2FC32C(SvFgNameSet* set); // 0x002FC32C
void Func2FECF8(SvFgNameSet* set, SvFgCategory::Name category, item::Group group, u32 c); // 0x002FECF8
void Func304A5C(SvFgNameSet* set, u32 a); // 0x00304A5C
void Func30031C(SvFgName* items, u32 num, SvFgNameSet* set); // 0x0030031C
u32 Func302FC0(const SvFgNameSet* set); // 0x00302FC0
// a check of the date (the first value and three values; the restocking uses (5, today, 0, 30, 1))
bool Func30141C(u32 a, nn::fnd::DateTimeParameters date, s32 b, s32 c, s32 d); // 0x0030141C
bool Func2FDE58(u32 a); // 0x002FDE58
// the value and a flag of an item by which the last places are sorted
u32 Func302EDC(const SvFgName* item); // 0x00302EDC
bool Func303C2C(const SvFgName* item); // 0x00303C2C
// a global object; the restocking reads the halfword at +2
const u8* Func303C0C(); // 0x00303C0C
// the remainder (low word) and the quotient (high word) of a division by 20
u64 Func77A6D4(s32 value); // 0x0077A6D4
