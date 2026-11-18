#pragma once

#include "decomp.h"

class SvFgName;
struct SvTownDataUnk1C7A8;
namespace script {
class StrMgr;
} // namespace script
namespace sead {
class Heap;
} // namespace sead

// (game code, not decompiled yet; names after the addresses) what ScStage calls, in the order of its functions

// EndInitialize
// the entries of SvTownDataUnk1C7A8 (its fishing tourney part; when the event 8 is active the players of the valid
// entries get the initiative 84)
s32 Func725C1C(const SvTownDataUnk1C7A8* data); // 0x00725C1C (the number of entries)
bool Func725CB4(const SvTownDataUnk1C7A8* data, s32 index); // 0x00725CB4
u32 Func725C48(const SvTownDataUnk1C7A8* data, s32 index); // 0x00725C48 (a player; the low byte is used)
void Func60D024(); // 0x0060D024 (after stage::Build)

// UpdateInitialize
// (the address of SvGardenPlus+0x290A4 is passed; the meaning of the object is unknown; Thumb)
void Func1193A0(void* object); // 0x001193A0
// places item (the value of Func319750) at unit x/z of layer of the stage (a camping car, CampingCarA/B)
u32 Func319750(const SvFgName* item); // 0x00319750
void Func319770(s32 x, s32 z, u8 layer, u32 value, u32 stage); // 0x00319770

// StartInitialize
void* Func6311CC(); // 0x006311CC (the object of Func1E8AAC and Func1E6488)
void Func1E8AAC(void* object); // 0x001E8AAC
void Func52E1B8(u32 mode); // 0x0052E1B8 (0 normal, 1 Downtown, 2 exhibition, 3 island demo, 4 LobbyIsland)
void Func4DF0D0(); // 0x004DF0D0 (village, auto camps and the Reset Centre; not in multiplayer)
void Func4E9C78(); // 0x004E9C78 (outdoor stages)
void Func5C0664(); // 0x005C0664
void Func4EA450(); // 0x004EA450 (title)
void Func25D018(); // 0x0025D018 (title)

// LoadSceneResources
// (true when loaded)
bool Func1C24D8(void* saveProcess, sead::Heap* heap); // 0x001C24D8

// EndFinalize
void Func4E188C(); // 0x004E188C
void Func62FD0C(); // 0x0062FD0C
void Func1E83DC(); // 0x001E83DC
void Func1E8A48(); // 0x001E8A48
void Func133CB8(script::StrMgr* strMgr); // 0x00133CB8 (the scene group of the strings is freed)
void Func4DEDC4(); // 0x004DEDC4
// (the items of SvTownDataUnkEE2C::unk_0x13428 when the next stage is RecycleShop)
void Func20D838(SvFgName* items); // 0x0020D838
void Func6D401C(); // 0x006D401C

// UpdateFinalize / StartFinalize
bool Func6D4574(); // 0x006D4574 (true when done)
void Func6D4128(); // 0x006D4128

// Calc
void Func52E654(); // 0x0052E654
void Func4DEC78(); // 0x004DEC78
void Func1E6488(void* object); // 0x001E6488

// a buffer that Calc clears every frame
struct UnknownAAEAF4
{
    u8 data[0x620];
    u8 flag; // 0x620
};
extern UnknownAAEAF4 g_UnknownAAEAF4;
