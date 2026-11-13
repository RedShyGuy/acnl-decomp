#pragma once

// Game functions that the SvFgName code of the save data calls (not decompiled yet; names are ours unless stated).

#include "decomp.h"

struct SvFgName;
struct SvFgNameSet;
struct SvPersonalId;
namespace item {
struct ParamItemEntry;
} // namespace item

// a random number in [0, max) (from a random float of the game)
s32 Func2FC2EC(s32 max); // 0x002FC2EC
// the price of an item (see item::GetPrice)
s32 Func3055C8(const SvFgName* name); // 0x003055C8
// fills a set of item names (the kind of set: 14 for the mushroom furniture)
void Func6F4004(SvFgNameSet* set, u32 kind); // 0x006F4004
// the item of an index in a set of item names
SvFgName Func1AD37C(s32 index, SvFgNameSet* set); // 0x001AD37C
// a test of an item (see SvPlayer)
bool Func769644(const SvFgName* item); // 0x00769644
// the item at 0x5E4 of an object of the game (two callers in the code around 0x692B00, which call its slot 0x90 first)
SvFgName Func6BB164(const void* object); // 0x006BB164
// sets a value of the flags of an item from the player data (the value of GetDefaultSaveNo or 0)
void Func304EA0(SvFgName* item, u32 a, u32 saveNo, u32 b); // 0x00304EA0

// the flowers: the genes of a flower field object from two parents, and the name of a flower of a kind with genes; the
// genes of the flower names of each kind (game code at 0x001BA1F0..0x001BA2D0)
u8 Func1BA2D0(u8 genesA, u8 genesB); // 0x001BA2D0
u16 Func1BA0A0(u8 fgKind, u8 genes); // 0x001BA0A0
u8 Func1BA1F0(u16 name); // 0x001BA1F0 (FG_KIND_VIOLET)
u8 Func1BA210(u16 name); // 0x001BA210 (FG_KIND_ROSE)
u8 Func1BA230(u16 name); // 0x001BA230 (FG_KIND_PANSY)
u8 Func1BA250(u16 name); // 0x001BA250 (FG_KIND_TULIP)
u8 Func1BA270(u16 name); // 0x001BA270 (FG_KIND_COSMOS)
u8 Func1BA290(u16 name); // 0x001BA290 (FG_KIND_LILY)
u8 Func1BA2B0(u16 name); // 0x001BA2B0 (FG_KIND_CARNATION)

// (copies of inline functions of the item headers) a value of the parameters of an item
bool Func749394(const item::ParamItemEntry* entry); // 0x00749394

// (the letter helper Func6BC774) the id is the given player / the id gets the value a (the player moves?)
bool Func311B44(const SvPersonalId* id, const SvPersonalId* player); // 0x00311B44
void Func1B50DC(SvPersonalId* id, u32 a); // 0x001B50DC
