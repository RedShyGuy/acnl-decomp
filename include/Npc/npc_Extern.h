#pragma once

#include "decomp.h"

struct SvNpc;

// (game code, not decompiled yet; names after the addresses) helpers for the ids of the characters: the villagers are
// 0..398, the special characters 0x1000..0x100F and 0x2000..0x2010
// a valid id
bool Func1B6350(u16 npcId); // 0x001B6350
// the model name of the character ("wol04" etc.; NULL without one)
const char* Func1B6330(u16 npcId); // 0x001B6330
// a villager (id below 399)
bool Func1B5A18(u16 npcId); // 0x001B5A18
// (reads the id as a word; ScStage keeps a camping car when it is false)
bool Func1B5CEC(const u32* npcId); // 0x001B5CEC
// applies the loaded camping car info data to the villager npc (Thumb)
void Func1134AC(SvNpc* npc, const void* data); // 0x001134AC
