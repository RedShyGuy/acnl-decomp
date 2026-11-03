#include "stage/stage_Api.h"
#include "Bs/dBsFtrMgr.h"
#include "Bs/dBsPlayerMgr.h"
#include "Bs/dBsScene.h"
#include "Other/dProcMgr.h"
#include "Other/dTourResult.h"
#include "Sc/dScStage.h"
#include "netgame/netgame_Api.h"
#include "stage/dTransitionMgr.h"
#include "stage/stage_Extern.h"
#include "stage/stage_Tables.h"

#include <cstring>

namespace stage {
// 0x00958340
u8 s_IsMiniGameStarted = 0;
// 0x00958341
Name s_Previous = NAME_NONE;
// 0x00958342
Name s_Current = NAME_NONE;
// 0x00958343
Name s_Next = NAME_NONE;
// 0x00958344
s8 s_OffsetX = 0;
// 0x00958345
s8 s_OffsetZ = 0;
// 0x00958346
u8 s_LastStructure = 0xFC;
// 0x00958348
ScStage* s_pScene = NULL;
// 0x0095834C
oml::framework::Process* s_pParent = NULL;
// 0x00958350
const char* s_pShopNameA = "";
// 0x00958354
const char* s_pShopNameB = "";

// 0x00958364
const Info* s_Infos[NAME_MAX];
// 0x009585F8
const char* s_Names[NAME_MAX];
// 0x00AC2948
TransitionMgr s_TransitionMgr;
// the start in the own house without a return point
// 0x00AC29C0
nn::math::VEC3 s_HouseStartPosition(255.0f, 0.0f, 440.0f);
// 0x0095835C
math::Angle16Vec3 s_ZeroRotation;

// the static initializer of the globals above
// 0x007849B0 (static initializer)

namespace {
// a fifth of a field unit, as a whole number (read nowhere)
// 0x00958358
f32 s_UnitFifth = static_cast<u16>(FIELD_UNIT_SIZE_X * 0.2f);

// the processes of a stage (command 0)
// 0x005B4950
bool CreateProcesses(const Command* command)
{
    if (command == NULL) {
        return false;
    }
    const ProcessEntry* entries = static_cast<const ProcessEntry*>(command->data);
    for (s32 i = 0; i < command->count; i++) {
        const ProcessEntry& entry = entries[i];
        ProcMgr::Create(static_cast<ProcName>(entry.name), s_pParent, entry.arg, PROC_TYPE_BASE);
    }
    return true;
}

// an actor of a stage (data of command 1)
struct ActorEntry
{
    u16 name;                       // 0x00 ProcName
    nn::math::VEC3_ position;       // 0x04
    math::Angle16Vec3 rotation;     // 0x10
    u32 arg;                        // 0x18
};

// the actors of a stage (command 1)
// 0x005B7588
bool CreateActors(const Command* command)
{
    if (command == NULL) {
        return false;
    }
    const ActorEntry* entries = static_cast<const ActorEntry*>(command->data);
    for (s32 i = 0; i < command->count; i++) {
        const ActorEntry& entry = entries[i];
        nn::math::VEC3_ position = entry.position;
        math::Angle16Vec3 rotation = entry.rotation;
        if (ActorCreate(entry.name, s_pParent, entry.arg, &position, &rotation) == NULL) {
            return false;
        }
    }
    return true;
}

// the furniture of a stage (command 3)
// 0x005B7564
bool CreateFurniture(const Command* command)
{
    if (command != NULL && BsFtrMgr::s_pInstance != NULL) {
        return BsFtrMgr::s_pInstance->CreateFurniture(command);
    }
    return false;
}

// where the player starts in a stage (data of command 2)
struct PlayerStart
{
    nn::math::VEC3_ position;       // 0x00
    math::Angle16Vec3 rotation;     // 0x0C
    u32 action;                     // 0x14
};

// moves the position to the side (along the X axis turned by the rotation)
inline void AddArrivalOffset(nn::math::VEC3_* position, const math::Angle16Vec3& rotation, f32 side)
{
    nn::math::VEC3_ offset;
    offset.x = side;
    offset.y = 0.0f;
    offset.z = 0.0f;
    RotateY(&offset, rotation.y);
    position->x += offset.x;
    position->y += offset.y;
    position->z += offset.z;
}

// places the player (command 2): at the start of the transition, or else at the start of the stage definition; in a
// multiplayer game the players stand side by side
// 0x005B7634
bool PlacePlayer(const Command* command)
{
    if (command == NULL) {
        return false;
    }
    const PlayerStart* start = static_cast<const PlayerStart*>(command->data);
    if (start == NULL) {
        return false;
    }
    nn::math::VEC3_ position = start->position;
    math::Angle16Vec3 rotation = start->rotation;
    u32 action = start->action;
    if (!s_TransitionMgr.m_IsWithoutPosition) {
        position = s_TransitionMgr.m_Position;
        math::Angle16Vec3 transitionRotation = s_TransitionMgr.m_Rotation;
        rotation = transitionRotation;
        action = s_TransitionMgr.m_Action;
    }
    Name current = s_Current;
    Name previous = s_Previous;
    if (netgame::IsMultiplePlayer()) {
        if (action == PLAYER_ACTION_EXIT_CONNECTION_WALK_SLOW || action == PLAYER_ACTION_EXIT_CONNECTION_WALK ||
            action == PLAYER_ACTION_EXIT_CONNECTION_RUN || action == PLAYER_ACTION_EXIT_CONNECTION_STAIR_UP ||
            action == PLAYER_ACTION_EXIT_CONNECTION_STAIR_DOWN) {
            u32 player = netgame::GetMyPlayerNo() & 3;
            if (current == NAME_TOUR_DESK) {
                if (previous == NAME_LOBBY_ISLAND) {
                    // 0x0088AC18
                    const f32 offsets[] = {-16.0f, 16.0f, -48.0f, 48.0f};
                    AddArrivalOffset(&position, rotation, offsets[player]);
                }
            } else if (current == NAME_DOWNTOWN || IsKind(KIND_TRAIN_STATION_ENVIRONMENT, current)) {
                // 0x0088AC28
                const f32 offsets[] = {-16.0f, 16.0f, -48.0f, 48.0f};
                AddArrivalOffset(&position, rotation, offsets[player]);
            } else if (IsKind(KIND_OUTDOOR, current)) {
                // 0x0088AC38
                const f32 offsets[] = {10.0f, -10.0f, 30.0f, -30.0f};
                AddArrivalOffset(&position, rotation, offsets[player]);
            } else if (IsKind(KIND_PLAYER_HOUSE, current) && (action == PLAYER_ACTION_EXIT_CONNECTION_STAIR_UP ||
                                                               action == PLAYER_ACTION_EXIT_CONNECTION_STAIR_DOWN)) {
                // 0x0088AC48
                const f32 offsets[] = {0.0f, -0.5f, 0.5f, 1.0f};
                AddArrivalOffset(&position, rotation, offsets[player]);
            } else if (IsKind(KIND_RV_CAR_ENVIRONMENT, current)) {
                // 0x0088AC58
                const f32 offsets[] = {0.0f, -0.5f, 0.5f, 1.0f};
                AddArrivalOffset(&position, rotation, offsets[player]);
            } else {
                // 0x0088AC68
                const f32 offsets[] = {8.0f, -8.0f, 17.0f, -17.0f};
                AddArrivalOffset(&position, rotation, offsets[player]);
            }
        }
    }
    if (!PlayerSetStart(netgame::PLAYER_NO_MY_NO, &position, &rotation, action, 0)) {
        return false;
    }
    if (current == NAME_VILLAGE && previous == NAME_DOWNTOWN && action == PLAYER_ACTION_EXIT_CONNECTION_RUN) {
        SetPlayerArrivalType(2);
    }
    return true;
}

// the command functions of the stages
// 0x0095888C
CommandFunc s_BuildFunctions[] = {CreateProcesses, CreateActors, NULL, CreateFurniture};
// 0x0095889C
CommandFunc s_MiniGame0Functions[] = {CreateProcesses, CreateActors, NULL, CreateFurniture};
// 0x009588AC
CommandFunc s_MiniGame1Functions[] = {CreateProcesses, CreateActors, NULL, CreateFurniture};
// only the player of a stage definition
// 0x0088AC78
const CommandFunc s_PlayerFunctions[] = {NULL, NULL, PlacePlayer, NULL};

// whether the stage is one of the rooms of the house of the village player
inline bool IsHouseRoomOf(u8 player, Name name)
{
    for (s32 i = 0; i < 6; i += 2) {
        if (detail::s_HouseRooms[player][i] == name || detail::s_HouseRooms[player][i + 1] == name) {
            return true;
        }
    }
    return false;
}
} // namespace

// 0x005B49AC (name is ours)
ScStage* GetScene()
{
    return s_pScene;
}

// 0x005B49BC (name is ours)
void ClearCurrent()
{
    s_Previous = s_Current;
    s_Current = NAME_NONE;
    s_Next = NAME_NONE;
}

// 0x005B49DC (name is ours)
bool RunCommands(const Info* info, const CommandFunc* functions, bool* pRan)
{
    if (pRan != NULL) {
        *pRan = false;
    }
    if (info == NULL) {
        return false;
    }
    bool result = true;
    for (s32 i = 0; i < info->commandCount; i++) {
        const Command* command = &info->commands[i];
        if (command != NULL && functions[command->id] != NULL) {
            result = functions[command->id](command);
            if (!result) {
                return false;
            }
            if (pRan != NULL) {
                *pRan = true;
            }
        } else if (!result) {
            return false;
        }
    }
    return result;
}

// 0x005B4A70 (name is ours)
s32 GetLightKind(Name name)
{
    if (name >= NAME_MAX) {
        return -1;
    }
    u8 kind = detail::s_LightKinds[name];
    if (kind != 21) {
        return kind;
    }
    if (s_Current == name) {
        SvHouse* house = GetHouseOfStage(name);
        if (house != NULL && house->GetRoomSize(3) >= 2 && house->GetRoomSize(4) >= 2 && house->GetRoomSize(5) >= 2) {
            return 3;
        }
    }
    return 2;
}

// 0x005B4B00 (name is ours)
bool IsOwnShowroom(Name name)
{
    if (IsKind(KIND_MUSEUM_SHOWROOM, name)) {
        u8 index = name - NAME_MUSEUM_SHOWROOM_1;
        s8 owner = GetGardenData()->m_Museum.GetShowroom(index)->owner;
        if (owner >= 0 && static_cast<u8>(owner) == PlayerGetSaveNumber(netgame::PLAYER_NO_MY_NO)) {
            return true;
        }
    }
    return false;
}

// 0x005B4B70 (name is ours)
void ApplyNext()
{
    s_Previous = s_Current;
    s_Current = s_Next;
    s_Next = NAME_NONE;
}

// 0x005B4B94 (name is ours)
bool GetAreaCenter(nn::math::VEC3_* pCenter)
{
    ScStage* scene = s_pScene;
    if (scene != NULL && scene->m_HasArea) {
        *pCenter = scene->m_AreaCenter;
        return true;
    }
    return false;
}

// 0x005B4BE4 | libgarden [tier A]
const char* GetName(Name name)
{
    if (name > NAME_CURRENT) {
        name = NAME_VILLAGE;
    } else if (name == NAME_CURRENT) {
        name = s_Current;
    }
    return s_Names[name];
}

// 0x005B4C08 | libgarden [tier A]
Name GetNext()
{
    return s_Next;
}

// 0x005B4C30 (name is ours)
bool BuildMiniGame0(oml::framework::Process* parent)
{
    s_pParent = parent;
    s_pScene = NULL;
    ResetStageList();
    const Info* info = GetInfoOf(s_Current);
    SetupField();
    RunCommands(info, s_MiniGame0Functions, NULL);
    return true;
}

// 0x005B4C80 (name is ours)
bool BuildMiniGame1(oml::framework::Process* parent)
{
    s_pParent = parent;
    s_pScene = NULL;
    ResetStageList();
    const Info* info = GetInfoOf(s_Current);
    SetupField();
    RunCommands(info, s_MiniGame1Functions, NULL);
    return true;
}

// 0x005B4CD0 (name is ours)
u8 GetLightingType(Name name)
{
    const Info* info = GetInfoOf(name == NAME_CURRENT ? s_Current : name);
    if (info->settings != NULL) {
        return info->settings->lightingType;
    }
    if (IsKind(KIND_VILLAGE, name)) {
        return 0;
    }
    return 2;
}

// 0x005B4D30 (name is ours)
bool GetArea(nn::math::VEC3_* pMin, nn::math::VEC3_* pMax, bool isWidened)
{
    ScStage* scene = s_pScene;
    if (scene == NULL || !scene->m_HasArea) {
        return false;
    }
    if (isWidened) {
        pMin->x = scene->m_AreaMin.x - FIELD_UNIT_SIZE_X;
        pMin->z = scene->m_AreaMin.z - FIELD_UNIT_SIZE_Z;
        pMax->x = scene->m_AreaMax.x + FIELD_UNIT_SIZE_X;
        pMax->z = scene->m_AreaMax.z + FIELD_UNIT_SIZE_Z;
    } else {
        *pMin = scene->m_AreaMin;
        *pMax = scene->m_AreaMax;
    }
    return true;
}

// 0x005B4DE4 (name is ours)
u32 GetMaxFurniture(Name name)
{
    if (name >= NAME_MAX) {
        return 0;
    }
    u32 capacity = detail::s_FurnitureCapacities[name];
    if (capacity > 48) {
        capacity = 48;
    }
    return capacity;
}

// 0x005B4E10 (name is ours)
void* GetSceneWork()
{
    if (s_pScene == NULL) {
        return NULL;
    }
    return s_pScene->m_Work;
}

// 0x005B4E28 (name is ours)
u8 GetStructure(Name name)
{
    if (name < NAME_MAX) {
        return detail::s_Structures[name];
    }
    return 0xFC;
}

// 0x005B4E40 (name is ours)
bool IsCampingCar(Name name)
{
    return name == NAME_CAMPING_CAR_A || name == NAME_CAMPING_CAR_B;
}

// 0x005B4E54 (name is ours)
bool IsIslandGameResult(Name name)
{
    for (u32 i = 0; i < 12; i += 2) {
        if (detail::s_IslandGameResults[i] == name || detail::s_IslandGameResults[i + 1] == name) {
            return true;
        }
    }
    return false;
}

// 0x005B4E98 (name is ours)
bool IsNookStore(Name name)
{
    return name == NAME_CONVENIENCE_STORE || name == NAME_SUPERMARKET || name == NAME_HOME_CENTER ||
           name == NAME_DEPARTMENT_STORE_1F;
}

// 0x005B4EBC (name is ours)
u8 GetPrivateRoomState()
{
    if (!IsInPrivateRoom()) {
        return 1;
    }
    if (IsOwnShowroom(s_Current) && netgame::IsMultiplePlayer()) {
        return 3;
    }
    if (IsAloneInRoom()) {
        return 0;
    }
    return 2;
}

// 0x005B4F10 (name is ours)
void RequestTransition(Name name, u8 typeA, u8 faderA, u8 typeB, u8 faderB)
{
    s_Next = name;
    if (typeA == BsScene::FADE_TYPE_KEEP) {
        typeA = BsScene::s_ChangeRequest.typeA;
    }
    if (typeB == BsScene::FADE_TYPE_KEEP) {
        typeB = BsScene::s_ChangeRequest.typeB;
    }
    BsScene::RequestChange(PROC_SC_STAGE, typeA, faderA, typeB, faderB, 0);
    s_TransitionMgr.m_Return.Clear();
}

// 0x005B4F84 (name is ours)
s32 GetIndexOfKind(u32 kind, Name name)
{
    if (!IsKind(kind, name)) {
        return -1;
    }
    s32 count = 0;
    for (s32 i = 0; i < NAME_MAX; i++) {
        Name other = static_cast<Name>(i);
        if (other == name) {
            return count;
        }
        if (IsKind(kind, other)) {
            count++;
        }
    }
    return -1;
}

// 0x005B4FF0 (name is ours)
u32 GetSoundGroup(Name name)
{
    if (name < NAME_MAX) {
        return detail::s_SoundGroups[name];
    }
    return 0;
}

// 0x005B5008 (name is ours)
bool IsOnIsland()
{
    Name current = s_Current;
    if (IsKind(KIND_ISLAND, current)) {
        return true;
    }
    if (IsKind(KIND_TOUR, current) && TourResult::s_pInstance != NULL && TourResult::s_pInstance->m_State == 2) {
        return true;
    }
    return false;
}

// 0x005B507C (name is ours)
bool IsInOwnHouse()
{
    Name current = s_Current;
    if (IsOwnShowroom(current)) {
        return true;
    }
    u8 player = PlayerGetSaveNumber(netgame::PLAYER_NO_MY_NO);
    if (IsVillagePlayer(player) && IsHouseRoomOf(player, current)) {
        u32 owner = GetHouseOwner(current);
        if (owner < 4) {
            u8 state = GetGarden()->m_Houses.Get(owner)->m_State;
            if (state != 0 && state != 8) {
                return true;
            }
        }
    }
    return false;
}

// 0x005B5160 (name is ours)
bool IsPolice(Name name)
{
    return IsKind(KIND_POLICE, name);
}

// 0x005B518C (name is ours)
bool IsTent(Name name)
{
    // the first floors of the player houses and of the model house
    // 0x005B5204
    const u8 firstFloors[] = {NAME_PLAYER_HOUSE_0_1F, NAME_PLAYER_HOUSE_1_1F, NAME_PLAYER_HOUSE_2_1F,
                              NAME_PLAYER_HOUSE_3_1F, NAME_MODEL_HOUSE_1F};
    for (u32 i = 0; i < 5; i++) {
        if (firstFloors[i] == name) {
            SvHouse* house = GetHouseOfStage(name);
            if (house != NULL) {
                return house->m_State == 0;
            }
        }
    }
    return false;
}

// 0x005B520C (name is ours)
bool IsInPrivateRoom()
{
    Name current = s_Current;
    if (IsKind(KIND_SCREEN, current) || !IsKind(KIND_INDOOR, current)) {
        return false;
    }
    u8 player = PlayerGetSaveNumber(netgame::PLAYER_NO_MY_NO);
    if (IsVillagePlayer(player) && IsHouseRoomOf(player, current)) {
        return true;
    }
    if (IsOwnShowroom(current)) {
        return true;
    }
    if (current == NAME_CHIP_TUTORIAL) {
        return true;
    }
    if (IsHutOfFossilTour(current)) {
        return true;
    }
    if (IsHutOfBorrowingRace(current)) {
        return true;
    }
    return false;
}

// 0x005B533C (name is ours)
bool IsAloneInPrivateRoom()
{
    if (IsInPrivateRoom() && !(IsOwnShowroom(s_Current) && netgame::IsMultiplePlayer()) && IsAloneInRoom()) {
        return true;
    }
    return false;
}

// 0x005B538C (name is ours)
u8 GetLastStructure()
{
    return s_LastStructure;
}

// 0x005B539C (name is ours)
bool IsOutdoorField()
{
    Name current = s_Current;
    if (IsKind(KIND_SCREEN, current) || !IsKind(KIND_OUTDOOR, current)) {
        return false;
    }
    if (IsKind(KIND_VILLAGE, current) || IsKind(KIND_ISLAND, current) || IsKind(KIND_TOUR, current)) {
        return true;
    }
    return false;
}

// 0x005B5414 (name is ours)
void SetLastStructure(u8 structure)
{
    s_LastStructure = structure;
}

// 0x005B5424 (name is ours)
u32 GetEnvironmentSound(Name name)
{
    if (name < NAME_MAX) {
        return detail::s_EnvironmentSounds[name];
    }
    return 0xFFFFFFFF;
}

// 0x005B543C (name is ours)
bool IsHutOfBorrowingRace(Name name)
{
    if (IsTourStage() && TourResult::s_pInstance != NULL && IsKind(KIND_ISLAND_HUT, name) &&
        GetTourType(TourResult::s_pInstance->m_Tour) == 11) {
        return true;
    }
    return false;
}

// 0x005B549C (name is ours)
bool IsEvenIslandHut(Name name)
{
    if (IsKind(KIND_ISLAND_HUT, name)) {
        return name & 1;
    }
    return false;
}

// 0x005B54C8 (name is ours)
bool IsOddIslandHut(Name name)
{
    if (IsKind(KIND_ISLAND_HUT, name) && (name - NAME_ISLAND_HUT_0) % 2 == 1) {
        return true;
    }
    return false;
}

// 0x005B5500 (name is ours)
s8 GetOffsetX()
{
    return s_OffsetX;
}

// 0x005B5510 (name is ours)
void ClearOffset()
{
    s_OffsetX = 0;
    s_OffsetZ = 0;
}

// 0x005B5528 (name is ours)
bool IsInOwnRoom()
{
    Name current = s_Current;
    if (IsKind(KIND_SCREEN, current) || !IsKind(KIND_INDOOR, current)) {
        return false;
    }
    u8 player = PlayerGetSaveNumber(netgame::PLAYER_NO_MY_NO);
    if (IsVillagePlayer(player) && IsHouseRoomOf(player, current)) {
        return true;
    }
    if (IsOwnShowroom(current)) {
        return true;
    }
    return false;
}

// 0x005B55F0 (name is ours)
s8 GetOffsetZ()
{
    return s_OffsetZ;
}

// 0x005B5600 (name is ours)
bool Build(ScStage* scene)
{
    s_pScene = scene;
    s_pParent = scene;
    ResetStageList();
    const Info* info = GetInfoOf(s_Current);
    SetupField();
    if (IsKind(KIND_KOTOBUKI, s_Current)) {
        RunCommands(&detail::s_KotobukiInfo, s_BuildFunctions, NULL);
    }
    const Info* playerInfo = GetInfoOf(s_Current);
    if (IsKind(KIND_INDOOR, s_Current)) {
        ProcMgr::Create(PROC_BS_INDOOR_DOOR_SE, s_pParent, 0, PROC_TYPE_BASE);
    }
    oml::framework::Process* playerMgr = ProcMgr::Create(PROC_BS_PLAYER_MGR, s_pParent, 0, PROC_TYPE_BASE);
    if (playerMgr != NULL) {
        bool isPlaced = false;
        if (RunCommands(playerInfo, s_PlayerFunctions, &isPlaced) && isPlaced) {
            static_cast<BsPlayerMgr*>(playerMgr)->PlaceOtherPlayers();
        }
    }
    RunCommands(&detail::s_CommonInfo, s_BuildFunctions, NULL);
    if (IsKind(KIND_INDOOR, s_Current)) {
        RunCommands(&detail::s_IndoorInfo, s_BuildFunctions, NULL);
    }
    RunCommands(info, s_BuildFunctions, NULL);
    return true;
}

// 0x005B5760 (name is ours)
void Destroy()
{
    DestroyField();
    s_pScene = NULL;
}

// 0x005B577C (name is ours)
bool HasWorldCurve(Name name)
{
    if (name < NAME_MAX && (KIND_WORLD_CURVE & ~detail::s_Kinds[name]) == 0) {
        return true;
    }
    return false;
}

// 0x005B57A8 (name is ours)
const Info* GetInfo(Name name)
{
    if (name == NAME_CURRENT) {
        name = s_Current;
    }
    return GetInfoOf(name);
}

// 0x005B74E0 (name is ours)
u32 GetFurnitureCapacity(Name name)
{
    if (name < NAME_MAX) {
        return detail::s_FurnitureCapacities[name];
    }
    return 0;
}

// 0x005B74F8 (name is ours)
bool IsOwnHouseRoom(Name name)
{
    u8 player = PlayerGetSaveNumber(netgame::PLAYER_NO_MY_NO);
    if (IsVillagePlayer(player) && IsHouseRoomOf(player, name)) {
        return true;
    }
    return false;
}
} // namespace stage
