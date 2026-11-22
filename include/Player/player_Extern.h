#pragma once

#include "decomp.h"

// Game functions that PlayerModel calls and that are not decompiled yet (names after the addresses while the purpose is
// unknown; the other names are ours).

class SvFgName;
struct SvPlayerAppearance;
struct SvPlayerOutfit;
struct SvMiiData;
struct ExchangeBankKeywordUnknown3;
class ExchangeModelBankBase;
namespace g3d {
class ResourceLoader;
class SkeletalModel;
} // namespace g3d

// the shared graphics file of the body model of the players
g3d::ResourceLoader* Func5C2938(); // 0x005C2938
// (called with the Mii of the player when the banks are set up; meaning unknown)
void Func583828(const SvMiiData* mii); // 0x00583828
// the face of an appearance (below 12, 0 otherwise)
s32 Func713758(const SvPlayerAppearance* appearance); // 0x00713758
// the colors of the tables 0x00AD65EA (the tan, below 8), 0x00AD6602 (the hair color, below 16) and 0x00AD65D8 (the eye
// color, below 6); the first entry otherwise
void Func20D3E8(ExchangeBankKeywordUnknown3* out, s32 tan); // 0x0020D3E8
void Func20D3A0(ExchangeBankKeywordUnknown3* out, s32 hairColor); // 0x0020D3A0
void Func20D358(ExchangeBankKeywordUnknown3* out, s32 eyeColor); // 0x0020D358
// the skin color of a Mii (out: four bytes, the color in the last three in reverse order; false: none)
bool Func753C10(const SvMiiData* mii, u8* out); // 0x00753C10
// the items of an outfit for the banks of the player (SvFgName::s_Empty when the item does not fit; the accessory of type
// 2, the tops of type 3, the shoes of type 6; the bottoms and the legs with more rules)
const SvFgName* Func71B370(const SvPlayerOutfit* outfit); // 0x0071B370
const SvFgName* Func71B3DC(const SvPlayerOutfit* outfit); // 0x0071B3DC
const SvFgName* Func71B400(const SvPlayerOutfit* outfit); // 0x0071B400
const SvFgName* Func71AEF8(const SvPlayerOutfit* outfit); // 0x0071AEF8
// (a test of the tops item: true draws the bottoms; meaning unknown)
bool Func76A68C(const SvFgName* item); // 0x0076A68C
// the shown model of a bank takes the skeleton of parent
void Func328778(ExchangeModelBankBase* bank, g3d::SkeletalModel* parent); // 0x00328778
// the brightness of the shown model resource index of a bank (1 without a model)
f32 Func3287B0(ExchangeModelBankBase* bank, u32 index); // 0x003287B0
// the tool animation of an item for the players (HumanAnimName, 0: none; from the kind entry of the item)
u16 Func6E61CC(const SvFgName* item); // 0x006E61CC
// the rhythm animation of the head of index & 3 (RHYTHM_HEAD_1..4, the table 0x00866D00)
u16 Func6E58D0(u32 index); // 0x006E58D0
// the animation id puts the model on the curve of the world (bit 3 of HumanAnimInfo::flags; false for invalid ids)
bool Func6E5880(u16 id); // 0x006E5880
