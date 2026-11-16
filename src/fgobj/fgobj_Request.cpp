// The requests of this console for changes of the field objects at a unit (by the action of the player: picking up,
// throwing, planting, burying, displaying a pattern, shaking, cutting, digging) and changes that are made directly
// (the name of the file and all names but GetShake are ours).
#include "fgobj/dPlace.h"
#include "fgobj/fgobj_Api.h"
#include "fgobj/fgobj_Exe.h"
#include "fgobj/fgobj_Extern.h"
#include "fgobj/dPacket.h"
#include "fgobj/dObjectBury.h"
#include "fgobj/dObjectSwing.h"
#include "fgobj/dObjectOther.h"
#include "fgobj/dObjectBase.h"
#include "Ac/dAcPlayer.h"
#include "demo/demo_Extern.h"
#include "field/dFdInfo.h"
#include "field/dField.h"
#include "Item/item_Api.h"
#include "Player/dPlayerApi.h"
#include "netgame/netgame_Api.h"
#include "script/dScriptExtern.h"
#include "stage/stage_Extern.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvCensus.h"
#include "Sv/dSvMail.h"
#include "Sv/dSvShopZakkaExtern.h"
#include "Sv/dSvFgNameExtern.h"
#include "Sv/dSvPlayer.h"

namespace fgobj {
namespace {
// the action at the unit x, z of the current stage ends at once and the objects of the unit are made new
inline void FinishPlaceActionAt(s32 x, s32 z)
{
    s32 index = FindPlaceAction(x, z, stage::GetCurrent());
    if (index != -1) {
        PlaceAction& action = s_PlaceActions[index];
        action.PlaceResult();
        action.SendUnlockRequest();
        action.placeType = PLACE_TYPE_NONE;
        action.state = PlaceAction::STATE_NONE;
    }
    RefreshUnit(x, z, true);
}

// the field of stage (NAME_CURRENT: the current field)
inline Field* GetFieldOfStage(stage::Name stage)
{
    if (stage == stage::NAME_CURRENT) {
        return FdInfo::GetCurrentField();
    }
    return stage::GetField(GetFieldName(stage));
}

// whether a shaken bee tree lets the bees out (only for the player alone, after the first day and without the states of
// the objects 0x00950534 and 0x00951155; meaning of these unknown)
inline bool CanReleaseBees()
{
    if (netgame::IsMultiPlayer()) {
        return false;
    }
    SvPlayer* player = g_UnkAAFF18.pPlayer;
    if (player == NULL || !player->playerFlags.finishedFirstDay) {
        return false;
    }
    if (g_pUnk950534 == NULL || *Func2AB114(g_pUnk950534, 12) != 0) {
        return false;
    }
    return g_Byte0x951155 == 0;
}

// the shake types of GetShake
inline TreeShakeType GetBeesShake()
{
    return CanReleaseBees() ? TREE_SHAKE_TYPE_BEES : TREE_SHAKE_TYPE_NONE;
}

// the angle within which the player looks in one of the eight directions
const s16 DIRECTION_TOLERANCE = 0x105B;
// the initiative of a broken ore rock in the village (meaning of the value unknown)
const s32 INITIATIVE_ORE_ROCK = 89;
// the type of the request for a rock that is hit with a tool: the coin stone of this player, a rock that breaks into an
// ore or a stone egg (after: the item), else a hit
inline PlaceType GetRockPlaceType(const SvFgName* fg, SvFgName* after)
{
    if (!netgame::IsMultiplePlayer() && fg->GetCoinStonePlayer() == SvGetPlayerSaveNo(netgame::PLAYER_NO_MY_NO)) {
        return PLACE_TYPE_HIT_COIN_STONE;
    }
    if (fg->IsOreRock()) {
        u32 index = fg->GetOreIndex();
        *after = SvFgName(static_cast<SvFgName::ID>(s_OreRockItems[index < 6 ? index : 0]));
        if (stage::IsKind(stage::KIND_VILLAGE, stage::GetCurrent())) {
            Func24BA04(&SvGetCurrentPlayer()->initiative, INITIATIVE_ORE_ROCK, 1);
        }
        return PLACE_TYPE_BREAK_ROCK;
    }
    if (!fg->IsEggRock()) {
        return PLACE_TYPE_HIT_ITEM;
    }
    *after = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_STONE_EGG));
    return PLACE_TYPE_BREAK_ROCK;
}

// a pick-up with full pockets waits for a confirmation (not for a pattern)
inline PlaceType GetPickUpType(const SvFgName& item, PlaceType type)
{
    if (!item.IsPattern() && g_UnkAAFF18.pPlayer->inventory.FindEmptySlot() < 0) {
        if (type == PLACE_TYPE_PICK_UP) {
            return PLACE_TYPE_PICK_UP_FULL;
        }
        if (type == PLACE_TYPE_PICK_UP_SPECIAL) {
            return PLACE_TYPE_PICK_UP_SPECIAL_FULL;
        }
        if (type == PLACE_TYPE_DIG_ITEM) {
            return PLACE_TYPE_UNKNOWN_15;
        }
    }
    return type;
}

// the chance (in percent) for a special stump (always with the silver axe)
const u32 SPECIAL_STUMP_CHANCE = 5;
} // namespace

// 0x005990AC | libgarden [tier A]
bool FindPlaceableUnitAroundPlayer(s32& x, s32& z, u8& layer, FindUnitForPlayerCheck check, bool placeInFront)
{
    AcPlayer* player = demo::PlayerGetActorEntry(netgame::PLAYER_NO_MY_NO, false);
    if (player == NULL) {
        return false;
    }
    long playerX;
    long playerZ;
    if (!PlayerGetPosition(playerX, playerZ, netgame::PLAYER_NO_MY_NO, true)) {
        return false;
    }
    Actor* actor = static_cast<Actor*>(player);
    // the direction of the player (within 23 degrees of one of the eight)
    u8 direction = 0;
    for (s32 i = 0; i < 8; i++) {
        s16 difference = actor->m_Rotation.y.value - s_PlayerDirectionAngles[i];
        if (difference < 0) {
            difference = -difference;
        }
        if (difference < DIRECTION_TOLERANCE) {
            direction = i;
            break;
        }
    }
    const s32* order = s_PlaceUnitOrders[direction];
    if (stage::IsKind(stage::KIND_INDOOR, stage::GetCurrent())) {
        // indoors first on something in front of the player
        s32 unitX = s_PlaceUnitOffsets[0][order[1]] + playerX;
        s32 unitZ = s_PlaceUnitOffsets[1][order[1]] + playerZ;
        if (Func4E3D48(unitX, unitZ)) {
            x = unitX;
            z = unitZ;
            layer = 1;
            return true;
        }
    }
    for (s32 i = 0; i < static_cast<s32>(PLACE_UNIT_NUM); i++) {
        s32 index = i;
        if (placeInFront) {
            index = (i + 1) % static_cast<s32>(PLACE_UNIT_NUM);
        }
        s32 unitX = s_PlaceUnitOffsets[0][order[index]] + playerX;
        s32 unitZ = s_PlaceUnitOffsets[1][order[index]] + playerZ;
        if (Func165460(unitX, unitZ, stage::NAME_CURRENT, check, true)) {
            if (!stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent()) || Func614B90(unitX, unitZ, static_cast<const nn::math::VEC3*>(&actor->m_Position))) {
                x = unitX;
                z = unitZ;
                layer = 0;
                return true;
            }
        }
        if (!stage::IsKind(stage::KIND_INDOOR, stage::GetCurrent())) {
            continue;
        }
        if (Func4E3D48(unitX, unitZ)) {
            x = unitX;
            z = unitZ;
            layer = 1;
            return true;
        }
        if (i == 3) {
            // the unit two steps in front of the player in the four main directions
            s32 frontX = s_PlaceUnitOffsets[0][order[1]] + playerX;
            s32 frontZ = s_PlaceUnitOffsets[1][order[1]] + playerZ;
            if (direction == 0) {
                frontZ++;
            } else if (direction == 2) {
                frontX++;
            } else if (direction == 4) {
                frontZ--;
            } else if (direction == 6) {
                frontX--;
            } else {
                continue;
            }
            if (!Func4E3D48(frontX, frontZ)) {
                continue;
            }
            x = frontX;
            z = frontZ;
            layer = 1;
            return true;
        }
    }
    return false;
}

// 0x005993A8 (name is ours)
s32 RequestCut(const SvFgName* fg, s32 x, s32 z)
{
    SvFgName after(*fg);
    if (fg->IsFg()) {
        const item::ParamFgEntry* param = item::GetParamFgEntry(*fg);
        if (fg->IsGrownTree() && param->size >= 1) {
            s32 size = param->size;
            PlaceType type = PLACE_TYPE_CUT_TREE;
            u8 shake = GetShake(after, *fg);
            if (shake == TREE_SHAKE_TYPE_BEES) {
                type = PLACE_TYPE_CUT_TREE_BEES;
            }
            const u32 counter = GetTreeCutCounter(x, z);
            if (size >= 4) {
                size--;
            }
            // the tree falls with the last cut
            bool isFelled = false;
            if (size <= static_cast<s32>(counter + 1)) {
                isFelled = true;
                GetStumpOfTree(&after);
                // (the shake type is compared with a place type here, so this never happens)
                if (shake == PLACE_TYPE_CUT_TREE_BEES) {
                    shake = TREE_SHAKE_TYPE_NONE;
                    type = PLACE_TYPE_CUT_TREE;
                }
            }
            AcPlayer* player = demo::PlayerGetActorEntry(netgame::PLAYER_NO_MY_NO, false);
            bool fallDirection = player == NULL ? false : static_cast<s16>(static_cast<Actor*>(player)->m_Rotation.y.value) > 0;
            s32 handle = RequestPlace(type, *fg, after, SvFgName::s_Empty, x, z, 0, fallDirection, shake, isFelled, counter);
            if (isFelled) {
                GetCensus()->AddTreesCutDown(1);
            }
            return handle;
        }
    }
    if (fg->GetFgKind() != SvFgName::FG_KIND_ROCK) {
        return -1;
    }
    PlaceType type = GetRockPlaceType(fg, &after);
    return RequestPlace(type, *fg, after, SvFgName::s_Empty, x, z, 0, 0, TREE_SHAKE_TYPE_NONE, false, 0);
}

// 0x0059990C (name is ours)
s32 RequestDig(const SvFgName* fg, s32 x, s32 z)
{
    SvFgName after(*fg);
    PlaceType type;
    if (fg->IsEmpty()) {
        // an empty unit: a hole in the ground, nothing on the stone ground
        switch (Func300B7C(x, z, false)) {
        case 0:
        case 1:
            type = PLACE_TYPE_DIG_HOLE;
            after = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_HOLE));
            break;
        case 2:
            type = PLACE_TYPE_NONE;
            break;
        default:
            return -1;
        }
    } else if (fg->IsBuried()) {
        type = fg->GetKind() == item::KIND_TIME_CAPSULE ? PLACE_TYPE_UNKNOWN_16 : PLACE_TYPE_DIG_ITEM;
        after = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_HOLE));
    } else if (fg->IsName(SvFgName::NAME_HOLE)) {
        after = SvFgName::s_Empty;
        type = PLACE_TYPE_FILL;
    } else if (fg->GetFgKind() == SvFgName::FG_KIND_ROCK) {
        type = GetRockPlaceType(fg, &after);
    } else {
        const u8 fgKind = fg->GetFgKind();
        if (SvFgName::IsFlowerFgKind(fgKind) || fg->IsName(SvFgName::NAME_LUCKY_CLOVERS_00C9) || fgKind == SvFgName::FG_KIND_WEED ||
            fgKind == SvFgName::FG_KIND_WILTED_FLOWER || fg->IsName(SvFgName::NAME_CLOVERS_WEED)) {
            type = PLACE_TYPE_DIG_HOLE;
            after = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_HOLE));
        } else {
            const u8 kind = fg->GetFgKind();
            if (kind == SvFgName::FG_KIND_STUMP_OR_CUT_TREE || SvFgName::IsSaplingFgKind(kind) || kind == SvFgName::FG_KIND_BUSH ||
                kind == SvFgName::FG_KIND_WILTED_PLANT) {
                type = PLACE_TYPE_DIG_STUMP;
                after = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_HOLE));
            } else {
                type = PLACE_TYPE_NONE;
            }
        }
    }
    SvPlayer* player = g_UnkAAFF18.pPlayer;
    switch (type) {
    case PLACE_TYPE_NONE:
        type = PLACE_TYPE_HIT_ITEM;
        break;
    case PLACE_TYPE_DIG_ITEM: {
        // the item goes into the pockets: with full pockets the request waits for a confirmation
        if (!fg->IsPattern() && player->inventory.FindEmptySlot() < 0) {
            type = PLACE_TYPE_UNKNOWN_15;
            break;
        }
        SvFgName item(*fg);
        if (player != NULL && player->inventory.Func72503C(item)) {
            type = PLACE_TYPE_UNKNOWN_19;
        }
        break;
    }
    case PLACE_TYPE_DIG_HOLE: {
        SvFgName item(*fg);
        if (player != NULL && player->inventory.Func72503C(item)) {
            type = PLACE_TYPE_UNKNOWN_1A;
        }
        break;
    }
    case PLACE_TYPE_UNKNOWN_16:
        if (!Func5C3750()) {
            type = PLACE_TYPE_UNKNOWN_18;
        } else if (Func71E954(player->letters) < 0) {
            type = PLACE_TYPE_UNKNOWN_17;
        }
        break;
    default:
        break;
    }
    return RequestPlace(type, *fg, after, SvFgName::s_Empty, x, z, 0, 0, TREE_SHAKE_TYPE_NONE, false, 0);
}

// 0x00599DE4 (name is ours)
void SwayFgAt(s32 x, s32 z)
{
    if (!stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent())) {
        return;
    }
    const SvFgName* fg = FdInfo::GetCurrentField()->GetFg(x, z, 0);
    if (fg == NULL || fg->GetFgKind() == SvFgName::FG_KIND_INVALID) {
        return;
    }
    SwayFieldObject(x, z);
}

// 0x00599E44 (name is ours)
s32 RequestPickUpIndoor(s32 x, s32 z)
{
    SvFgName item;
    PlaceType type = PLACE_TYPE_NONE;
    u32 layer;
    // the item on the floor first, then the one on something
    for (layer = 0; layer < 2; layer++) {
        const SvFgName* fg = FdInfo::GetCurrentField()->GetFg(x, z, layer);
        if (fg == NULL || fg->IsEmpty()) {
            continue;
        }
        type = PLACE_TYPE_NONE;
        if (fg->IsUsual() && !Func30B658(fg)) {
            type = PLACE_TYPE_PICK_UP;
        }
        if (Func769190(fg)) {
            type = PLACE_TYPE_PICK_UP;
        }
        if (type == PLACE_TYPE_NONE) {
            continue;
        }
        item = *fg;
        break;
    }
    if (type == PLACE_TYPE_NONE) {
        return -1;
    }
    type = GetPickUpType(item, type);
    return RequestPlace(type, item, SvFgName::s_Empty, SvFgName::s_Empty, x, z, layer, 0, TREE_SHAKE_TYPE_NONE, false, 0);
}

// 0x0059A02C (name is ours)
void SwayFgAtPosition(const nn::math::VEC3* position)
{
    s32 x;
    s32 z;
    FdInfo::ToUnit(x, z, *position);
    SwayFieldObject(x, z);
    Field* field = FdInfo::GetCurrentField();
    if (field == NULL) {
        return;
    }
    const SvFgName* fg = field->GetFg(x, z, 0);
    if (fg == NULL) {
        return;
    }
    const u8 fgKind = fg->GetFgKind();
    if (fgKind != SvFgName::FG_KIND_WILTED_FLOWER && !SvFgName::IsFlowerFgKind(fgKind)) {
        return;
    }
    Func1703C0(fg, x, z, false);
}

// 0x0059A09C (name is ours)
s32 RequestPickUpOutdoor(s32 x, s32 z)
{
    const SvFgName* fg = FdInfo::GetCurrentField()->GetFg(x, z, 0);
    if (fg == NULL || fg->IsEmpty()) {
        return -1;
    }
    fg->GetFgKind();
    PlaceType type;
    if (fg->IsName(SvFgName::NAME_LUCKY_CLOVERS_00C9) || fg->GetKind() == item::KIND_MUSH_FTR) {
        type = PLACE_TYPE_PICK_UP_SPECIAL;
    } else if (fg->GetKind() == item::KIND_LOST_QUEST) {
        type = Func5C3750() ? PLACE_TYPE_PICK_UP_SPECIAL : PLACE_TYPE_PICK_UP_SPECIAL_RELEASE;
    } else {
        const u8 fgKind = fg->GetFgKind();
        if (fgKind == SvFgName::FG_KIND_WEED || fgKind == SvFgName::FG_KIND_WILTED_FLOWER || fg->IsName(SvFgName::NAME_CLOVERS_WEED)) {
            type = PLACE_TYPE_PULL;
        } else if ((fg->IsUsual() && !fg->IsBuried()) || (Func769190(fg) && !fg->IsBuried()) || fg->IsFlower()) {
            type = PLACE_TYPE_PICK_UP;
        } else {
            return -1;
        }
    }
    type = GetPickUpType(*fg, type);
    return RequestPlace(type, *fg, SvFgName::s_Empty, SvFgName::s_Empty, x, z, 0, 0, TREE_SHAKE_TYPE_NONE, false, 0);
}

// 0x0059A26C (name is ours)
void ResetPlaceRequests()
{
    for (s32 i = 0; i < static_cast<s32>(PLACE_REQUEST_NUM); i++) {
        PlaceRequest& request = s_PlaceRequests[i];
        request.type = PLACE_TYPE_NONE;
        request.state = PlaceRequest::STATE_NONE;
        request.index = i;
    }
    s_CoinStone.isStarted = false;
    s_CoinStone.timer = -1;
}

namespace {
// the height above the falling position up to which a unit can take the item
const f32 DROP_HEIGHT_LIMIT = 8.0f;
} // namespace

// 0x0059A2B4 (name is ours)
bool DropItem(const nn::math::VEC3* position, const SvFgName* item)
{
    s32 x;
    s32 z;
    FdInfo::ToUnit(x, z, *position);
    if (FdInfo::IsPart95E93ENonZero(*position, FIELD_NAME_CURRENT)) {
        PlacePresent(*item, x, z, position, 4);
        return false;
    }
    const stage::Name stage = stage::GetCurrent();
    const f32 limit = FdInfo::GetHeight(*position, FIELD_NAME_CURRENT) + DROP_HEIGHT_LIMIT;
    for (u32 i = 0; i < PLACE_UNIT_NUM; i++) {
        const s32 unitX = s_DropUnitOffsets[0][i] + x;
        const s32 unitZ = s_DropUnitOffsets[1][i] + z;
        if (!Func165460(unitX, unitZ, stage, 0, false) || GetUnitLockHandle(unitX, unitZ, stage) != -1) {
            continue;
        }
        if (i != 0 && FdInfo::GetUnitHeight(unitX, unitZ, FIELD_NAME_CURRENT) > limit) {
            continue;
        }
        LockUnit(PLACE_TYPE_FALL_ITEM, unitX, unitZ, stage::GetCurrent(), true);
        Packet packet;
        packet.SetFallItem(item, unitX, unitZ, stage::GetCurrent());
        packet.Place();
        PlacePresent(*item, unitX, unitZ, position, 0);
        return true;
    }
    PlacePresent(*item, x, z, position, 2);
    return false;
}

// 0x0059A4A0 (name is ours)
s32 RequestThrowSwap(const SvFgName* item)
{
    FindPendingRequest();
    s32 x;
    s32 z;
    u8 layer;
    if (!FindPlaceableUnitAroundPlayer(x, z, layer, FIND_UNIT_FOR_PLAYER_CHECK_PUT, false)) {
        return -1;
    }
    SvFgName placed(*item);
    if (stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent()) && item->IsName(SvFgName::NAME_FOUNTAIN_FIREWORK)) {
        placed = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_USED_FIREWORK));
    }
    return RequestPlace(PLACE_TYPE_THROW_SWAP_AFTER_PICK_UP, SvFgName::s_Empty, placed, *item, x, z, layer, 0, TREE_SHAKE_TYPE_NONE,
                        false, 0);
}

// 0x0059A58C (name is ours)
bool FindHoleAroundPlayer(s32* x, s32* z)
{
    if (Func71B394(&g_UnkAAFF18.pPlayer->playerAppearance.playerOutfit)->GetKind() != item::KIND_SHOVEL) {
        return false;
    }
    AcPlayer* player = demo::PlayerGetActorEntry(netgame::PLAYER_NO_MY_NO, false);
    if (player == NULL) {
        return false;
    }
    long playerX;
    long playerZ;
    if (!PlayerGetPosition(playerX, playerZ, netgame::PLAYER_NO_MY_NO, true)) {
        return false;
    }
    Actor* actor = static_cast<Actor*>(player);
    u8 direction = 0;
    for (s32 i = 0; i < 8; i++) {
        s16 difference = actor->m_Rotation.y.value - s_PlayerDirectionAngles[i];
        if (difference < 0) {
            difference = -difference;
        }
        if (difference < DIRECTION_TOLERANCE) {
            direction = i;
            break;
        }
    }
    const s32* order = s_PlaceUnitOrders[direction];
    Field* field = FdInfo::GetCurrentField();
    if (field == NULL) {
        return false;
    }
    // the unit in front and the two next to it
    for (s32 i = 1; i < 4; i++) {
        const s32 unitX = s_PlaceUnitOffsets[0][order[i]] + playerX;
        const s32 unitZ = s_PlaceUnitOffsets[1][order[i]] + playerZ;
        const SvFgName* fg = field->GetFg(unitX, unitZ, 0);
        if (fg == NULL || !fg->IsName(SvFgName::NAME_HOLE) || GetUnitLockHandle(unitX, unitZ, stage::NAME_CURRENT) != -1) {
            continue;
        }
        nn::math::VEC3 position;
        FdInfo::ToCenterWorldWithUnitHeight(&position, playerX, playerZ);
        if (!stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent()) || Func614B90(unitX, unitZ, &position)) {
            *x = unitX;
            *z = unitZ;
            return true;
        }
    }
    return false;
}

// 0x0059A754 (name is ours)
bool CanPutItemAroundPlayer()
{
    s32 x;
    s32 z;
    u8 layer;
    return FindPlaceableUnitAroundPlayer(x, z, layer, FIND_UNIT_FOR_PLAYER_CHECK_PUT, false);
}

// 0x0059A888 (name is ours)
void StartBuryActionEntry(const void* unused, s32 x, s32 z)
{
    StartBuryAction(x, z);
}

// 0x0059A894 (name is ours)
void StartBuryAction(s32 x, s32 z)
{
    s32 index = FindPlaceAction(x, z, stage::GetCurrent());
    if (index == -1) {
        return;
    }
    PlaceAction& action = s_PlaceActions[index];
    bool isStarted;
    if (action.placeType == PLACE_TYPE_BURY) {
        isStarted = action.StartBury(true, ObjectBury::ANIMATION_PRESS);
    } else if (action.placeType == PLACE_TYPE_FILL) {
        isStarted = action.StartBury(false, ObjectBury::ANIMATION_PRESS);
    } else {
        return;
    }
    if (isStarted) {
        return;
    }
    action.PlaceResult();
    action.SendUnlockRequest();
    action.placeType = PLACE_TYPE_NONE;
    action.state = PlaceAction::STATE_NONE;
    RefreshUnit(action.pos.x, action.pos.y, false);
}

// 0x0059B598 (name is ours)
bool PlaceOther(const SvFgName& item, const nn::math::VEC3* position, const nn::math::VEC3* scale, s32 id, f32 shadowSize,
                bool faceCameraIndoors, bool hasShadow, s16 rotateY, bool withFlag40000)
{
    if (s_pProc == NULL) {
        return false;
    }
    for (ObjectExe* object = static_cast<ObjectExe*>(s_pProc->m_ExeObjects.GetFirst()); object != NULL;
         object = static_cast<ObjectExe*>(object->m_pNext)) {
        if (object->Refresh(&item, position, scale, id)) {
            return true;
        }
    }
    ObjectOther* object = ObjectOther::Create(item, position, scale, id, shadowSize, faceCameraIndoors, hasShadow, rotateY, withFlag40000);
    if (object == NULL) {
        return false;
    }
    s_pProc->m_ExeObjects.InsertByPriority(object);
    return true;
}

// 0x00594E64 (name is ours)
ObjectBase* TakeObjectOfArray(ObjectBase** objects, s32 id)
{
    for (u32 i = 0; i < OBJECT_ARRAY_NUM; i++) {
        if (objects[i] != NULL && objects[i]->m_Unk0x114 == id) {
            ObjectBase* object = objects[i];
            objects[i] = NULL;
            return object;
        }
    }
    return NULL;
}

// 0x00594EA8 (name is ours)
bool AddObjectToArray(ObjectBase** objects, ObjectBase* object)
{
    for (u32 i = 0; i < OBJECT_ARRAY_NUM; i++) {
        if (objects[i] == NULL) {
            objects[i] = object;
            return true;
        }
    }
    return false;
}

// 0x0059C0D8 (name is ours)
void ResetFieldBit(u8 x, u8 z)
{
    if (s_pProc == NULL) {
        return;
    }
    s_pProc->m_FieldBit.Reset(x, z);
}

// 0x0059C104 (name is ours)
FgModel* GetFgModelOf(const SvFgName* item)
{
    Proc* proc = s_pProc;
    if (!s_IsOutdoor || proc == NULL) {
        return NULL;
    }
    if (!item->IsFg()) {
        return NULL;
    }
    const u8 model = item::GetParamFgEntry(*item)->fgModel;
    if (model >= Proc::FG_MODEL_NUM || model == 0) {
        return NULL;
    }
    FgModel* models = proc->GetFgModels();
    if (models == NULL) {
        return NULL;
    }
    return &models[model];
}

// 0x0059967C (name is ours)
bool LockPitfall(s32 x, s32 z)
{
    if (netgame::IsMultiplePlayer()) {
        return false;
    }
    SvFgName pitfall;
    if (stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent())) {
        const SvFgName* fg = FdInfo::GetCurrentField()->GetFg(x, z, 0);
        if (fg != NULL && GetUnitLockHandle(x, z, stage::NAME_CURRENT) == -1 && fg->IsBuried() && fg->GetKind() == item::KIND_PITFALL_SEED) {
            pitfall = *fg;
            const stage::Name stage = stage::GetCurrent();
            if (GetUnitLockHandle(x, z, stage) == -1 && LockUnit(PLACE_TYPE_UNKNOWN_24, x, z, stage, true) != -1) {
                return true;
            }
        }
    }
    return false;
}

// 0x0059978C (name is ours)
bool SwingTreeAt(s32 x, s32 z)
{
    const SvFgName* fg = FdInfo::GetCurrentField()->GetFg(x, z, 0);
    if (fg == NULL) {
        return false;
    }
    if (fg->IsFg()) {
        const item::ParamFgEntry* param = item::GetParamFgEntry(*fg);
        if (fg->IsGrownTree() && param->size >= 2) {
            return SwingTreeObject(fg, x, z, ObjectSwing::KIND_SHAKE_L_WAIT);
        }
    }
    return false;
}

// 0x00599808 (name is ours)
bool SwingFgAt(s32 x, s32 z)
{
    const stage::Name stage = stage::GetCurrent();
    const SvFgName* fg = FdInfo::GetCurrentField()->GetFg(x, z, 0);
    if (fg == NULL) {
        return false;
    }
    if (GetUnitLockHandle(x, z, stage) != -1) {
        return false;
    }
    if (fg->IsFg()) {
        const item::ParamFgEntry* param = item::GetParamFgEntry(*fg);
        if (fg->IsGrownTree() && param->size >= 1) {
            return SwingTreeObject(fg, x, z, ObjectSwing::KIND_SHAKE_S);
        }
    }
    if (fg->IsBush() && item::GetParamFgEntry(*fg)->size >= 1) {
        return SwingBushObject(fg, x, z);
    }
    if (fg->GetFgKind() != SvFgName::FG_KIND_RAFFLESIA) {
        return false;
    }
    SwayFieldObject(x, z);
    return true;
}

// 0x00596D80 (name is ours)
void PlaceBuryResult(s32 x, s32 z)
{
    s32 index = FindPlaceAction(x, z, stage::GetCurrent());
    if (index == -1) {
        return;
    }
    PlaceAction& action = s_PlaceActions[index];
    if (action.placeType != PLACE_TYPE_BURY) {
        return;
    }
    if (action.fg1.IsBuried()) {
        PlaceScale(action.fg1, x, z, false);
        return;
    }
    PlacePlant(action.fg1, x, z, true);
    action.OnPlanted(&action.fg1);
}

// 0x00596E1C (name is ours)
void CancelPendingRequest()
{
    s32 index = FindPendingRequest();
    if (index == -1) {
        return;
    }
    PlaceRequest& request = s_PlaceRequests[index];
    Packet packet;
    packet.Assign(request, Packet::UNLOCK_UNIT_ALL);
    packet.SendToHost();
    request.Clear(index);
}

// 0x00596ED8 (name is ours)
void FillHole(s32 x, s32 z, stage::Name stage)
{
    Field* field = GetFieldOfStage(stage);
    if (field == NULL) {
        return;
    }
    const SvFgName* fg = field->GetFg(x, z, 0);
    if (fg == NULL) {
        return;
    }
    if (!fg->IsName(SvFgName::NAME_HOLE)) {
        return;
    }
    Func168E20(x, z, 0, stage, SvFgName::s_Empty);
    if (stage != stage::NAME_CURRENT && stage::GetCurrent() != stage) {
        return;
    }
    RefreshUnit(x, z, false);
    SvFgName hole(static_cast<SvFgName::ID>(SvFgName::NAME_HOLE));
    PlaceBury(hole, x, z, ObjectBury::FINISH_MODE_KEEP, ObjectBury::ANIMATION_SHRINK);
}

// 0x00596FD4 (name is ours)
s32 FinishPendingRequest()
{
    s32 index = FindPendingRequest();
    if (index == -1) {
        return index;
    }
    PlaceRequest& request = s_PlaceRequests[index];
    if (request.type == PLACE_TYPE_UNKNOWN_1A) {
        // the item of the first place of the pockets is taken out
        SvPlayerInventory* inventory = &SvGetCurrentPlayer()->inventory;
        SvFgName item(*inventory->GetItem(0, NULL));
        Func2BFF98(inventory, 0, &SvFgName::s_Empty, 0, 1);
        ConfirmPendingRequest(&item, index, true);
    } else {
        ConfirmPendingRequest(&request.replace, index, true);
    }
    request.Clear(index);
    return index;
}

// 0x00597154 (name is ours)
s32 RequestUnitAction(s32 x, s32 z, UnitAction action)
{
    if (!stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent())) {
        return -1;
    }
    const SvFgName* fg = FdInfo::GetCurrentField()->GetFg(x, z, 0);
    if (fg == NULL) {
        return -1;
    }
    if (GetUnitLockHandle(x, z, stage::NAME_CURRENT) != -1) {
        // the action at the unit is still running (only for the player alone)
        if (netgame::IsMultiplePlayer() || action == UNIT_ACTION_SHOVEL) {
            return -1;
        }
        if (action == UNIT_ACTION_SHAKE) {
            FinishPlaceActionAt(x, z);
        } else if (action == UNIT_ACTION_AXE) {
            s32 index = FindPlaceAction(x, z, stage::GetCurrent());
            if (index == -1 || (s_PlaceActions[index].param.isCutdown)) {
                return -1;
            }
            FinishPlaceActionAt(x, z);
        }
    }
    switch (action) {
    case UNIT_ACTION_SHAKE: {
        if (!fg->IsFg()) {
            return -1;
        }
        SvFgName item(*fg);
        if (!fg->IsFg()) {
            return -1;
        }
        const item::ParamFgEntry* param = item::GetParamFgEntry(*fg);
        if (!fg->IsGrownTree() || param->size < 2) {
            return -1;
        }
        PlaceType type = PLACE_TYPE_SHAKE_TREE;
        TreeShakeType shake = GetShake(item, *fg);
        if (shake == TREE_SHAKE_TYPE_BEES) {
            type = PLACE_TYPE_SHAKE_TREE_BEES;
        }
        AcPlayer* player = demo::PlayerGetActorEntry(netgame::PLAYER_NO_MY_NO, false);
        bool fallDirection = player == NULL ? false : static_cast<s16>(static_cast<Actor*>(player)->m_Rotation.y.value) > 0;
        return RequestPlace(type, *fg, item, SvFgName::s_Empty, x, z, 0, fallDirection, shake, false, 0);
    }
    case UNIT_ACTION_AXE:
        return RequestCut(fg, x, z);
    case UNIT_ACTION_SHOVEL:
        return RequestDig(fg, x, z);
    default:
        return -1;
    }
}

// 0x00597440 (name is ours)
s32 RequestPickUp(s32 x, s32 z)
{
    if (stage::IsKind(stage::KIND_INDOOR, stage::GetCurrent())) {
        return RequestPickUpIndoor(x, z);
    }
    if (!stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent())) {
        return -1;
    }
    return RequestPickUpOutdoor(x, z);
}

// 0x005974A8 (name is ours)
s32 RequestBury(const SvFgName& item, s32 x, s32 z)
{
    SvFgName buried(item);
    SvFgName sprite(item);
    bool isFlower = ConvertForBury(&buried, &sprite);
    SvFgName hole(static_cast<SvFgName::ID>(SvFgName::NAME_HOLE));
    return RequestPlace(PLACE_TYPE_BURY, hole, buried, sprite, x, z, 0, isFlower, TREE_SHAKE_TYPE_NONE, false, 0);
}

// 0x00597548 (name is ours)
s32 FinishPendingRequest(const SvFgName* item)
{
    s32 index = FindPendingRequest();
    if (index != -1) {
        ConfirmPendingRequest(item, index, false);
        s_PlaceRequests[index].Clear(index);
    }
    return index;
}

// 0x00597598 (name is ours)
bool ConvertLuckyClover(SvFgName* item)
{
    if (!item->IsName(SvFgName::NAME_LUCKY_CLOVERS_00C9)) {
        return false;
    }
    *item = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_LUCKY_CLOVERS_2959));
    return true;
}

// 0x005975DC (name is ours)
void RemoveFgOfUnit(s32 x, s32 z)
{
    s32 handle = GetUnitLockHandle(x, z, stage::GetCurrent());
    if (handle != -1) {
        UnitLock& lock = s_UnitLocks[handle];
        if (lock.type == PLACE_TYPE_UNKNOWN_23 || lock.type == PLACE_TYPE_UNKNOWN_24) {
            lock.stage = stage::NAME_NONE;
        }
    }
    const stage::Name stage = stage::GetCurrent();
    const SvFgName* fg = GetFieldOfStage(stage)->GetFg(x, z, 0);
    Func1758B0(fg, x, z, false);
    Func168E20(x, z, 0, stage, SvFgName::s_Empty);
    RefreshUnit(x, z, false);
}

// 0x005976B4 (name is ours)
s32 RequestPlant(const SvFgName* item)
{
    s32 x;
    s32 z;
    u8 layer;
    if (!FindPlaceableUnitAroundPlayer(x, z, layer, FIND_UNIT_FOR_PLAYER_CHECK_PLANT, false)) {
        return -1;
    }
    SvFgName planted;
    Func768A60(item, &planted);
    return RequestPlace(PLACE_TYPE_PLANT, SvFgName::s_Empty, planted, *item, x, z, 0, 0, TREE_SHAKE_TYPE_NONE, false, 0);
}

// 0x00597744 (name is ours)
s32 RequestThrowEntry(const void* unused, const SvFgName* item)
{
    return RequestThrow(item);
}

// 0x0059774C (name is ours)
s32 RequestThrow(const SvFgName* item)
{
    // a fountain firework is put in front of the player outdoors
    bool placeInFront = false;
    if (stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent()) && item->IsName(SvFgName::NAME_FOUNTAIN_FIREWORK)) {
        placeInFront = true;
    }
    s32 x;
    s32 z;
    u8 layer;
    if (!FindPlaceableUnitAroundPlayer(x, z, layer, FIND_UNIT_FOR_PLAYER_CHECK_PUT, placeInFront)) {
        return -1;
    }
    SvFgName placed(*item);
    if (stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent()) && item->IsName(SvFgName::NAME_FOUNTAIN_FIREWORK)) {
        placed = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_USED_FIREWORK));
    }
    return RequestPlace(PLACE_TYPE_THROW, SvFgName::s_Empty, placed, *item, x, z, layer, 0, TREE_SHAKE_TYPE_NONE, false, 0);
}

// 0x00597878 (name is ours)
void SwingTree(s32 x, s32 z)
{
    const stage::Name stage = stage::GetCurrent();
    const SvFgName* fg = GetFieldOfStage(stage)->GetFg(x, z, 0);
    if (!fg->IsFg()) {
        return;
    }
    const item::ParamFgEntry* param = item::GetParamFgEntry(*fg);
    if (!fg->IsGrownTree() || param->size < 2) {
        return;
    }
    SvFgName tree(*fg);
    PlaceParam placeParam = {};
    AddPlaceAction(PLACE_TYPE_UNKNOWN_23, netgame::PLAYER_NO_HOST, tree, tree, tree, x, z, stage, placeParam, NULL);
    PlaceSwing(tree, SvFgName::s_Empty, x, z, NULL, ObjectSwing::KIND_SHAKE_L, false);
}

// 0x0059797C (name is ours)
bool GetStumpOfTree(SvFgName* item)
{
    const item::ParamFgEntry* param = item::GetParamFgEntry(*item);
    SvFgName stump(static_cast<SvFgName::ID>(static_cast<u32>(param->unk_0x0) < item::FG_NUM ? param->unk_0x0 : 0));
    SvPlayer* player = g_UnkAAFF18.pPlayer;
    if (player == NULL || !Func71B394(&player->playerAppearance.playerOutfit)->IsName(SvFgName::NAME_SILVER_AXE)) {
        if (GetRandom(100) >= SPECIAL_STUMP_CHANCE) {
            *item = stump;
            return true;
        }
    }
    // a special stump: the six forms of the tree or cedar stump
    u32 random = GetRandom(100);
    if (stump.IsName(SvFgName::NAME_TREE_STUMP)) {
        if (random < 20) {
            stump = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_TREE_STUMP_HEART));
        } else if (random < 40) {
            stump = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_TREE_STUMP_BUTTERFLY));
        } else if (random < 60) {
            stump = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_TREE_STUMP_CITRUS));
        } else if (random < 80) {
            stump = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_TREE_STUMP_REDDS_LEAF));
        } else if (random < 90) {
            stump = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_TREE_STUMP_CROSS));
        } else {
            stump = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_TREE_STUMP_TRIFORCE));
        }
    } else if (stump.IsName(SvFgName::NAME_CEDAR_STUMP)) {
        if (random < 20) {
            stump = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_CEDAR_STUMP_STAR));
        } else if (random < 40) {
            stump = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_CEDAR_STUMP_CAT));
        } else if (random < 60) {
            stump = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_CEDAR_STUMP_FLOWER));
        } else if (random < 80) {
            stump = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_CEDAR_STUMP_LEAF));
        } else if (random < 90) {
            stump = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_CEDAR_STUMP_MUSICAL_NOTE));
        } else {
            stump = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_CEDAR_STUMP_CLOVER));
        }
    }
    *item = stump;
    return true;
}

// 0x005980B0 (name is ours)
void PlaceHoleEntry(const void* unused, s32 x, s32 z)
{
    PlaceHole(x, z);
}

// 0x005980BC (name is ours)
void PlaceHole(s32 x, s32 z)
{
    Field* field = FdInfo::GetCurrentField();
    if (field == NULL) {
        return;
    }
    const SvFgName* fg = field->GetFg(x, z, 0);
    if (fg == NULL || fg->IsName(SvFgName::NAME_HOLE)) {
        return;
    }
    RefreshUnit(x, z, true);
    SvFgName hole(static_cast<SvFgName::ID>(SvFgName::NAME_HOLE));
    Func168E20(x, z, 0, stage::GetCurrent(), hole);
    PlaceScale(hole, x, z, true);
}

// 0x00598160 (name is ours)
s32 RequestUnknown1E(s32 x, s32 z)
{
    if (!stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent())) {
        return -1;
    }
    const SvFgName* fg = FdInfo::GetCurrentField()->GetFg(x, z, 0);
    if (fg == NULL) {
        return -1;
    }
    if (GetUnitLockHandle(x, z, stage::NAME_CURRENT) != -1) {
        return -1;
    }
    if (fg->IsName(SvFgName::NAME_HOLE)) {
        return RequestPlace(PLACE_TYPE_UNKNOWN_1F, *fg, SvFgName::s_Empty, SvFgName::s_Empty, x, z, 0, 0, TREE_SHAKE_TYPE_NONE, false, 0);
    }
    if (fg->IsFg() && (item::GetParamFgEntry(*fg)->unk_0xF & 8) != 0) {
        return -1;
    }
    return RequestPlace(PLACE_TYPE_UNKNOWN_1E, *fg, SvFgName::s_Empty, SvFgName::s_Empty, x, z, 0, 0, TREE_SHAKE_TYPE_NONE, false, 0);
}

// 0x0059826C (name is ours)
void StartPlaceActionAtEntry(const void* unused, s32 x, s32 z)
{
    StartPlaceActionAt(x, z);
}

// 0x00598278 (name is ours)
void StartPlaceActionAt(s32 x, s32 z)
{
    s32 index = FindPlaceAction(x, z, stage::GetCurrent());
    if (index == -1) {
        return;
    }
    s_PlaceActions[index].Start();
}

// 0x005982C0 (name is ours)
void PlaceItemOnCurrentStage(s32 x, s32 z, u8 layer, const SvFgName& item)
{
    Func168E20(x, z, layer, stage::GetCurrent(), item);
}

// 0x005982F4 (name is ours)
bool GetPlaceableLayer(s32 x, s32 z, u8* layer)
{
    if (Func165460(x, z, stage::NAME_CURRENT, 0, false)) {
        *layer = 0;
        return true;
    }
    if (Func4E3D48(x, z)) {
        *layer = 1;
        return true;
    }
    return false;
}

// 0x005985D8 (name is ours)
void PlantDirect(const SvFgName* item, s32 x, s32 z)
{
    const stage::Name stage = stage::GetCurrent();
    SvFgName replace(*GetFieldOfStage(stage)->GetFg(x, z, 0));
    SvFgName planted;
    Func768A60(item, &planted);
    PlaceParam param = {};
    AddPlaceAction(PLACE_TYPE_UNKNOWN_23, netgame::PLAYER_NO_HOST, replace, planted, replace, x, z, stage, param, NULL);
    PlacePlant(planted, x, z, true);
}

// 0x005986B0 (name is ours)
bool LockUnitOfType23(u8 x, u8 z)
{
    if (netgame::IsMultiplePlayer()) {
        return false;
    }
    const stage::Name stage = stage::GetCurrent();
    if (GetUnitLockHandle(x, z, stage) != -1) {
        return false;
    }
    if (LockUnit(PLACE_TYPE_UNKNOWN_23, x, z, stage, true) == -1) {
        return false;
    }
    return true;
}

// 0x00598718 (name is ours)
s32 RequestPitfall(s32 x, s32 z)
{
    SvFgName pitfall;
    if (stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent())) {
        const SvFgName* fg = FdInfo::GetCurrentField()->GetFg(x, z, 0);
        if (fg != NULL && GetUnitLockHandle(x, z, stage::NAME_CURRENT) == -1 && fg->IsBuried() && fg->GetKind() == item::KIND_PITFALL_SEED) {
            pitfall = *fg;
            return RequestPlace(PLACE_TYPE_PITFALL, pitfall, SvFgName::s_Empty, SvFgName::s_Empty, x, z, 0, 0, TREE_SHAKE_TYPE_NONE, false, 0);
        }
    }
    return -1;
}

// 0x00598810 (name after libgarden)
TreeShakeType GetShake(SvFgName& out, const SvFgName& shaken)
{
    // 0x009536AC (guard 0x009536A8)
    static const SvFgName s_Tree(static_cast<SvFgName::ID>(SvFgName::NAME_TREE));
    // 0x009536B0 (guard 0x009536A4)
    static const SvFgName s_Cedar(static_cast<SvFgName::ID>(SvFgName::NAME_CEDAR));
    if (shaken.IsName(SvFgName::NAME_TREE_BEES)) {
        out = s_Tree;
        return GetBeesShake();
    }
    if (shaken.IsName(SvFgName::NAME_CEDAR_BEES)) {
        out = s_Cedar;
        return GetBeesShake();
    }
    if (shaken.IsFruitRipe()) {
        out = shaken;
        out.ResetFruitAfterHarvest();
        // a perfect tree may die when its fruit is shaken (the more likely the more it decayed)
        s32 decay = shaken.GetPerfectFruitDecay();
        if (decay == -1 || decay <= 3) {
            return TREE_SHAKE_TYPE_FRUIT;
        }
        if (decay - 3 <= static_cast<s32>(GetRandom(4))) {
            return TREE_SHAKE_TYPE_FRUIT;
        }
        out = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_DEAD_PERFECT_TREE));
        return TREE_SHAKE_TYPE_FRUIT_DIE;
    }
    if (shaken.IsName(SvFgName::NAME_TREE_ITEM) || shaken.IsName(SvFgName::NAME_TREE_MONEY_1) || shaken.IsName(SvFgName::NAME_TREE_EGG_0075)) {
        out = s_Tree;
        return TREE_SHAKE_TYPE_ITEM;
    }
    if (shaken.IsName(SvFgName::NAME_TREE_MONEY_2)) {
        if (IsTownState(1)) {
            return TREE_SHAKE_TYPE_NONE;
        }
        out = s_Tree;
        return TREE_SHAKE_TYPE_ITEM;
    }
    if (shaken.IsName(SvFgName::NAME_CEDAR_ITEM) || shaken.IsName(SvFgName::NAME_CEDAR_MONEY_1) || shaken.IsName(SvFgName::NAME_CEDAR_EGG)) {
        out = s_Cedar;
        return TREE_SHAKE_TYPE_ITEM;
    }
    if (shaken.IsName(SvFgName::NAME_CEDAR_MONEY_2)) {
        if (IsTownState(1)) {
            return TREE_SHAKE_TYPE_NONE;
        }
        out = s_Cedar;
        return TREE_SHAKE_TYPE_ITEM;
    }
    return TREE_SHAKE_TYPE_NONE;
}

// 0x00598BA8 (name is ours)
bool IsLargeFgAt(s32 x, s32 z)
{
    stage::GetCurrent();
    const SvFgName* fg = FdInfo::GetCurrentField()->GetFg(x, z, 0);
    if (fg == NULL) {
        return false;
    }
    if (fg->IsFg()) {
        const item::ParamFgEntry* param = item::GetParamFgEntry(*fg);
        if (fg->IsGrownTree() && param->size >= 1) {
            return true;
        }
    }
    if (fg->IsBush() && item::GetParamFgEntry(*fg)->size >= 1) {
        return true;
    }
    return fg->GetFgKind() == SvFgName::FG_KIND_RAFFLESIA;
}

// 0x00598C4C (name is ours)
bool FindRafflesiaPosition(nn::math::VEC3* out)
{
    Field* field = FdInfo::GetCurrentField();
    if (field == NULL) {
        return false;
    }
    const s32 width = field->GetUnitWidth();
    const s32 height = field->GetUnitHeight();
    for (u32 z = 0; z < static_cast<u32>(height); z++) {
        for (u32 x = 0; x < static_cast<u32>(width); x++) {
            const SvFgName* fg = field->GetFg(x, z, 0);
            if (fg != NULL && fg->IsName(SvFgName::NAME_RAFFLESIA)) {
                GetObjectPosition(out, *fg, x, z, false);
                return true;
            }
        }
    }
    return false;
}

// 0x00598D00 (name is ours)
s32 RequestUnknown07(s32 x, s32 z)
{
    if (!stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent())) {
        return -1;
    }
    const SvFgName* fg = FdInfo::GetCurrentField()->GetFg(x, z, 0);
    if (fg == NULL || fg->IsEmpty()) {
        return -1;
    }
    if (!fg->IsName(SvFgName::NAME_HOLE) && !fg->IsName(SvFgName::NAME_PATTERN)) {
        return -1;
    }
    return RequestPlace(PLACE_TYPE_UNKNOWN_07, *fg, SvFgName::s_Empty, SvFgName::s_Empty, x, z, 0, 0, TREE_SHAKE_TYPE_NONE, false, 0);
}

// 0x00598DC0 (name is ours)
s32 RequestDisplayPattern(u32 design, u32 saveNo)
{
    if (saveNo == netgame::PLAYER_NO_MY_NO) {
        saveNo = SvGetPlayerSaveNo(netgame::PLAYER_NO_MY_NO);
    }
    s32 x;
    s32 z;
    u8 layer;
    if (!FindPlaceableUnitAroundPlayer(x, z, layer, FIND_UNIT_FOR_PLAYER_CHECK_DISPLAY, false)) {
        return -1;
    }
    SvFgName pattern(static_cast<SvFgName::ID>(SvFgName::NAME_PATTERN));
    if (!Func304EA0(&pattern, 0, saveNo, design)) {
        return -1;
    }
    return RequestPlace(PLACE_TYPE_DISPLAY, SvFgName::s_Empty, pattern, SvFgName::s_Empty, x, z, 0, 0, TREE_SHAKE_TYPE_NONE, false, 0);
}

// 0x00598E94 (name is ours)
s32 RequestUnknown1D(s32 x, s32 z)
{
    if (!stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent())) {
        return -1;
    }
    const SvFgName* fg = FdInfo::GetCurrentField()->GetFg(x, z, 0);
    if (fg == NULL) {
        return -1;
    }
    if (GetUnitLockHandle(x, z, stage::NAME_CURRENT) != -1) {
        return -1;
    }
    if (!fg->Func769DE0(true)) {
        return -1;
    }
    return RequestPlace(PLACE_TYPE_UNKNOWN_1D, *fg, *fg, SvFgName::s_Empty, x, z, 0, 0, TREE_SHAKE_TYPE_NONE, false, 0);
}
} // namespace fgobj
