#pragma once

#include "decomp.h"
#include "math/dAngle16.h"
#include "nn/math/math_Vector3.h"
#include "netgame/netgame_Api.h"
#include "Player/dPlayerApi.h"
#include "Sv/dSvApi.h"
#include "stage/stage_Api.h"

// The functions and objects of the game that the stage code uses, as far as it needs them. Their classes are not
// decompiled yet; all names are ours unless noted, the declarations move to their classes later.

// the house save of a player house stage, or of the model house (NULL for another stage)
SvPlayerHouse* GetHouseOfStage(stage::Name name); // 0x006F9774
// the owner (village player 0..3) of a player house stage; 4 for another stage
u32 GetHouseOwner(stage::Name name); // 0x006FAF5C

// the players
// places the player at the start of the stage
bool PlayerSetStart(netgame::PlayerNo no, const nn::math::VEC3_* position, const math::Angle16Vec3* rotation, u32 action,
                    u32 arg); // 0x005C2260
void SetPlayerArrivalType(u8 type); // 0x005C5DE8
// whether the player can take a return point at the entrance (the own player actor exists and agrees)
bool PlayerCanReturn(); // 0x005C3708
// the position and rotation of the own player; returns the action to take on return
u32 PlayerGetReturnPoint(nn::math::VEC3_* pPosition, math::Angle16Vec3* pRotation); // 0x005C7B18
// two positions in the campground stages (meaning unknown; near one of them the entrance becomes the return point)
void GetAutoCampPoints(nn::math::VEC3_* pPoint0, nn::math::VEC3_* pPoint1); // 0x00150258

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
extern const u32 PLAYER_ACTION_HIDE;
extern const u32 PLAYER_ACTION_IDLE;
extern const u32 PLAYER_ACTION_EXIT_CONNECTION_WALK;
extern const u32 PLAYER_ACTION_EXIT_CONNECTION_WALK_SLOW;
extern const u32 PLAYER_ACTION_EXIT_CONNECTION_RUN;
extern const u32 PLAYER_ACTION_EXIT_AUTOCAMP;
extern const u32 PLAYER_ACTION_EXIT_CONNECTION_STAIR_UP;
extern const u32 PLAYER_ACTION_EXIT_CONNECTION_STAIR_DOWN;
extern const u32 PLAYER_ACTION_DOOR_EXIT_WAIT;
extern const u32 PLAYER_ACTION_DOOR_EXIT_BEGIN;
extern const u32 PLAYER_ACTION_FTR_BED_SLEEP;
extern const f32 PLAYER_BED_SLEEP_OFFSET;
// the start and entrance position of the player in CampingCarA/B
extern const nn::math::VEC3 CAMPING_CAR_ENTRANCE_POSITION;

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
// the position and rotation of the way to the campground in the village
const nn::math::VEC3_* GetAutoCampPosition(); // 0x006A76A0
const u16* GetAutoCampRotationY(); // 0x006A7744
// the stage of the campground (AutoCampL: west, AutoCampR: east)
stage::Name GetAutoCampStage(); // 0x006FA0A0

// the structures of the field: the building IDs of the building ID list https://redshyguy.github.io/ACNL_ID_Lists/
// (0..3 the player houses, 8..0x11 the villager houses, 0x42..0x4B "Invalid Villager House" in the list, which are
// also the structures of Island_Hut0..9 (stage - 63, TransitionFromTourHut), 0x4C.. the special PWPs, 0x81..0x8F the
// buildings of Main Street; 0xFC "Empty Space")
enum StructureId : u8
{
    STRUCTURE_TOWN_HALL_CLASSIC = 0x50,
    STRUCTURE_TOWN_HALL_ZEN = 0x51,
    STRUCTURE_TOWN_HALL_FAIRY_TALE = 0x52,
    STRUCTURE_TOWN_HALL_MODERN = 0x53,
    STRUCTURE_CAMPING_CAR_1 = 0x7F,
    STRUCTURE_CAMPING_CAR_2 = 0x80,
    STRUCTURE_NOOKLING_STORES = 0x85,
    STRUCTURE_PHOTO_BOOTH = 0x8D,
    STRUCTURE_EMPTY_SPACE = 0xFC,
};
bool IsStructurePlaced(u8 structure); // 0x0030940C
// the position in front of the structure; false if it is not on the field
bool GetStructurePosition(nn::math::VEC3_* pPosition, u8 structure); // 0x003071EC
stage::Name GetStageOfStructure(u8 structure); // 0x005CAE68
// the structures with variants: 0x50..0x53 the town hall, 0x54..0x57 the train station (classic, zen, fairy-tale, modern)
bool IsTownHallStructure(u8 structure); // 0x006CD4A8
bool IsStationStructure(u8 structure); // 0x006CD4CC
u8 GetTownHallStructure(); // 0x006CC580
u8 GetStationStructure(); // 0x006CC734
bool IsPlayerHouseStructure(u8 structure); // 0x006CB82C
// the model name of the Nookling General Stores on Main Street ("shop" .. "depart")
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

// the type of a tour of the island (TransitionFromTourHut: the huts lead back to Island_FossilDig for 9 and to
// Island_BorrowingRace for 11)
s8 GetTourType(s8 tour); // 0x006C4DD0
// whether the current stage is a tour or a hut of the island
bool IsTourStage(); // 0x006E167C
bool IsHutOfFossilTour(stage::Name name); // 0x004E750C
// whether no one else is in the room of the current stage
bool IsAloneInRoom(); // 0x006E0960

