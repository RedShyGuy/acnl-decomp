#pragma once

#include "decomp.h"
#include "Sv/dSvFgName.h"

// Game functions the script code calls that are not decompiled yet. Names after what script does with them (ours);
// FuncXXXXXX while the purpose is unknown.
struct SvMail;
struct SvPlayer;
struct SvTownId;

namespace script {
class IWord;
class WordCPtrSv;
template <u32 N>
class WordFix;
} // namespace script

// the filters of the inventory slots for the menus of the flow event 17 (IFlowRecept::s_Menus; purpose of each unknown)
bool Func1769F8(const SvFgName* item, u32 value); // 0x001769F8
bool Func176A44(const SvFgName* item, u32 value); // 0x00176A44
bool Func179AA8(const SvFgName* item, u32 value); // 0x00179AA8
bool Func179AE0(const SvFgName* item, u32 value); // 0x00179AE0
bool Func17A280(const SvFgName* item, u32 value); // 0x0017A280
bool Func17A360(const SvFgName* item, u32 value); // 0x0017A360
bool Func17A398(const SvFgName* item, u32 value); // 0x0017A398
bool Func17C528(const SvFgName* item, u32 value); // 0x0017C528
bool Func17C560(const SvFgName* item, u32 value); // 0x0017C560
bool Func17D6B0(const SvFgName* item, u32 value); // 0x0017D6B0
bool Func18069C(const SvFgName* item, u32 value); // 0x0018069C
bool Func1806E4(const SvFgName* item, u32 value); // 0x001806E4
bool Func180A9C(const SvFgName* item, u32 value); // 0x00180A9C
bool Func1811A0(const SvFgName* item, u32 value); // 0x001811A0
bool Func181C6C(const SvFgName* item, u32 value); // 0x00181C6C

// the body of the letter (193 characters)
script::WordCPtrSv GetMailBody(const SvMail* mail); // 0x00759D90
// a header text of the letter into the word (by the kind at +0x30; purpose of the kinds unknown)
void Func759DFC(const SvMail* mail, script::IWord* word); // 0x00759DFC
// the name of the town of the save data into the word
void GetTownName(script::IWord* word); // 0x0056EB80

// a name of the game into the word (the players of the town by number; purpose of the numbers unknown)
void Func56DE5C(script::WordFix<9>* word, s32 number); // 0x0056DE5C
// a player name with its sex into the word
void MakePlayerName(script::WordFix<9>* word, const char16* name, u32 sex); // 0x0056DF70
// (name after the address) the name of the villager of a letter person (as MakePlayerName)
void Func56E310(script::WordFix<9>* word, const void* npc); // 0x0056E310
// the name of the town into the word
void MakeTownName(script::WordFix<9>* word, const SvTownId* town); // 0x0056EB34
// a player of the save data (purpose unknown)
SvPlayer* Func71AD8C(void* data); // 0x0071AD8C
// the impression of the player (0..50; purpose of the value unknown)
u32 Func7713C8(); // 0x007713C8
// the conversions of the measures of Tag4_19 / Tag4_20 by region (purpose unknown)
f32 Func5D2A54(u16 value); // 0x005D2A54
f32 Func5D2B2C(u16 value); // 0x005D2B2C
f32 Func5D2A6C(u16 value); // 0x005D2A6C
// a random number in [0, max)
u32 GetRandom(u32 max); // 0x002FF63C

// a set of item names to pick from at random (the purpose of the kinds is unknown; names are ours)
struct SvFgNameSet
{
    SvFgNameSet(); // 0x002FC318

    u8 _data[0x340];
};
void SetupFgNameSet(SvFgNameSet* set, s32 kind); // 0x002FC334
void RemoveFgName(SvFgNameSet* set, const SvFgName* name); // 0x002FED0C
SvFgName PickFgName(SvFgNameSet* set); // 0x002FED5C
u16 GetFgNameId(const SvFgName* name); // 0x0030B6BC

// The game functions the branch tags of TalkSeq call (the tags pick a label by their result; purposes unknown unless
// noted).
class SvFgName;
struct SvMuseum;
struct SvPlayerOutfit;
struct SvPlayerAppearance;
struct SvPlayerInventory;
// the speech sound of a character (TalkSeq/TalkRuby)
// the player reacts to a choice (value: ChoiceItem::m_Sound < 47); false: no reaction (game code, name is ours)
bool Func5C8F30(u8 value, u32 arg); // 0x005C8F30
u32 Func71B33C(const SvPlayerOutfit* outfit); // 0x0071B33C
bool Func76D45C(u32 value); // 0x0076D45C
bool Func76A518(u32 value); // 0x0076A518
// the share of the unlocked items of a kind in percent (0..100)
u32 Func770F68(const u32* unlockedItems, u32 kind); // 0x00770F68
bool Func713404(SvPlayer* player); // 0x00713404
s32 Func240734(u32 value); // 0x00240734
s32 Func713768(const SvPlayerAppearance* appearance); // 0x00713768
s32 Func713798(const SvPlayerAppearance* appearance); // 0x00713798
u32 Func768FF8(const SvFgName* item); // 0x00768FF8
void* Func5C7BDC(); // 0x005C7BDC
s32 Func717120(); // 0x00717120
u32 Func71E70C(const SvPlayerInventory* inventory); // 0x0071E70C
u32 Func72519C(const SvPlayerInventory* inventory); // 0x0072519C
bool Func630C3C(); // 0x00630C3C
s32 Func76E230(SvMuseum* museum); // 0x0076E230
s32 Func76E4C8(SvMuseum* museum); // 0x0076E4C8
s32 Func76E44C(SvMuseum* museum); // 0x0076E44C
s32 Func76E5CC(SvMuseum* museum); // 0x0076E5CC
const SvFgName* Func71B394(const SvPlayerOutfit* outfit); // 0x0071B394
bool Func76BA8C(const SvFgName* item); // 0x0076BA8C
s32 Func311840(const SvFgName* item); // 0x00311840

// (FlowSystem) clears the second byte of the flags 0x00954B34 of the file of ObjTalkRecept before a flow starts
void Func23CCB4(); // 0x0023CCB4

// The game functions the flow handlers of IFlowRecept call (purposes unknown unless noted).
struct SvCensus;
struct SvGardenData;
struct SvStrcData;
struct SvMyDesign;
struct SvDesignStand;
namespace libms {
class FlowNodeBranch;
} // namespace libms
namespace script {
template <u32 N>
class WordFix;
class ITalkRecept;
} // namespace script

// the value given to the process of the flow event 100 (Func1A9920 sets it up; 100 bytes)
struct FlowProcessArg
{
    u32 _unknown0x0;  // 0x00
    u32 param;        // 0x04
    u32 arg0;         // 0x08
    u32 arg1;         // 0x0C
    u32 _unknown0x10; // 0x10
    s32 result;       // 0x14 read by the branch 155
    u8 _unknown0x18[0x4C];
};
ASSERT_SIZE(FlowProcessArg, 0x64);
extern FlowProcessArg g_FlowProcessArg;
void InitFlowProcessArg(FlowProcessArg* arg); // 0x001A9920

// an object of a CRO module (the imports 0x00150280.. are thunks into it)
struct FlowModuleObject
{
    u8 _unknown0x0[0x660];
    script::ITalkRecept* pTalkRecept; // 0x660
    u8 _unknown0x664[0x38D8 - 0x664];
    u32 value;                        // 0x38D8
};
bool Func150280(); // 0x00150280
FlowModuleObject* Func150288(); // 0x00150288
bool Func150290(); // 0x00150290
// a byte of a CRO module (the original loads a pointer the module loader sets)
extern u8 g_ModuleStandIndex;

struct FlowObject951138
{
    u8 _unknown0x0[0x1C];
    u8 isDone; // 0x1C
};
extern FlowObject951138* g_pObject951138;
extern void* g_pObject94FD54;
extern u8 g_Byte0x951155;
extern u8 g_Byte0xAAEAD4;

void Func1B4F6C(void* design, SvDesignStand* stand); // 0x001B4F6C
const s32* Func1BB904(); // 0x001BB904
void Func1E0818(u32 value); // 0x001E0818
void Func1E0870(u32 value); // 0x001E0870
void Func1FE400(u32 kind); // 0x001FE400
void Func1FE458(); // 0x001FE458
void Func1FE478(u32 kind, script::WordFix<33>* word); // 0x001FE478
void* Func20C4C8(SvMyDesign* designs, u32 index); // 0x0020C4C8
s32 Func23C368(u32 value); // 0x0023C368
bool Func23CB00(); // 0x0023CB00
bool Func23CB14(u32* value); // 0x0023CB14
bool Func23CBC0(); // 0x0023CBC0
bool Func23CBF0(); // 0x0023CBF0
void Func23CC08(u32 value); // 0x0023CC08
bool Func251930(); // 0x00251930
void Func2912B8(u32 value); // 0x002912B8
void Func293264(); // 0x00293264
void Func2BBDE8(void* flags, u32 index, bool value); // 0x002BBDE8
void Func2BFDE0(SvPlayerInventory* inventory, u32 a, u32 b, u32 c); // 0x002BFDE0
void Func2BFF98(SvPlayerInventory* inventory, u32 slot, const SvFgName* item, u32 a, u32 b); // 0x002BFF98
void Func2C00A8(SvPlayerInventory* inventory, u32 value); // 0x002C00A8
void Func2C02C8(SvPlayerInventory* inventory, u32 value); // 0x002C02C8
void Func2C1304(u8* data, u32 index, bool value); // 0x002C1304
void* Func300928(u8* data, s32 entry); // 0x00300928 (Thumb)
u32 Func3037DC(void* data); // 0x003037DC
bool Func305240(void* entry); // 0x00305240
bool Func3107F4(const SvFgName* item); // 0x003107F4
bool Func350D40(); // 0x00350D40
bool Func5C3750(); // 0x005C3750
void Func5C8CC4(); // 0x005C8CC4
void Func5CE494(SvStrcData* data, u32 value); // 0x005CE494
s32 Func606170(void* object, u32* index, const libms::FlowNodeBranch& node); // 0x00606170
u8 Func62FB14(); // 0x0062FB14
void Func6B9468(SvCensus* census, s32 value); // 0x006B9468
bool Func710100(); // 0x00710100
void* Func7171D4(void* object, u8 index); // 0x007171D4
u32 Func71E9A4(const u8* data, u32 index); // 0x0071E9A4
bool Func720FC0(void* entry); // 0x00720FC0
bool Func758CDC(SvGardenData* data); // 0x00758CDC
u32 Func75A27C(SvStrcData* data); // 0x0075A27C
bool Func769774(const SvFgName* item); // 0x00769774
bool Func76A650(const SvFgName* item); // 0x0076A650
u32 Func118C8C(u8* data); // 0x00118C8C (Thumb)
s32 Func118EAC(u8* data, u32 value); // 0x00118EAC (Thumb)
s32 Func118FCC(u8* data); // 0x00118FCC (Thumb)
