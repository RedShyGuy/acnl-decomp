#pragma once

#include "decomp.h"
#include "math/dAngle16.h"
#include "nn/math/math_Vector3.h"
#include "Other/dField.h"
#include "sead/seadSafeStringBase.h"

namespace oml {
namespace framework {
class Process;
} // namespace framework
} // namespace oml

namespace stage {
// The stages of the game (names after the stage name table 0x009585F8 of the binary, written like libgarden's
// stage::Name; the comments give the room names of the room ID list https://redshyguy.github.io/ACNL_ID_Lists/).
enum Name : u8
{
    NAME_VILLAGE = 0, // "Village": Town
    NAME_DOWNTOWN = 1, // "Downtown": Main Street
    NAME_STATION = 2, // "Station": Train Station
    NAME_PLAYER_HOUSE_0_1F = 3, // "PlayerHouse0_1F": Player 1 Main Room
    NAME_PLAYER_HOUSE_0_2F = 4, // "PlayerHouse0_2F": Player 1 Upstairs
    NAME_PLAYER_HOUSE_0_BF = 5, // "PlayerHouse0_BF": Player 1 Basement
    NAME_PLAYER_HOUSE_0_RIGHT = 6, // "PlayerHouse0_Right": Player 1 Right Room
    NAME_PLAYER_HOUSE_0_LEFT = 7, // "PlayerHouse0_Left": Player 1 Left Room
    NAME_PLAYER_HOUSE_0_BACK = 8, // "PlayerHouse0_Back": Player 1 Top Room
    NAME_PLAYER_HOUSE_1_1F = 9, // "PlayerHouse1_1F": Player 2 Main Room
    NAME_PLAYER_HOUSE_1_2F = 10, // "PlayerHouse1_2F": Player 2 Upstairs
    NAME_PLAYER_HOUSE_1_BF = 11, // "PlayerHouse1_BF": Player 2 Basement
    NAME_PLAYER_HOUSE_1_RIGHT = 12, // "PlayerHouse1_Right": Player 2 Right Room
    NAME_PLAYER_HOUSE_1_LEFT = 13, // "PlayerHouse1_Left": Player 2 Left Room
    NAME_PLAYER_HOUSE_1_BACK = 14, // "PlayerHouse1_Back": Player 2 Top Room
    NAME_PLAYER_HOUSE_2_1F = 15, // "PlayerHouse2_1F": Player 3 Main Room
    NAME_PLAYER_HOUSE_2_2F = 16, // "PlayerHouse2_2F": Player 3 Upstairs
    NAME_PLAYER_HOUSE_2_BF = 17, // "PlayerHouse2_BF": Player 3 Basement
    NAME_PLAYER_HOUSE_2_RIGHT = 18, // "PlayerHouse2_Right": Player 3 Right Room
    NAME_PLAYER_HOUSE_2_LEFT = 19, // "PlayerHouse2_Left": Player 3 Left Room
    NAME_PLAYER_HOUSE_2_BACK = 20, // "PlayerHouse2_Back": Player 3 Top Room
    NAME_PLAYER_HOUSE_3_1F = 21, // "PlayerHouse3_1F": Player 4 Main Room
    NAME_PLAYER_HOUSE_3_2F = 22, // "PlayerHouse3_2F": Player 4 Upstairs
    NAME_PLAYER_HOUSE_3_BF = 23, // "PlayerHouse3_BF": Player 4 Basement
    NAME_PLAYER_HOUSE_3_RIGHT = 24, // "PlayerHouse3_Right": Player 4 Right Room
    NAME_PLAYER_HOUSE_3_LEFT = 25, // "PlayerHouse3_Left": Player 4 Left Room
    NAME_PLAYER_HOUSE_3_BACK = 26, // "PlayerHouse3_Back": Player 4 Top Room
    NAME_NPC_HOUSE_0 = 27, // "NpcHouse0": Villager 1 House
    NAME_NPC_HOUSE_1 = 28, // "NpcHouse1": Villager 2 House
    NAME_NPC_HOUSE_2 = 29, // "NpcHouse2": Villager 3 House
    NAME_NPC_HOUSE_3 = 30, // "NpcHouse3": Villager 4 House
    NAME_NPC_HOUSE_4 = 31, // "NpcHouse4": Villager 5 House
    NAME_NPC_HOUSE_5 = 32, // "NpcHouse5": Villager 6 House
    NAME_NPC_HOUSE_6 = 33, // "NpcHouse6": Villager 7 House
    NAME_NPC_HOUSE_7 = 34, // "NpcHouse7": Villager 8 House
    NAME_NPC_HOUSE_8 = 35, // "NpcHouse8": Villager 9 House
    NAME_NPC_HOUSE_9 = 36, // "NpcHouse9": Villager 10 House
    NAME_CAMP_NPC = 37, // "CampNpc": Camping Grounds
    NAME_OFFICE = 38, // "Office": Town Hall
    NAME_STATION_ENTRY_DEMO = 39, // "StationEntryDemo": Arriving in town via train
    NAME_STATION_RETIRE_DEMO = 40, // "StationRetireDemo": Leaving a town via train
    NAME_STATION_COME_HOME_DEMO = 41, // "StationComeHomeDemo": Arriving in your town from another town via train
    NAME_STATION_PROLOGUE_DEMO = 42, // "StationPrologueDemo": First time arriving from the train (start of game)
    NAME_RECYCLE_SHOP = 43, // "RecycleShop": Retail
    NAME_INSECT_AWARD_TENT = 44, // "InsectAwardTent": Nat's Tent (Bug-Off)
    NAME_INSECT_AWARD_TENT_DEMO = 45, // "InsectAwardTentDemo": Bug-Off Ceremony
    NAME_FISH_AWARD_TENT = 46, // "FishAwardTent": Chip's Tent (Fishing Tourney)
    NAME_FISH_AWARD_TENT_DEMO = 47, // "FishAwardTentDemo": Fishing Tourney Ceremony
    NAME_POLICE_BOX_A = 48, // "PoliceBoxA": Police Station (Copper)
    NAME_POLICE_BOX_B = 49, // "PoliceBoxB": Police Station (Booker)
    NAME_CAFE = 50, // "Cafe": The Roost Café
    NAME_CAFE_ARBEIT = 51, // "CafeArbeit": Working In The Roost Café
    NAME_RESET_MONITORING_CENTER = 52, // "ResetMonitoringCenter": Reset Centre
    NAME_GALLERY = 53, // "Gallery": Redd's Tent
    NAME_FORTUNE_TENT = 54, // "FortuneTent": Katrina's Tent
    NAME_FORTUNE = 55, // "Fortune": Katrina's Main Street Shop
    NAME_SHOP = 56, // "Shop": Nook's Cranny
    NAME_CONVENIENCE_STORE = 57, // "ConvenienceStore": T&T Mart
    NAME_SUPERMARKET = 58, // "Supermarket": Super T&T
    NAME_HOME_CENTER = 59, // "HomeCenter": TIY
    NAME_DEPARTMENT_STORE_1F = 60, // "DepartmentStore_1F": T&T Emporium - Ground Floor
    NAME_DEPARTMENT_STORE_2F = 61, // "DepartmentStore_2F": T&T Emporium - Floor 1
    NAME_DEPARTMENT_STORE_3F = 62, // "DepartmentStore_3F": GracieGrace (T&T Emporium - Floor 2)
    NAME_TAILOR = 63, // "Tailor": Able Sisters - Mabel & Sable (Clothing)
    NAME_ACCESSORY_SHOP = 64, // "AccessoryShop": Able Sisters - Labelle (Accessories)
    NAME_REAL_ESTATE_AGENCY = 65, // "RealEstateAgency": Nook's Homes (House Exteriors)
    NAME_SHOE_SHOP = 66, // "ShoeShop": Kicks (Shoe Shop)
    NAME_FLORISTS = 67, // "Florists": Gardening Store (Initial)
    NAME_FLORISTS_HOME_IMPROVEMENT_CENTER = 68, // "Florists_HomeImprovementCenter": Gardening Store (Joint w/ TIY)
    NAME_CLUB444 = 69, // "CLUB444": Club LOL
    NAME_CLUB444_DJKK = 70, // "CLUB444_DJKK": Club LOL Night Disco
    NAME_CLUB444_LIVE = 71, // "CLUB444_Live": Club LOL Saturday Night (K.K. Songs)
    NAME_BEAUTY_SALON = 72, // "BeautySalon": Shampoodle
    NAME_MUSEUM_ENTRANCE = 73, // "Museum_Entrance": Museum - Ground Floor
    NAME_MUSEUM_FISH_0 = 74, // "Museum_Fish0": Fish Gallery - Main Room
    NAME_MUSEUM_FISH_1 = 75, // "Museum_Fish1": Fish Gallery - North West Room
    NAME_MUSEUM_FISH_2 = 76, // "Museum_Fish2": Fish Gallery - North East Room
    NAME_MUSEUM_INSECT_0 = 77, // "Museum_Insect0": Bug Gallery - Main Room
    NAME_MUSEUM_INSECT_1 = 78, // "Museum_Insect1": Bug Gallery - North East Room
    NAME_MUSEUM_INSECT_2 = 79, // "Museum_Insect2": Bug Gallery - North West Room
    NAME_MUSEUM_FOSSIL_0 = 80, // "Museum_Fossil0": Fossil Gallery - Main Room
    NAME_MUSEUM_FOSSIL_1 = 81, // "Museum_Fossil1": Fossil Gallery - South Room
    NAME_MUSEUM_FOSSIL_2 = 82, // "Museum_Fossil2": Fossil Gallery - North West Room
    NAME_MUSEUM_PICTURE_0 = 83, // "Museum_Picture0": Art Gallery - Main Room
    NAME_MUSEUM_PICTURE_1 = 84, // "Museum_Picture1": Art Gallery - East Room
    NAME_MUSEUM_SHOP = 85, // "Museum_Shop": Museum - 1st Floor (Shop)
    NAME_MUSEUM_SHOWROOM_1 = 86, // "Museum_Showroom1": Museum Exhibit - North West
    NAME_MUSEUM_SHOWROOM_2 = 87, // "Museum_Showroom2": Museum Exhibit - South West
    NAME_MUSEUM_SHOWROOM_3 = 88, // "Museum_Showroom3": Museum Exhibit - North East
    NAME_MUSEUM_SHOWROOM_4 = 89, // "Museum_Showroom4": Museum Exhibit - South East
    NAME_POST_OFFICE = 90, // "PostOffice": Post Office
    NAME_DREAM = 91, // "Dream": Dream Suite
    NAME_PHOTO_BOX = 92, // "PhotoBox": Photo Booth
    NAME_TRAIN = 93, // "Train": Game's Initial Setup Train Ride
    NAME_TITLE = 94, // "Title": Title Screen
    NAME_VILLAGE_PROLOGUE = 95, // "VillagePrologue": 1st arrival at train station (start of game)
    NAME_SAVE = 96, // "Save": Saving Screen
    NAME_LOAD = 97, // "Load": After Save & Quit (Black Screen -> Title Screen)
    NAME_FORMAT = 98, // "Format": Black Screen
    NAME_PLAYER_SELECT = 99, // "PlayerSelect": Isabelle Spotlight/ Load Town Area
    NAME_DREAM_RETURN = 100, // "DreamReturn": Coming Back From A Dream (Dream Suite Bed)
    NAME_SEA_DEPARTURE = 101, // "SeaDeparture": On Kappn's Boat Going To The Island
    NAME_SEA_RETURN = 102, // "SeaReturn": On Kappn's Boat Leaving The Island
    NAME_TOUR_DESK = 103, // "TourDesk": Island Shack
    NAME_LOBBY_ISLAND = 104, // "LobbyIsland": Island
    NAME_ISLAND_HIDE_AND_SEEK = 105, // "Island_HideAndSeek": Tour - Hide and Seek
    NAME_ISLAND_HIDE_AND_SEEK_RESULT = 106, // "Island_HideAndSeekResult": Tour - Hide and Seek (Results)
    NAME_ISLAND_FLOWER_PICKING = 107, // "Island_FlowerPicking": Tour - Gardening Tour
    NAME_ISLAND_FLOWER_PICKING_RESULT = 108, // "Island_FlowerPickingResult": Tour - Gardening Tour (Results)
    NAME_ISLAND_FISHING = 109, // "Island_Fishing": Tour - Fishing
    NAME_ISLAND_FISHING_RESULT = 110, // "Island_FishingResult": Tour - Fishing (Results)
    NAME_ISLAND_INSECT_CATCHING = 111, // "Island_InsectCatching": Tour - Bug Catching
    NAME_ISLAND_INSECT_CATCHING_RESULT = 112, // "Island_InsectCatchingResult": Tour - Bug Catching (Results)
    NAME_ISLAND_BALLOON = 113, // "Island_Balloon": Tour - Balloon-Hunt
    NAME_ISLAND_BALLOON_RESULT = 114, // "Island_BalloonResult": Tour - Balloon-Hunt (Results)
    NAME_ISLAND_TREASURE_HUNT = 115, // "Island_TreasureHunt": Tour - Scavenger Hunt
    NAME_ISLAND_TREASURE_HUNT_RESULT = 116, // "Island_TreasureHuntResult": Tour - Scavenger Hunt (Results)
    NAME_ISLAND_PIKO_HAN = 117, // "Island_PikoHan": Tour - Hammer Tour
    NAME_ISLAND_PIKO_HAN_RESULT = 118, // "Island_PikoHanResult": Tour - Hammer Tour (Results)
    NAME_ISLAND_MAZE = 119, // "Island_Maze": Tour - Labyrinth Tours
    NAME_ISLAND_MAZE_RESULT = 120, // "Island_MazeResult": Tour - Labyrinth Tours (Results)
    NAME_ISLAND_MINING = 121, // "Island_Mining": Tour - Ore Tours
    NAME_ISLAND_MINING_RESULT = 122, // "Island_MiningResult": Tour - Ore Tours (Results)
    NAME_ISLAND_FOSSIL_DIG = 123, // "Island_FossilDig": Tour - Fossil Hunter Tours
    NAME_ISLAND_FOSSIL_DIG_RESULT = 124, // "Island_FossilDigResult": Tour - Fossil Hunter Tours (Results)
    NAME_ISLAND_DIVING = 125, // "Island_Diving": Tour - Diving Tour
    NAME_ISLAND_DIVING_RESULT = 126, // "Island_DivingResult": Tour - Diving Tour (Results)
    NAME_ISLAND_BORROWING_RACE = 127, // "Island_BorrowingRace": Tour - Item-Matching
    NAME_ISLAND_BORROWING_RACE_RESULT = 128, // "Island_BorrowingRaceResult": Tour - Item-Matching (Results)
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
    NAME_EXHIBITION_ENTRANCE = 142, // "Exhibition_Entrance": Happy Home Showcase Entrance
    NAME_EXHIBITION_NORTH = 143, // "Exhibition_North": Happy Home Showcase North Section
    NAME_EXHIBITION_EAST = 144, // "Exhibition_East": Happy Home Showcase East Section
    NAME_EXHIBITION_WEST = 145, // "Exhibition_West": Happy Home Showcase West Section
    NAME_MODEL_HOUSE_1F = 146, // "ModelHouse_1F": Model House Main Room
    NAME_MODEL_HOUSE_2F = 147, // "ModelHouse_2F": Model House Upstairs
    NAME_MODEL_HOUSE_BF = 148, // "ModelHouse_BF": Model House Basement
    NAME_MODEL_HOUSE_RIGHT = 149, // "ModelHouse_Right": Model House Right Room
    NAME_MODEL_HOUSE_LEFT = 150, // "ModelHouse_Left": Model House Left Room
    NAME_MODEL_HOUSE_BACK = 151, // "ModelHouse_Back": Model House Top Room
    NAME_INCLUDE_SAVE = 152, // "IncludeSave"
    NAME_GARDEN_MANUAL_IMPORT = 153, // "GardenManualImport": Move town to AC:WA version of game
    NAME_TAKUMI_IMPORT = 154, // "TakumiImport": Isabelle on stage, talking about AC:HHD in ACNL
    NAME_GARDEN_PLUS_UPDATE = 155, // "GardenPlusUpdate": Isabelle on train, introduction to WA features
    NAME_UPDATE_SELECT = 156, // "UpdateSelect": Player select for Isabelle on train (9C -> 9B)
    NAME_MINI_GAME_0 = 157, // "MiniGame0": Desert Island Escape Minigame (Wii U)
    NAME_MINI_GAME_1 = 158, // "MiniGame1": Puzzle League Minigame
    NAME_AUTO_CAMP_L = 159, // "AutoCampL": Campground - West
    NAME_AUTO_CAMP_R = 160, // "AutoCampR": Campground - East
    NAME_CAMPING_CAR_A = 161, // "CampingCarA": Special Character RV
    NAME_CAMPING_CAR_B = 162, // "CampingCarB": Villager RV
    NAME_CHIP_TUTORIAL = 163, // "ChipTutorial": Move Furniture Like AC:HHD Tutorial (w/ Lottie)
    NAME_AMIIBO_CAMERA = 164, // "AmiiboCamera": Amiibo Camera
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
    KIND_KOTOBUKI = 1 << 17,       // the island stages: TourDesk, LobbyIsland, the tours, the huts, RandomMatching*, PrepareTour
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
    KIND_AUTO_CAMP = 1 << 28,      // AutoCampL, AutoCampR
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

// an actor of a stage (data of command 1)
struct ActorEntry
{
    u16 name;                       // 0x00 ProcName
    nn::math::VEC3_ position;       // 0x04
    math::Angle16Vec3 rotation;     // 0x10
    u32 arg;                        // 0x18
};
ASSERT_SIZE(ActorEntry, 0x1C);

// where the player starts in a stage (data of command 2)
struct PlayerStart
{
    nn::math::VEC3_ position;       // 0x00
    math::Angle16Vec3 rotation;     // 0x0C
    u32 action;                     // 0x14 PlayerAction
};
ASSERT_SIZE(PlayerStart, 0x18);

// the argument of a special NPC of a stage: the actor gets its address
struct alignas(4) NpcInfo
{
    // (inline in the original: the stage file builds the NPC infos in its static initializer)
    NpcInfo(u8 kind, u8 flag, const char* model) : kind(kind), flag(flag)
    {
        for (s32 i = 0; i < 3; i++) {
            this->model[i] = model[i];
        }
        this->model[3] = sead::SafeStringBase<char>::s_NullChar;
    }

    u8 kind;        // 0x0 BsNpcMgr counts the kinds 0 and 1 apart (all stage NPCs are 1)
    u8 flag;        // 0x1 (meaning unknown; set in the campgrounds)
    u16 _2;
    char model[4];  // 0x4 the three letters of the NPC model ("ttl", "owl", ...)
};
ASSERT_SIZE(NpcInfo, 0x8);

// a special NPC of a stage (data of command 3, created by BsNpcMgr)
struct NpcEntry
{
    u16 name;                       // 0x00 ProcName
    nn::math::VEC3_ position;       // 0x04
    math::Angle16Vec3 rotation;     // 0x10
    NpcInfo info;                   // 0x18
};
ASSERT_SIZE(NpcEntry, 0x20);

// a way into a stage: where the player stands, and where the exit of the entrance leads
struct Entrance
{
    u8 targets[4];                  // 0x00 the stage behind it, per house owner (or one of the TARGET_ values)
    u8 targetEntrance;              // 0x04 the entrance of the target stage
    nn::math::VEC3_ position;       // 0x08
    math::Angle16Vec3 rotation;     // 0x14
    u32 action;                     // 0x1C the player action on arrival (PlayerAction)
    u8 type;                        // 0x20 EntranceType
};
ASSERT_SIZE(Entrance, 0x24);

// Entrance::targets: the special targets
enum EntranceTarget : u8
{
    TARGET_EXIT = 166,              // out of the building in front of its structure (TransitionFromStructure)
    TARGET_STRUCTURE = 168,         // out of the building, the entrance position relative to its structure
    TARGET_RESET_CENTER = 169,      // the village in front of the reset center (out of the reset center)
    TARGET_PLAYER_HOUSE_0 = 170,    // in front of the house of village player 0..3
    TARGET_PLAYER_HOUSE_1 = 171,
    TARGET_PLAYER_HOUSE_2 = 172,
    TARGET_PLAYER_HOUSE_3 = 173,
    TARGET_STATION = 174,           // in front of the train station (from Save, Load and Format)
    TARGET_LAST_STRUCTURE = 175,    // in front of the structure that was entered last (from ModelHouse_1F)
    TARGET_TOUR_HUT = 176,          // out of Island_Hut0..9 to the field of the tour (TransitionFromTourHut)
};

// Entrance::type: where the way leads (names are ours, after the entrances that use each value)
enum EntranceType : u8
{
    ENTRANCE_TYPE_NONE = 0,
    ENTRANCE_TYPE_FLOOR_UP = 1,         // to the floor above (arrival: EXIT_CONNECTION_STAIR_DOWN)
    ENTRANCE_TYPE_FLOOR_DOWN = 2,       // to the floor below (arrival: EXIT_CONNECTION_STAIR_UP)
    ENTRANCE_TYPE_SAME_FLOOR = 3,       // to another room on the same floor
    ENTRANCE_TYPE_EXIT = 4,             // mostly out of the building (TARGET_EXIT), also between the Exhibition sections
    ENTRANCE_TYPE_TO_VILLAGE = 5,       // from Downtown to Village
    ENTRANCE_TYPE_TO_DOWNTOWN = 6,      // from Village to Downtown
    ENTRANCE_TYPE_TO_AUTO_CAMP = 7,     // from Village to AutoCampL/AutoCampR
};

struct EntranceList
{
    const Entrance* entrances;  // 0x0
    u32 count;                  // 0x4
};
ASSERT_SIZE(EntranceList, 0x8);

// the field of a stage that is not the village: its acres
struct FieldLayout
{
    const u16* acres;   // 0x0 the acre IDs, row by row
    u32 width;          // 0x4 in acres
    u32 height;         // 0x8
    u8 lightingType;    // 0xC the type returned by GetLightingType
};
ASSERT_SIZE(FieldLayout, 0x10);

// the CRO modules that a stage needs ("Module<name>.cro" in the romfs)
struct ModuleList
{
    const char* const* names;   // 0x0
    u16 count;                  // 0x4
};
ASSERT_SIZE(ModuleList, 0x8);

// the definition of a stage
struct Info
{
    u16 commandCount;                   // 0x00
    const Command* commands;            // 0x04
    const FieldLayout* field;           // 0x08 NULL: the field of the field table
    const EntranceList* entrances;      // 0x0C
    const ModuleList* modules;          // 0x10
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
// the lighting type of the field layout of the stage (without one: 0 in the village, else 2)
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
// whether the stage is T&T Mart, Super T&T, TIY or the ground floor of T&T Emporium (not Shop)
bool IsNooklingStore(Name name); // 0x005B4E98
// 1: not in a private room, 3: in the own showroom with other players, 0: alone, 2: with others
u8 GetPrivateRoomState(); // 0x005B4EBC
// requests the scene change to the stage directly (without the checks of the TransitionMgr)
void RequestTransition(Name name, u8 typeA, u8 faderA, u8 typeB, u8 faderB); // 0x005B4F10
// the position of the stage among the stages with those kinds; -1 for none
s32 GetIndexOfKind(u32 kind, Name name); // 0x005B4F84
u32 GetSoundGroup(Name name); // 0x005B4FF0
// on the island, or on a tour in state 2
bool IsOnIsland(); // 0x005B5008
// in the own showroom, or in a room of the own house whose size is neither 0 (the starting tent) nor 8
bool IsInOwnHouse(); // 0x005B507C
bool IsPolice(Name name); // 0x005B5160
// whether the stage is the first floor of a house that is still a tent
bool IsTent(Name name); // 0x005B518C
// in an indoor stage that belongs to the player alone: the own house or showroom, ChipTutorial, an island hut
// during the tour types 9 and 11
bool IsInPrivateRoom(); // 0x005B520C
bool IsAloneInPrivateRoom(); // 0x005B533C
u8 GetLastStructure(); // 0x005B538C
// on an outdoor field: the village, the island or a tour
bool IsOutdoorField(); // 0x005B539C
void SetLastStructure(u8 structure); // 0x005B5414
u32 GetEnvironmentSound(Name name); // 0x005B5424
// whether the stage is an island hut during tour type 11 (whose huts lead back to Island_BorrowingRace)
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
