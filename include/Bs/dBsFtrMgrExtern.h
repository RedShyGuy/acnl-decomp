#pragma once

#include "decomp.h"
#include "nn/math/math_Vector3.h"
#include "math/dAngle16.h"

class Actor;
struct SvFgName;
class Field;
struct SvRoom;
namespace item {
struct ParamItemEntry;
}

// Functions of the game that BsFtrMgr calls and that are not decompiled yet (names after the addresses until their
// owners are known).

// the item has a furniture actor of its own
bool Func749394(const item::ParamItemEntry* entry); // 0x00749394
// the process class of the furniture actor kind (table 0x00A194E4; the default for 0 or kind >= 86)
u16 GetFtrProcName(u32 kind); // 0x005367BC (name is ours)
u16 GetFtrProcNameOfEntry(const item::ParamItemEntry* entry); // 0x0074922C (name is ours; inline copy)
// (the object of Func4EA6D0) all its furniture actors are on the screen? (meaning unknown)
bool Func746234(void* object); // 0x00746234
// the actor of the player of the index (meaning of the flag unknown)
const nn::math::VEC3* Func5C2308(u32 player, bool flag); // 0x005C2308
bool Func5C1B04(u32 player); // 0x005C1B04
// queries of a furniture actor (inline copies of AcFtr; meanings unknown): it takes part in the search of the TV /
// it is a TV and its channel / it is a theremin and the place of its sound / its position
bool Func7510AC(void* actor); // 0x007510AC
bool Func752824(void* actor); // 0x00752824
u8 Func752840(void* actor); // 0x00752840
bool Func7510B8(void* actor); // 0x007510B8
void Func75242C(void* actor, nn::math::VEC3* position, u32 value); // 0x0075242C
const nn::math::VEC3* Func7519D4(void* actor); // 0x007519D4
// the frame of the music of the furniture actor (meaning unknown)
f32 Func5711E4(void* actor); // 0x005711E4
struct SvRoomActiveLayers;
struct SvRoomActiveFlags;
// the active flags of the room of the stage (NULL: none) / a layer (0, 1) of them is copied from source
SvRoomActiveLayers* Func5831D0(u32 stage); // 0x005831D0
void Func5831B8(SvRoomActiveLayers* layers, const SvRoomActiveFlags* source, u32 layer); // 0x005831B8
// tests of an item (meanings unknown; EndInitialize): Func76C828 is true for some custom design kinds,
// Func76910C for items with a ParamItemEntry and a furniture actor of their own
bool Func76C828(const SvFgName* item); // 0x0076C828
bool Func76910C(const SvFgName* item); // 0x0076910C
// queries of a furniture actor (inline copies of AcFtr; meanings unknown): a kind 0/1 (2 and more: 0) / its item
u8 Func752A3C(void* actor); // 0x00752A3C
SvFgName* Func752074(void* actor); // 0x00752074
// a flag of the object 0x009539E4 (the search of the furniture is off)
bool Func4E2B6C(); // 0x004E2B6C
// the unit in front of position looking at angle
void Func5C3200(s32* x, s32* z, const nn::math::VEC3* position, math::Angle16 angle); // 0x005C3200
// (the object of Func4E1338) the field object at a unit (NULL: none) / at a position
void* Func4EA0B0(void* object, s32 x, s32 z, u32 unknown); // 0x004EA0B0
// queries of a furniture actor (meanings unknown): it can be searched (Func7510D0) / it is not searched (Func751EEC) /
// a kind (4: as on the upper layer; Func752D50)
bool Func7510D0(void* actor); // 0x007510D0
bool Func751EEC(void* actor); // 0x00751EEC
u8 Func752D50(void* actor); // 0x00752D50
// (the object of Func4EA6D0) the index of the furniture actor
s32 Func746284(void* object, void* actor); // 0x00746284
// the unit two in front can be reached from position (center: the center of the unit)
bool Func17C5C0(s32 x, s32 z, const nn::math::VEC3* center, const nn::math::VEC3* position); // 0x0017C5C0
namespace bgcheck {
// the movement test of the actors against the collision (not decompiled yet; names are ours)
struct MoveTest
{
    MoveTest(); // 0x006153BC
    ~MoveTest() {}
    // the default mask of the test
    static u32 GetDefaultMask(); // 0x00615238
    // a body of radius moves from from to to
    void Test(const nn::math::VEC3* to, const nn::math::VEC3* from, u32 unknown, f32 radius, u32 unknown2, u32 mask,
              u32 unknown3); // 0x0061553C
    // the test hit something of mask
    bool IsHit(u32 mask) const; // 0x0075EC1C

    u8 _0[0x4C];
};
} // namespace bgcheck
// the units an item covers on the field: its shape and its rotation (game code; names are ours)
struct FtrUnits
{
    // rotation 4: the rotation of the item
    FtrUnits(const SvFgName* item, u32 rotation); // 0x004E9C3C
    // the number of units / the offset (x, z) of a unit from the unit of the item / the center of the item at x, z
    u32 GetNum() const; // 0x0074643C
    const s32* GetOffset(u32 index) const; // 0x0074645C
    void GetCenter(nn::math::VEC3* out, s32 x, s32 z) const; // 0x00746310

    u8 shape;    // 0x0
    u8 rotation; // 0x1
};
// the parameters of an item for the rooms (game code; byte 4 bit 2: the item can stand where the player is)
const u8* Func5360C8(const SvFgName* item); // 0x005360C8
// the type of the furniture of an item (2, 3: it can carry items on top; 5: special wall item)
u8 Func7493C4(const item::ParamItemEntry* entry); // 0x007493C4
// the furniture of the item hangs on the wall
bool Func7491EC(const item::ParamItemEntry* entry); // 0x007491EC
// the bounds of the room (the units of the walls: minimum x/z, maximum x/z)
void Func2854A0(s32* minX, s32* minZ, s32* maxX, s32* maxZ); // 0x002854A0
// the item of a unit of a field (game code; meanings of the values unknown)
const SvFgName* Func4E1374(s32 x, s32 z, u32 unknown, Field* field, u32 unknown2); // 0x004E1374
// a query of a furniture actor (meaning unknown)
bool Func7518B0(void* actor); // 0x007518B0
// a flag of the object 0x009539E4 (meaning unknown)
bool Func4E7890(); // 0x004E7890
// functions of the furniture actors (AcFtr, not decompiled yet; meanings unknown)
bool Func7529D0(void* actor); // 0x007529D0
bool Func75285C(void* actor); // 0x0075285C
u8 Func75288C(void* actor); // 0x0075288C (the kind: 30 AcFtrCloth, 31 AcFtrStand)
void Func56F190(void* actor, u32 state); // 0x0056F190
void Func575054(void* actor, u32 value); // 0x00575054
void Func57298C(void* actor); // 0x0057298C
void Func572FD4(void* actor); // 0x00572FD4
void Func574AF4(void* actor); // 0x00574AF4
void Func574C50(void* actor); // 0x00574C50
void Func752298(void* actor, s32* a, s32* b, u32 unknown, s16* c); // 0x00752298
// the functions of AcFtrCloth (kind 30) and AcFtrStand (kind 31) in ModuleFtr, reached through the import thunks of
// static.crs (meanings unknown)
void* Func150308(void* actor); // 0x00150308 (ModuleFtr +0x0026A8)
void* Func150310(void* actor); // 0x00150310 (ModuleFtr +0x002C84)
void* Func150318(void* actor); // 0x00150318 (ModuleFtr +0x002674)
void* Func150320(void* actor); // 0x00150320 (ModuleFtr +0x002C50)
// a furniture actor can be made of the item (besides Func30B658; meaning unknown)
bool Func76D474(const SvFgName* item); // 0x0076D474
// the custom design of the item becomes design (false: none)
bool Func304E74(SvFgName* item, u32 design); // 0x00304E74
// the data of the custom design of the item (byte 105: the kind of the design; NULL: none)
const void* Func76AD48(const SvFgName* item); // 0x0076AD48
// the weather of the day (the index of the weather program of the TV, 0..10)
u8 Func630B28(); // 0x00630B28
// the stage allows talking to furniture (meaning unknown)
bool Func4E79CC(u32 stage); // 0x004E79CC
