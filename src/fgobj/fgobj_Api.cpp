#include "fgobj/fgobj_Api.h"

#include "fgobj/dPacket.h"
#include "fgobj/dPlace.h"
#include "fgobj/dProc.h"
#include "Ac/dAcPlayer.h"
#include "Audio/dAudioObj1.h"
#include "field/dField.h"
#include "stage/stage_Extern.h"
#include "Bs/dBsSvMgr.h"
#include "Sound/dSoundLabel.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvPlayer.h"
#include "netgame/dGameGlobals.h"
#include "script/dScriptExtern.h"
#include "demo/demo_Api.h"
#include "demo/demo_Extern.h"
#include "fgobj/dObjectBury.h"
#include "fgobj/dObjectCoinStone.h"
#include "fgobj/dObjectDecoSwing.h"
#include "fgobj/dObjectDig.h"
#include "fgobj/dObjectSwing.h"
#include "fgobj/dObjectTimer.h"
#include "fgobj/dObjectFruit.h"
#include "fgobj/dObjectHanabi.h"
#include "fgobj/dObjectHoneycomb.h"
#include "fgobj/dObjectLeaf.h"
#include "fgobj/dObjectStumpAnim.h"
#include "netgame/dGameFunctions.h"
#include "fgobj/dObjectThrowHole.h"
#include "fgobj/fgobj_Exe.h"
#include "Other/dTourData.h"
#include "Other/dTourResult.h"
#include "field/dFdInfo.h"
#include "field/dFieldAttribute.h"
#include "fgobj/fgobj_Extern.h"
#include "fgobj/fgobj_Globals.h"
#include <cstring>
#include "Item/item_Api.h"
#include "fgobj/dObjectPresent.h"
#include "sead/seadFixedSafeString.h"
#include "sead/seadStringFormatter.h"
#include "nw/gfx/gfx_Api.h"
#include "nn/math/math_Api.h"
#include "g3d/dResourceLoader.h"
#include "ssys/ma/dFileData.h"
#include "Sv/dSvFgName.h"
#include "nw/gfx/gfx_Api.h"
#include "nn/math/math_Api.h"
#include "netgame/netgame_Api.h"
#include "Other/dActor.h"

namespace fgobj {
// (name is ours)
// 0x009536B4
s32 s_LastCutUnitX;
// (name is ours)
// 0x009536B8
s32 s_LastCutUnitZ;

// (name is ours)
// 0x009536C0
CoinStone s_CoinStone;

// (name is ours)
// 0x009B517C
PlaceRequest s_PlaceRequests[PLACE_REQUEST_NUM];
// 0x009B51C4
UnitLock s_UnitLocks[UNIT_LOCK_NUM];
// (names are ours)
// 0x009536F0
ssys::ma::FileData* s_pFgParam;
// 0x009536F4
g3d::ResourceLoader* s_pFgLut;
// (name after libgarden)
// 0x009B5264
PlaceAction s_PlaceActions[PLACE_ACTION_NUM];
// (name is ours)
// 0x009B5524
u32 s_TreeCutCounters[320];

// 0x00595874 | libgarden [tier A]
g3d::ResourceLoader* GetFgLut()
{
    return s_pFgLut;
}

// 0x0059A950 (name is ours)
bool IsGrownTreeOfSize2(const SvFgName* item)
{
    if (item->IsFg()) {
        const item::ParamFgEntry* param = item::GetParamFgEntry(*item);
        if (item->IsGrownTree() && param->size >= 2) {
            return true;
        }
    }
    return false;
}

// 0x00599D7C (name is ours)
bool GetUnitOfDirection(s32* outX, s32* outZ, s32 x, s32 z, u8 direction)
{
    u32 stepX = direction & 7;
    u32 stepZ = (direction & 0x70) >> 4;
    if (direction == 0 || stepX == 0 || stepZ == 0) {
        return false;
    }
    if (stepX >= 8 || stepZ >= 8) {
        return false;
    }
    *outX = x + s_DirectionOffsets[stepX];
    *outZ = z + s_DirectionOffsets[stepZ];
    return true;
}

// 0x005A11BC | libgarden [tier A]
s32 GetUnitLockHandle(u8 x, u8 y, stage::Name stage)
{
    if (stage == stage::NAME_CURRENT) {
        stage = stage::GetCurrent();
    }
    for (s32 i = 0; i < static_cast<s32>(UNIT_LOCK_NUM); i++) {
        const UnitLock& lock = s_UnitLocks[i];
        if (lock.stage != stage::NAME_NONE && lock.pos.x == x && lock.pos.y == y && lock.stage == stage) {
            return i;
        }
    }
    return -1;
}

// 0x005A1228 (name is ours)
void UnlockUnitOfType23Or24(u8 x, u8 y)
{
    s32 handle = GetUnitLockHandle(x, y, stage::GetCurrent());
    if (handle != -1) {
        UnitLock& lock = s_UnitLocks[handle];
        if (lock.type == PLACE_TYPE_UNKNOWN_23 || lock.type == PLACE_TYPE_UNKNOWN_24) {
            lock.stage = stage::NAME_NONE;
        }
    }
}

// 0x005A1278 | libgarden [tier A]
void UnlockUnit(u8 x, u8 y, stage::Name stage, netgame::PlayerNo player)
{
    s32 handle = GetUnitLockHandle(x, y, stage);
    if (handle == -1) {
        return;
    }
    UnitLock& lock = s_UnitLocks[handle];
    if (player == netgame::PLAYER_NO_TARGET_ALL) {
        lock.confirm = 0;
    } else {
        lock.confirm &= ~static_cast<u8>(1 << player);
        if (lock.confirm != 0) {
            return;
        }
    }
    if (netgame::IsMultiplePlayer() && netgame::IsHost()) {
        Packet packet;
        packet.player = netgame::GetMyPlayerNo();
        packet.placeType = PLACE_TYPE_NONE;
        packet.action = Packet::REQUEST_DONE_UNLOCK_UNIT;
        packet.unknown = 0;
        packet.param.layer = 0;
        packet.param.fallDirection = 0;
        packet.param.treeShakeType = TREE_SHAKE_TYPE_NONE;
        packet.param.isCutdown = 0;
        packet.param.cutCounter = 0;
        packet.pos.x = x;
        packet.pos.y = y;
        packet.replace = SvFgName::s_Empty;
        packet.fg1 = SvFgName::s_Empty;
        packet.fg2 = SvFgName::s_Empty;
        packet.stage = stage;
        packet.Send(netgame::PLAYER_NO_TARGET_ALL);
    }
    lock.stage = stage::NAME_NONE;
}

// 0x005A13C4 | libgarden [tier A]
s32 LockUnit(PlaceType type, u8 x, u8 y, stage::Name stage, bool netgameSend)
{
    s32 handle;
    for (handle = 0; handle < static_cast<s32>(UNIT_LOCK_NUM); handle++) {
        if (s_UnitLocks[handle].stage == stage::NAME_NONE) {
            break;
        }
    }
    if (handle == static_cast<s32>(UNIT_LOCK_NUM)) {
        handle = -1;
    } else if (handle != -1) {
        UnitLock& lock = s_UnitLocks[handle];
        lock.pos.x = x;
        lock.pos.y = y;
        lock.stage = stage;
        lock.type = type;
        if (netgameSend) {
            if (netgame::IsMultiplePlayer() && netgame::IsHost()) {
                Packet packet;
                packet.player = netgame::GetMyPlayerNo();
                packet.placeType = PLACE_TYPE_NONE;
                packet.action = Packet::REQUEST_BEGIN_LOCK_UNIT;
                packet.unknown = 0;
                packet.param.layer = 0;
        packet.param.fallDirection = 0;
        packet.param.treeShakeType = TREE_SHAKE_TYPE_NONE;
        packet.param.isCutdown = 0;
        packet.param.cutCounter = 0;
                packet.pos.x = x;
                packet.pos.y = y;
                packet.replace = SvFgName::s_Empty;
                packet.fg1 = SvFgName::s_Empty;
                packet.fg2 = SvFgName::s_Empty;
                packet.stage = stage;
                packet.Send(netgame::PLAYER_NO_TARGET_ALL);
            }
            lock.ResetConfirm();
        }
    }
    return handle;
}
} // namespace fgobj

namespace fgobj {
// 0x005A19E0 (name is ours)
s32 FindPlaceAction(s32 x, s32 y, stage::Name stage)
{
    for (s32 i = 0; i < static_cast<s32>(PLACE_ACTION_NUM); i++) {
        const PlaceAction& action = s_PlaceActions[i];
        if (action.state != PlaceAction::STATE_NONE && action.pos.x == x && action.pos.y == y && action.stage == stage) {
            return i;
        }
    }
    return -1;
}

namespace {
// the hole of PlaceDig: start scale and its cylinder
const f32 DIG_START_SCALE = 0.01f;
const f32 DIG_CYLINDER_RADIUS = 1.5f;
const f32 DIG_CYLINDER_HEIGHT = 64.0f;
const u32 DIG_BODY_UNK8 = 4;
const u32 DIG_BODY_UNKC = 0x7FFFFFFF;
// the names of the field objects that can stay after a swing (ParamFgEntry::unk_0x0)
const s32 SWING_STAY_NAME_END = 254;
// the start of the objects that fall out of a tree: the height and the distance to the left or right
const f32 FALL_START_HEIGHT = 45.0f;
const f32 FALL_START_X = 24.0f;
// the height above the player from which an item is thrown
const f32 THROW_START_HEIGHT = 20.0f;
// the tour games whose results count planted flowers (OnPlanted; meaning of the values unknown)
const s8 TOUR_GAME_PLANT_SCORE = 1;
// the coin stone: its frames, the most hits, the chance of a lucky stone with the silver shovel, the weights of the ores
const s16 COIN_STONE_FRAMES = 270;
const u8 COIN_STONE_MAX_HITS = 8;
const f32 LUCKY_STONE_PERCENT = 20.0f;
const u32 LUCKY_WEIGHT_TOTAL = 110;
// the effect of a broken rock, the height from which the items of rocks fall, the count of a coin stone (meaning of the
// value unknown)
const u32 BREAK_ROCK_EFFECT_ID = 502;
const f32 DROP_START_HEIGHT = 10.0f;
const s32 COUNT_COIN_STONE = 31;
// the tour game whose results count ores (Start; meaning of the value unknown)
const s8 TOUR_GAME_ORE = 8;
// the counts of a pulled weed (meaning of the values unknown) and the most weeds of the player counter
const s32 CENSUS_PULLED_WEED = 10;
const s32 COUNT_PULLED_WEED = 14;
const u32 PULLED_WEED_MAX = 30;
const s32 PULLED_WEED_PACKET_KIND = 4;
// the census count of a cut tree (meaning of the value unknown)
const s32 CENSUS_CUT_TREE = 5;
// the census counts and the counts of Func24BD74 for planting (meaning of the values unknown)
const s32 CENSUS_PLANTED_SAPLING = 4;
const s32 CENSUS_PLANTED_FLOWER = 3;
const s32 COUNT_PLANTED = 9;

} // namespace

// 0x005A1A44 (name is ours)
void fgobj::PlaceAction::PlaceResult()
{
    switch (placeType) {
    case PLACE_TYPE_NONE:
    case PLACE_TYPE_HIT_ITEM:
    case PLACE_TYPE_HIT_COIN_STONE:
    case PLACE_TYPE_UNKNOWN_1D:
    case PLACE_TYPE_UNKNOWN_1E:
        break;
    case PLACE_TYPE_SHAKE_TREE:
        if (param.treeShakeType != TREE_SHAKE_TYPE_NONE) {
            Func168E20(pos.x, pos.y, param.layer, stage, fg1);
        }
        break;
    case PLACE_TYPE_CUT_TREE: {
        if (param.treeShakeType != TREE_SHAKE_TYPE_NONE || param.isCutdown) {
            Func168E20(pos.x, pos.y, param.layer, stage, fg1);
        }
        u32 counter;
        if (param.isCutdown) {
            counter = 0;
        } else {
            counter = param.cutCounter + 1;
            if (counter >= 3) {
                counter = 0;
            }
        }
        SetTreeCutCounter(pos.x, pos.y, counter);
        break;
    }
    case PLACE_TYPE_CUT_TREE_BEES:
        Func168E20(pos.x, pos.y, param.layer, stage, fg1);
        SetTreeCutCounter(pos.x, pos.y, param.cutCounter + 1);
        break;
    case PLACE_TYPE_BREAK_ROCK:
        Func168E20(pos.x, pos.y, param.layer, stage, fg1);
        break;
    case PLACE_TYPE_PICK_UP:
    case PLACE_TYPE_PICK_UP_FULL:
    case PLACE_TYPE_PICK_UP_SPECIAL:
    case PLACE_TYPE_PICK_UP_SPECIAL_FULL:
    case PLACE_TYPE_PICK_UP_SPECIAL_RELEASE:
    case PLACE_TYPE_PULL:
    case PLACE_TYPE_UNKNOWN_07:
    case PLACE_TYPE_SHAKE_TREE_BEES:
    case PLACE_TYPE_THROW:
    case PLACE_TYPE_BURY:
    case PLACE_TYPE_PLANT:
    case PLACE_TYPE_DISPLAY:
    case PLACE_TYPE_DIG_HOLE:
    case PLACE_TYPE_DIG_ITEM:
    case PLACE_TYPE_UNKNOWN_15:
    case PLACE_TYPE_UNKNOWN_16:
    case PLACE_TYPE_UNKNOWN_17:
    case PLACE_TYPE_UNKNOWN_18:
    case PLACE_TYPE_UNKNOWN_19:
    case PLACE_TYPE_UNKNOWN_1A:
    case PLACE_TYPE_DIG_STUMP:
    case PLACE_TYPE_FILL:
    case PLACE_TYPE_UNKNOWN_1F:
    case PLACE_TYPE_PITFALL:
    case PLACE_TYPE_FALL_ITEM:
    case PLACE_TYPE_THROW_SWAP_AFTER_PICK_UP:
    case PLACE_TYPE_UNKNOWN_23:
    case PLACE_TYPE_UNKNOWN_24:
        Func168E20(pos.x, pos.y, param.layer, stage, fg1);
        break;
    }
}

// 0x005A1C5C (name is ours)
void fgobj::PlaceAction::OnPlanted(const SvFgName* planted)
{
    if (netgame::GetMyPlayerNo() != player) {
        return;
    }
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    if (SvFgName::IsSaplingFgKind(planted->GetFgKind())) {
        Func6F0C80(CENSUS_PLANTED_SAPLING, 1);
        Func24BD74(COUNT_PLANTED, 1);
        Func2A83B0();
        return;
    }
    if (TourData::s_pInstance->Func76F6E8() == TOUR_GAME_PLANT_SCORE &&
        FdInfo::GetAttribute(pos.x, pos.y, FIELD_NAME_CURRENT) == FIELD_ATTRIBUTE_SOIL_AC && Func6E13E0()) {
        TourResult* tourResult = TourResult::s_pInstance;
        if (tourResult != NULL) {
            tourResult->ChangeScore(player, 1, *planted);
        }
    }
    if (TourData::s_pInstance->Func76F6E8() == TOUR_GAME_PLANT_SCORE) {
        return;
    }
    if (fg2.GetKind() == item::KIND_SEED || (fg2.IsName(SvFgName::NAME_FLOWER_SEED_SPRITE) && !param.fallDirection)) {
        Func6F0C80(CENSUS_PLANTED_FLOWER, 1);
        Func24BD74(COUNT_PLANTED, 1);
        Func2A83B0();
    }
}

// 0x005A22F4 (name is ours)
void fgobj::PlaceAction::Update()
{
    if (stage::GetCurrent() != stage) {
        PlaceResult();
        SendUnlockRequest();
        placeType = PLACE_TYPE_NONE;
        state = STATE_NONE;
        return;
    }
    if (state != STATE_PENDING) {
        return;
    }
    switch (placeType) {
    case PLACE_TYPE_BURY: {
        nn::math::VEC3 from;
        f32 scale;
        if (param.fallDirection) {
            scale = 1.0f;
            if (!Func5C3D1C(&from, player)) {
                return;
            }
        } else {
            scale = 0.0f;
            AcPlayer* actor = demo::PlayerGetActorEntry(player, false);
            if (actor == NULL) {
                return;
            }
            from = actor->m_Position;
            from.y += THROW_START_HEIGHT;
        }
        void* object = Func5C119C(player);
        if (object != NULL) {
            Func2505E0(object, SE_ACT_ITEM_CMN_THROW_OUT);
        }
        if (!PlaceThrowHole(fg2, pos.x, pos.y, &from, scale)) {
            return;
        }
        if (netgame::GetMyPlayerNo() == player) {
            Func5C3B70(pos.x, pos.y, demo::CheckOrder(demo::PLAYER_THINK, false), &fg2);
        }
        break;
    }
    case PLACE_TYPE_THROW:
    case PLACE_TYPE_THROW_SWAP_AFTER_PICK_UP: {
        nn::math::VEC3 from;
        f32 scale;
        if (param.fallDirection) {
            scale = 1.0f;
            if (!Func5C3D1C(&from, player)) {
                return;
            }
        } else {
            scale = 0.0f;
            AcPlayer* actor = demo::PlayerGetActorEntry(player, false);
            if (actor == NULL) {
                return;
            }
            from = actor->m_Position;
            from.y += THROW_START_HEIGHT;
        }
        void* object = Func5C119C(player);
        if (object != NULL) {
            Func2505E0(object, SE_ACT_ITEM_CMN_THROW_OUT);
        }
        if (!PlaceThrow(fg2, pos.x, pos.y, param.layer, scale, &from)) {
            return;
        }
        if (stage::IsKind(stage::KIND_INDOOR, stage::GetCurrent())) {
            Func168E20(pos.x, pos.y, param.layer, stage, fg1);
            SetFieldBit(pos.x, pos.y);
        }
        break;
    }
    case PLACE_TYPE_PLANT: {
        void* object = Func5C119C(player);
        if (object != NULL) {
            Func2505E0(object, SE_OBJ_PLANT_CMN_GROW);
        }
        OnPlanted(&fg1);
        if (!PlacePlant(fg1, pos.x, pos.y, false)) {
            return;
        }
        break;
    }
    case PLACE_TYPE_DISPLAY: {
        void* object = Func5C119C(player);
        if (object != NULL) {
            Func2505E0(object, SE_ACT_MYDESIGN_S_CHANGE);
        }
        if (!PlaceScale(fg1, pos.x, pos.y, false)) {
            return;
        }
        break;
    }
    default:
        break;
    }
    state = STATE_FINISHED;
}

// 0x005A1D9C (name is ours)
void fgobj::PlaceAction::Start()
{
    bool isBush = true;
    if (netgame::GetMyPlayerNo() == player && (placeType == PLACE_TYPE_PICK_UP || placeType == PLACE_TYPE_PICK_UP_FULL) &&
        TourData::s_pInstance != NULL && TourData::s_pInstance->Func76F6E8() == TOUR_GAME_ORE) {
        u32 kind = replace.GetKind();
        if (kind == item::KIND_ORE || kind == item::KIND_RUDE_ORE) {
            TourResult::s_pInstance->Func70D7AC(&replace);
        }
    }
    switch (placeType) {
    case PLACE_TYPE_PICK_UP:
    case PLACE_TYPE_PICK_UP_FULL:
    case PLACE_TYPE_PICK_UP_SPECIAL:
    case PLACE_TYPE_PICK_UP_SPECIAL_FULL:
    case PLACE_TYPE_PICK_UP_SPECIAL_RELEASE:
    case PLACE_TYPE_BURY:
        Func168E20(pos.x, pos.y, param.layer, stage, fg1);
        RefreshUnit(pos.x, pos.y, false);
        break;
    case PLACE_TYPE_PULL: {
        Func168E20(pos.x, pos.y, param.layer, stage, fg1);
        RefreshUnit(pos.x, pos.y, false);
        Func1758B0(this, pos.x, pos.y, false);
        if (netgame::GetMyPlayerNo() != player) {
            break;
        }
        Func6F0C80(CENSUS_PULLED_WEED, 1);
        Func24BD74(COUNT_PULLED_WEED, 1);
        Func2A83B0();
        if (Func5C3750() && replace.GetFgKind() == SvFgName::FG_KIND_WEED) {
            SvPlayer* current = SvGetCurrentPlayer();
            if (current != NULL) {
                Func20C6E0(current);
            }
        }
        PlayerSaveNumber save = SvGetPlayerSaveNo(player);
        GetGardenData();
        if (!SvIsNetPlayerSaveNo(save)) {
            break;
        }
        SvPlayer* data = SvGetPlayer(save);
        u32 weeds = data->unk_0x8F77.unk_0x7_0;
        if (weeds + 1 > PULLED_WEED_MAX) {
            weeds = PULLED_WEED_MAX;
        } else {
            weeds++;
        }
        data->unk_0x8F77.unk_0x7_0 = weeds;
        u16 pid = data->playerInfo.playerData.pid;
        u8* pids = static_cast<u8*>(netgame::g_pUnknown95488C);
        if (pids != NULL && player < netgame::PLAYER_NO_INVALID) {
            reinterpret_cast<u16*>(pids + 0xA0)[player] = pid;
        }
        PulledWeedPacket packet;
        packet.pid = pid;
        packet.player = player;
        Func7D1DCC(PULLED_WEED_PACKET_KIND, &packet);
        break;
    }
    case PLACE_TYPE_UNKNOWN_07:
        if (StartBury(false, ObjectBury::ANIMATION_PRESS_DEEP)) {
            return;
        }
        break;
    case PLACE_TYPE_SHAKE_TREE:
        if (StartSwing(false)) {
            return;
        }
        break;
    case PLACE_TYPE_SHAKE_TREE_BEES:
        if (StartSwing(false)) {
            return;
        }
        break;
    case PLACE_TYPE_CUT_TREE:
        if (StartSwing(true)) {
            return;
        }
        break;
    case PLACE_TYPE_CUT_TREE_BEES:
        if (StartSwing(true)) {
            return;
        }
        break;
    case PLACE_TYPE_HIT_COIN_STONE:
        if (HitCoinStone()) {
            return;
        }
        break;
    case PLACE_TYPE_BREAK_ROCK:
        if (BreakRock()) {
            return;
        }
        break;
    case PLACE_TYPE_DIG_HOLE: {
        SvFgName hole(static_cast<SvFgName::ID>(SvFgName::NAME_HOLE));
        bool failed = !PlaceDig(hole, pos.x, pos.y);
        if (failed) {
            PlaceResult();
            SendUnlockRequest();
            placeType = PLACE_TYPE_NONE;
            state = STATE_NONE;
            RefreshUnit(pos.x, pos.y, false);
            return;
        }
        Func168E20(pos.x, pos.y, param.layer, stage, SvFgName::s_Empty);
        RefreshUnit(pos.x, pos.y, false);
        Func1758B0(this, pos.x, pos.y, true);
        return;
    }
    case PLACE_TYPE_DIG_ITEM:
    case PLACE_TYPE_UNKNOWN_15:
    case PLACE_TYPE_UNKNOWN_16:
    case PLACE_TYPE_UNKNOWN_17:
    case PLACE_TYPE_UNKNOWN_18:
    case PLACE_TYPE_UNKNOWN_19:
    case PLACE_TYPE_UNKNOWN_1A: {
        SvFgName hole(static_cast<SvFgName::ID>(SvFgName::NAME_HOLE));
        bool failed = !PlaceDig(hole, pos.x, pos.y);
        if (failed) {
            PlaceResult();
            SendUnlockRequest();
            placeType = PLACE_TYPE_NONE;
            state = STATE_NONE;
            RefreshUnit(pos.x, pos.y, false);
            return;
        }
        Func168E20(pos.x, pos.y, param.layer, stage, SvFgName::s_Empty);
        RefreshUnit(pos.x, pos.y, false);
        return;
    }
    case PLACE_TYPE_DIG_STUMP:
        if (!replace.IsBush() || item::GetParamFgEntry(replace)->size < 1) {
            isBush = false;
        }
        if (!PlaceStump(replace, pos.x, pos.y, player, isBush)) {
            break;
        }
        Func168E20(pos.x, pos.y, param.layer, stage, SvFgName::s_Empty);
        RefreshUnit(pos.x, pos.y, false);
        return;
    case PLACE_TYPE_FILL:
        Func168E20(pos.x, pos.y, param.layer, stage, SvFgName::s_Empty);
        RefreshUnit(pos.x, pos.y, false);
        break;
    default:
        break;
    }
    SendUnlockRequest();
    placeType = PLACE_TYPE_NONE;
    state = STATE_NONE;
}

// 0x005A2688 (name is ours)
bool fgobj::PlaceAction::StartBury(bool deep, u8 animation)
{
    u8 finishMode = deep ? ObjectBury::FINISH_MODE_TAKE_UP : ObjectBury::FINISH_MODE_END;
    SvFgName buried;
    if (replace.IsFg()) {
        buried = replace;
    } else {
        buried = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_HOLE));
    }
    if (!PlaceBury(buried, pos.x, pos.y, finishMode, animation)) {
        return false;
    }
    Func168E20(pos.x, pos.y, param.layer, stage, SvFgName::s_Empty);
    RefreshUnit(pos.x, pos.y, false);
    return true;
}

// 0x005A2774 (name is ours)
bool fgobj::PlaceAction::StartSwing(bool isCut)
{
    const u8* drop = NULL;
    if (param.treeShakeType == TREE_SHAKE_TYPE_FRUIT || param.treeShakeType == TREE_SHAKE_TYPE_FRUIT_DIE) {
        drop = dropOffset;
    }
    SvFgName after(SvFgName::s_Empty);
    u8 kind;
    if (!isCut) {
        kind = ObjectSwing::KIND_SHAKE_L;
    } else {
        kind = ObjectSwing::KIND_SHAKE_M;
        if (param.isCutdown) {
            after = fg1;
            kind = param.fallDirection ? ObjectSwing::KIND_CUT_RIGHT : ObjectSwing::KIND_CUT_LEFT;
            if (netgame::GetMyPlayerNo() == player) {
                Func6F0C80(CENSUS_CUT_TREE, 1);
            }
            s_LastCutUnitX = pos.x;
            s_LastCutUnitZ = pos.y;
        }
    }
    SvFgName item(replace);
    bool isFallAll = false;
    if (param.treeShakeType == TREE_SHAKE_TYPE_FRUIT_DIE) {
        item = fg1;
        isFallAll = true;
    }
    if (!PlaceSwing(item, after, pos.x, pos.y, drop, kind, isFallAll)) {
        return false;
    }
    if (param.treeShakeType == TREE_SHAKE_TYPE_ITEM && dropOffset[0] != 0) {
        PlaceFall(fg2, pos.x, pos.y, dropOffset[0], param.fallDirection);
    }
    if (param.treeShakeType == TREE_SHAKE_TYPE_BEES && dropOffset[0] != 0) {
        PlaceHoneycomb(fg2, pos.x, pos.y, dropOffset[0], param.fallDirection);
    }
    return true;
}

// 0x005A2930 (name is ours)
bool fgobj::PlaceAction::HitCoinStone()
{
    if (!fgobj::HitCoinStone(pos.x, pos.y)) {
        return false;
    }
    stage::Name actionStage = stage;
    u8 x = pos.x;
    u8 z = pos.y;
    for (s32 dz = -1; dz <= 1; dz++) {
        s32 dropZ = z + dz;
        for (s32 dx = -1; dx <= 1; dx++) {
            if ((dz | dx) == 0) {
                continue;
            }
            s32 dropX = x + dx;
            if (!Func165460(dropX, dropZ, actionStage, 0, true)) {
                continue;
            }
            PlaceCoinStone(replace, pos.x, pos.y);
            u16 name;
            if (s_CoinStone.isLucky) {
                u32 random = netgame::Func2FF63C(LUCKY_WEIGHT_TOTAL);
                u32 sum = 0;
                u32 i;
                for (i = 0; i < 6; i++) {
                    sum += s_LuckyStoneWeights[i];
                    if (!(random > sum)) {
                        break;
                    }
                }
                name = i < 6 ? s_LuckyStoneItems[i] : static_cast<u16>(SvFgName::NAME_GOLD_NUGGET);
            } else {
                s32 level = 1;
                if (netgame::GetNetgameType() != netgame::TYPE_DREAM && g_UnkAAFF18.pPlayer != NULL) {
                    if (g_Unk954F88 == 0) {
                        level = 2;
                    } else if (g_Unk954F88 == 1) {
                        level = 0;
                    }
                }
                name = s_CoinStoneItems[s_CoinStone.hits + level - 1];
            }
            SvFgName item(static_cast<SvFgName::ID>(name));
            if (item::IsMoney(item)) {
                item.SetUnkFlag0();
            }
            u32 sound = SE_OBJ_1UP;
            if (item.GetModelID() == item::MODEL_ID_COIN) {
                sound--;
            }
            nn::math::VEC3 soundPosition;
            GetObjectPosition(&soundPosition, item, pos.x, pos.y, false);
            {
                AudioObj1 audio;
                audio.Start(&soundPosition);
                audio.StartSound(sound);
            }
            if (LockUnit(PLACE_TYPE_FALL_ITEM, dropX, dropZ, actionStage, true) != -1) {
                {
                    Packet packet;
                    packet.SetFallItem(&item, dropX, dropZ, actionStage);
                    packet.Place();
                }
                nn::math::VEC3 from;
                GetObjectPosition(&from, replace, pos.x, pos.y, false);
                from.y += DROP_START_HEIGHT;
                if (PlaceThrow(item, dropX, dropZ, false, 0.0f, &from)) {
                    Func24BA04(&SvGetCurrentPlayer()->initiative, COUNT_COIN_STONE, 1);
                }
            }
            return true;
        }
    }
    return false;
}

// 0x005A2C24 (name is ours)
bool fgobj::PlaceAction::BreakRock()
{
    nn::math::VEC3 position;
    GetObjectPosition(&position, replace, pos.x, pos.y, false);
    Func207B90(BREAK_ROCK_EFFECT_ID, &position, &ANGLE16_VEC3_ZERO, &VEC3_ONE, true, 0);
    nn::math::VEC3 soundPosition;
    GetObjectPosition(&soundPosition, replace, pos.x, pos.y, false);
    {
        AudioObj1 audio;
        audio.Start(&soundPosition);
        audio.StartSound(SE_OBJ_FG_STON_BRAKE);
    }
    Func168E20(pos.x, pos.y, param.layer, stage, SvFgName::s_Empty);
    RefreshUnit(pos.x, pos.y, false);
    position.y += DROP_START_HEIGHT;
    PlaceThrow(fg1, pos.x, pos.y, false, 0.0f, &position);
    return true;
}

// 0x005A334C (name is ours)
bool HitCoinStone(s32 x, s32 z)
{
    if (!s_CoinStone.isStarted) {
        s_CoinStone.isStarted = true;
        SvPlayer* player = g_UnkAAFF18.pPlayer;
        if (player != NULL && Func71B394(&player->playerAppearance.playerOutfit)->IsName(SvFgName::NAME_SILVER_SHOVEL) &&
            !(GetRandomF(100.0f) > LUCKY_STONE_PERCENT)) {
            s_CoinStone.isLucky = true;
        } else {
            s_CoinStone.isLucky = false;
        }
        s_CoinStone.timer = COIN_STONE_FRAMES;
        s_CoinStone.x = x;
        s_CoinStone.z = z;
        s_CoinStone.hits = 1;
        return true;
    }
    if (s_CoinStone.timer == 0) {
        return false;
    }
    if (s_CoinStone.hits < COIN_STONE_MAX_HITS && s_CoinStone.x == x && s_CoinStone.z == z) {
        s_CoinStone.hits++;
        return true;
    }
    return false;
}

// 0x005A3434 (name is ours)
void UpdateCoinStone()
{
    if (!s_CoinStone.isStarted || s_CoinStone.timer <= 0) {
        return;
    }
    s_CoinStone.timer--;
    if (s_CoinStone.timer != 0) {
        return;
    }
    Field* field = FdInfo::GetCurrentField();
    SvFgName rock(*field->GetFg(s_CoinStone.x, s_CoinStone.z, 0));
    rock.ClearUnique();
    field->PlaceItem(rock, s_CoinStone.x, s_CoinStone.z, 0);
}

// 0x005A34C4 (name is ours)
void FinishCoinStone()
{
    if (!s_CoinStone.isStarted || s_CoinStone.timer <= 0) {
        return;
    }
    Field* field = FdInfo::GetCurrentField();
    SvFgName rock(*field->GetFg(s_CoinStone.x, s_CoinStone.z, 0));
    rock.ClearUnique();
    field->PlaceItem(rock, s_CoinStone.x, s_CoinStone.z, 0);
    s_CoinStone.timer = 0;
}

// 0x005A2D34 (name is ours)
void FinishPlaceRequest(s32 x, s32 z, stage::Name stage)
{
    s32 index = FindPlaceAction(x, z, stage);
    if (index != -1) {
        PlaceAction& action = s_PlaceActions[index];
        action.PlaceResult();
        action.SendUnlockRequest();
        action.placeType = PLACE_TYPE_NONE;
        action.state = PlaceAction::STATE_NONE;
    }
}

// 0x005A2D78 (name is ours)
void AddPlaceAction(PlaceType type, netgame::PlayerNo player, const SvFgName& replace, const SvFgName& fg1, const SvFgName& fg2,
                    u8 x, u8 y, stage::Name stage, PlaceParam param, const u8* dropOffset)
{
    s32 index = FindPlaceAction(x, y, stage);
    if (index != -1) {
        PlaceAction& old = s_PlaceActions[index];
        old.PlaceResult();
        old.placeType = PLACE_TYPE_NONE;
        old.state = PlaceAction::STATE_NONE;
    }
    s32 free;
    for (free = 0; free < static_cast<s32>(PLACE_ACTION_NUM); free++) {
        if (s_PlaceActions[free].state == PlaceAction::STATE_NONE) {
            break;
        }
    }
    if (free >= static_cast<s32>(PLACE_ACTION_NUM)) {
        free = -1;
    }
    PlaceAction& action = s_PlaceActions[free];
    action.placeType = type;
    action.player = player;
    action.replace = replace;
    action.fg1 = fg1;
    action.fg2 = fg2;
    action.pos.x = x;
    action.pos.y = y;
    action.param = param;
    action.stage = stage;
    if (dropOffset != NULL) {
        action.dropOffset[0] = dropOffset[0];
        action.dropOffset[1] = dropOffset[1];
        action.dropOffset[2] = dropOffset[2];
    } else {
        action.dropOffset[0] = 0;
        action.dropOffset[1] = 0;
        action.dropOffset[2] = 0;
    }
    action.state = PlaceAction::STATE_PENDING;
    bool isHere = stage::GetCurrent() == action.stage;
    isHere &= Proc::Exists();
    if (Func5C481C(action.player, true) != 0 && isHere) {
        action.Update();
        return;
    }
    action.PlaceResult();
    action.SendUnlockRequest();
    action.placeType = PLACE_TYPE_NONE;
    action.state = PlaceAction::STATE_NONE;
}

// 0x005A2EE8 (name is ours)
void UpdatePlaceActions()
{
    for (s32 i = 0; i < static_cast<s32>(PLACE_ACTION_NUM); i++) {
        PlaceAction& action = s_PlaceActions[i];
        if (action.state != PlaceAction::STATE_NONE) {
            action.Update();
        }
    }
}

// 0x005A2F20 (name is ours)
void fgobj::PlaceAction::SendUnlockRequest()
{
    Packet packet;
    u8 x = pos.x;
    u8 y = pos.y;
    stage::Name actionStage = stage;
    packet.player = netgame::GetMyPlayerNo();
    packet.placeType = PLACE_TYPE_NONE;
    packet.requestIndex = 0;
    packet.action = Packet::REQUEST_UNLOCK_UNIT;
    packet.param.layer = 0;
    packet.param.fallDirection = 0;
    packet.param.treeShakeType = 0;
    packet.param.isCutdown = 0;
    packet.param.cutCounter = 0;
    packet.pos.x = x;
    packet.pos.y = y;
    packet.replace = SvFgName::s_Empty;
    packet.fg1 = SvFgName::s_Empty;
    packet.fg2 = SvFgName::s_Empty;
    packet.stage = actionStage;
    if (!netgame::IsMultiplePlayer() || netgame::IsHost()) {
        packet.Process();
    } else {
        packet.Send(netgame::PLAYER_NO_HOST);
    }
}

// 0x005A301C (name is ours)
void FinishPlaceActions()
{
    for (s32 i = 0; i < static_cast<s32>(PLACE_ACTION_NUM); i++) {
        PlaceAction& action = s_PlaceActions[i];
        if (action.state != PlaceAction::STATE_NONE) {
            action.PlaceResult();
            action.SendUnlockRequest();
            action.placeType = PLACE_TYPE_NONE;
            action.state = PlaceAction::STATE_NONE;
        }
    }
}

// 0x005A30B4 (name is ours)
void ClearTreeCutCounters()
{
    std::memset(s_TreeCutCounters, 0, sizeof(s_TreeCutCounters));
    s_LastCutUnitX = -1;
    s_LastCutUnitZ = -1;
}

// 0x005A30E0 (name is ours)
bool IsLastCutUnit(s32 x, s32 z)
{
    return s_LastCutUnitX == x && s_LastCutUnitZ == z;
}

// 0x0059AEDC (name is ours)
void SetFieldBit(s32 x, s32 z)
{
    if (s_pProc != NULL) {
        s_pProc->m_FieldBit.Set(x, z);
    }
}

// 0x0059BB68 (name is ours)
bool PlaceThrow(const SvFgName& item, s32 x, s32 z, bool useObjectHeight, f32 scale, const nn::math::VEC3* from)
{
    if (s_pProc == NULL || !FdInfo::GetCurrentField()->IsInside(x, z)) {
        return false;
    }
    ObjectThrow* object;
    if (s_IsOutdoor && item.IsName(SvFgName::NAME_FOUNTAIN_FIREWORK)) {
        ObjectHanabi* hanabi = NewObject<ObjectHanabi>(s_SmallHeaps, SMALL_HEAP_NUM);
        if (hanabi != NULL) {
            hanabi->Setup(item, x, z, useObjectHeight);
            hanabi->StartThrow(from, scale);
            hanabi->m_State = ObjectHanabi::STATE_THROW;
            hanabi->m_HoldsRequest = true;
        }
        object = hanabi;
    } else {
        object = NewObject<ObjectThrow>(s_SmallHeaps, SMALL_HEAP_NUM);
        if (object != NULL) {
            object->Setup(item, x, z, useObjectHeight);
            object->StartThrow(from, scale);
        }
    }
    if (object == NULL) {
        return false;
    }
    s_pProc->m_ExeObjects.InsertByPriority(object);
    return true;
}

// 0x0059C6A0 (name is ours)
bool PlaceThrowHole(const SvFgName& item, s32 x, s32 z, const nn::math::VEC3* from, f32 scale)
{
    if (s_pProc == NULL || !FdInfo::GetCurrentField()->IsInside(x, z)) {
        return false;
    }
    ObjectThrowHole* object = ObjectThrowHole::Create(item, x, z, from, scale);
    if (object == NULL) {
        return false;
    }
    s_pProc->m_ExeObjects.InsertByPriority(object);
    return true;
}

// 0x0059B494 (name is ours)
void GetObjectPosition(nn::math::VEC3* out, const SvFgName& item, s32 x, s32 z, bool useObjectHeight)
{
    FdInfo::ToCenterWorldWithUnitHeight(out, x, z);
    if (useObjectHeight) {
        out->y = Func4EA670(x, z);
    }
    const stage::Name current = stage::GetCurrent();
    if (current == stage::NAME_RECYCLE_SHOP || stage::IsKind(stage::KIND_AUTO_CAMP, current)) {
        out->x += FdInfo::UNIT_WORLD_SIZE_HALF_X;
        return;
    }
    if (!s_IsOutdoor || item.IsBuried()) {
        return;
    }
    const u8 fgKind = item.GetFgKind();
    const item::Kind kind = item.GetKind();
    if (fgKind == SvFgName::FG_KIND_WEED || kind == item::KIND_SHELL) {
        const f32* offset = s_ScatterOffsets[z & 3][x & 3];
        out->x += offset[0];
        out->z += offset[1];
    }
}

// 0x0059C468 (name is ours)
bool PlaceStump(const SvFgName& item, s32 x, s32 z, netgame::PlayerNo player, bool isBush)
{
    if (s_pProc == NULL || !FdInfo::GetCurrentField()->IsInside(x, z)) {
        return false;
    }
    math::Angle16 angle;
    angle.value = 0;
    GetAngleFromPlayer(&angle, item, x, z, player);
    ObjectStump* stump;
    if (isBush) {
        stump = ObjectStumpAnim::Create(item, &x, &z, &angle);
    } else {
        stump = ObjectStump::Create(item, &x, &z, &angle);
    }
    if (stump == NULL) {
        return false;
    }
    Proc* proc = s_pProc;
    ObjectBase* object = static_cast<ObjectBase*>(proc->m_ExeObjects.GetFirst());
    while (object != NULL) {
        ObjectBase* next = static_cast<ObjectBase*>(object->m_pNext);
        if (object->m_X == x && object->m_Z == z) {
            proc->RemoveObject(object, false, true);
        }
        object = next;
    }
    s_pProc->m_ExeObjects.InsertByPriority(stump);
    return true;
}

// 0x0059C5BC (name is ours)
bool PlaceHoneycomb(const SvFgName& item, s32 x, s32 z, u8 fallDirection, bool isRight)
{
    if (s_pProc == NULL || !FdInfo::GetCurrentField()->IsInside(x, z)) {
        return false;
    }
    nn::math::VEC3 offset;
    offset.y = FALL_START_HEIGHT;
    offset.z = 0.0f;
    u8 direction;
    if (isRight) {
        offset.x = FALL_START_X;
        direction = ObjectFall::DIRECTION_X_PLUS_Z_PLUS;
    } else {
        offset.x = -FALL_START_X;
        direction = ObjectFall::DIRECTION_X_MINUS_Z_PLUS;
    }
    ObjectHoneycomb* honeycomb = ObjectHoneycomb::Create(item, x, z, &offset, direction);
    if (honeycomb == NULL) {
        return false;
    }
    honeycomb->StartFall(fallDirection);
    s_pProc->m_ExeObjects.InsertByPriority(honeycomb);
    return true;
}

// 0x0059CB78 (name is ours)
void GetAngleFromPlayer(math::Angle16* out, const SvFgName& item, s32 x, s32 z, netgame::PlayerNo player)
{
    math::Angle16 angle;
    AcPlayer* actor = demo::PlayerGetActorEntry(player, false);
    if (actor == NULL) {
        angle.value = 0;
    } else {
        nn::math::VEC3 position;
        GetObjectPosition(&position, item, x, z, false);
        angle = Actor::CalcAngleY(&actor->m_Position, &position);
    }
    *out = angle;
}

// 0x0059D6CC (name is ours)
bool PlaceFall(const SvFgName& item, s32 x, s32 z, u8 fallDirection, bool isRight)
{
    if (s_pProc == NULL || !FdInfo::GetCurrentField()->IsInside(x, z)) {
        return false;
    }
    nn::math::VEC3 offset;
    offset.y = FALL_START_HEIGHT;
    offset.z = 0.0f;
    u8 direction;
    if (isRight) {
        offset.x = FALL_START_X;
        direction = ObjectFall::DIRECTION_X_PLUS_Z_PLUS;
    } else {
        offset.x = -FALL_START_X;
        direction = ObjectFall::DIRECTION_X_MINUS_Z_PLUS;
    }
    bool isLeaf = !item.HasIcon();
    ObjectFall* object;
    if (!isLeaf) {
        object = ObjectFruit::Create(item, x, z, &offset, 0.0f, direction, false);
    } else {
        object = ObjectLeaf::Create(item, x, z, &offset, direction, fallDirection);
    }
    if (object == NULL) {
        return false;
    }
    if (!isLeaf) {
        object->StartFall(fallDirection);
    }
    s_pProc->m_ExeObjects.InsertByPriority(object);
    return true;
}

// 0x0059D80C (name is ours)
bool PlaceCoinStone(const SvFgName& item, s32 x, s32 z)
{
    if (s_pProc == NULL || !FdInfo::GetCurrentField()->IsInside(x, z)) {
        return false;
    }
    math::Angle16 angle;
    angle.value = 0;
    GetAngleFromPlayer(&angle, item, x, z, netgame::GetMyPlayerNo());
    ObjectCoinStone* stone = NewObject<ObjectCoinStone>(s_SmallHeaps, SMALL_HEAP_NUM);
    if (stone != NULL) {
        stone->Setup(item, x, z, false);
        stone->m_Direction = angle.value;
        stone->m_Phase = 0;
        stone->InitializeShadow();
    }
    if (stone == NULL) {
        return false;
    }
    s_pProc->m_ExeObjects.InsertByPriority(stone);
    s_pProc->RemoveObjectsAt(x, z, Proc::REMOVE_MODE_ALL);
    s_pProc->m_FieldBit.Set(x, z);
    return true;
}

// 0x0059A9D0 (name is ours)
bool PlaceDig(const SvFgName& item, s32 x, s32 z)
{
    if (s_pProc == NULL || !FdInfo::GetCurrentField()->IsInside(x, z)) {
        return false;
    }
    ObjectDig* dig = NewObject<ObjectDig>(s_SmallHeaps, SMALL_HEAP_NUM);
    if (dig != NULL) {
        dig->Setup(item, x, z, false);
        dig->m_Frame = 0;
        nn::math::VEC3 scale(DIG_START_SCALE, DIG_START_SCALE, DIG_START_SCALE);
        dig->m_Model.SetScale(scale);
        dig->m_Model.Update();
        dig->m_Cylinder.Set(&dig->m_Position, DIG_CYLINDER_RADIUS, DIG_CYLINDER_HEIGHT, DIG_BODY_UNK8, DIG_BODY_UNKC);
        bgcheck::AddBody(&dig->m_Cylinder);
    }
    if (dig == NULL) {
        return false;
    }
    s_pProc->m_ExeObjects.InsertByPriority(dig);
    return true;
}

// 0x0059ADC0 (name is ours)
bool PlaceBury(const SvFgName& item, s32 x, s32 z, u8 finishMode, u8 animation)
{
    if (s_pProc == NULL || !FdInfo::GetCurrentField()->IsInside(x, z)) {
        return false;
    }
    ObjectBury* bury = NewObject<ObjectBury>(s_SmallHeaps, SMALL_HEAP_NUM);
    if (bury != NULL) {
        bury->Setup(item, x, z, false);
        bury->m_Frame = 0;
        bury->m_FinishMode = finishMode;
        bury->m_Animation = animation;
    }
    if (bury == NULL) {
        return false;
    }
    s_pProc->m_ExeObjects.InsertByPriority(bury);
    return true;
}

// 0x0059B77C (name is ours)
bool PlaceSwing(const SvFgName& item, const SvFgName& after, s32 x, s32 z, const u8* dropOffset, u8 kind, bool isFallAll)
{
    if (s_pProc == NULL || !FdInfo::GetCurrentField()->IsInside(x, z)) {
        return false;
    }
    bool isCut = kind == ObjectSwing::KIND_CUT_LEFT || kind == ObjectSwing::KIND_CUT_RIGHT;
    ObjectSwing* swing = ObjectSwing::Create(item, x, z, kind, isFallAll);
    if (swing == NULL) {
        return false;
    }
    s_pProc->RemoveObjectsAt(x, z, Proc::REMOVE_MODE_NO_FLAG_4);
    Proc* proc = s_pProc;
    ObjectBase* object = static_cast<ObjectBase*>(proc->m_ExeObjects.GetFirst());
    while (object != NULL) {
        ObjectBase* next = static_cast<ObjectBase*>(object->m_pNext);
        if (object->m_X == x && object->m_Z == z) {
            proc->RemoveObject(object, false, true);
        }
        object = next;
    }
    s_pProc->m_ExeObjects.InsertByPriority(swing);
    SvFgName stay;
    if (!after.IsEmpty()) {
        stay = after;
    } else {
        s32 name = item::GetParamFgEntry(item)->unk_0x0;
        if (name >= SWING_STAY_NAME_END) {
            name = 0;
        }
        stay = SvFgName(static_cast<SvFgName::ID>(name));
    }
    u8 frames = swing->m_FadeFrame - 1;
    ObjectTimer* timer = NewObject<ObjectTimer>(s_SmallHeaps, SMALL_HEAP_NUM);
    if (timer != NULL) {
        timer->Setup(stay, x, z, false);
        timer->m_Timer = frames;
        timer->m_HasShadow = isCut;
        timer->m_Frame = 0;
    }
    if (timer != NULL) {
        s_pProc->m_ExeObjects.InsertByPriority(timer);
        timer->m_Flags |= ObjectBase::FLAG_GROWN_TREE;
    }
    if (item.IsName(SvFgName::NAME_CEDAR_XMAS)) {
        ObjectDecoSwing* deco = ObjectDecoSwing::Create(item, &x, &z, swing->m_Animation);
        if (deco != NULL) {
            s_pProc->m_ExeObjects.InsertByPriority(deco);
        }
    }
    if (dropOffset != NULL) {
        for (s32 i = 0; i < 3; i++) {
            if (dropOffset[i] == 0) {
                continue;
            }
            ObjectBase* hanging = static_cast<ObjectBase*>(s_pProc->m_FieldObjects.GetFirst());
            while (hanging != NULL) {
                ObjectBase* next = static_cast<ObjectBase*>(hanging->m_pNext);
                if (hanging->m_X == x && hanging->m_Z == z && (hanging->m_Flags & ObjectBase::FLAG_UNK_4) &&
                    hanging->GetFallDirection() == i) {
                    break;
                }
                hanging = next;
            }
            if (hanging != NULL) {
                Proc* current = s_pProc;
                current->m_FieldObjects.Erase(hanging);
                current->m_ExeObjects.InsertByPriority(hanging);
                hanging->StartFall(dropOffset[i]);
            } else if (dropOffset[i] != 255) {
                s32 fallX;
                s32 fallZ;
                GetUnitOfDirection(&fallX, &fallZ, x, z, dropOffset[i]);
                FinishPlaceRequest(fallX, fallZ, stage::GetCurrent());
                s_pProc->m_FieldBit.Reset(fallX, fallZ);
            }
        }
    }
    s_pProc->m_FieldBit.Set(x, z);
    return true;
}

// 0x0059F144 (name is ours)
void RefreshUnit(s32 x, s32 z, bool removeRunning)
{
    if (s_pProc == NULL) {
        return;
    }
    if (s_pProc->m_FieldBit2.IsSet(x, z)) {
        s_pProc->m_FieldBit2.Reset(x, z);
        s_pProc->m_FieldBit.Set(x, z);
        return;
    }
    s_pProc->RemoveObjectsAt(x, z, Proc::REMOVE_MODE_ALL);
    if (removeRunning) {
        Proc* proc = s_pProc;
        ObjectBase* object = static_cast<ObjectBase*>(proc->m_ExeObjects.GetFirst());
        while (object != NULL) {
            ObjectBase* next = static_cast<ObjectBase*>(object->m_pNext);
            if (object->m_X == x && object->m_Z == z) {
                proc->RemoveObject(object, false, true);
            }
            object = next;
        }
    }
    s_pProc->m_FieldBit.Reset(x, z);
}

// 0x0059A788 (name is ours)
bool ConvertForBury(SvFgName* item, SvFgName* sprite)
{
    const SvFgName original(*item);
    const item::Kind kind = item->GetKind();
    if (!Func768A60(&original, item)) {
        item->SetBuried();
    }
    u16 name;
    if (kind == item::KIND_SEED) {
        name = SvFgName::NAME_FLOWER_SEED_SPRITE;
    } else if (kind == item::KIND_FLOWER) {
        *sprite = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_FLOWER_SEED_SPRITE));
        return true;
    } else if (kind == item::KIND_SEEDLING) {
        name = original.IsName(SvFgName::NAME_SAPLING) ? SvFgName::NAME_BASIC_SAPLING_SPRITE : SvFgName::NAME_CEDAR_SAPLING_SPRITE;
    } else if (kind == item::KIND_SEEDLING_BUSH) {
        name = SvFgName::NAME_PALM_SAPLING_SPRITE;
    } else {
        return false;
    }
    *sprite = SvFgName(static_cast<SvFgName::ID>(name));
    return false;
}

// 0x0059C180 (name is ours)
const void* GetFgSkeletalAnim(const char* name, u8 animKind)
{
    if (!(s_IsOutdoor && s_pProc != NULL)) {
        return NULL;
    }
    sead::FixedSafeString<32> path;
    {
        sead::StringFormatter formatter(&path);
        formatter.append("%s_t_%s").append(name).append(ObjectSwing::s_AnimationSuffixes[animKind]).flush();
    }
    const char* cstr = path.getCString();
    return s_pProc->m_AnimLoader.m_GraphicsFile.ptr()->skeletalAnims.GetData(cstr);
}

// 0x0059C2B0 (name is ours)
bool PlacePresent(const SvFgName& item, s32 x, s32 z, const nn::math::VEC3* position, u8 state)
{
    if (s_pProc == NULL || !FdInfo::GetCurrentField()->IsInside(x, z)) {
        return false;
    }
    ObjectPresent* object = ObjectPresent::Create(item, x, z, position, state);
    if (object == NULL) {
        return false;
    }
    s_pProc->m_ExeObjects.InsertByPriority(object);
    return true;
}

namespace {
// the frames of the two kinds of the wobble
const u8 WOBBLE_FRAMES_SHORT = 8;
const u8 WOBBLE_FRAMES_LONG = 12;
} // namespace

// 0x0059CE4C (name is ours)
bool UpdateWobble(s16* x, s16* y, s16* z)
{
    if (s_Unk94FE62 == 0) {
        if (s_Unk94FE61 == 0) {
            return false;
        }
        s_Unk94FE60 = GetRandom(2) == 0;
        s_Unk94FE61 = 0;
        s_Unk94FE62 = s_Unk94FE60 ? WOBBLE_FRAMES_SHORT : WOBBLE_FRAMES_LONG;
        s_Unk94FE64 = GetRandom(0x222) + 0x4FA;
        s_Unk94FE78 = 1.5f + GetRandomF01() * 1.5f;
    }
    if (s_Unk94FE62 != 0) {
        s_Unk94FE62--;
    }
    if (s_Unk94FE61 != 0) {
        s_Unk94FE61--;
    }
    const u32 frame = s_Unk94FE62;
    if (frame == 0) {
        *x = 0;
        *y = 0;
        *z = 0;
        return true;
    }
    s16 swing;
    s16 turn;
    if (s_Unk94FE60) {
        swing = frame << 11;
        turn = (frame << 14) & 0xFFFF;
    } else {
        swing = frame * 0x555;
        turn = ((frame * 0x1555) << 1) & 0xFFFF;
    }
    const f32 amplitude = nn::math::SinIdx(static_cast<u16>(swing)) * static_cast<f32>(s_Unk94FE64);
    *z = static_cast<s16>(amplitude * nn::math::CosIdx(static_cast<u16>(turn)));
    if (s_Unk94FE60) {
        if (frame <= 4) {
            s_Unk94FE7C = nn::math::SinIdx(static_cast<u16>((frame << 29) >> 16)) * (s_Unk94FE78 * 0.5f);
        } else {
            s_Unk94FE7C = nn::math::SinIdx(static_cast<u16>(((frame << 28) + 0xC0000000) >> 16)) * s_Unk94FE78;
        }
    } else {
        if (frame <= 6) {
            s_Unk94FE7C = nn::math::SinIdx(static_cast<u16>(frame * 0x1555)) * (s_Unk94FE78 * 0.5f);
        } else {
            s_Unk94FE7C = nn::math::SinIdx(static_cast<u16>((frame - 6) * 0x1555)) * s_Unk94FE78;
        }
    }
    *x = 0;
    *y = 0;
    return true;
}

// 0x0059C590 (name is ours)
s32 Func59C590(s32 a, s32 b)
{
    if (s_pProc == NULL) {
        return -1;
    }
    return Func150220(s_pProc, a, b);
}

// 0x0059D290 (name is ours)
s32 Func59D290(s32 a, s32 b, s32 c)
{
    if (s_pProc == NULL) {
        return 0;
    }
    if (c == -1) {
        return 0;
    }
    return Func150228(s_pProc, a, b, c);
}

// 0x0059C34C (name is ours)
void SetUnk94FE5C()
{
    s_Unk94FE5C = 1;
}

// 0x0059C360 (name is ours)
const void* GetFgAnimModel(const char* name, bool isSpecial)
{
    if (!(s_IsOutdoor && s_pProc != NULL)) {
        return NULL;
    }
    sead::FixedSafeString<32> path;
    if (isSpecial) {
        sead::StringFormatter formatter(&path);
        formatter.append("%s_s_t").append(name).flush();
    } else {
        sead::StringFormatter formatter(&path);
        formatter.append("%s_t").append(name).flush();
    }
    const char* cstr = path.getCString();
    return nw::gfx::GetModel(reinterpret_cast<const void* const*>(&s_pProc->m_AnimLoader.m_GraphicsFile), cstr);
}

// 0x0059A99C (name is ours)
const void* GetFgModel(const char* name)
{
    if (!(s_IsOutdoor && s_pProc != NULL)) {
        return NULL;
    }
    return nw::gfx::GetModel(reinterpret_cast<const void* const*>(&s_pProc->m_FgLoader.m_GraphicsFile), name);
}
} // namespace fgobj
