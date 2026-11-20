#pragma once

#include "decomp.h"
#include "nn/math/math_Vector3.h"

class Actor;
class AcStrc;

// Functions of the game that BsStrcMgr calls and that are not decompiled yet (names after the addresses until their
// owners are known).

// (objects of other files that BsStrcMgr uses; meanings unknown)
// cleared in EndInitialize outside the multiplayer
extern u8 g_Unknown998C70[18];
// its float at +0x204 above 0 sets BsStrcMgr::m_Unknown6B5D (BsPlayerMgr clears two floats of it)
struct UnknownAAFF90
{
    u8 _0[0x204];
    f32 unk204; // 0x204
};
extern UnknownAAFF90 g_UnknownAAFF90;

struct FieldObjectList;
struct FieldObjectEntry;
// the entry of the lists 12 and 13 of the field objects (meanings unknown)
const FieldObjectEntry* Func7191A8(FieldObjectList* list, u32 index); // 0x007191A8
const FieldObjectEntry* Func7128EC(FieldObjectList* list, u32 index); // 0x007128EC
// the process of a structure kind (an actor of the structure)
u16 Func5CA378(u8 kind); // 0x005CA378
// tests of structure kinds (meanings unknown; Func6CB77C: in the table 0x0088F4F4)
bool Func6CB77C(u8 kind); // 0x006CB77C
// the structure is STRUCTURE_ISLAND_STORAGE_BIN or STRUCTURE_DOCK
bool Func6CD470(u8 kind); // 0x006CD470

// two positions of other files (meanings unknown)
extern nn::math::VEC3 g_Unknown998D5C;
extern nn::math::VEC3 g_Unknown998DA4;
// (before g_Unknown998C70 is cleared; meaning unknown)
void Func3080D0(); // 0x003080D0
// (functions of the module of the actor of the structure kind 89 and of the player house; meanings unknown)
void Func1503E8(Actor* actor); // 0x001503E8 (ModuleOutdoor +0x0381DC)
void Func1503F0(Actor* actor); // 0x001503F0 (ModuleOutdoor +0x0383A8)
void Func150438(AcStrc* strc); // 0x00150438 (ModuleOutdoor +0x054514)

// the object index of the field at a unit (-1: none) / a structure kind is a building of Main Street? (meanings unknown)
s32 Func24063C(s32 x, s32 z, u32 unknown); // 0x0024063C
u32 Func5CAD4C(u8 kind); // 0x005CAD4C
bool Func68F5E8(); // 0x0068F5E8

// an object of another file: its pointer at +0x234 (the bit 0 of its byte +0x5706 is tested; meaning unknown)
struct UnknownAAFF18
{
    u8 _0[0x234];
    void* unk234; // 0x234
};
extern UnknownAAFF18 g_UnknownAAFF18;
extern nn::math::VEC3 g_Unknown998D68;
// tests of the town (meanings unknown)
bool Func2FD624(const u8* data, u32 value); // 0x002FD624
bool Func630854(); // 0x00630854
bool Func630428(); // 0x00630428
// (module functions of the structures; meanings unknown)
f32 Func1503F8(AcStrc* strc); // 0x001503F8 (ModuleOutdoor +0x0540E8)
void Func150400(Actor* actor); // 0x00150400 (ModuleOutdoor +0x03845C)

extern nn::math::VEC3 g_Unknown998D74;
extern nn::math::VEC3 g_Unknown998D80;
// the field gets / loses a placed structure at a unit (for the player houses also 0x006CCB88 / 0x006CCED8; meanings unknown)
void Func6CBFB8(s32 x, s32 z, u8 kind); // 0x006CBFB8
void Func6CCB88(s32 x, s32 z, u8 kind); // 0x006CCB88
void Func6CC320(s32 x, s32 z, u8 kind); // 0x006CC320
void Func6CCED8(s32 x, s32 z, u8 kind); // 0x006CCED8
// a test of the shop of Leif (meaning unknown)
bool Func71D010(const s32* data); // 0x0071D010
void Func150408(Actor* actor); // 0x00150408 (ModuleOutdoor +0x03846C)

// flags of the object 0x009539E4 (meanings unknown)
bool Func4E2B7C(); // 0x004E2B7C
// the unit is free for the structure kinds 220/221 (the data of the structures of the town; meaning unknown)
bool Func27A878(const u8* strcData, s32 x, s32 z); // 0x0027A878
// a structure kind for m_Unknown68C0 (meaning unknown)
bool Func5CB000(u8 kind); // 0x005CB000
// the field gets a placed structure (meanings unknown)
void Func6CD1A4(s32 x, s32 z, u8 kind); // 0x006CD1A4
void Func242E44(s32 x, s32 z, u8 kind); // 0x00242E44
void Func2425D8(s32 x, s32 z, u8 kind); // 0x002425D8
void Func242D00(); // 0x00242D00
struct FieldRect;
void Func6FB77C(FieldRect* rect, bool flag); // 0x006FB77C

// the position of a structure (a module function)
const nn::math::VEC3* Func150428(AcStrc* strc); // 0x00150428 (ModuleOutdoor +0x07E690)

// a value of the day of the stage (island or town; 50 makes it day, 51 night for the structures; meaning unknown)
u32 Func63070C(); // 0x0063070C

// the functions of AcStrc (ModuleOutdoor) and other structure classes of the modules, reached through the import thunks of
// static.crs (meanings unknown)
// the kind of the structure (EndInitialize: 144.. are special)
u8 Func1500B0(AcStrc* strc); // 0x001500B0 (ModuleOutdoor +0x07E768)
bool Func150420(AcStrc* strc); // 0x00150420 (ModuleOutdoor +0x07E31C)
// (ModuleDowntown) the Main Street building stays out of the arrays (meaning unknown)
bool Func150410(AcStrc* strc); // 0x00150410 (ModuleDowntown +0x010780)
// (ModuleExhibition) the structure is a model house (meaning unknown)
bool Func150418(AcStrc* strc); // 0x00150418 (ModuleExhibition +0x0017D8)
// the houses of the sections of the Happy Home Showcase (an object of the game and its list of a section 0..2)
void* Func537D80(); // 0x00537D80
const u8* Func71E484(void* object, u32 section); // 0x0071E484
// the offset of the ticket machine from a town hall (a static function of the module)
const nn::math::VEC3* Func150430(); // 0x00150430 (ModuleOutdoor +0x05C340)
