#pragma once

#include "decomp.h"
#include "math/dAngle16.h"
#include "nn/math/math_Vector3.h"
#include "Other/dField.h"

namespace oml {
namespace framework {
class Process;
} // namespace framework
} // namespace oml

namespace stage {
// The stages of the game: the village, the houses, the shops, the island and the screens without a field (names after
// the stage name table 0x009585F8 of the binary, written like libgarden's stage::Name).
enum Name : u8
{
    NAME_VILLAGE = 0, // "Village"
    NAME_DOWNTOWN = 1, // "Downtown"
    NAME_STATION = 2, // "Station"
    NAME_PLAYER_HOUSE_0_1F = 3, // "PlayerHouse0_1F"
    NAME_PLAYER_HOUSE_0_2F = 4, // "PlayerHouse0_2F"
    NAME_PLAYER_HOUSE_0_BF = 5, // "PlayerHouse0_BF"
    NAME_PLAYER_HOUSE_0_RIGHT = 6, // "PlayerHouse0_Right"
    NAME_PLAYER_HOUSE_0_LEFT = 7, // "PlayerHouse0_Left"
    NAME_PLAYER_HOUSE_0_BACK = 8, // "PlayerHouse0_Back"
    NAME_PLAYER_HOUSE_1_1F = 9, // "PlayerHouse1_1F"
    NAME_PLAYER_HOUSE_1_2F = 10, // "PlayerHouse1_2F"
    NAME_PLAYER_HOUSE_1_BF = 11, // "PlayerHouse1_BF"
    NAME_PLAYER_HOUSE_1_RIGHT = 12, // "PlayerHouse1_Right"
    NAME_PLAYER_HOUSE_1_LEFT = 13, // "PlayerHouse1_Left"
    NAME_PLAYER_HOUSE_1_BACK = 14, // "PlayerHouse1_Back"
    NAME_PLAYER_HOUSE_2_1F = 15, // "PlayerHouse2_1F"
    NAME_PLAYER_HOUSE_2_2F = 16, // "PlayerHouse2_2F"
    NAME_PLAYER_HOUSE_2_BF = 17, // "PlayerHouse2_BF"
    NAME_PLAYER_HOUSE_2_RIGHT = 18, // "PlayerHouse2_Right"
    NAME_PLAYER_HOUSE_2_LEFT = 19, // "PlayerHouse2_Left"
    NAME_PLAYER_HOUSE_2_BACK = 20, // "PlayerHouse2_Back"
    NAME_PLAYER_HOUSE_3_1F = 21, // "PlayerHouse3_1F"
    NAME_PLAYER_HOUSE_3_2F = 22, // "PlayerHouse3_2F"
    NAME_PLAYER_HOUSE_3_BF = 23, // "PlayerHouse3_BF"
    NAME_PLAYER_HOUSE_3_RIGHT = 24, // "PlayerHouse3_Right"
    NAME_PLAYER_HOUSE_3_LEFT = 25, // "PlayerHouse3_Left"
    NAME_PLAYER_HOUSE_3_BACK = 26, // "PlayerHouse3_Back"
    NAME_NPC_HOUSE_0 = 27, // "NpcHouse0"
    NAME_NPC_HOUSE_1 = 28, // "NpcHouse1"
    NAME_NPC_HOUSE_2 = 29, // "NpcHouse2"
    NAME_NPC_HOUSE_3 = 30, // "NpcHouse3"
    NAME_NPC_HOUSE_4 = 31, // "NpcHouse4"
    NAME_NPC_HOUSE_5 = 32, // "NpcHouse5"
    NAME_NPC_HOUSE_6 = 33, // "NpcHouse6"
    NAME_NPC_HOUSE_7 = 34, // "NpcHouse7"
    NAME_NPC_HOUSE_8 = 35, // "NpcHouse8"
    NAME_NPC_HOUSE_9 = 36, // "NpcHouse9"
    NAME_CAMP_NPC = 37, // "CampNpc"
    NAME_OFFICE = 38, // "Office"
    NAME_STATION_ENTRY_DEMO = 39, // "StationEntryDemo"
    NAME_STATION_RETIRE_DEMO = 40, // "StationRetireDemo"
    NAME_STATION_COME_HOME_DEMO = 41, // "StationComeHomeDemo"
    NAME_STATION_PROLOGUE_DEMO = 42, // "StationPrologueDemo"
    NAME_RECYCLE_SHOP = 43, // "RecycleShop"
    NAME_INSECT_AWARD_TENT = 44, // "InsectAwardTent"
    NAME_INSECT_AWARD_TENT_DEMO = 45, // "InsectAwardTentDemo"
    NAME_FISH_AWARD_TENT = 46, // "FishAwardTent"
    NAME_FISH_AWARD_TENT_DEMO = 47, // "FishAwardTentDemo"
    NAME_POLICE_BOX_A = 48, // "PoliceBoxA"
    NAME_POLICE_BOX_B = 49, // "PoliceBoxB"
    NAME_CAFE = 50, // "Cafe"
    NAME_CAFE_ARBEIT = 51, // "CafeArbeit"
    NAME_RESET_MONITORING_CENTER = 52, // "ResetMonitoringCenter"
    NAME_GALLERY = 53, // "Gallery"
    NAME_FORTUNE_TENT = 54, // "FortuneTent"
    NAME_FORTUNE = 55, // "Fortune"
    NAME_SHOP = 56, // "Shop"
    NAME_CONVENIENCE_STORE = 57, // "ConvenienceStore"
    NAME_SUPERMARKET = 58, // "Supermarket"
    NAME_HOME_CENTER = 59, // "HomeCenter"
    NAME_DEPARTMENT_STORE_1F = 60, // "DepartmentStore_1F"
    NAME_DEPARTMENT_STORE_2F = 61, // "DepartmentStore_2F"
    NAME_DEPARTMENT_STORE_3F = 62, // "DepartmentStore_3F"
    NAME_TAILOR = 63, // "Tailor"
    NAME_ACCESSORY_SHOP = 64, // "AccessoryShop"
    NAME_REAL_ESTATE_AGENCY = 65, // "RealEstateAgency"
    NAME_SHOE_SHOP = 66, // "ShoeShop"
    NAME_FLORISTS = 67, // "Florists"
    NAME_FLORISTS_HOME_IMPROVEMENT_CENTER = 68, // "Florists_HomeImprovementCenter"
    NAME_CLUB444 = 69, // "CLUB444"
    NAME_CLUB444_DJKK = 70, // "CLUB444_DJKK"
    NAME_CLUB444_LIVE = 71, // "CLUB444_Live"
    NAME_BEAUTY_SALON = 72, // "BeautySalon"
    NAME_MUSEUM_ENTRANCE = 73, // "Museum_Entrance"
    NAME_MUSEUM_FISH_0 = 74, // "Museum_Fish0"
    NAME_MUSEUM_FISH_1 = 75, // "Museum_Fish1"
    NAME_MUSEUM_FISH_2 = 76, // "Museum_Fish2"
    NAME_MUSEUM_INSECT_0 = 77, // "Museum_Insect0"
    NAME_MUSEUM_INSECT_1 = 78, // "Museum_Insect1"
    NAME_MUSEUM_INSECT_2 = 79, // "Museum_Insect2"
    NAME_MUSEUM_FOSSIL_0 = 80, // "Museum_Fossil0"
    NAME_MUSEUM_FOSSIL_1 = 81, // "Museum_Fossil1"
    NAME_MUSEUM_FOSSIL_2 = 82, // "Museum_Fossil2"
    NAME_MUSEUM_PICTURE_0 = 83, // "Museum_Picture0"
    NAME_MUSEUM_PICTURE_1 = 84, // "Museum_Picture1"
    NAME_MUSEUM_SHOP = 85, // "Museum_Shop"
    NAME_MUSEUM_SHOWROOM_1 = 86, // "Museum_Showroom1"
    NAME_MUSEUM_SHOWROOM_2 = 87, // "Museum_Showroom2"
    NAME_MUSEUM_SHOWROOM_3 = 88, // "Museum_Showroom3"
    NAME_MUSEUM_SHOWROOM_4 = 89, // "Museum_Showroom4"
    NAME_POST_OFFICE = 90, // "PostOffice"
    NAME_DREAM = 91, // "Dream"
    NAME_PHOTO_BOX = 92, // "PhotoBox"
    NAME_TRAIN = 93, // "Train"
    NAME_TITLE = 94, // "Title"
    NAME_VILLAGE_PROLOGUE = 95, // "VillagePrologue"
    NAME_SAVE = 96, // "Save"
    NAME_LOAD = 97, // "Load"
    NAME_FORMAT = 98, // "Format"
    NAME_PLAYER_SELECT = 99, // "PlayerSelect"
    NAME_DREAM_RETURN = 100, // "DreamReturn"
    NAME_SEA_DEPARTURE = 101, // "SeaDeparture"
    NAME_SEA_RETURN = 102, // "SeaReturn"
    NAME_TOUR_DESK = 103, // "TourDesk"
    NAME_LOBBY_ISLAND = 104, // "LobbyIsland"
    NAME_ISLAND_HIDE_AND_SEEK = 105, // "Island_HideAndSeek"
    NAME_ISLAND_HIDE_AND_SEEK_RESULT = 106, // "Island_HideAndSeekResult"
    NAME_ISLAND_FLOWER_PICKING = 107, // "Island_FlowerPicking"
    NAME_ISLAND_FLOWER_PICKING_RESULT = 108, // "Island_FlowerPickingResult"
    NAME_ISLAND_FISHING = 109, // "Island_Fishing"
    NAME_ISLAND_FISHING_RESULT = 110, // "Island_FishingResult"
    NAME_ISLAND_INSECT_CATCHING = 111, // "Island_InsectCatching"
    NAME_ISLAND_INSECT_CATCHING_RESULT = 112, // "Island_InsectCatchingResult"
    NAME_ISLAND_BALLOON = 113, // "Island_Balloon"
    NAME_ISLAND_BALLOON_RESULT = 114, // "Island_BalloonResult"
    NAME_ISLAND_TREASURE_HUNT = 115, // "Island_TreasureHunt"
    NAME_ISLAND_TREASURE_HUNT_RESULT = 116, // "Island_TreasureHuntResult"
    NAME_ISLAND_PIKO_HAN = 117, // "Island_PikoHan"
    NAME_ISLAND_PIKO_HAN_RESULT = 118, // "Island_PikoHanResult"
    NAME_ISLAND_MAZE = 119, // "Island_Maze"
    NAME_ISLAND_MAZE_RESULT = 120, // "Island_MazeResult"
    NAME_ISLAND_MINING = 121, // "Island_Mining"
    NAME_ISLAND_MINING_RESULT = 122, // "Island_MiningResult"
    NAME_ISLAND_FOSSIL_DIG = 123, // "Island_FossilDig"
    NAME_ISLAND_FOSSIL_DIG_RESULT = 124, // "Island_FossilDigResult"
    NAME_ISLAND_DIVING = 125, // "Island_Diving"
    NAME_ISLAND_DIVING_RESULT = 126, // "Island_DivingResult"
    NAME_ISLAND_BORROWING_RACE = 127, // "Island_BorrowingRace"
    NAME_ISLAND_BORROWING_RACE_RESULT = 128, // "Island_BorrowingRaceResult"
    NAME_ISLAND_HUT_0 = 129, // "Island_Hut0"
    NAME_ISLAND_HUT_1 = 130, // "Island_Hut1"
    NAME_ISLAND_HUT_2 = 131, // "Island_Hut2"
    NAME_ISLAND_HUT_3 = 132, // "Island_Hut3"
    NAME_ISLAND_HUT_4 = 133, // "Island_Hut4"
    NAME_ISLAND_HUT_5 = 134, // "Island_Hut5"
    NAME_ISLAND_HUT_6 = 135, // "Island_Hut6"
    NAME_ISLAND_HUT_7 = 136, // "Island_Hut7"
    NAME_ISLAND_HUT_8 = 137, // "Island_Hut8"
    NAME_ISLAND_HUT_9 = 138, // "Island_Hut9"
    NAME_RANDOM_MATCHING_ENTRY = 139, // "RandomMatchingEntry"
    NAME_RANDOM_MATCHING_RETIRE = 140, // "RandomMatchingRetire"
    NAME_PREPARE_TOUR = 141, // "PrepareTour"
    NAME_EXHIBITION_ENTRANCE = 142, // "Exhibition_Entrance"
    NAME_EXHIBITION_NORTH = 143, // "Exhibition_North"
    NAME_EXHIBITION_EAST = 144, // "Exhibition_East"
    NAME_EXHIBITION_WEST = 145, // "Exhibition_West"
    NAME_MODEL_HOUSE_1F = 146, // "ModelHouse_1F"
    NAME_MODEL_HOUSE_2F = 147, // "ModelHouse_2F"
    NAME_MODEL_HOUSE_BF = 148, // "ModelHouse_BF"
    NAME_MODEL_HOUSE_RIGHT = 149, // "ModelHouse_Right"
    NAME_MODEL_HOUSE_LEFT = 150, // "ModelHouse_Left"
    NAME_MODEL_HOUSE_BACK = 151, // "ModelHouse_Back"
    NAME_INCLUDE_SAVE = 152, // "IncludeSave"
    NAME_GARDEN_MANUAL_IMPORT = 153, // "GardenManualImport"
    NAME_TAKUMI_IMPORT = 154, // "TakumiImport"
    NAME_GARDEN_PLUS_UPDATE = 155, // "GardenPlusUpdate"
    NAME_UPDATE_SELECT = 156, // "UpdateSelect"
    NAME_MINI_GAME_0 = 157, // "MiniGame0"
    NAME_MINI_GAME_1 = 158, // "MiniGame1"
    NAME_AUTO_CAMP_L = 159, // "AutoCampL"
    NAME_AUTO_CAMP_R = 160, // "AutoCampR"
    NAME_CAMPING_CAR_A = 161, // "CampingCarA"
    NAME_CAMPING_CAR_B = 162, // "CampingCarB"
    NAME_CHIP_TUTORIAL = 163, // "ChipTutorial"
    NAME_AMIIBO_CAMERA = 164, // "AmiiboCamera"
    NAME_MAX = 165,
    NAME_NONE = 165, // "None" (libgarden: DUMMY/CURRENT/INVALID): as an argument, the current stage
    NAME_CURRENT = 165,
};

// the kinds of a stage, bits of the kind table (names after libgarden, except where noted)
enum Kind : u32
{
    KIND_DEBUG = 1 << 0,
    KIND_WORLD_CURVE = 1 << 1, // the field is drawn curved (name is ours; libgarden: OUTDOOR_2)
    KIND_OUTDOOR = 1 << 2,
    KIND_INDOOR = 1 << 3,
    KIND_SCREEN = 1 << 4,
    KIND_VILLAGE = 1 << 5,
    KIND_NPC_HOUSE = 1 << 6,
    KIND_PLAYER_HOUSE = 1 << 7,
    KIND_HOUSE_OR_GALLERY = 1 << 8,
    KIND_DOWNTOWN = 1 << 9,
    KIND_EXHIBITION = 1 << 10,
    KIND_INCLUDE_SAVE = 1 << 11,
    KIND_OUTDOOR_OTHER = 1 << 12,
    KIND_MUSEUM_SHOWROOM = 1 << 13,
    KIND_MODEL_HOUSE = 1 << 14,
    KIND_MUSEUM = 1 << 15,
    KIND_POLICE = 1 << 16,
    KIND_KOTOBUKI = 1 << 17,
    KIND_ISLAND = 1 << 18,
    KIND_TOUR = 1 << 19,
    KIND_ISLAND_HUT = 1 << 20,
    KIND_ISLAND_DEMO = 1 << 21,
    KIND_TRAIN_STATION_ENVIRONMENT = 1 << 22,
    KIND_CLUB = 1 << 23,
    KIND_CAFE_ENVIRONMENT = 1 << 24,
    KIND_HAS_W_OR_E_AND_N_OR_S_ENTRANCE = 1 << 25,
    KIND_HAS_W_OR_E_ENTRANCE = 1 << 26,
    KIND_MINI_GAME = 1 << 27,
    KIND_AUTO_CAMP = 1 << 28,
    KIND_RV_CAR_ENVIRONMENT = 1 << 29,
};

// a field of the game (libgarden: FieldName, an index into the field table)
enum FieldName : u8
{
    FIELD_NAME_NONE = 0,
};

// (the following types and names are ours)

// a command of a stage definition: the stage runs the command functions in turn when it is built
struct Command
{
    u16 id;             // 0x0 the index into the table of command functions
    u16 count;          // 0x2 the entries
    const void* data;   // 0x4
};
ASSERT_SIZE(Command, 0x8);

// a process that a stage creates (data of command 0)
struct ProcessEntry
{
    u16 name;   // 0x0 ProcName
    u32 arg;    // 0x4
};
ASSERT_SIZE(ProcessEntry, 0x8);

// a way into a stage: where the player stands, and where the exit of the entrance leads
struct Entrance
{
    u8 targets[4];                  // 0x00 the stage behind it, per house owner (or one of the TARGET_ values)
    u8 targetEntrance;              // 0x04 the entrance of the target stage
    u8 _pad5[3];
    nn::math::VEC3_ position;       // 0x08
    math::Angle16Vec3 rotation;     // 0x14
    u8 _pad1A[2];
    u32 action;                     // 0x1C the player action on arrival (PlayerAction)
    u8 structure;                   // 0x20 the structure the entrance belongs to (0xFC: none)
    u8 _pad21[3];
};
ASSERT_SIZE(Entrance, 0x24);

// Entrance::targets: the special targets
enum EntranceTarget : u8
{
    TARGET_EXIT = 166,              // out of the building in front of its structure (TransitionFromStructure)
    TARGET_STRUCTURE = 168,         // out of the building, the entrance position relative to its structure
    TARGET_PIER = 169,              // the village at the pier (the field object of kind 79)
    TARGET_PLAYER_HOUSE_0 = 170,    // in front of the house of village player 0..3
    TARGET_PLAYER_HOUSE_1 = 171,
    TARGET_PLAYER_HOUSE_2 = 172,
    TARGET_PLAYER_HOUSE_3 = 173,
    TARGET_SHOP = 174,              // in front of the shop of the Nooks
    TARGET_LAST_STRUCTURE = 175,    // in front of the structure that was entered last
    TARGET_TOUR_HUT = 176,          // out of a tour hut to the field of the tour
};

struct EntranceList
{
    const Entrance* entrances;  // 0x0
    u32 count;                  // 0x4
};
ASSERT_SIZE(EntranceList, 0x8);

// the scene settings of a stage
struct SceneSettings
{
    const void* data;   // 0x0
    u32 _4;             // 0x4
    u32 _8;             // 0x8
    u8 lightingType;    // 0xC the type returned by GetLightingType
};

// the definition of a stage
struct Info
{
    u16 commandCount;                   // 0x00
    const Command* commands;            // 0x04
    const SceneSettings* settings;      // 0x08
    const EntranceList* entrances;      // 0x0C
    const void* _10;                    // 0x10
};
ASSERT_SIZE(Info, 0x14);

// a command function: false stops the building of the stage
typedef bool (*CommandFunc)(const Command* command);

// the field rectangle of the current stage (also see ScStage)
struct FieldRect;

// (names are ours) the state of the stages
extern u8 s_IsMiniGameStarted;
extern Name s_Previous;
extern Name s_Current; // (name after libgarden)
extern Name s_Next;
extern s8 s_OffsetX;
extern s8 s_OffsetZ;
extern u8 s_LastStructure;
extern ScStage* s_pScene;
extern oml::framework::Process* s_pParent;
extern const char* s_pShopNameA;
extern const char* s_pShopNameB;
extern const Info* s_Infos[NAME_MAX];
extern const char* s_Names[NAME_MAX];
namespace detail {
extern const u32 s_Kinds[NAME_MAX];
} // namespace detail

// libgarden (the functions in the shared area are inline copies)
DECOMP_EMIT_COPY inline Name GetCurrent() // 0x002F7384 | libgarden [tier A]
{
    return s_Current;
}
DECOMP_EMIT_COPY inline bool IsKind(u32 kind, Name name) // 0x002F7414 | libgarden [tier A]
{
    if (name != NAME_NONE && (kind & ~detail::s_Kinds[name]) == 0) {
        return true;
    }
    return false;
}
// the field of the field table
DECOMP_EMIT_COPY inline Field* GetField(FieldName name) // 0x002FE95C | libgarden [tier A]
{
    return Field::s_pFields[name];
}
const char* GetName(Name name); // 0x005B4BE4 | libgarden [tier A]
Name GetNext(); // 0x005B4C08 | libgarden [tier A]

// (names are ours)
ScStage* GetScene(); // 0x005B49AC
void ClearCurrent(); // 0x005B49BC
// runs the commands of the stage definition with the command functions (pRan: whether one ran); false when one failed
DECOMP_NOINLINE bool RunCommands(const Info* info, const CommandFunc* functions, bool* pRan); // 0x005B49DC
// the lighting kind of the stage (the first floor of a player house: by the size of its rooms); -1 for no stage
s32 GetLightKind(Name name); // 0x005B4A70
// whether the showroom of the museum belongs to the own player
bool IsOwnShowroom(Name name); // 0x005B4B00
void ApplyNext(); // 0x005B4B70
// the center of the field area of the current stage; false without one
bool GetAreaCenter(nn::math::VEC3_* pCenter); // 0x005B4B94
// builds the stage of a mini game (they have no ScStage)
bool BuildMiniGame0(oml::framework::Process* parent); // 0x005B4C30
bool BuildMiniGame1(oml::framework::Process* parent); // 0x005B4C80
// the lighting type of the scene settings of the stage (without them: 0 in the village, else 2)
u8 GetLightingType(Name name); // 0x005B4CD0
// the field area of the current stage, optionally widened by a unit on each side; false without one
bool GetArea(nn::math::VEC3_* pMin, nn::math::VEC3_* pMax, bool isWidened); // 0x005B4D30
// the furniture of the stage at most, at most 48
u32 GetMaxFurniture(Name name); // 0x005B4DE4
// a part of the stage scene (type unknown); NULL without a scene
void* GetSceneWork(); // 0x005B4E10
// the structure of the stage (0xFC: none)
u8 GetStructure(Name name); // 0x005B4E28
bool IsCampingCar(Name name); // 0x005B4E40
bool IsIslandGameResult(Name name); // 0x005B4E54
// the stages of the shop of the Nooks after its first upgrade
bool IsNookStore(Name name); // 0x005B4E98
// 1: not in a private room, 3: in the own showroom with other players, 0: alone, 2: with others
u8 GetPrivateRoomState(); // 0x005B4EBC
// requests the scene change to the stage directly (without the checks of the TransitionMgr)
void RequestTransition(Name name, u8 typeA, u8 faderA, u8 typeB, u8 faderB); // 0x005B4F10
// the position of the stage among the stages with those kinds; -1 for none
s32 GetIndexOfKind(u32 kind, Name name); // 0x005B4F84
u32 GetSoundGroup(Name name); // 0x005B4FF0
// on the island, or on a tour in state 2
bool IsOnIsland(); // 0x005B5008
// in the own showroom, or in a room of the own house that is no tent
bool IsInOwnHouse(); // 0x005B507C
bool IsPolice(Name name); // 0x005B5160
// whether the stage is the first floor of a house that is still a tent
bool IsTent(Name name); // 0x005B518C
// in an indoor stage that belongs to the player alone: the own house or showroom, the chip tutorial, a tour hut
bool IsInPrivateRoom(); // 0x005B520C
bool IsAloneInPrivateRoom(); // 0x005B533C
u8 GetLastStructure(); // 0x005B538C
// on an outdoor field: the village, the island or a tour
bool IsOutdoorField(); // 0x005B539C
void SetLastStructure(u8 structure); // 0x005B5414
u32 GetEnvironmentSound(Name name); // 0x005B5424
// whether the stage is a hut of a tour of the borrowing race
bool IsHutOfBorrowingRace(Name name); // 0x005B543C
bool IsEvenIslandHut(Name name); // 0x005B549C
bool IsOddIslandHut(Name name); // 0x005B54C8
// two offsets (meaning unknown) that SetOffset sets and ClearOffset clears
s8 GetOffsetX(); // 0x005B5500
void ClearOffset(); // 0x005B5510
// in a room of the own house or the own showroom
bool IsInOwnRoom(); // 0x005B5528
s8 GetOffsetZ(); // 0x005B55F0
// builds the current stage: the field, the processes, the player
bool Build(ScStage* scene); // 0x005B5600
void Destroy(); // 0x005B5760
// whether the field of the stage is drawn curved
bool HasWorldCurve(Name name); // 0x005B577C
const Info* GetInfo(Name name); // 0x005B57A8
u32 GetFurnitureCapacity(Name name); // 0x005B74E0
bool IsOwnHouseRoom(Name name); // 0x005B74F8

// (inline; copies of the original)
DECOMP_EMIT_COPY inline Name GetPrevious() // 0x0030426C
{
    return s_Previous;
}
// sets the offsets that are not zero
DECOMP_EMIT_COPY inline void SetOffset(s8 x, s8 z) // 0x0030048C
{
    if (x != 0) {
        s_OffsetX = x;
    }
    if (z != 0) {
        s_OffsetZ = z;
    }
}
// the position of the NPC house among the NPC houses; -1 for another stage
DECOMP_EMIT_COPY inline s32 GetNpcHouseIndex(Name name) // 0x0031336C
{
    if (!IsKind(KIND_NPC_HOUSE, name)) {
        return -1;
    }
    s32 count = 0;
    for (Name i = NAME_VILLAGE; i < NAME_MAX; i = static_cast<Name>(i + 1)) {
        if (i == name) {
            return count;
        }
        if (IsKind(KIND_NPC_HOUSE, i)) {
            count++;
        }
    }
    return -1;
}
// the definition of a stage that is no NAME_CURRENT (out of range: the village)
inline const Info* GetInfoOf(Name name)
{
    return s_Infos[name < NAME_MAX ? name : NAME_VILLAGE];
}
} // namespace stage
