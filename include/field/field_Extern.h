#pragma once

#include "decomp.h"
#include "field/dFieldTypes.h"

class SvFgName;
class ScStage;
class Field;
struct SvNpc;
struct FgDataItem;
struct FieldRect;
class FgDataLoader;
struct SvFgNameSet;
namespace sead {
class Random;
} // namespace sead
struct FgPlacement;
namespace g3d {
class ResourceLoader;
} // namespace g3d
struct FieldGrass;
struct SvTownDataUnk5C;

// Functions of the game that the field code calls and that are not decompiled yet (names after the addresses until their
// owners are known).

// item takes over source with the orientation of the usual items (bits 14..15 of the flags)
void Func311860(SvFgName* item, SvFgName source, u32 orientation); // 0x00311860
// !Func63061C(): the camping villager has the tent (else the igloo; meaning unknown)
bool Func6CC54C(); // 0x006CC54C
// a state of the shop of Gracie (+0x14 >= 4 or the byte +1; the bargain room; meaning unknown)
bool Func302940(const void* shop); // 0x00302940
// the house structures 18..65 of the players / their data (NULL: none)
bool Func6CB818(u8 structure); // 0x006CB818
void* Func6CC658(u8 structure); // 0x006CC658
// the room of the stage in a house / its ground and its data (the rooms at +0x10 of the house data)
u32 Func6FBB80(u32 stage); // 0x006FBB80
u16 Func6FB9E8(void* rooms, u32 room); // 0x006FB9E8
void* Func30521C(void* rooms, u32 room); // 0x0030521C
// the item of a unit of a room (layer 0: 10 x 10, layer 1: 8 x 8)
const SvFgName* Func721010(void* room, s32 x, s32 z, u32 layer); // 0x00721010
// whether the block x, z is one of the 5 x 4 blocks of the town with items
bool Func6141DC(s32 x, s32 z); // 0x006141DC
// the items / the grass of the town map
struct SvFgBlock;
SvFgBlock* Func2FAECC(SvTownDataUnk5C* map, s32 x, s32 z); // 0x002FAECC
FieldGrass* Func614230(SvTownDataUnk5C* map); // 0x00614230
// the acre of a map position (x, y) of the town map from SvTownDataUnk5C::oceanSide on (the acres follow at +4)
u16* Func75EBB4(u8* townMap, s32 x, s32 y); // 0x0075EBB4

// (FieldFactory::Create) the functions that it calls for the rooms
// a function with the value of Func31336C of the stage (sound_Extern.h); its result is not used
bool Func51E1D4(s32 value); // 0x0051E1D4
// the villager of the town with that value (NULL: none; Thumb)
SvNpc* Func305AC0(SvNpc* npcs, s32 value); // 0x00305AC0
// the items of the room of a villager from the layouts 3 / 2 of Func6E565C (the items of a ground of the layout, always true)
bool Func6E5698(FgDataItem* out, u32 size, u32 bg); // 0x006E5698
bool Func318FDC(FgDataItem* out, u32 size, u32 bg); // 0x00318FDC
// the items of the room of a villager into the blocks of the layers 0 and 1 (Thumb)
void Func11A818(SvNpc* npc, const FgDataItem* items, SvFgBlock* layer0, SvFgBlock* layer1); // 0x0011A818
void Func1199C4(SvNpc* npc, const FgDataItem* items, SvFgBlock* layer0, SvFgBlock* layer1); // 0x001199C4
// the layouts of items of the grounds (an array of 5 at 0x009B4B08; the first one for an index of 5 or more)
FgDataLoader* Func6E565C(u32 index); // 0x006E565C
// the items (24) of the camping car index when the scene has no camping car info for it (ScStage::GetCampingCarInfo)
FgPlacement Func2CFE88(s32 index); // 0x002CFE88
// the grounds of the downtown blocks (size bytes) for the ocean side of the town
void Func6FB288(u16* out, u32 size, u8 oceanSide); // 0x006FB288
// the variants of the town hall / the train station of the town (0..3)
u32 Func6CBA2C(); // 0x006CBA2C
u32 Func6CBA48(); // 0x006CBA48
// the grass of the exhibition from the loaded file
void Func6F9A68(Field* field, g3d::ResourceLoader* loader); // 0x006F9A68
// the kind of an acre (the table 0x0094FA78 of 361 bytes)
u8 Func6060E8(u16 acre); // 0x006060E8

// (the tours of the island) the level of the tour (0..2) of the tour data from TourData::m_Seed on (it reads the tour at
// +0xC)
u8 Func76F7E4(const u32* tourSeed); // 0x0076F7E4
// the item of a villager hut is remembered for the hut / the random number of the tour is set (TourData from m_Seed on)
void Func6C59BC(u32* tourSeed, const SvFgName* item, u32 hut); // 0x006C59BC
void Func6C5A20(u32* tourSeed, u32 random); // 0x006C5A20
// the items of the room of a villager into the blocks of the layers 0 and 1 for a hut of the tour (Thumb)
void Func112300(const FgDataItem* items, SvFgBlock* layer0, SvFgBlock* layer1, sead::Random* random, u16 villager,
                u32 hut, bool isLevel0); // 0x00112300
// the stage of a field of the field table (the table 0x00953728)
u8 Func6F9FE4(u8 field); // 0x006F9FE4
// a byte of an item (the fossils of the huts remember it)
u8 Func76A388(const SvFgName* item); // 0x0076A388
// an item is added to the set / one of the set at random (the random number scaled to the items in it)
void Func302F80(SvFgNameSet* set, const SvFgName* item); // 0x00302F80
SvFgName Func1AD2E0(SvFgNameSet* set, u32 random); // 0x001AD2E0
// the angle of a direction 0..3 (0, 0x4000, 0x8000, 0xC000) / the orientation of an item (0..3) for an angle
u16 Func6947A0(u8 direction); // 0x006947A0
u32 GetQuarterOfAngle(const u16* angle); // 0x00692070 (in the file of BsFtrMgr; name is ours)
// the angle of a quarter (0..3; others: 0)
u16 GetAngleOfQuarter(u32 quarter); // 0x006947A0 (in the file of BsFtrMgr; name is ours)

// (the tour builders, isfactory) an object without members whose function places something on the field 31 of the
// island with the random numbers / the block of a position on the island (x, z) / something of the kind at a unit (the
// kind 106 at the unit 7, 6 of that block) / something with the field of a village tour
struct Unknown1DC370
{
    void Func1DC370(sead::Random* random); // 0x001DC370
    // (the fossil dig: called with the pairs 66/67, 68/69, ... for each player)
    void Func1DD618(u8 first, u8 second, sead::Random* random); // 0x001DD618
    // (hide and seek: called for the ids 66..70 and five ids of HideAndSeekBuilder::PickRandomId)
    void Func1DC77C(u8 id, sead::Random* random); // 0x001DC77C
    // (the borrowing race: called for the ids 66..70, one for each hut)
    void Func1DC910(u8 id, sead::Random* random); // 0x001DC910
};
void Func6FAD4C(s32* x, s32* z); // 0x006FAD4C
void Func2427C8(s32 unitX, s32 unitZ, u32 kind); // 0x002427C8 (the position by value)
void Func6FAA84(Field* field); // 0x006FAA84
// the kind of the fishing / insect tour of the tour data from TourData::m_Seed on (isfactory::CreateBuilder)
s32 Func76FB3C(const u32* tourSeed); // 0x0076FB3C
s32 Func76FB7C(const u32* tourSeed); // 0x0076FB7C
// two kinds of the rocks of the mining tour from TourData::m_Seed on (SvFgName::Func6BA880 values 0..5)
u8 Func76FABC(const u32* tourSeed); // 0x0076FABC
u8 Func76F764(const u32* tourSeed); // 0x0076F764
// an item at a random free unit of units of the field (the unit is returned; isfactory helper file 0x527xxx)
Vector2i Func527094(Field* field, const FieldRect* units, const SvFgName* item, sead::Random* random); // 0x00527094
// a test of an item (the mining tour leaves those items out; meaning unknown)
bool Func769C18(const SvFgName* item); // 0x00769C18
// all items of an item kind are added to the set (the item name range of the kind)
void Func302978(SvFgNameSet* set, u8 kind); // 0x00302978
// two tests of an id 0..251 (HideAndSeekBuilder::PickRandomId takes the ids for which both hold)
bool Func5CAF84(u8 id); // 0x005CAF84
bool Func5CB03C(u8 id); // 0x005CB03C
// items are added to the set (the treasure hunt: 5 with 20..23 and whether the net game type is 2; meaning unknown)
void Func2FECF8(SvFgNameSet* set, u32 a, u32 b, bool flag); // 0x002FECF8
// (the flower picking) a flower at a random free unit of units / something is done to the set with a value / the set
// gets the flowers of a value of the item (Func769264 111: Func769828, 112: Func76BF1C)
void Func527734(Field* field, const FieldRect* units, const SvFgName* item, sead::Random* random); // 0x00527734
void Func302A90(SvFgNameSet* set, u32 value); // 0x00302A90 (the flower picking passes 74; meaning unknown)
u32 Func769828(const SvFgName* item); // 0x00769828
u32 Func76BF1C(const SvFgName* item); // 0x0076BF1C
void Func6F3ECC(SvFgNameSet* set, u32 value); // 0x006F3ECC
void Func6F1524(SvFgNameSet* set, u32 value); // 0x006F1524
// (see dOrderList.h)
u8 Func769264(const SvFgName* name);
