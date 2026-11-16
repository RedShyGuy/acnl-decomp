#include "fgobj/dPacket.h"
#include "fgobj/dPlace.h"
#include "fgobj/fgobj_Extern.h"
#include "field/dFdInfo.h"
#include "field/dField.h"
#include "g3d/dResourceLoader.h"
#include "sead/seadNew.h"
#include "ssys/ma/dFileData.h"
#include "Item/item_Api.h"
#include "netgame/netgame_Api.h"
#include "script/dScriptExtern.h"
#include "stage/stage_Extern.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "Sv/dSvPlayer.h"

namespace fgobj {
namespace {
// the initiatives of AddPlantingInitiative by the planted item (meaning of the values unknown)
const s32 INITIATIVE_SEED = 95;
const s32 INITIATIVE_SAPLING = 23;
const s32 INITIATIVE_MANURE = 99;
const s32 INITIATIVE_TOWN_FRUIT = 94;
const s32 INITIATIVE_PALM_FRUIT = 93;
// the drop offset of an object that falls without a unit (GetUnitOfDirection gives none)
const u8 DROP_OFFSET_NONE = 255;
// the fruits of a shaken fruit tree (a palm has one less)
const s32 FRUIT_NUM = 3;
// the units around a shaken tree that are tried
const s32 DROP_UNIT_NUM = 8;

// whether an object can fall from the unit fromX, fromZ to x, z (indoors always)
inline bool CanFallFrom(s32 x, s32 z, s32 fromX, s32 fromZ)
{
    nn::math::VEC3 position;
    FdInfo::ToCenterWorldWithUnitHeight(&position, fromX, fromZ);
    return !stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent()) || Func614B90(x, z, &position);
}
} // namespace

// 0x0059F718 (name is ours)
PlaceType GetPlaceRequestType(s32 handle)
{
    return s_PlaceRequests[handle].type;
}

// 0x0059F730 (name is ours)
s32 FindPendingRequest()
{
    for (s32 i = 0; i < static_cast<s32>(PLACE_REQUEST_NUM); i++) {
        const PlaceRequest& request = s_PlaceRequests[i];
        if (request.state != PlaceRequest::STATE_NONE && IsPendingPlaceType(request.type)) {
            return i;
        }
    }
    return -1;
}

// 0x0059F7A4 (name is ours)
void ConfirmPendingRequest(const SvFgName* item, s32 index, bool isUnknown)
{
    PlaceRequest& request = s_PlaceRequests[index];
    request.param.fallDirection = request.type != PLACE_TYPE_PICK_UP_FULL && isUnknown;
    switch (request.type) {
    case PLACE_TYPE_PICK_UP_FULL:
    case PLACE_TYPE_PICK_UP_SPECIAL_FULL:
    case PLACE_TYPE_PICK_UP_SPECIAL_RELEASE:
        request.type = PLACE_TYPE_THROW;
        request.fg1 = *item;
        request.fg2 = *item;
        if (request.fg1.IsFlower() || request.fg1.IsName(SvFgName::NAME_LUCKY_CLOVERS_00C9)) {
            request.type = PLACE_TYPE_PLANT;
        }
        if (stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent()) && item->IsName(SvFgName::NAME_FOUNTAIN_FIREWORK)) {
            request.fg1 = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_USED_FIREWORK));
        }
        break;
    case PLACE_TYPE_UNKNOWN_15:
    case PLACE_TYPE_UNKNOWN_17:
    case PLACE_TYPE_UNKNOWN_18:
    case PLACE_TYPE_UNKNOWN_19:
    case PLACE_TYPE_UNKNOWN_1A:
        request.type = PLACE_TYPE_BURY;
        request.fg1 = *item;
        request.fg1.ClearBuried();
        request.fg2 = *item;
        request.fg2.ClearBuried();
        ConvertForBury(&request.fg1, &request.fg2);
        break;
    default:
        return;
    }
    Packet packet;
    packet.Assign(request, Packet::CONFIRM_PENDING_REQUEST);
    packet.SendToHost();
}

namespace {
// the initiatives of a planted seed (only on the host) or sapling (fg2 of the request: the planted item); false for other
// items
inline bool AddSeedInitiative(const PlaceRequest& request)
{
    const item::Kind kind = request.fg2.GetKind();
    if (kind == item::KIND_SEED || request.fg2.IsName(SvFgName::NAME_FLOWER_SEED_SPRITE)) {
        if (!netgame::IsMultiplePlayer() || netgame::IsHost()) {
            Func24BA04(&SvGetCurrentPlayer()->initiative, INITIATIVE_SEED, 1);
        }
        return true;
    }
    if (kind == item::KIND_SEEDLING || kind == item::KIND_SEEDLING_BUSH || request.fg2.IsName(SvFgName::NAME_BASIC_SAPLING_SPRITE) ||
        request.fg2.IsName(SvFgName::NAME_CEDAR_SAPLING_SPRITE) || request.fg2.IsName(SvFgName::NAME_PALM_SAPLING_SPRITE)) {
        Func24BA04(&SvGetCurrentPlayer()->initiative, INITIATIVE_SAPLING, 1);
        return true;
    }
    return false;
}
} // namespace

// 0x0059F9C0 (name is ours)
void AddPlantingInitiative(s32 index)
{
    const PlaceRequest& request = s_PlaceRequests[index];
    if (AddSeedInitiative(request)) {
        return;
    }
    const item::Kind planted = request.fg2.GetKind();
    if (planted == item::KIND_MANURE) {
        Func24BA04(&SvGetCurrentPlayer()->initiative, INITIATIVE_MANURE, 1);
        return;
    }
    if (planted != item::KIND_FRUIT && planted != item::KIND_GOOD_FRUIT) {
        return;
    }
    // the fruit of the town (of the town of the player as a guest)
    SvFgName townFruit(GetGardenPlus()->data.townData.townFruit);
    if (netgame::IsMultiplePlayer() && !netgame::IsHost()) {
        townFruit = SvGetCurrentPlayer()->unk_0xA47A.item;
    }
    SvFgName fruit(request.fg2);
    if (planted == item::KIND_GOOD_FRUIT) {
        fruit.ToNormalFruit();
    }
    if (Func2FE704(&townFruit, &fruit)) {
        if (!netgame::IsMultiplePlayer() || netgame::IsHost()) {
            Func24BA04(&SvGetCurrentPlayer()->initiative, INITIATIVE_TOWN_FRUIT, 1);
        }
    } else if (request.fg2.IsName(SvFgName::NAME_COCONUT) || request.fg2.IsName(SvFgName::NAME_BANANA)) {
        if (Func300B7C(request.pos.x, request.pos.y, true) == 1) {
            Func24BA04(&SvGetCurrentPlayer()->initiative, INITIATIVE_PALM_FRUIT, 1);
        }
    }
}

// 0x0059834C (name after libgarden)
PlaceRequest::State GetPlaceRequestState(s32 handle)
{
    PlaceRequest& request = s_PlaceRequests[handle];
    const PlaceRequest::State state = request.state;
    if (state == PlaceRequest::STATE_IN_PROGRESS) {
        return state;
    }
    const PlaceType type = request.type;
    if (state == PlaceRequest::STATE_SUCCESS) {
        if (type == PLACE_TYPE_THROW_SWAP_AFTER_PICK_UP) {
            // the item of the pending pick-up was put down: the pick-up ends without a change
            s32 pending = FindPendingRequest();
            if (pending != -1) {
                PlaceRequest& pendingRequest = s_PlaceRequests[pending];
                Packet packet;
                packet.Assign(pendingRequest, Packet::UNLOCK_UNIT_ALL);
                packet.SendToHost();
                pendingRequest.Clear(pending);
            }
        }
        if (stage::IsKind(stage::KIND_VILLAGE, stage::GetCurrent())) {
            if (type == PLACE_TYPE_PLANT) {
                AddSeedInitiative(request);
            } else if (type == PLACE_TYPE_BURY) {
                AddPlantingInitiative(handle);
            }
        }
        if (IsPendingPlaceType(type)) {
            return state;
        }
    } else if (state != PlaceRequest::STATE_FAIL) {
        return state;
    }
    request.Clear(handle);
    return state;
}

// 0x0059FC7C (name after libgarden)
s32 RequestPlace(PlaceType type, const SvFgName& replace, const SvFgName& fg1, const SvFgName& fg2, u8 x, u8 z, u8 layer, u8 treeFallDir,
                 u8 shakeType, bool fellTree, u8 treeCutCount)
{
    s32 handle;
    for (handle = 0; handle < static_cast<s32>(PLACE_REQUEST_NUM); handle++) {
        if (s_PlaceRequests[handle].state == PlaceRequest::STATE_NONE) {
            break;
        }
    }
    if (handle == static_cast<s32>(PLACE_REQUEST_NUM)) {
        return -1;
    }
    if (handle == -1) {
        return handle;
    }
    PlaceRequest& request = s_PlaceRequests[handle];
    request.replace = replace;
    request.fg1 = fg1;
    request.fg2 = fg2;
    request.param.layer = layer;
    request.param.fallDirection = treeFallDir;
    request.param.treeShakeType = shakeType;
    request.param.isCutdown = fellTree;
    request.pos.x = x;
    request.pos.y = z;
    request.state = PlaceRequest::STATE_IN_PROGRESS;
    request.type = type;
    request.param.cutCounter = treeCutCount;
    // a hit item changes nothing at the unit
    if (request.type == PLACE_TYPE_HIT_ITEM) {
        request.state = PlaceRequest::STATE_SUCCESS;
        return handle;
    }
    Packet packet;
    packet.Assign(request);
    packet.SendToHost();
    return handle;
}

// 0x0059FE44 (name is ours)
bool LoadResources()
{
    if (s_pFgParam == NULL) {
        sead::Heap* heap = sead::HeapMgr::s_pInstance->getCurrentHeap();
        s_pFgParam = new (heap, 4) ssys::ma::FileData();
        if (!s_pFgParam->Load("Fg/Param/Fg.bin", heap, 0)) {
            return false;
        }
    }
    if (s_pFgLut == NULL) {
        sead::Heap* heap = sead::HeapMgr::s_pInstance->getCurrentHeap();
        s_pFgLut = new (heap, 4) g3d::ResourceLoader();
        if (!s_pFgLut->Load(sead::SafeStringBase<char>("Fg/Bcres/FgLut.bcres"), heap, 128)) {
            return false;
        }
    }
    return true;
}

// 0x0059FFC0 | libgarden [tier A]
void fgobj::Packet::Process()
{
    switch (action) {
    case HANDLE_REQUEST:
        HandleRequest();
        break;
    case REQUEST_SUCCEEDED:
        s_PlaceRequests[requestIndex].state = PlaceRequest::STATE_SUCCESS;
        break;
    case REQUEST_FAILED:
        s_PlaceRequests[requestIndex].state = PlaceRequest::STATE_FAIL;
        break;
    case PLACE:
        AddPlaceAction(static_cast<PlaceType>(placeType), static_cast<netgame::PlayerNo>(player), replace, fg1, fg2, pos.x, pos.y, stage,
                       param, dropOffset);
        break;
    case REQUEST_UNLOCK_UNIT:
        UnlockUnit(pos.x, pos.y, stage, static_cast<netgame::PlayerNo>(player));
        break;
    case UNLOCK_UNIT_ALL:
    case REQUEST_DONE_UNLOCK_UNIT:
        UnlockUnit(pos.x, pos.y, stage, netgame::PLAYER_NO_TARGET_ALL);
        break;
    case CONFIRM_PENDING_REQUEST: {
        s32 handle = GetUnitLockHandle(pos.x, pos.y, stage);
        if (handle == -1) {
            break;
        }
        UnitLock& lock = s_UnitLocks[handle];
        lock.type = static_cast<PlaceType>(placeType);
        lock.ResetConfirm();
        Place();
        break;
    }
    case PLACE_AND_SEND:
        action = PLACE;
        Process();
        Send(netgame::PLAYER_NO_TARGET_ALL);
        break;
    case REQUEST_BEGIN_LOCK_UNIT:
        LockUnit(static_cast<PlaceType>(placeType), pos.x, pos.y, stage, false);
        break;
    default:
        break;
    }
}

// 0x0059FF40 (name is ours)
void fgobj::Packet::Place()
{
    if (netgame::IsMultiplePlayer()) {
        if (player != netgame::PLAYER_NO_HOST) {
            action = PLACE_AND_SEND;
            Send(static_cast<netgame::PlayerNo>(player));
            return;
        }
        action = PLACE_AND_SEND;
        Process();
        return;
    }
    action = PLACE;
    Process();
}

// 0x005A0204 (name is ours)
void fgobj::Packet::SetFallItem(const SvFgName* item, u8 x, u8 y, stage::Name packetStage)
{
    player = netgame::GetMyPlayerNo();
    pos.x = x;
    pos.y = y;
    param.layer = 0;
    requestIndex = 0;
    placeType = PLACE_TYPE_FALL_ITEM;
    action = PLACE;
    param.fallDirection = 0;
    param.treeShakeType = 0;
    param.isCutdown = 0;
    param.cutCounter = 0;
    replace = SvFgName::s_Empty;
    fg1 = *item;
    fg2 = SvFgName::s_Empty;
    stage = packetStage;
}

// 0x005A02B0 (name after libgarden)
void fgobj::Packet::Assign(const PlaceRequest& request, Type type)
{
    player = netgame::GetMyPlayerNo();
    requestIndex = request.index;
    action = type;
    placeType = request.type;
    param = request.param;
    pos.x = request.pos.x;
    pos.y = request.pos.y;
    replace = request.replace;
    fg1 = request.fg1;
    fg2 = request.fg2;
    stage = stage::GetCurrent();
}

// 0x005A0364 (name is ours)
void fgobj::Packet::HandleRequest()
{
    if (!CanPlace() || GetUnitLockHandle(pos.x, pos.y, stage) != -1) {
        action = REQUEST_FAILED;
    } else if (LockUnit(static_cast<PlaceType>(placeType), pos.x, pos.y, stage, true) == -1) {
        action = REQUEST_FAILED;
    } else {
        const u32 type = placeType;
        if (type == PLACE_TYPE_SHAKE_TREE || type == PLACE_TYPE_CUT_TREE) {
            switch (param.treeShakeType) {
            case TREE_SHAKE_TYPE_FRUIT:
            case TREE_SHAKE_TYPE_FRUIT_DIE:
                ProcessShakeFruitTree();
                break;
            case TREE_SHAKE_TYPE_ITEM:
                ProcessShakeItemTree();
                break;
            }
        } else if (type == PLACE_TYPE_SHAKE_TREE_BEES || type == PLACE_TYPE_CUT_TREE_BEES) {
            ProcessShakeItemTree();
        }
        Place();
        action = REQUEST_SUCCEEDED;
    }
    if (netgame::IsMultiplePlayer() && netgame::GetMyPlayerNo() != player) {
        Send(static_cast<netgame::PlayerNo>(player));
        return;
    }
    Process();
}

// 0x005A050C (name after libgarden)
void fgobj::Packet::ProcessShakeItemTree()
{
    SvFgName item;
    bool isPicked = false;
    switch (GetFgNameId(&replace)) {
    case SvFgName::NAME_TREE_MONEY_1:
    case SvFgName::NAME_TREE_MONEY_2:
    case SvFgName::NAME_CEDAR_MONEY_1:
    case SvFgName::NAME_CEDAR_MONEY_2:
        if (IsTownState(0)) {
            item = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_200_BELLS));
        } else {
            item = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_100_BELLS));
        }
        item.SetUnkFlag0();
        break;
    case SvFgName::NAME_TREE_ITEM:
    case SvFgName::NAME_CEDAR_ITEM: {
        SvFgNameSet set;
        if (IsTownState(6)) {
            Func6F3458(&set, 5, 8, true, 0);
            Func6F3458(&set, 5, 9, true, 0);
        } else if (IsTownState(7)) {
            Func6F3458(&set, 5, 5, true, 0);
        } else {
            Func6F3458(&set, 5, 1, true, 0);
        }
        item = PickFgName(&set);
        isPicked = true;
        break;
    }
    case SvFgName::NAME_TREE_EGG_0075:
    case SvFgName::NAME_CEDAR_EGG:
        item = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_TREE_EGG_2119));
        break;
    case SvFgName::NAME_TREE_BEES:
    case SvFgName::NAME_CEDAR_BEES:
        item = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_BEEHIVE));
        break;
    default:
        return;
    }
    fg2 = item;
    dropOffset[0] = isPicked ? 0 : DROP_OFFSET_NONE;
    dropOffset[1] = 0;
    dropOffset[2] = 0;
    s32 firstX = pos.x;
    s32 firstZ = pos.y;
    for (s32 i = 0; i < DROP_UNIT_NUM; i++) {
        s32 dx = s_ShakeDropOffsets[0][i];
        s32 dz = s_ShakeDropOffsets[1][i];
        if (!param.fallDirection) {
            dx = -dx;
        }
        s32 x = pos.x + dx;
        s32 z = pos.y + dz;
        if (i == 0) {
            firstX = x;
            firstZ = z;
        }
        if (!Func165460(x, z, stage, 0, false)) {
            continue;
        }
        if (GetUnitLockHandle(x, z, stage) != -1) {
            continue;
        }
        if (i != 0 && !CanFallFrom(x, z, pos.x, pos.y) && !CanFallFrom(x, z, firstX, firstZ)) {
            continue;
        }
        if (LockUnit(PLACE_TYPE_FALL_ITEM, x, z, stage, true) == -1) {
            continue;
        }
        Packet packet;
        packet.SetFallItem(&item, x, z, stage);
        packet.Place();
        dropOffset[0] = EncodeDirection(dx, dz);
        break;
    }
}

// 0x005A0AA0 (name is ours)
void fgobj::Packet::ProcessShakeFruitTree()
{
    s32 num = FRUIT_NUM;
    if (replace.GetFgKind() == SvFgName::FG_KIND_PALM_TREE) {
        num = FRUIT_NUM - 1;
    }
    SvFgName fruit;
    if (!replace.GetFruit(&fruit)) {
        return;
    }
    if (item::IsMoney(fruit)) {
        fruit.SetUnkFlag0();
    }
    fg2 = fruit;
    s32 i;
    for (i = 0; i < num; i++) {
        dropOffset[i] = DROP_OFFSET_NONE;
    }
    for (; i < FRUIT_NUM; i++) {
        dropOffset[i] = 0;
    }
    // the fruit that spoils when the tree dies
    u32 spoiled = GetRandom(FRUIT_NUM);
    s32 firstX[FRUIT_NUM] = {pos.x, pos.x, pos.x};
    s32 firstZ[FRUIT_NUM] = {pos.y, pos.y, pos.y};
    for (s32 j = 0; j < FRUIT_NUM; j++) {
        for (s32 k = 0; k < DROP_UNIT_NUM; k++) {
            if (dropOffset[j] != DROP_OFFSET_NONE) {
                continue;
            }
            s32 dx = s_FruitDropOffsets[0][j][k];
            s32 dz = s_FruitDropOffsets[1][j][k];
            s32 x = pos.x + dx;
            s32 z = pos.y + dz;
            if (k == 0) {
                firstX[j] = x;
                firstZ[j] = z;
            }
            if (!Func165460(x, z, stage, 0, false)) {
                continue;
            }
            if (GetUnitLockHandle(x, z, stage) != -1) {
                continue;
            }
            if (k != 0 && !CanFallFrom(x, z, pos.x, pos.y) && !CanFallFrom(x, z, firstX[j], firstZ[j])) {
                continue;
            }
            if (LockUnit(PLACE_TYPE_FALL_ITEM, x, z, stage, true) == -1) {
                continue;
            }
            SvFgName item(fruit);
            if (replace.HasPerfectFruitAt(j)) {
                item.ToPerfectFruit();
            }
            if (param.treeShakeType == TREE_SHAKE_TYPE_FRUIT_DIE && item.GetKind() == item::KIND_GOOD_FRUIT && static_cast<u32>(j) == spoiled) {
                item.SpoilPerfectFruit();
            }
            Packet packet;
            packet.SetFallItem(&item, x, z, stage);
            packet.Place();
            dropOffset[j] = EncodeDirection(dx, dz);
        }
        if (dropOffset[0] != DROP_OFFSET_NONE && dropOffset[1] != DROP_OFFSET_NONE && dropOffset[2] != DROP_OFFSET_NONE) {
            return;
        }
    }
}

// 0x005A0EF8 (name is ours)
bool fgobj::Packet::CanPlace() const
{
    Field* field;
    if (stage == stage::NAME_CURRENT) {
        field = FdInfo::GetCurrentField();
    } else {
        field = stage::GetField(GetFieldName(stage));
    }
    const SvFgName* fg = field->GetFg(pos.x, pos.y, param.layer);
    if (fg == NULL) {
        return false;
    }
    switch (placeType) {
    case PLACE_TYPE_PICK_UP:
    case PLACE_TYPE_PICK_UP_FULL:
    case PLACE_TYPE_PICK_UP_SPECIAL:
    case PLACE_TYPE_PICK_UP_SPECIAL_FULL:
    case PLACE_TYPE_PICK_UP_SPECIAL_RELEASE:
        if (fg->IsBuried()) {
            return false;
        }
        return Func2FE704(fg, &replace);
    case PLACE_TYPE_PULL:
    case PLACE_TYPE_UNKNOWN_07:
    case PLACE_TYPE_SHAKE_TREE_BEES:
    case PLACE_TYPE_BURY:
    case PLACE_TYPE_CUT_TREE_BEES:
    case PLACE_TYPE_HIT_COIN_STONE:
    case PLACE_TYPE_BREAK_ROCK:
    case PLACE_TYPE_DIG_HOLE:
    case PLACE_TYPE_DIG_STUMP:
    case PLACE_TYPE_FILL:
    case PLACE_TYPE_UNKNOWN_1D:
    case PLACE_TYPE_UNKNOWN_1E:
    case PLACE_TYPE_UNKNOWN_1F:
        return Func2FE704(fg, &replace);
    case PLACE_TYPE_THROW:
    case PLACE_TYPE_PLANT:
    case PLACE_TYPE_DISPLAY:
    case PLACE_TYPE_UNKNOWN_1A:
    case PLACE_TYPE_THROW_SWAP_AFTER_PICK_UP:
        return fg->IsEmpty();
    case PLACE_TYPE_SHAKE_TREE:
        if (!Func2FE704(fg, &replace)) {
            return false;
        }
        if (param.treeShakeType == TREE_SHAKE_TYPE_FRUIT || param.treeShakeType == TREE_SHAKE_TYPE_FRUIT_DIE) {
            return fg->IsFruitRipe();
        }
        return true;
    case PLACE_TYPE_CUT_TREE:
        if (!Func2FE704(fg, &replace)) {
            return false;
        }
        // a tree without fruit or items is cut with the counter of its unit
        if (param.treeShakeType == TREE_SHAKE_TYPE_NONE && GetTreeCutCounter(pos.x, pos.y) != param.cutCounter) {
            return false;
        }
        if (param.treeShakeType == TREE_SHAKE_TYPE_FRUIT || param.treeShakeType == TREE_SHAKE_TYPE_FRUIT_DIE) {
            return fg->IsFruitRipe();
        }
        return true;
    case PLACE_TYPE_DIG_ITEM:
    case PLACE_TYPE_UNKNOWN_15:
    case PLACE_TYPE_UNKNOWN_16:
    case PLACE_TYPE_UNKNOWN_17:
    case PLACE_TYPE_UNKNOWN_18:
    case PLACE_TYPE_UNKNOWN_19:
        if (!Func2FE704(fg, &replace)) {
            return false;
        }
        return fg->IsBuried();
    case PLACE_TYPE_PITFALL:
        if (!fg->IsBuried()) {
            return false;
        }
        return Func2FE704(fg, &replace);
    case PLACE_TYPE_FALL_ITEM:
        return true;
    default:
        return false;
    }
}

// 0x005A1134 | libgarden [tier A]
fgobj::Packet::Packet()
{
    stage = stage::NAME_NONE;
    pos.x = 0;
    pos.y = 0;
    dropOffset[0] = 0;
    dropOffset[1] = 0;
    dropOffset[2] = 0;
    player = 0;
    placeType = 0;
    requestIndex = 0;
    action = TYPE_NONE;
    unknown = 0;
    param.layer = 0;
    param.fallDirection = 0;
    param.treeShakeType = TREE_SHAKE_TYPE_NONE;
    param.isCutdown = 0;
    param.cutCounter = 0;
}
} // namespace fgobj
