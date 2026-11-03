#include "stage/dTransitionMgr.h"
#include "Bs/dBsScene.h"
#include "Other/dActor.h"
#include "Other/dProcMgr.h"
#include "Player/dPlayerApi.h"
#include "Other/dTourResult.h"
#include "applet/dErrEulaMgr.h"
#include "netgame/dNetGameMgr.h"
#include "netgame/netgame_Api.h"
#include "stage/stage_Extern.h"
#include "stage/stage_Tables.h"

#include <cmath>
#include <cstring>

namespace stage {
namespace {
// the start in front of a structure, a block before its entrance
inline void MoveBeforeEntrance(nn::math::VEC3_* position)
{
    position->z += FIELD_UNIT_SIZE_Z;
}

// a structure with variants: the variant that stands on the field
inline u8 ResolveStructure(u8 structure)
{
    if (IsStationStructure(structure)) {
        return GetStationStructure();
    }
    if (IsTownHallStructure(structure)) {
        return GetTownHallStructure();
    }
    return structure;
}

// the place in front of the house of the own player, or else the default place of the stage
inline void GetFallbackPosition(nn::math::VEC3_* position, math::Angle16Vec3* rotation, Name target)
{
    if (IsKind(KIND_VILLAGE, target)) {
        rotation->y.value = 0;
        position->x = 1872.0f;
        position->z = 528.0f;
    } else if (IsKind(KIND_DOWNTOWN, target)) {
        rotation->y.value = 0x8000;
        position->x = 1040.0f;
        position->z = 496.0f;
    } else {
        rotation->y.value = 0;
        position->x = 768.0f;
        position->z = 768.0f;
    }
}
} // namespace

// 0x005B57D0 (name is ours)
void TransitionMgr::AddHandle(s32 handle)
{
    m_Handles[m_HandleCount] = handle;
    m_HandleCount++;
}

// 0x005B57EC (name is ours)
bool TransitionMgr::TransitionToReturnPoint(u8 typeA, u8 faderA, u8 typeB, u8 faderB)
{
    if (s_Next < NAME_MAX) {
        return false;
    }
    if (TransitionTo(m_Return.stage, &m_Return.position, &m_Return.rotation, m_Return.action, typeA, faderA, typeB,
                     faderB)) {
        m_Return.Clear();
        return true;
    }
    return false;
}

// 0x005B5864 | libgarden [tier A]
bool TransitionMgr::Transition(Name stage, u8 typeA, u8 faderA, u8 typeB, u8 faderB)
{
    if (s_Next < NAME_MAX) {
        return false;
    }
    if (netgame::NetGameMgr::s_pInstance->m_IsSceneChanging && !applet::ErrEulaMgr::s_pInstance->m_FB0) {
        return false;
    }
    s_Next = stage;
    ProcName scene = PROC_SC_STAGE;
    if (IsKind(KIND_MINI_GAME, stage)) {
        scene = stage == NAME_MINI_GAME_0 ? PROC_SC_STAGE_MINI_GAME_0 : PROC_SC_STAGE_MINI_GAME_1;
    }
    BsScene::RequestChange(scene, typeA, faderA, typeB, faderB, 0);
    m_IsToStructure = false;
    m_IsWithoutPosition = true;
    m_Structure = 0xFC;
    if (netgame::IsMultiPlayer() && netgame::NetGameMgr::Get54() == 0) {
        netgame::NetGameMgr::s_pInstance->m_IsSceneChanging = true;
    }
    for (s32 i = 0; i < HANDLE_MAX - 1; i++) {
        m_Handles[i] = -1;
    }
    m_HandleCount = 0;
    m_Handles[HANDLE_MAX - 1] = -1;
    g_PlayerStartInfo.Reset();
    return true;
}

// 0x005B5998 (name is ours)
void TransitionMgr::RemoveHandle(s32 handle)
{
    for (s32 i = 0; i < HANDLE_MAX; i++) {
        if (m_Handles[i] == handle) {
            m_Handles[i] = m_Handles[m_HandleCount - 1];
            m_Handles[m_HandleCount - 1] = -1;
            m_HandleCount--;
            return;
        }
    }
}

// 0x005B5A5C (name is ours)
void TransitionMgr::SetReturnPoint(const nn::math::VEC3_& position, u32 action, u16 rotationY)
{
    m_Return.stage = s_Current;
    m_Return.action = action;
    m_Return.position = position;
    m_Return.rotation = math::Angle16Vec3{0, rotationY, 0};
    m_Return.isFromPlayer = false;
}

// 0x005B5AC8 (name is ours)
Name TransitionMgr::GetEntranceTarget(u32 index) const
{
    const Info* info = GetInfoOf(s_Current);
    if (info == NULL || info->entrances == NULL || info->entrances->count <= index) {
        return NAME_NONE;
    }
    const Entrance& entrance = info->entrances->entrances[index];
    u32 owner = 0;
    if (IsKind(KIND_PLAYER_HOUSE, s_Current)) {
        owner = GetHouseOwner(s_Current);
        if (owner >= 4) {
            owner = 0;
        }
    }
    return static_cast<Name>(entrance.targets[owner]);
}

// 0x005B5B50 (name is ours)
void TransitionMgr::ClearReturnPoint()
{
    m_Return.Clear();
}

// 0x005B5B68 (name is ours)
bool TransitionMgr::GetExitPoint(ExitPoint* pPoint, u32 index) const
{
    if (pPoint == NULL) {
        return false;
    }
    const Info* info = GetInfoOf(s_Current);
    if (info == NULL || info->entrances == NULL) {
        return false;
    }
    if (info->entrances->count <= index) {
        return false;
    }
    const Entrance& entrance = info->entrances->entrances[index];
    pPoint->type = entrance.type;
    nn::math::VEC3_ position;
    math::Angle16Vec3 rotation;
    if (IsKind(KIND_VILLAGE, s_Current) && IsKind(KIND_AUTO_CAMP, static_cast<Name>(entrance.targets[0]))) {
        // the way to the campground stands on a place of its own
        position = *GetAutoCampPosition();
        rotation = math::Angle16Vec3{0, *GetAutoCampRotationY(), 0};
    } else {
        position = entrance.position;
        rotation = entrance.rotation;
    }
    pPoint->rotation = rotation;
    pPoint->rotation.y.value -= 0x8000;
    pPoint->position = position;
    nn::math::VEC3_ offset;
    offset.x = 0.0f;
    offset.y = 0.0f;
    offset.z = FIELD_UNIT_SIZE_Z;
    RotateY(&offset, pPoint->rotation.y);
    pPoint->position.x += offset.x;
    pPoint->position.y += offset.y;
    pPoint->position.z += offset.z;
    return true;
}

// 0x005B5D14 (name is ours)
bool TransitionMgr::TransitionByEntrance(u32 index, u8 typeA, u8 faderA, u8 typeB, u8 faderB)
{
    return TransitionByEntrance(s_Current, index, typeA, faderA, typeB, faderB);
}

// 0x005B5D58 (name is ours)
bool TransitionMgr::TransitionToStructure(u8 structure, const nn::math::VEC3_* offset, const math::Angle16Vec3* rotation,
                                          u8 typeA, u8 faderA, u8 typeB, u8 faderB)
{
    if (s_Next < NAME_MAX) {
        return false;
    }
    structure = ResolveStructure(structure);
    Name target = NAME_VILLAGE;
    if (structure != 0xFC) {
        target = GetStageOfStructure(structure);
    }
    nn::math::VEC3_ position = VEC3_ZERO;
    math::Angle16Vec3 targetRotation = *rotation;
    u32 action = PLAYER_ACTION_IDLE;
    if (structure == 0xFC) {
        position.x = 1872.0f;
        position.z = 528.0f;
    } else if (IsStructurePlaced(structure)) {
        GetStructurePosition(&position, structure);
        if (structure == STRUCTURE_NOOK_STORE) {
            if (std::strncmp(s_pShopNameB, GetShopName(), std::strlen(s_pShopNameB)) == 0) {
                position.z -= FIELD_HALF_UNIT_SIZE_Z;
            }
        } else if (structure == STRUCTURE_PHOTO_BOOTH) {
            position.z -= FIELD_HALF_UNIT_SIZE_Z;
        }
        if (IsTownHallStructure(structure) && GetTownHallStructure() != STRUCTURE_TOWN_HALL_CLASSIC) {
            position.z += FIELD_UNIT_SIZE_Z;
        }
    } else {
        u8 player = PlayerGetSaveNumber(netgame::PLAYER_NO_MY_NO);
        if (player >= 4 || !GetStructurePosition(&position, player)) {
            GetFallbackPosition(&position, &targetRotation, target);
        }
    }
    position.x += offset->x;
    position.y += offset->y;
    position.z += offset->z;
    if (TransitionTo(target, &position, &targetRotation, action, typeA, faderA, typeB, faderB)) {
        return true;
    }
    return false;
}

// 0x005B6054 (name is ours)
bool TransitionMgr::TransitionFromStructure(u8 typeA, u8 faderA, u8 typeB, u8 faderB, Name stage)
{
    if (s_Next < NAME_MAX) {
        return false;
    }
    if (stage == NAME_CURRENT) {
        stage = s_Current;
    }
    u8 structure = stage < NAME_MAX ? detail::s_Structures[stage] : 0xFC;
    structure = ResolveStructure(structure);
    bool isCampingCar = structure == STRUCTURE_CAMPING_CAR_A || structure == STRUCTURE_CAMPING_CAR_B;
    Name target = NAME_VILLAGE;
    if (isCampingCar) {
        target = GetAutoCampStage();
    } else if (structure != 0xFC) {
        target = GetStageOfStructure(structure);
    }
    nn::math::VEC3_ position = VEC3_ZERO;
    math::Angle16Vec3 rotation = ANGLE16_VEC3_ZERO;
    u32 action = PLAYER_ACTION_DOOR_EXIT_WAIT;
    if (structure == 0xFC) {
        position.x = 1872.0f;
        position.z = 528.0f;
        action = PLAYER_ACTION_IDLE;
        MoveBeforeEntrance(&position);
    } else if (IsStructurePlaced(structure)) {
        GetStructurePosition(&position, structure);
        if (isCampingCar) {
            position.x += FIELD_HALF_UNIT_SIZE_X;
            rotation = math::Angle16Vec3{0, 0x2000, 0};
            position.z += FIELD_HALF_UNIT_SIZE_Z;
        }
        if (structure == STRUCTURE_NOOK_STORE) {
            if (std::strncmp(s_pShopNameA, GetShopName(), std::strlen(s_pShopNameA)) == 0) {
                position.z -= FIELD_HALF_UNIT_SIZE_Z;
                action = PLAYER_ACTION_DOOR_EXIT_BEGIN;
            }
        } else if (structure == STRUCTURE_PHOTO_BOOTH) {
            position.z -= FIELD_HALF_UNIT_SIZE_Z;
        }
        if (IsTownHallStructure(structure) && GetTownHallStructure() != STRUCTURE_TOWN_HALL_CLASSIC) {
            position.z += FIELD_UNIT_SIZE_Z;
        }
        if (!isCampingCar) {
            MoveBeforeEntrance(&position);
        }
    } else if (isCampingCar) {
        // the camping car is not placed: at the side of the campground
        if (GetAutoCampStage() == NAME_AUTO_CAMP_L) {
            rotation.y.value = 0xC000;
            position.x = 460.0f;
            position.z = 340.0f;
        } else {
            rotation.y.value = 0x4000;
            position.x = 50.0f;
            position.z = 340.0f;
        }
        action = PLAYER_ACTION_IDLE;
    } else {
        action = PLAYER_ACTION_IDLE;
        u8 player = PlayerGetSaveNumber(netgame::PLAYER_NO_MY_NO);
        if (player >= 4 || !GetStructurePosition(&position, player)) {
            GetFallbackPosition(&position, &rotation, target);
        }
        MoveBeforeEntrance(&position);
    }
    position.x += GetRandomF(0.1f) - 0.05f;
    if (TransitionTo(target, &position, &rotation, action, typeA, faderA, typeB, faderB)) {
        m_Structure = structure;
        m_IsToStructure = true;
        return true;
    }
    return false;
}

// 0x005B64AC | libgarden [tier A]
void TransitionMgr::SetReturnParam()
{
    m_Return.stage = s_Current;
    if (IsKind(KIND_AUTO_CAMP, s_Current) && !netgame::IsMultiplePlayer()) {
        const Info* info = GetInfoOf(s_Current);
        if (info != NULL && info->entrances != NULL && info->entrances->count != 0) {
            // near a door of the camping car: the entrance of the campground becomes the return point
            const Entrance* entrance = info->entrances->entrances;
            const Actor* player = reinterpret_cast<const Actor*>(PlayerGetActor(netgame::PLAYER_NO_MY_NO, false));
            nn::math::VEC3_ door0;
            nn::math::VEC3_ door1;
            GetAutoCampDoors(&door0, &door1);
            f32 distance0;
            f32 distance1;
            if (player != NULL) {
                f32 dx0 = door0.x - player->m_Position.x;
                f32 dz0 = door0.z - player->m_Position.z;
                distance0 = std::sqrt(dx0 * dx0 + dz0 * dz0);
                f32 dx1 = door1.x - player->m_Position.x;
                f32 dz1 = door1.z - player->m_Position.z;
                distance1 = std::sqrt(dx1 * dx1 + dz1 * dz1);
            } else {
                distance0 = distance1 = FIELD_UNIT_SIZE_X * 16.0f;
            }
            f32 limit = FIELD_UNIT_SIZE_X * 1.1f;
            if ((distance0 < limit || limit > distance1) && PlayerCanReturn()) {
                m_Return.action = PLAYER_ACTION_IDLE;
                m_Return.position = entrance->position;
                m_Return.rotation = entrance->rotation;
                m_Return.isFromPlayer = false;
                return;
            }
        }
    }
    m_Return.action = PlayerGetReturnPoint(&m_Return.position, &m_Return.rotation);
    m_Return.isFromPlayer = true;
}

// 0x005B6660 (name is ours)
bool TransitionMgr::TransitionTo(Name stage, const nn::math::VEC3_* position, const math::Angle16Vec3* rotation, u32 action,
                                 u8 typeA, u8 faderA, u8 typeB, u8 faderB)
{
    if (s_Next < NAME_MAX) {
        return false;
    }
    m_Position = *position;
    m_Rotation = *rotation;
    m_Action = action;
    if (Transition(stage, typeA, faderA, typeB, faderB)) {
        m_IsWithoutPosition = false;
        return true;
    }
    return false;
}

// 0x005B66FC (name is ours)
bool TransitionMgr::TransitionToPlayerHouse(u8 owner, u8 typeA, u8 faderA, u8 typeB, u8 faderB)
{
    if (s_Next < NAME_MAX) {
        return false;
    }
    nn::math::VEC3_ position = VEC3_ZERO;
    math::Angle16Vec3 rotation = ANGLE16_VEC3_ZERO;
    u32 action = PLAYER_ACTION_DOOR_EXIT_WAIT;
    if (IsPlayerHouseStructure(owner)) {
        GetStructurePosition(&position, owner);
    } else {
        position.x = 950.0f;
        position.z = 600.0f;
        action = PLAYER_ACTION_IDLE;
    }
    MoveBeforeEntrance(&position);
    if (TransitionTo(NAME_VILLAGE, &position, &rotation, action, typeA, faderA, typeB, faderB)) {
        m_Structure = owner;
        m_IsToStructure = true;
        return true;
    }
    return false;
}

// 0x005B6820 (name is ours)
bool TransitionMgr::TransitionWithStartPositions(Name stage, u32 action, u32 startAction, const nn::math::VEC3_* position0,
                                                 const math::Angle16Vec3* rotation0, const nn::math::VEC3_* position1,
                                                 const math::Angle16Vec3* rotation1, const nn::math::VEC3_* position2,
                                                 const math::Angle16Vec3* rotation2, const nn::math::VEC3_* position3,
                                                 const math::Angle16Vec3* rotation3, u8 typeA, u8 faderA, u8 typeB,
                                                 u8 faderB)
{
    return TransitionWithStartPositions(stage, action, startAction, action, startAction, position0, rotation0, position1,
                                        rotation1, position2, rotation2, position3, rotation3, typeA, faderA, typeB,
                                        faderB);
}

// 0x005B6848 (name is ours)
bool TransitionMgr::TransitionWithStartPositions(Name stage, u32 hostAction, u32 startHostAction, u32 guestAction,
                                                 u32 startGuestAction, const nn::math::VEC3_* position0,
                                                 const math::Angle16Vec3* rotation0, const nn::math::VEC3_* position1,
                                                 const math::Angle16Vec3* rotation1, const nn::math::VEC3_* position2,
                                                 const math::Angle16Vec3* rotation2, const nn::math::VEC3_* position3,
                                                 const math::Angle16Vec3* rotation3, u8 typeA, u8 faderA, u8 typeB,
                                                 u8 faderB)
{
    PlayerStartPosition positions[4] = {
        PlayerStartPosition(*position0, *rotation0),
        PlayerStartPosition(*position1, *rotation1),
        PlayerStartPosition(*position2, *rotation2),
        PlayerStartPosition(*position3, *rotation3),
    };
    PlayerStartPosition& own = positions[netgame::GetMyPlayerNo() & 3];
    math::Angle16Vec3 rotation = {0, own.rotationY, 0};
    u32 action = netgame::IsHost() ? hostAction : guestAction;
    if (!TransitionTo(stage, &own.position, &rotation, action, typeA, faderA, typeB, faderB)) {
        return false;
    }
    g_PlayerStartInfo.Set(stage, startHostAction, startGuestAction, positions[0], positions[1], positions[2],
                          positions[3]);
    return true;
}

// 0x005B6944 (name is ours)
bool TransitionMgr::TransitionFromTourHut(u8 typeA, u8 faderA, u8 typeB, u8 faderB)
{
    if (s_Next < NAME_MAX) {
        return false;
    }
    u8 structure = 0xFC;
    if (IsKind(KIND_ISLAND_HUT, s_Current)) {
        structure = s_Current - 63;
    }
    if (structure == 0xFC) {
        return false;
    }
    if (TourResult::s_pInstance == NULL) {
        return false;
    }
    Name target;
    s8 type = GetTourType(TourResult::s_pInstance->m_Tour);
    if (type == 11) {
        target = NAME_ISLAND_BORROWING_RACE;
    } else if (type == 9) {
        target = NAME_ISLAND_FOSSIL_DIG;
    } else {
        return false;
    }
    nn::math::VEC3_ position = VEC3_ZERO;
    math::Angle16Vec3 rotation = ANGLE16_VEC3_ZERO;
    u32 action = PLAYER_ACTION_DOOR_EXIT_WAIT;
    GetStructurePosition(&position, structure);
    MoveBeforeEntrance(&position);
    if (!TransitionTo(target, &position, &rotation, action, typeA, faderA, typeB, faderB)) {
        return false;
    }
    m_Structure = structure;
    m_IsToStructure = true;
    return true;
}

// the first floor of the house of the own player
static Name GetOwnHouseFirstFloor()
{
    switch (PlayerGetSaveNumber(netgame::PLAYER_NO_MY_NO)) {
    case 1:
        return NAME_PLAYER_HOUSE_1_1F;
    case 2:
        return NAME_PLAYER_HOUSE_2_1F;
    case 3:
        return NAME_PLAYER_HOUSE_3_1F;
    default:
        return NAME_PLAYER_HOUSE_0_1F;
    }
}

// 0x005B6AA0 (name is ours)
bool TransitionMgr::ReturnOrGoHome(u8 typeA, u8 faderA, u8 typeB, u8 faderB)
{
    bool result = false;
    if (m_Return.stage == NAME_NONE) {
        result = GetTransitionMgr()->TransitionTo(GetOwnHouseFirstFloor(), &s_HouseStartPosition, &s_ZeroRotation,
                                                  PLAYER_ACTION_IDLE, 0, 0, 1, 1);
    } else if (s_Next >= NAME_MAX &&
               TransitionTo(m_Return.stage, &m_Return.position, &m_Return.rotation, m_Return.action, typeA, faderA,
                            typeB, faderB)) {
        m_Return.Clear();
        result = true;
    }
    if (result) {
        netgame::NetGameMgr::s_pInstance->m_Wait.Reset(true);
    }
    return result;
}

// 0x005B6BCC (name is ours)
bool TransitionMgr::ReturnOrGoHomeKeepWait(u8 typeA, u8 faderA, u8 typeB, u8 faderB)
{
    bool result = false;
    if (m_Return.stage == NAME_NONE) {
        return GetTransitionMgr()->TransitionTo(GetOwnHouseFirstFloor(), &s_HouseStartPosition, &s_ZeroRotation,
                                                PLAYER_ACTION_IDLE, 0, 0, 1, 1);
    }
    if (s_Next >= NAME_MAX &&
        TransitionTo(m_Return.stage, &m_Return.position, &m_Return.rotation, m_Return.action, typeA, faderA, typeB,
                     faderB)) {
        m_Return.Clear();
        result = true;
        netgame::NetGameMgr::s_pInstance->m_Wait.Reset(true);
    }
    return result;
}

// 0x005B6CF0 (name is ours)
bool TransitionToMiniGame0(u32, u32, u8 type, u8 typeB, u8 faderB)
{
    bool result = s_TransitionMgr.Transition(NAME_MINI_GAME_0, type, type, typeB, faderB);
    s_IsMiniGameStarted = 1;
    return result;
}

// 0x005B6D30 (name is ours)
bool TransitionMgr::TransitionToLastStructure(u8 typeA, u8 faderA, u8 typeB, u8 faderB)
{
    if (s_Next < NAME_MAX) {
        return false;
    }
    u8 structure = s_LastStructure;
    Name target = NAME_VILLAGE;
    if (structure != 0xFC) {
        target = GetStageOfStructure(structure);
    }
    nn::math::VEC3_ position = VEC3_ZERO;
    math::Angle16Vec3 rotation = ANGLE16_VEC3_ZERO;
    u32 action = PLAYER_ACTION_DOOR_EXIT_WAIT;
    if (structure == 0xFC) {
        position.x = 950.0f;
        position.z = 600.0f;
        action = PLAYER_ACTION_IDLE;
    } else {
        GetStructurePosition(&position, structure);
    }
    MoveBeforeEntrance(&position);
    if (TransitionTo(target, &position, &rotation, action, typeA, faderA, typeB, faderB)) {
        m_Structure = structure;
        m_IsToStructure = true;
        return true;
    }
    return false;
}

// 0x005B6E60 (name is ours)
bool TransitionMgr::TransitionByEntrance(Name stage, u32 index, u8 typeA, u8 faderA, u8 typeB, u8 faderB)
{
    if (s_Next < NAME_MAX) {
        return false;
    }
    const Info* info = GetInfoOf(stage == NAME_CURRENT ? s_Current : stage);
    if (info == NULL) {
        return false;
    }
    const EntranceList* list = info->entrances;
    if (list == NULL || list->entrances == NULL || list->count <= index) {
        return false;
    }
    const Entrance& entrance = list->entrances[index];
    u32 owner = 0;
    if (IsKind(KIND_PLAYER_HOUSE, s_Current)) {
        owner = GetHouseOwner(s_Current);
        if (owner >= 4) {
            owner = 0;
        }
    }
    u8 target = entrance.targets[owner];
    switch (target) {
    case TARGET_EXIT:
        return TransitionFromStructure(typeA, faderA, typeB, faderB, NAME_CURRENT);
    case TARGET_STRUCTURE: {
        u8 structure = s_Current < NAME_MAX ? detail::s_Structures[s_Current] : 0xFC;
        nn::math::VEC3_ position = entrance.position;
        math::Angle16Vec3 rotation = entrance.rotation;
        if (IsTownHallStructure(structure)) {
            u8 townHall = GetTownHallStructure();
            if (townHall == STRUCTURE_TOWN_HALL_ZEN || townHall == STRUCTURE_TOWN_HALL_FAIRY_TALE ||
                townHall == STRUCTURE_TOWN_HALL_MODERN) {
                position.z -= FIELD_UNIT_SIZE_Z;
            }
        }
        return TransitionToStructure(structure, &position, &rotation, typeA, faderA, typeB, faderB);
    }
    case TARGET_LAST_STRUCTURE:
        return TransitionToLastStructure(typeA, faderA, typeB, faderB);
    case TARGET_TOUR_HUT:
        return TransitionFromTourHut(typeA, faderA, typeB, faderB);
    case TARGET_RESET_CENTER: {
        // the village in front of the reset center (the field object of building 0x4F), with the default fades
        nn::math::VEC3_ position;
        position.x = 1864.0f;
        position.y = 112.0f;
        position.z = 528.0f;
        FieldObjectList* objects = GetFieldObjectList(0);
        FieldObjectIterator found = GetFieldObjectEnd(objects);
        for (FieldObjectIterator it(objects); it != GetFieldObjectEnd(objects); ++it) {
            if (it.kind == 79) {
                found = it;
                break;
            }
        }
        if (found != GetFieldObjectEnd(objects)) {
            UnitToWorld(&position, found.unitX, found.unitZ);
        }
        math::Angle16Vec3 rotation = {0, 0, 0};
        return TransitionTo(NAME_VILLAGE, &position, &rotation, PLAYER_ACTION_IDLE, 0, 0, 1, 1);
    }
    case TARGET_PLAYER_HOUSE_0:
    case TARGET_PLAYER_HOUSE_1:
    case TARGET_PLAYER_HOUSE_2:
    case TARGET_PLAYER_HOUSE_3:
        return TransitionToPlayerHouse(target - TARGET_PLAYER_HOUSE_0, typeA, faderA, typeB, faderB);
    case TARGET_STATION: {
        if (s_Next < NAME_MAX) {
            return false;
        }
        u8 structure = GetStationStructure();
        nn::math::VEC3_ position = VEC3_ZERO;
        math::Angle16Vec3 rotation = ANGLE16_VEC3_ZERO;
        u32 action = PLAYER_ACTION_DOOR_EXIT_WAIT;
        GetStructurePosition(&position, structure);
        MoveBeforeEntrance(&position);
        if (TransitionTo(NAME_VILLAGE, &position, &rotation, action, typeA, faderA, typeB, faderB)) {
            m_Structure = structure;
            m_IsToStructure = true;
            return true;
        }
        return false;
    }
    default:
        break;
    }
    // a stage: the entrance of the target stage
    Name next = target == NAME_CURRENT ? s_Current : static_cast<Name>(target);
    u8 targetEntrance = entrance.targetEntrance;
    const Info* targetInfo = GetInfoOf(next);
    if (targetInfo == NULL || targetInfo->entrances == NULL || targetInfo->entrances->entrances == NULL) {
        return false;
    }
    m_Entrance = targetInfo->entrances->entrances[targetEntrance];
    nn::math::VEC3_ position;
    math::Angle16Vec3 rotation = {0, 0, 0};
    if (IsKind(KIND_AUTO_CAMP, s_Current) && entrance.targets[0] == NAME_VILLAGE) {
        // out of the campground: in front of the way to it in the village
        rotation = math::Angle16Vec3{0, *GetAutoCampRotationY(), 0};
        position = *GetAutoCampPosition();
        nn::math::VEC3_ offset;
        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = FIELD_UNIT_SIZE_Z;
        RotateY(&offset, rotation.y);
        position.x += offset.x;
        position.y += offset.y;
        position.z += offset.z;
    } else {
        position = m_Entrance.position;
        rotation = m_Entrance.rotation;
    }
    return TransitionTo(static_cast<Name>(target), &position, &rotation, m_Entrance.action, typeA, faderA, typeB, faderB);
}
} // namespace stage
