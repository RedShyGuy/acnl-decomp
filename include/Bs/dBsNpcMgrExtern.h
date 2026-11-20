#pragma once

#include "decomp.h"
#include "nn/math/math_Vector3.h"
#include "netgame/dGameGlobals.h"
#include "Other/dBitFlag8.h"

namespace g3d {
class ResourceLoader;
} // namespace g3d
namespace nn {
namespace fnd {
struct DateTimeParameters;
} // namespace fnd
} // namespace nn

// (game code that BsNpcMgr calls and that is not decompiled yet; names after the addresses)

// the two strings are equal (inline copy in the shared code: SafeStrings of both, then isEqual; name is ours)
bool IsEqualString(const char* a, const char* b); // 0x0071C460
// an empty function (BsNpcMgr::Initialize calls it first)
void Func5D4C4C(); // 0x005D4C4C
// empties the list of another manager (0x009863B0)
void Func24FAA4(); // 0x0024FAA4
// the resource files of other managers that the accessories are set up against
g3d::ResourceLoader* Func536700(); // 0x00536700
g3d::ResourceLoader* Func5C22D8(); // 0x005C22D8
// the path of the accessory resources of a kind (Thumb)
const char* Func10BB30(u8 kind); // 0x0010BB30 (Thumb)
// the date matches the day value of the accessory table (meaning unknown)
bool Func309C6C(u8 day, const nn::fnd::DateTimeParameters* date); // 0x00309C6C
// the villager is in a state that takes the accessory away (Thumb; meaning unknown)
bool Func11A3A8(void* npc); // 0x0011A3A8 (Thumb)
// the index of the villager of the stage (Thumb; -1: none)
s32 Func118F30(void* npcs, u8 index); // 0x00118F30 (Thumb)
// after the queued NPCs are created
void Func2C51B0(); // 0x002C51B0

// the limits of an area of units (a global of another file: 0x00994484; names are ours)
struct Unknown994484
{
    // (inline) the limits; the position is clamped into them
    void SetLimits(u8 minX, u8 minZ, u8 maxX, u8 maxZ)
    {
        m_MinX = minX;
        m_MinZ = minZ;
        m_MaxX = maxX;
        m_MaxZ = maxZ;
        if (m_X < m_MinX) {
            m_X = m_MinX;
        } else if (m_X > m_MaxX) {
            m_X = m_MaxX;
        }
        if (m_Z < m_MinZ) {
            m_Z = m_MinZ;
        } else if (m_Z > m_MaxZ) {
            m_Z = m_MaxZ;
        }
    }

    u8 m_X;    // 0x0
    u8 m_Z;    // 0x1
    u8 m_MinX; // 0x2
    u8 m_MinZ; // 0x3
    u8 m_MaxX; // 0x4
    u8 m_MaxZ; // 0x5

    // (meaning unknown) the unit x, z of the area
    void Func7CFF58(s32 x, s32 z); // 0x007CFF58
};
extern Unknown994484 g_Unknown994484;

// the accessories of a season: an event or a day (kind 0: Func309C6C on the date, 1: event::IsActive) and the
// accessory kind (names are ours)
struct AccessoryEntry
{
    u8 eventOrDay; // 0x0
    u8 kind;       // 0x1
    u8 accessory;  // 0x2
};

// the functions of the modules that queue the NPCs of their stage (BsNpcMgr::QueueNpc); static.crs patches them into the
// table of BsNpcMgr::CreateQueuedNpcs (names are ours, after the stages of the table; the module offset in the comment)
void QueueSeaNpcs();             // ModuleEventDemo +0x3100: Sea departure / return
void QueueStationEntryNpcs();    // ModuleStation +0x7104
void QueueStationRetireNpcs();   // ModuleStation +0x71A4
void QueueStationComeHomeNpcs(); // ModuleStation +0x71E4
void QueueStationPrologueNpcs(); // ModuleStation +0x7224
void QueueTourResultNpcs();      // ModuleTour +0x88AC: the results of all tours
void QueueFishAwardNpcs();       // ModuleEventDemo +0x94C
void QueueInsectAwardNpcs();     // ModuleEventDemo +0x2920
void QueueCafeArbeitNpcs();      // ModuleCafe +0x62EC
void QueueAmiiboCameraNpcs();    // ModuleAmiiboCamera +0x2EE8

struct SvFgName;
namespace sead {
template <typename T>
class BufferedSafeStringBase;
} // namespace sead

// the reference height of the units (IsStandableUnit)
f32 Func6FA0C4(); // 0x006FA0C4
// the unit is free (the meanings of the flags unknown)
bool Func51FDB8(s32 x, s32 z, bool a, bool b); // 0x0051FDB8
bool Func3000F0(s32 x, s32 z, u32 value); // 0x003000F0
// a random value below max
u32 Func2FF638(u32 max); // 0x002FF638
// the model of a special NPC with an item (the clothes?; meaning unknown)
void Func20601C(sead::BufferedSafeStringBase<char>* model, SvFgName* item, u32 value); // 0x0020601C
// an item of the island box / of a dresser / of a room of a house
const SvFgName* Func720E88(const SvFgName* items, u32 index); // 0x00720E88
const SvFgName* Func76FEE0(const SvFgName* dressers, u32 a, u32 b, u32 c); // 0x0076FEE0
void* Func70FA3C(void* houses, u8 saveNo); // 0x0070FA3C
const SvFgName* Func770034(void* house, u32 room, u32 z, u32 x); // 0x00770034
// the owner of a field of the second list (save number)
const s8* Func76DE58(void* data, u8 index); // 0x0076DE58
// (static.crs import thunks into a module)
void Func150350(void* house, bool on); // 0x00150350
void Func150358(void* house, bool on); // 0x00150358
// a position for a walking villager from the units of the others, with a unit test (false: none)
bool Func180784(nn::math::VEC3* position, s8 (*units)[2], s32 num, const void* test); // 0x00180784
// the unit tests of PlaceMethod0 / PlaceMethod2 (types unknown)
void Func175178(); // 0x00175178
void Func17C1F8(); // 0x0017C1F8

// the positions of the NPCs (an object of the game behind netgame::g_pUnknown954884; names are ours): a unit as 12-bit
// x and z and the stage
struct NpcPositionEntry
{
    u32 GetX() const { return packed[0] + ((packed[1] & 0xF) << 8); }
    u32 GetZ() const { return (packed[1] >> 4) + (packed[2] << 4); }
    void Set(u32 x, u32 z)
    {
        packed[0] = x;
        packed[1] = (packed[1] & ~0xF) | ((x & 0xF00) >> 8);
        packed[1] = (packed[1] & ~0xF0) | (z << 4);
        packed[2] = z >> 4;
        packed[3] = 0;
    }

    u8 packed[4]; // 0x0
    u8 stage;     // 0x4
    u8 _5[0xB];
};
struct NpcPositionTable
{
    // (63 entries; the index is checked)
    NpcPositionEntry& GetEntry(u32 index) { return entries[static_cast<u8>(index) < 63 ? static_cast<u8>(index) : 0]; }

    u8 _0[0x10];
    NpcPositionEntry entries[63]; // 0x10
};
inline NpcPositionTable* GetNpcPositions()
{
    return static_cast<NpcPositionTable*>(netgame::g_pUnknown954884);
}

// (the system 0x0051Dxxx; meanings unknown)
void Func51D35C(u32 value); // 0x0051D35C
s32 Func51E3DC(); // 0x0051E3DC
void Func51E3EC(s32 value); // 0x0051E3EC
// (Thumb) a value of the villagers (compared with the NPC house of the stage)
s32 Func119244(void* npcs); // 0x00119244 (Thumb)
// the unit test of BsNpcMgr::FindUnitOutside (BkUnitSearchCandCb::Callback)
bool Func174164(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, u32 arg); // 0x00174164
// the second guard box of the town exists / its position
bool Func6FA888(); // 0x006FA888
const nn::math::VEC3_* Func6A76A0(); // 0x006A76A0
// (meaning unknown) a test of two values
bool Func6D0084(u32 a, u32 b, u32 c); // 0x006D0084
class BsNpcMgr;
// (static.crs import thunk into a module) the guard box of a found place
void Func150360(BsNpcMgr* mgr, u32 a, u32 b, s32 index); // 0x00150360
// the callbacks of the guard boxes of the town (ModuleVillage; static.crs patches them into the code of
// SetupVillageGuards; names are ours)
void VillageStationGuard(const nn::math::VEC3* contact, u32 flags, void* object); // ModuleVillage +0x19074
void VillageGuardB(const nn::math::VEC3* contact, u32 flags, void* object);       // ModuleVillage +0x190EC

struct SvNpc;
// (Thumb) a villager gets the state value (meaning unknown)
void Func113A14(SvNpc* npc, u32 value); // 0x00113A14 (Thumb)
// the block of the countdown of the new year (meaning of the block unknown)
void Func6FB074(s32* blockX, s32* blockZ); // 0x006FB074
// the unit of the position is free (the flags unknown)
bool Func51FC50(const nn::math::VEC3* position, bool a, bool b); // 0x0051FC50
// a free position near from (into out) with a unit test (true: found; the arguments unknown)
bool Func51CCF4(nn::math::VEC3* out, const nn::math::VEC3* from, u32 range, const void* test, u32 a, bool b, u32 c); // 0x0051CCF4
// the unit can be stood on (also the unit test of Func51CCF4)
bool Func51D0FC(s32 x, s32 z); // 0x0051D0FC

// the map of the ways between the quarters of the blocks of the town (BsNpcMgr::BuildWalkMap; a global of another file
// whose limits follow at 0x00994484; names are ours): the open directions (0 -x, 1 +x, 2 -z, 3 +z) and a kind (3: the
// block cannot be entered)
struct WalkMapEntry
{
    BitFlag8 dirs; // 0x0
    u8 kind;       // 0x1
};
static const u32 WALK_MAP_WIDTH = 14;
static const u32 WALK_MAP_HEIGHT = 12;
extern WalkMapEntry g_WalkMap[WALK_MAP_WIDTH * WALK_MAP_HEIGHT];
// the block belongs to the town proper (meaning unknown)
bool Func6141C0(s32 blockX, s32 blockZ); // 0x006141C0
// (Thumb) a number of the villagers (meaning unknown)
s32 Func118E74(SvNpc* npcs); // 0x00118E74 (Thumb)
struct BitSet32;
// (static.crs import thunk into a module) the villagers who come along into Main Street
void Func150368(BsNpcMgr* mgr, BitSet32* set); // 0x00150368

// (CalcStageControl) the house of the player / the state test of a villager / a message (meanings unknown)
void* Func52CF24(); // 0x0052CF24
bool Func309B70(SvNpc* npc, u32 value); // 0x00309B70 (Thumb)
bool Func11972C(SvNpc* npcs, u32 index, u32 value); // 0x0011972C (Thumb)
void Func7D2190(u32 kind, const void* data); // 0x007D2190
// the placement functions of the special characters of events (BsNpcMgr::s_PlaceFuncs)
struct NpcInfo;
namespace math {
struct Angle16Vec3;
} // namespace math
namespace stage {
struct NpcInfo;
} // namespace stage
bool Func4DC498(nn::math::VEC3* position, math::Angle16Vec3* rotation, stage::NpcInfo* info); // 0x004DC498
bool Func4DCBB0(nn::math::VEC3* position, math::Angle16Vec3* rotation, stage::NpcInfo* info); // 0x004DCBB0
bool Func4DC9A4(nn::math::VEC3* position, math::Angle16Vec3* rotation, stage::NpcInfo* info); // 0x004DC9A4
bool Func4DCED8(nn::math::VEC3* position, math::Angle16Vec3* rotation, stage::NpcInfo* info); // 0x004DCED8
bool Func4DCE84(nn::math::VEC3* position, math::Angle16Vec3* rotation, stage::NpcInfo* info); // 0x004DCE84
bool Func4DCAF4(nn::math::VEC3* position, math::Angle16Vec3* rotation, stage::NpcInfo* info); // 0x004DCAF4
bool Func10BCDC(nn::math::VEC3* position, math::Angle16Vec3* rotation, stage::NpcInfo* info); // 0x0010BCDC (Thumb)
bool Func10BB3C(nn::math::VEC3* position, math::Angle16Vec3* rotation, stage::NpcInfo* info); // 0x0010BB3C (Thumb)

// the free floor of the room: a bit per unit x in row z (BsNpcMgr::BuildRoomFloor; a global of another file)
extern u16 g_RoomFloor[16];
namespace sead {
template <typename T>
class BufferedSafeStringBase;
} // namespace sead
// the word list of the speech recognition, lines "<id>:<word>" ending with "END" (BsNpcMgr::SetupAsrWords; a string of
// another file, its buffer and owner unknown)
extern sead::BufferedSafeStringBase<char> g_AsrWordList;
// (meanings unknown) the villagers into set / a value of a villager
void Func18D83C(BitSet32* set, bool flag); // 0x0018D83C
u32 Func119FFC(SvNpc* npc, u32 value); // 0x00119FFC (Thumb)

struct FieldRect;
struct SvPlayerBadges;
// (meanings unknown) a test of a date / of a state / of the badges of the player
bool Func751014(const nn::fnd::DateTimeParameters* date, u32 value); // 0x00751014
bool Func630428(); // 0x00630428
bool Func716C78(const SvPlayerBadges* badges); // 0x00716C78
// a rectangle of units of the town (meaning unknown)
void Func6FBF80(FieldRect* rect); // 0x006FBF80
// the block where Isabelle plants the tree
void Func6FB2BC(s32* x, s32* z); // 0x006FB2BC
// after Isabelle is placed at the station
void Func5C933C(); // 0x005C933C
// (Thumb) the structure in front of which Resetti stands
u8 Func106314(); // 0x00106314 (Thumb)
// two constants of another file (Isabelle's offset at the tree)
extern const f32 g_Unknown8822A4;
extern const f32 g_Unknown8822A8;

// (Thumb) a villager gets the value (meaning unknown)
void Func113A0C(SvNpc* npc, u32 value); // 0x00113A0C (Thumb)
// a variable of ModuleKotobuki that BsNpcMgr sets before Tortimer announces (static.crs patches its address; name is
// ours)
extern u8 g_KotobukiAnnounceState; // ModuleKotobuki +0x7800

// a table of the game with entries for the special characters (0x00A8513C; names are ours)
struct UnknownA8513C
{
    struct Entry
    {
        u8 _0[4];
        s8 villager; // 0x4 the index of a villager (-1: none)
        u8 _5[7];
    };

    u32 _0;
    Entry* entries; // 0x4 (72)
};
extern UnknownA8513C g_UnknownA8513C;
// the index of the model in the table (0xFFFF: none)
u16 Func724650(UnknownA8513C* table, const char* model); // 0x00724650
// the stored position of an NPC is updated (meaning unknown)
void Func2069EC(NpcPositionEntry* entry); // 0x002069EC

// the structure the villagers gather at (s_State.unk0F; meaning unknown) and its kind, unit and center
void* Func30CEDC(); // 0x0030CEDC
u32 Func302704(void* object); // 0x00302704
s32 Func718E6C(void* object); // 0x00718E6C
s32 Func718E90(void* object); // 0x00718E90
void Func3361FC(nn::math::VEC3* center, u32 kind); // 0x003361FC
// the direction of a kind of structure (0..3) / its rectangle of units (left, ?, right, front)
u32 Func6CC2E0(u32 kind); // 0x006CC2E0
const s8* Func5CAD04(u32 kind); // 0x005CAD04
bool Func5CAF48(); // 0x005CAF48
// (static.crs import thunk into a module) an offset of the kind of structure
void Func150370(nn::math::VEC3* offset, u32 kind); // 0x00150370


// (used by BsNpcMgr::Func69ED50; meanings unknown)
s32 Func119378(void* npcs); // 0x00119378 (Thumb)
// (meanings unknown) a villager may be in the state 5 / 6 / 21 / 8 and 22 (tests of the time and the host)
bool Func112104(); // 0x00112104 (Thumb)
bool Func111FA8(); // 0x00111FA8 (Thumb)
bool Func1120B8(); // 0x001120B8 (Thumb)
bool Func112004(); // 0x00112004 (Thumb)
s32 Func51D8A4(); // 0x0051D8A4
void Func2FE520(s32 index); // 0x002FE520
bool Func7179B8(u8* club); // 0x007179B8
bool Func717DA4(u8* club, u16 villagerId); // 0x00717DA4
bool Func3094C0(void* data, const nn::fnd::DateTimeParameters* date); // 0x003094C0
// 64 bits returned in memory (bit 16 tested by Func69ED50)
struct Bits64
{
    u8 bytes[8];
};
Bits64 Func30E90C(SvNpc* npcs, SvNpc* npc, u32 a, u32 b, u32 c, u32 d); // 0x0030E90C (Thumb)
struct SvPersonalId;
bool Func11A7DC(SvNpc* npc, SvPersonalId* player); // 0x0011A7DC (Thumb)
bool Func309DDC(u8* data); // 0x00309DDC
bool Func75863C(u8* data, const nn::fnd::DateTimeParameters* date, u32 value); // 0x0075863C
s32 Func30FCCC(); // 0x0030FCCC
bool Func309DC8(void* data); // 0x00309DC8
// (static.crs import thunk into ModuleNpcDt; meaning unknown) the position of a villager in a stage of AcNpcDt
void Func150158(nn::math::VEC3* position, bool flag); // 0x00150158
// (meanings unknown) a test of a villager / a villager index of the villagers (CreateAnniversaryNpcs)
bool Func11B244(const SvNpc* npc); // 0x0011B244 (Thumb)
s32 Func30977C(void* npcs, u32 value); // 0x0030977C (Thumb)
// (meaning unknown) no event of Func308E74 or the event 10 / a test of a villager
bool Func51D0CC(); // 0x0051D0CC
bool Func119900(const SvNpc* npc); // 0x00119900 (Thumb)
// (used by the daily plans of the villagers, BsNpcMgr::Func6A0C98/Func6A0F9C/Func6A10D8; meanings unknown)
// the states into states (with the value) / some of them out again
void Func30A270(BitSet32* states, u32 value); // 0x0030A270
void Func51DD94(BitSet32* states); // 0x0051DD94
s32 Func11A084(const SvNpc* npc); // 0x0011A084 (Thumb)
const u8* Func30A1C4(SvNpc* npc, SvPersonalId* player); // 0x0030A1C4 (Thumb)
bool Func11A668(const SvNpc* npc); // 0x0011A668 (Thumb)
bool Func11AB1C(const SvNpc* npc, s32 index); // 0x0011AB1C (Thumb)
bool Func119474(SvNpc* npcs, s32 value); // 0x00119474 (Thumb)
s32 Func1194F8(SvNpc* npcs, u32 index); // 0x001194F8 (Thumb)
s32 Func309A70(void* extra); // 0x00309A70
const u8* Func309D90(SvNpc* npc, SvPersonalId* player); // 0x00309D90 (Thumb)
bool Func717934(u8* club); // 0x00717934
bool Func7176C4(u8* club); // 0x007176C4
// (meaning unknown) the cafe guest of the key may come on the date (data: the garden data + 0x70E60)
bool Func720B4C(u8* data, u8 key, const nn::fnd::DateTimeParameters* date); // 0x00720B4C
// (used by BsNpcMgr::CreateVillageNpcs; meanings unknown) static.crs import thunks into ModuleDream (the NPCs of the
// Dream Suite), ModuleRealVillage and ModuleCeremony (the ceremony of a structure: setup, the place of the secretary
// and a guest)
bool Func150328(BsNpcMgr* mgr); // 0x00150328
bool Func150330(); // 0x00150330
void Func150338(u32 kind, s32 x, s32 z); // 0x00150338
void Func150340(nn::math::VEC3* position, u32 kind); // 0x00150340
struct CeremonyGuest
{
    const char* model; // 0x0
    f32 x;             // 0x4 offset from the structure
    f32 z;             // 0x8
};
const CeremonyGuest* Func150348(u32 kind); // 0x00150348
bool Func333628(); // 0x00333628
bool Func70BEDC(void* object); // 0x0070BEDC
void Func2D4BC0(u8* data, const stage::NpcInfo* info); // 0x002D4BC0
// (meanings unknown) a stage of the value / the id of the NPC of a kind (0xFFFF: none; BsNpcMgr::CreateShopNpcs)
u8 Func51DFB8(u32 a, u32 b); // 0x0051DFB8
u16 Func51D990(u8 kind, u32 value); // 0x0051D990
// (the Thumb part of BsNpcMgr; meanings unknown) static.crs import thunks into the modules that create the NPCs of
// their stages
bool Func14FCD0(BsNpcMgr* mgr); // 0x0014FCD0 (ModuleDowntown)
bool Func14FCD8(BsNpcMgr* mgr); // 0x0014FCD8 (ModuleDowntown)
bool Func14FCE0(BsNpcMgr* mgr); // 0x0014FCE0 (ModuleIndoor)
bool Func14FCE8(BsNpcMgr* mgr); // 0x0014FCE8 (ModuleTour)
bool Func14FCF0(BsNpcMgr* mgr); // 0x0014FCF0 (ModuleIndoor)
bool Func14FCF8(BsNpcMgr* mgr); // 0x0014FCF8 (ModuleIndoor)
bool Func14FD00(BsNpcMgr* mgr); // 0x0014FD00 (ModuleIndoor)
bool Func14FD08(BsNpcMgr* mgr); // 0x0014FD08 (ModuleIndoor)
bool Func14FD10(BsNpcMgr* mgr); // 0x0014FD10 (ModuleIndoor)
bool Func14FD18(BsNpcMgr* mgr); // 0x0014FD18 (ModuleIndoor)
bool Func14FD20(BsNpcMgr* mgr); // 0x0014FD20 (ModuleIndoor)
bool Func14FD28(BsNpcMgr* mgr); // 0x0014FD28 (ModuleExhibition)
bool Func14FD30(BsNpcMgr* mgr); // 0x0014FD30 (ModuleIndoor)
void Func14FD38(BsNpcMgr* mgr); // 0x0014FD38 (ModuleDowntown)
void Func14FD40(BsNpcMgr* mgr); // 0x0014FD40 (ModuleExhibition)
void Func14FCB0(); // 0x0014FCB0 (ModuleIndoor)
// (game code, meanings unknown)
void Func30B458(u32 value); // 0x0030B458
void Func111064(SvNpc* npcs); // 0x00111064 (Thumb)
void Func51DB48(); // 0x0051DB48
void Func29D0E0(void* object); // 0x0029D0E0
void Func68FCFC(); // 0x0068FCFC
void Func12DD40(BsNpcMgr* mgr); // 0x0012DD40
void Func12DAE4(); // 0x0012DAE4
struct SvPlayerFlags;
void Func1E29AC(SvPlayerFlags* flags, u32 value); // 0x001E29AC
// the road search of CreatePlayerGhostInHouse (RoadSearchCand<16, 16, IRootLocateZero>, not decompiled yet): constructed,
// prepared, given the callback, started at the unit (7, 13); a free unit (false: none)
struct HouseGhostSearch
{
    u8 _0[0x1460];
};
void Func7D05DC(HouseGhostSearch* search); // 0x007D05DC
void Func7CF740(HouseGhostSearch* search); // 0x007CF740
void Func7CF8F0(HouseGhostSearch* search, void* callback); // 0x007CF8F0
void Func7D2F64(HouseGhostSearch* search, s32 x, s32 z, u32 value); // 0x007D2F64
bool Func826F84(HouseGhostSearch* search, s32* x, s32* z); // 0x00826F84
// a value holding a player and the NPC info of it (kind 2 = stage::NpcInfo::KIND_VALUE)
struct SvPlayer;
void Func29CF18(u32* value, SvPlayer* player); // 0x0029CF18
void Func29CF30(stage::NpcInfo* info, const u32* value); // 0x0029CF30
