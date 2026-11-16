#pragma once

#include "decomp.h"
#include "netgame/netgame_Api.h"
#include "stage/stage_Api.h"
#include "Sv/dSvApi.h"
#include <cstring>

class SvFgName;
struct SvPlayer;
#include "nn/math/math_Vector3.h"

// Functions of the game that the field object code calls and that are not decompiled yet (names after the addresses
// until their owners are known).

// the objects 0x009C8AD4 / 0x009C8A14 (types unknown)
void* Func4E1338(); // 0x004E1338
void* Func4EA6D0(); // 0x004EA6D0
// a value of the unit x, z in the object of Func4E1338 (-1 / 0 without; the last argument is a layer)
s32 Func7464A0(void* object, s32 x, s32 z, u32 layer); // 0x007464A0
// the entry index of the object of Func4EA6D0 (NULL if out of range)
void* Func4E9C14(void* object, s32 index); // 0x004E9C14
// the u16 at +0xC of the entry is the value 0x008880A2
bool Func7529D0(const void* entry); // 0x007529D0
// a height at the unit x, z: of an entry of the object 0x009C8AD4 there, else the height of the unit (FdInfo)
f32 Func4EA670(s32 x, s32 z); // 0x004EA670
// the light set of the models of the items on the current stage (0: none)
s32 Func1E8920(); // 0x001E8920
// the kind of ground of the unit x, z (1: sand; the attributes of the units around are compared)
s32 Func300B7C(s32 x, s32 z, bool unknown); // 0x00300B7C
// functions with the actor of this player (PlayerGetActor(4)) when a balloon present has landed (on the ground / in the
// water; meaning unknown)
void Func67D0F8(); // 0x0067D0F8
void Func5C8834(); // 0x005C8834
// functions of the game at the position where an ObjectHoneycomb has landed (meaning unknown; 0x002AAF80 lies in the code
// that reads s_Unk950530, 0x005C61E4 works with the actor of this player)
void Func2AAF80(const nn::math::VEC3* position); // 0x002AAF80
void Func5C61E4(const nn::math::VEC3* position); // 0x005C61E4
// the field object item at the unit x, z of layer on stage (the field code; name after the address)
void Func168E20(s32 x, s32 z, u8 layer, stage::Name stage, const SvFgName& item); // 0x00168E20
// a count of the game after Func6F0C80 / Func24BD74 (meaning unknown; it works on the garden data)
void Func2A83B0(); // 0x002A83B0
// the byte 0x00951800 (meaning unknown)
bool Func6E13E0(); // 0x006E13E0
// the position of a carried item of the player (meaning unknown; false without it)
bool Func5C3D1C(nn::math::VEC3* out, netgame::PlayerNo player); // 0x005C3D1C
// an object of the actor of the player (+0x5EC; NULL without the actor) and a sound with it (names after the addresses)
void* Func5C119C(netgame::PlayerNo player); // 0x005C119C
void Func2505E0(void* object, u32 soundId); // 0x002505E0
// after the player buried item at x, z (isOrdered: demo::CheckOrder(demo::PLAYER_THINK)); meaning unknown
void Func5C3B70(s32 x, s32 z, bool isOrdered, const SvFgName* item); // 0x005C3B70
// after a pulled weed or a dug hole at x, z (isHole: the hole of a dig; meaning unknown)
void Func1758B0(const void* action, s32 x, s32 z, bool isHole); // 0x001758B0
// a chance for the player after a pulled weed (meaning unknown)
void Func20C6E0(SvPlayer* player); // 0x0020C6E0
// the pid of a player that pulled a weed goes to the network (kind 4; meaning unknown)
struct PulledWeedPacket
{
    u16 pid;
    u8 player;
};
void Func7D1DCC(s32 kind, const PulledWeedPacket* packet); // 0x007D1DCC
// whether an item can fall onto the unit x, z of stage (meaning of the last arguments unknown)
bool Func165460(s32 x, s32 z, stage::Name stage, u32 unknown, bool unknown2); // 0x00165460
// progress of an initiative of the player (id, count; nothing in the dream; meaning of the ids unknown)
struct SvPlayerInitiative;
void Func24BA04(SvPlayerInitiative* initiative, s32 id, s32 count); // 0x0024BA04
// whether the unit x, z can be reached from position (the item of a shaken tree falls only there; meaning unknown)
bool Func614B90(s32 x, s32 z, const nn::math::VEC3* position); // 0x00614B90
// items of a group are added to set (the item of a shaken item tree: group 5 with the values 1, 5, 8 or 9 by
// g_Unk954F88; meaning of the arguments unknown)
struct SvFgNameSet;
void Func6F3458(SvFgNameSet* set, s32 group, s32 value, bool unknown, u32 unknown2); // 0x006F3458
// a byte of the game (the coin stone gives more money for 0, less for 1; owner and meaning unknown)
extern u8 g_Unk954F88;
// the special state of the town for the items of trees and coin stones (g_Unk954F88; meaning unknown)
inline bool IsTownState(u8 value)
{
    return netgame::GetNetgameType() != netgame::TYPE_DREAM && g_UnkAAFF18.pPlayer != NULL && g_Unk954F88 == value;
}
// the effects of a trampled flower fg at the unit x, z (isTrampled: it is removed; meaning unknown)
void Func1703C0(const SvFgName* fg, s32 x, s32 z, bool isTrampled); // 0x001703C0
// the pointer 0x009B5A24 is set (meaning unknown)
bool Func6A7480(); // 0x006A7480
// a function with the player after a flower was watered (meaning unknown)
void Func20C714(SvPlayer* player); // 0x0020C714
// whether an item can be put on something at the unit x, z (layer 1; meaning unknown)
bool Func4E3D48(s32 x, s32 z); // 0x004E3D48
// a test of the letters of a player (SvPlayer::letters; negative: none free?; meaning unknown)
struct SvMail;
s32 Func71E954(const SvMail* letters); // 0x0071E954
// a function with the loaded graphics file of the season and the heap of it (meaning unknown)
namespace g3d {
class ResourceLoader;
} // namespace g3d
namespace sead {
class Heap;
} // namespace sead
void Func179330(g3d::ResourceLoader* loader, sead::Heap* heap); // 0x00179330
// the field objects indoors: a function of ModuleFtr through an import thunk (meaning unknown)
namespace fgobj {
class Proc;
} // namespace fgobj
void Func150218(fgobj::Proc* proc); // 0x00150218
s32 Func150220(fgobj::Proc* proc, s32 a, s32 b); // 0x00150220
// the field object indoors is registered with ModuleFtr (the handle goes to ObjectBase::m_Unk0x114)
namespace fgobj {
class ObjectBase;
} // namespace fgobj
s32 Func150230(fgobj::ObjectBase* object); // 0x00150230
s32 Func150228(fgobj::Proc* proc, s32 a, s32 b, s32 c); // 0x00150228
// whether a sphere of radius at position is outside the view (meaning unknown) / the shadow goes to the lists of the shadow
// drawing (names after the addresses)
bool Func68F5A8(const nn::math::VEC3* position, f32 radius); // 0x0068F5A8
namespace graphic {
struct Shadow;
} // namespace graphic
void Func6EFBF0(graphic::Shadow* shadow); // 0x006EFBF0
// a kind of the ground of a block (the byte table 0x0094FBE1; meaning unknown)
u8 Func6060F8(u16 bgName); // 0x006060F8
// a pointer of the import table of a module (in the code; NULL while the module is not loaded) and the function of it
// that gives the scale of an object of the field indoors (false: the object keeps no scale; meaning unknown)
extern void* g_pUnk7B2C88;
bool Func150238(void* module, f32* scale, s32 id); // 0x00150238
// the object 0x00950534 and a value of it (index 12; meaning unknown)
extern void* g_pUnk950534;
const u32* Func2AB114(const void* object, s32 index); // 0x002AB114
// the last three byte pairs of a BytePairPacket in the town data from SvTownData::unk_0xEE0E (pairs at +0x10, number
// +0x16, index of the last +0x17; meaning unknown)
void Func52B188(void* object, u8 a, u8 b); // 0x0052B188
// the actor of the player (PlayerGetActor) when two of its flags at +0x66 are set, else NULL (meaning unknown)
void* Func5C481C(netgame::PlayerNo player, bool flag); // 0x005C481C
// a value of the table 0x0095314C (ten columns) in the row of the season part of the current date (time::GetSeasonPart);
// the field objects take the index of their effects from it
s32 Func1E6444(s32 column); // 0x001E6444

namespace fgobj {
class ObjectBase;
} // namespace fgobj

// functions of ModuleFtr that fgobj calls through import thunks indoors (static.crs: anonymous imports of the module)
void Func150200(fgobj::ObjectBase* object); // 0x00150200 (ModuleFtr.cro +0x01D1B0)
void Func150208(s16 handle); // 0x00150208 (ModuleFtr.cro +0x01C450)
s16 Func150210(fgobj::ObjectBase* object); // 0x00150210 (ModuleFtr.cro +0x01C400)

// set when a thrown item of item::KIND_CANDY, KIND_LOLLIPOP, KIND_BAD_FRUIT or KIND_BAD_KABU (s_Unk950530) / of KIND_DUST
// or KIND_BAD_KABU (s_Unk950531) lands outdoors without other players (ObjectThrow::Execute); read by the game code at
// 0x002AAB8C.. (owner and meaning unknown)
extern bool s_Unk950530;
extern bool s_Unk950531;

// a bit for each unit of a field of 112 x 112 units and a flag that one was set (owner and meaning unknown: the fountain
// fireworks set the units around them while they burn; read by AcFsFdShadow::vf_0x60 and fgobj::Proc::Calc)
struct UnitBitsAAF736
{
    static const u32 SIZE = 112;

    // (inline) the bit of the unit x, z (nothing outside)
    void Set(s32 x, s32 z)
    {
        u32 index = (static_cast<u32>(x) < SIZE && static_cast<u32>(z) < SIZE) ? x + z * SIZE : SIZE * SIZE;
        if (index < SIZE * SIZE) {
            bits[index >> 3] |= 1 << (index & 7);
            isSet = true;
        }
    }

    // (inline) all bits and the flag are cleared (fgobj::Proc::Calc)
    void Clear()
    {
        std::memset(bits, 0, sizeof(bits));
        isSet = false;
    }

    u8 bits[SIZE * SIZE / 8];   // 0x000
    bool isSet;                 // 0x620
};
extern UnitBitsAAF736 g_UnitBitsAAF736;
