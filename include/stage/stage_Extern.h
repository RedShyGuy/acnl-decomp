#pragma once

#include "decomp.h"
#include "math/dAngle16.h"
#include "nn/math/math_Vector3.h"
#include "netgame/netgame_Api.h"
#include "Player/dPlayerApi.h"
#include "stage/stage_Api.h"

// The functions and objects of the game that the stage code uses, as far as it needs them. Their classes are not
// decompiled yet; all names are ours unless noted, the declarations move to their classes later.

// the save data
struct SvShowroom
{
    s8 owner; // 0x0 the player save number of the owner (-1: none)
};
class SvMuseum
{
public:
    SvShowroom* GetShowroom(u32 index); // 0x006BCED8
};
class SvHouse
{
public:
    // the size of a room of the house
    u8 GetRoomSize(u32 room) const; // 0x0072108C

    u8 _unknown0[4];
    u8 m_State; // 0x4 0: a tent, 8: being built (meaning of the other values unknown)
};
class SvHouseList
{
public:
    SvHouse* Get(u32 owner); // 0x0030F770
};
// the save data in memory (only the parts that the stage code reads)
class Garden
{
public:
    u8 _unknown0[0x5D900];
    SvHouseList m_Houses; // 0x5D900
};
class GardenData
{
public:
    u8 _unknown0[0x6AE38];
    SvMuseum m_Museum; // 0x6AE38
};
Garden* GetGarden(); // 0x002FB344
// the save data behind the header of the save file
GardenData* GetGardenData(); // 0x002FAEAC
// the house save of a player house stage, or of the model house (NULL for another stage)
SvHouse* GetHouseOfStage(stage::Name name); // 0x006F9774
// the owner (village player 0..3) of a player house stage; 4 for another stage
u32 GetHouseOwner(stage::Name name); // 0x006FAF5C

// the players
PlayerSaveNumber PlayerGetSaveNumber(netgame::PlayerNo no); // 0x002FF6CC
bool IsVillagePlayer(u8 saveNumber); // 0x0027B0FC
// places the player at the start of the stage
bool PlayerSetStart(netgame::PlayerNo no, const nn::math::VEC3_* position, const math::Angle16Vec3* rotation, u32 action,
                    u32 arg); // 0x005C2260
void SetPlayerArrivalType(u8 type); // 0x005C5DE8
// whether the player can take a return point at the entrance (the own player actor exists and agrees)
bool PlayerCanReturn(); // 0x005C3708
// the position and rotation of the own player; returns the action to take on return
u32 PlayerGetReturnPoint(nn::math::VEC3_* pPosition, math::Angle16Vec3* pRotation); // 0x005C7B18
// the two door positions of the camping car in the auto camp
void GetAutoCampDoors(nn::math::VEC3_* pDoor0, nn::math::VEC3_* pDoor1); // 0x00150258

// a start position of the players after a scene change
struct PlayerStartPosition
{
    PlayerStartPosition(); // 0x005C02CC
    PlayerStartPosition(const nn::math::VEC3_& position, const math::Angle16Vec3& rotation); // 0x005C029C

    nn::math::VEC3_ position; // 0x00
    u16 rotationY;            // 0x0C
    u32 _10;                  // 0x10
};
// where the players start in the next stage
class PlayerStartInfo
{
public:
    void Reset(); // 0x005C03C0
    void Set(stage::Name name, u32 hostAction, u32 guestAction, const PlayerStartPosition& position0,
             const PlayerStartPosition& position1, const PlayerStartPosition& position2,
             const PlayerStartPosition& position3); // 0x005C02FC

    PlayerStartPosition m_Positions[4]; // 0x00
    stage::Name m_Stage;                // 0x50
    u32 m_HostAction;                   // 0x54
    u32 m_GuestAction;                  // 0x58
};
extern PlayerStartInfo g_PlayerStartInfo;

// the actions of the player on arrival (values of libgarden's PlayerAction::Name, as constants of another file)
extern const u32 PLAYER_ACTION_IDLE;
extern const u32 PLAYER_ACTION_EXIT_CONNECTION_WALK;
extern const u32 PLAYER_ACTION_EXIT_CONNECTION_WALK_SLOW;
extern const u32 PLAYER_ACTION_EXIT_CONNECTION_RUN;
extern const u32 PLAYER_ACTION_EXIT_CONNECTION_STAIR_UP;
extern const u32 PLAYER_ACTION_EXIT_CONNECTION_STAIR_DOWN;
extern const u32 PLAYER_ACTION_DOOR_EXIT_WAIT;
extern const u32 PLAYER_ACTION_DOOR_EXIT_BEGIN;

// the field
extern const f32 FIELD_UNIT_SIZE_X; // one unit in world units
extern const f32 FIELD_UNIT_SIZE_Z;
extern const f32 FIELD_HALF_UNIT_SIZE_X;
extern const f32 FIELD_HALF_UNIT_SIZE_Z;
extern const nn::math::VEC3 VEC3_ZERO;
extern const math::Angle16Vec3 ANGLE16_VEC3_ZERO;
// rotates the vector around the Y axis
void RotateY(nn::math::VEC3_* v, math::Angle16 angle); // 0x0056AD0C
// a random value in [0, max)
f32 GetRandomF(f32 max); // 0x005666F8
void SetupField(); // 0x006A6AFC
void DestroyField(); // 0x006A7424
void ResetStageList(); // 0x006E4FE0
// the position and rotation of the auto camp in the village
const nn::math::VEC3_* GetAutoCampPosition(); // 0x006A76A0
const u16* GetAutoCampRotationY(); // 0x006A7744
// the stage of the auto camp (AutoCampL or AutoCampR)
stage::Name GetAutoCampStage(); // 0x006FA0A0

// the structures of the field (0..3: the player houses)
bool IsStructurePlaced(u8 structure); // 0x0030940C
// the position in front of the structure; false if it is not on the field
bool GetStructurePosition(nn::math::VEC3_* pPosition, u8 structure); // 0x003071EC
stage::Name GetStageOfStructure(u8 structure); // 0x005CAE68
// the structures with variants: 80..83 the museum, 84..87 the shop of the Nooks (names of the groups are ours)
bool IsMuseumStructure(u8 structure); // 0x006CD4A8
bool IsShopStructure(u8 structure); // 0x006CD4CC
u8 GetMuseumStructure(); // 0x006CC580
u8 GetShopStructure(); // 0x006CC734
bool IsPlayerHouseStructure(u8 structure); // 0x006CB82C
// the model name of the shop of the Nooks ("shop" .. "depart")
const char* GetShopName(); // 0x006DD8A0

// the objects of the field: a registry of 14 lists
struct FieldObject;
struct FieldObjectList;
struct FieldObjectIterator
{
    FieldObjectIterator(FieldObjectList* list); // 0x00314928 the first object
    FieldObjectIterator& operator++(); // 0x003149E4
    bool operator!=(const FieldObjectIterator& other) const; // 0x00314990

    u32 index;              // 0x00
    FieldObjectList* list;  // 0x04
    s32 unitX;              // 0x08
    s32 unitZ;              // 0x0C
    u8 kind;                // 0x10
    u8 _pad11[7];
};
FieldObjectList* GetFieldObjectList(u32 type); // 0x00314890
FieldObjectIterator GetFieldObjectEnd(FieldObjectList* list); // 0x003148A8
// the world position of a unit
void UnitToWorld(nn::math::VEC3_* pPosition, s32 unitX, s32 unitZ); // 0x006A69E4

// creates an actor with its position
oml::framework::Process* ActorCreate(u16 name, oml::framework::Process* parent, u32 arg, const nn::math::VEC3_* position,
                                     const math::Angle16Vec3* rotation); // 0x0057C4C4

// the type of a tour of the island (9: fossil digging, 11: borrowing race)
s8 GetTourType(s8 tour); // 0x006C4DD0
// whether the current stage is a tour or a hut of the island
bool IsTourStage(); // 0x006E167C
bool IsHutOfFossilTour(stage::Name name); // 0x004E750C
// whether no one else is in the room of the current stage
bool IsAloneInRoom(); // 0x006E0960

