#pragma once

#include "decomp.h"
#include "Sv/dSvFgName.h"
#include "fgobj/dPacket.h"
#include "fgobj/fgobj_Api.h"
#include "netgame/netgame_Api.h"
#include "stage/stage_Api.h"

// The requests and actions that change the field objects at a unit (layouts, the arrays and RequestPlace after libgarden
// fgobj/PlaceRequest.hpp and fgobj/PlaceAction.hpp; all other names are ours).
namespace fgobj {
// a change requested by this console (libgarden: fgobj::PlaceRequest)
struct PlaceRequest
{
    enum State : u8
    {
        STATE_NONE,
        STATE_IN_PROGRESS,
        STATE_SUCCESS,
        STATE_FAIL,
    };

    // (inline; a copy for the array constructor)
    DECOMP_EMIT_COPY PlaceRequest() {} // 0x0059FE04

    // the request is free again (index: its own index in s_PlaceRequests; name is ours)
    void Clear(u8 index)
    {
        this->index = index;
        type = PLACE_TYPE_NONE;
        state = STATE_NONE;
    }

    SvFgName replace;   // 0x00
    SvFgName fg1;       // 0x04
    SvFgName fg2;       // 0x08
    Position8 pos;      // 0x0C
    PlaceType type;     // 0x0E
    State state;        // 0x0F
    u8 index;           // 0x10
    PlaceParam param;   // 0x11
};
ASSERT_SIZE(PlaceRequest, 0x12);

const u32 PLACE_REQUEST_NUM = 4;
// (name is ours)
extern PlaceRequest s_PlaceRequests[PLACE_REQUEST_NUM];

// a change that runs at a unit, of any player (libgarden: fgobj::PlaceAction)
struct PlaceAction
{
    enum State : u8
    {
        STATE_NONE,
        STATE_PENDING,
        STATE_FINISHED,
    };

    // (inline; a copy for the array constructor)
    DECOMP_EMIT_COPY PlaceAction() {} // 0x005A3074

    // the field object after the action at the unit (the unit gets fg1; for some types only with parameters)
    DECOMP_NOINLINE void PlaceResult(); // 0x005A1A44
    // the player planted planted (counts for the census and the tours)
    DECOMP_NOINLINE void OnPlanted(const SvFgName* planted); // 0x005A1C5C
    // the action starts: the objects and effects of its type
    DECOMP_NOINLINE void Start(); // 0x005A1D9C
    // the actions of the types that wait for the player (throw, bury, plant, display) start
    DECOMP_NOINLINE void Update(); // 0x005A22F4
    // the item of the unit (an empty unit: a hole) is buried (deep: ObjectBury::FINISH_MODE_TAKE_UP); false without an object
    DECOMP_NOINLINE bool StartBury(bool deep, u8 animation); // 0x005A2688
    // the tree is shaken or cut (with the objects that fall out of it); false without an object
    DECOMP_NOINLINE bool StartSwing(bool isCut); // 0x005A2774
    // a rock that gives money is hit / a rock is broken
    DECOMP_NOINLINE bool HitCoinStone(); // 0x005A2930
    DECOMP_NOINLINE bool BreakRock(); // 0x005A2C24
    // the action ends at the other consoles: the unlock request of the unit
    DECOMP_NOINLINE void SendUnlockRequest(); // 0x005A2F20

    SvFgName replace;       // 0x00
    SvFgName fg1;           // 0x04
    SvFgName fg2;           // 0x08
    PlaceParam param;       // 0x0C
    netgame::PlayerNo player; // 0x0D
    Position8 pos;          // 0x0E
    PlaceType placeType;    // 0x10
    State state;            // 0x11
    u8 dropOffset[3];       // 0x12
    stage::Name stage;      // 0x15
};
ASSERT_SIZE(PlaceAction, 0x16);

const u32 PLACE_ACTION_NUM = 32;
// (name after libgarden)
extern PlaceAction s_PlaceActions[PLACE_ACTION_NUM];

// the rock that gives money while it is hit (one at a time; names are ours)
struct CoinStone
{
    s16 timer;      // 0x0 the frames until the rock is spent (CoinStone::Update)
    u8 x;           // 0x2
    u8 z;           // 0x3
    u8 hits;        // 0x4 at most 8
    bool isStarted; // 0x5
    bool isLucky;   // 0x6 with the silver shovel: ores instead of money (20 % of the rocks)
};
ASSERT_SIZE(CoinStone, 0x8);
// (name is ours)
extern CoinStone s_CoinStone;
// the items of a coin stone by the number of hits and the level (money) / of a lucky one with their weights (ores)
extern const u16 s_CoinStoneItems[10];
extern const u16 s_LuckyStoneItems[6];
extern const s32 s_LuckyStoneWeights[6];
// whether the rock at x, z gives something for one more hit (the first hit starts it)
DECOMP_NOINLINE bool HitCoinStone(s32 x, s32 z); // 0x005A334C
// the time of the rock runs out: the rock at its unit is spent (SvFgName::ClearUnique)
DECOMP_NOINLINE void UpdateCoinStone(); // 0x005A3434
// the rock is spent now
DECOMP_NOINLINE void FinishCoinStone(); // 0x005A34C4

// (names are ours) the type of the request of handle
PlaceType GetPlaceRequestType(s32 handle); // 0x0059F718
// the request in progress of a pending type (IsPendingPlaceType; -1: none)
s32 FindPendingRequest(); // 0x0059F730
// the pending request index is confirmed with item: a pick-up becomes a throw (or a plant of a flower), a dig a bury;
// isUnknown goes to PlaceParam::fallDirection (not for PLACE_TYPE_PICK_UP_FULL)
void ConfirmPendingRequest(const SvFgName* item, s32 index, bool isUnknown); // 0x0059F7A4
// the initiatives of the current player for the item planted by the request index (seeds, saplings, manure, fruit)
void AddPlantingInitiative(s32 index); // 0x0059F9C0
// a request of this console for a change at the unit x, z (handle, -1: none free; libgarden)
s32 RequestPlace(PlaceType type, const SvFgName& replace, const SvFgName& fg1, const SvFgName& fg2, u8 x, u8 z, u8 layer, u8 treeFallDir,
                 u8 shakeType, bool fellTree, u8 treeCutCount); // 0x0059FC7C
// the state of the request of handle (libgarden); a finished request is freed (not the pending types), a successful
// plant or bury counts for the initiatives in the village, a successful THROW_SWAP_AFTER_PICK_UP cancels the pending
// request
PlaceRequest::State GetPlaceRequestState(s32 handle); // 0x0059834C

// (names are ours) what the player does with a unit of the field (RequestUnitAction)
enum UnitAction
{
    UNIT_ACTION_SHAKE,  // with the hands: a grown tree is shaken
    UNIT_ACTION_AXE,    // RequestCut
    UNIT_ACTION_SHOVEL, // RequestDig
};
// the pending request ends without a change: the units are unlocked for all
void CancelPendingRequest(); // 0x00596E1C
// the pending request is confirmed with its own item (with PLACE_TYPE_UNKNOWN_1A with the item of the first place of the
// pockets, which is taken out) / with item and freed (-1: none)
s32 FinishPendingRequest(); // 0x00596FD4
s32 FinishPendingRequest(const SvFgName* item); // 0x00597548
// a hole at the unit x, z of stage is filled (on the current stage with an ObjectBury)
void FillHole(s32 x, s32 z, stage::Name stage); // 0x00596ED8
// a request for action at the unit x, z of the field (-1: none; for the player alone a running action there ends first)
s32 RequestUnitAction(s32 x, s32 z, UnitAction action); // 0x00597154
// the requests of the axe and the shovel for the field object fg at the unit x, z (not decompiled yet)
s32 RequestCut(const SvFgName* fg, s32 x, s32 z); // 0x005993A8
s32 RequestDig(const SvFgName* fg, s32 x, s32 z); // 0x0059990C
// the unit of a buried pitfall seed at x, z is locked for PLACE_TYPE_UNKNOWN_24 (only for the player alone)
bool LockPitfall(s32 x, s32 z); // 0x0059967C
// a grown tree of a size of at least 2 at the unit x, z swings (ObjectSwing::KIND_SHAKE_L_WAIT) / the field object at the
// unit x, z swings a little (a tree or bush of a size of at least 1, a rafflesia sways; the callers test IsLargeFgAt
// first)
bool SwingTreeAt(s32 x, s32 z); // 0x0059978C
bool SwingFgAt(s32 x, s32 z); // 0x00599808
// the ores of a broken rock (SvFgName::GetOreIndex)
extern const u16 s_OreRockItems[6];
// a request to pick up the item at the unit x, z (indoors / outdoors; not decompiled yet)
s32 RequestPickUp(s32 x, s32 z); // 0x00597440
s32 RequestPickUpIndoor(s32 x, s32 z); // 0x00599E44
s32 RequestPickUpOutdoor(s32 x, s32 z); // 0x0059A09C
// the field object at the unit x, z of the field sways (not FG_KIND_INVALID) / the one at position, and a flower there gets
// the effect of a flower that was not trampled
void SwayFgAt(s32 x, s32 z); // 0x00599DE4
void SwayFgAtPosition(const nn::math::VEC3* position); // 0x0059A02C
// all requests are freed and the coin stone ends
void ResetPlaceRequests(); // 0x0059A26C
// item falls at position (a dropped item): it lands on a free unit around it that is not higher than position + 8
// (PLACE_TYPE_FALL_ITEM, placed at once), else PlacePresent with state 2 (4 on the special ground of IsPart95E93ENonZero)
bool DropItem(const nn::math::VEC3* position, const SvFgName* item); // 0x0059A2B4
// the units around the player of DropItem ([0]: x, [1]: z)
extern const s8 s_DropUnitOffsets[2][PLACE_UNIT_NUM];
// a request to put item down after a pick-up (PLACE_TYPE_THROW_SWAP_AFTER_PICK_UP) at a unit next to the player
s32 RequestThrowSwap(const SvFgName* item); // 0x0059A4A0
// with a shovel in the hands: a hole in front of the player (the three units in front; false: none)
bool FindHoleAroundPlayer(s32* x, s32* z); // 0x0059A58C
// whether there is a unit around the player where an item can be put
bool CanPutItemAroundPlayer(); // 0x0059A754
// the bury action (or the fill of a hole) at the unit x, z of the current stage starts (ObjectBury::ANIMATION_PRESS); without
// an object it ends at once
void StartBuryActionEntry(const void* unused, s32 x, s32 z); // 0x0059A888
void StartBuryAction(s32 x, s32 z); // 0x0059A894
// a request to bury item at the unit x, z (a hole becomes the buried item)
s32 RequestBury(const SvFgName& item, s32 x, s32 z); // 0x005974A8
// the lucky clover of the field becomes the item of the pockets (false for other items)
bool ConvertLuckyClover(SvFgName* item); // 0x00597598
// the field object of the unit x, z of the current stage is removed (its lock of PLACE_TYPE_UNKNOWN_23/24 is freed)
void RemoveFgOfUnit(s32 x, s32 z); // 0x005975DC
// a request to plant / to put down item at a unit next to the player (-1: no unit)
s32 RequestPlant(const SvFgName* item); // 0x005976B4
s32 RequestThrowEntry(const void* unused, const SvFgName* item); // 0x00597744
s32 RequestThrow(const SvFgName* item); // 0x0059774C
// the grown tree at the unit x, z swings without a request (an action of PLACE_TYPE_UNKNOWN_23)
void SwingTree(s32 x, s32 z); // 0x00597878
// the stump of the tree item (ParamFgEntry::unk_0x0); with 5 % (always with the silver axe) one of the six special stumps
bool GetStumpOfTree(SvFgName* item); // 0x0059797C
// the unit x, z becomes a hole (a growing ObjectScale)
void PlaceHoleEntry(const void* unused, s32 x, s32 z); // 0x005980B0
void PlaceHole(s32 x, s32 z); // 0x005980BC
// requests for a field object at the unit x, z (PLACE_TYPE_UNKNOWN_1E, a hole PLACE_TYPE_UNKNOWN_1F / PLACE_TYPE_PITFALL
// for a buried pitfall seed / PLACE_TYPE_UNKNOWN_07 for a hole or a pattern / PLACE_TYPE_UNKNOWN_1D)
s32 RequestUnknown1E(s32 x, s32 z); // 0x00598160
s32 RequestPitfall(s32 x, s32 z); // 0x00598718
s32 RequestUnknown07(s32 x, s32 z); // 0x00598D00
s32 RequestUnknown1D(s32 x, s32 z); // 0x00598E94
// the action at the unit x, z of the current stage starts
void StartPlaceActionAtEntry(const void* unused, s32 x, s32 z); // 0x0059826C
void StartPlaceActionAt(s32 x, s32 z); // 0x00598278
// item is put at the unit x, z of layer on the current stage
void PlaceItemOnCurrentStage(s32 x, s32 z, u8 layer, const SvFgName& item); // 0x005982C0
// the layer at which an item can be put at the unit x, z (0: the ground, 1: Func4E3D48; false: none)
bool GetPlaceableLayer(s32 x, s32 z, u8* layer); // 0x005982F4
// item is planted at the unit x, z without a request (an action of PLACE_TYPE_UNKNOWN_23)
void PlantDirect(const SvFgName* item, s32 x, s32 z); // 0x005985D8
// the unit x, z of the current stage is locked for PLACE_TYPE_UNKNOWN_23 (only for the player alone)
bool LockUnitOfType23(u8 x, u8 z); // 0x005986B0
// what a shaken tree gives: out is the tree after it (libgarden)
TreeShakeType GetShake(SvFgName& out, const SvFgName& shaken); // 0x00598810
// a grown tree or bush of a size of at least 1 or a rafflesia at the unit x, z
bool IsLargeFgAt(s32 x, s32 z); // 0x00598BA8
// the position of the first rafflesia of the current field
bool FindRafflesiaPosition(nn::math::VEC3* out); // 0x00598C4C
// a request to display the pattern design of the player of saveNo (PLAYER_NO_MY_NO: this player) next to the player
s32 RequestDisplayPattern(u32 design, u32 saveNo); // 0x00598DC0

// the directions around a shaken tree where its item / its fruits (one row per fruit) fall, in the order they are tried
// ([0]: x, [1]: z; the x offsets go to the other side when PlaceParam::fallDirection is not set)
extern const s32 s_ShakeDropOffsets[2][8];
extern const s8 s_FruitDropOffsets[2][3][8];
// the offset dx, dz as a direction of GetUnitOfDirection
inline u8 EncodeDirection(s32 dx, s32 dz)
{
    return ((dx + 4) & 7) | (((dz + 4) & 7) << 4);
}

// the action at the unit x, y of stage (-1: none)
DECOMP_NOINLINE s32 FindPlaceAction(s32 x, s32 y, stage::Name stage); // 0x005A19E0
// a new action (an action at the same unit ends first; dropOffset NULL: none); it starts at once on the current stage
// when the player is there
DECOMP_NOINLINE void AddPlaceAction(PlaceType type, netgame::PlayerNo player, const SvFgName& replace, const SvFgName& fg1, const SvFgName& fg2,
                    u8 x, u8 y, stage::Name stage, PlaceParam param, const u8* dropOffset); // 0x005A2D78
// the tree cut counters of the units of the town (2 bits per unit, 80 x 64 units from the unit 16, 16)
const u32 TREE_CUT_COUNTER_WIDTH = 80;
const u32 TREE_CUT_COUNTER_ORIGIN = 16;
extern u32 s_TreeCutCounters[320];
// the cut counter of the tree at the unit x, z
inline u32 GetTreeCutCounter(u8 x, u8 z)
{
    s32 index = x + z * TREE_CUT_COUNTER_WIDTH - (TREE_CUT_COUNTER_ORIGIN + TREE_CUT_COUNTER_ORIGIN * TREE_CUT_COUNTER_WIDTH);
    u32 shift = (index & 15) << 1;
    return (s_TreeCutCounters[index >> 4] & (3u << shift)) >> shift;
}
inline void SetTreeCutCounter(u8 x, u8 z, u32 counter)
{
    s32 index = x + z * TREE_CUT_COUNTER_WIDTH - (TREE_CUT_COUNTER_ORIGIN + TREE_CUT_COUNTER_ORIGIN * TREE_CUT_COUNTER_WIDTH);
    u32 shift = (index & 15) << 1;
    u32* word = &s_TreeCutCounters[index >> 4];
    *word = (*word & ~(3u << shift)) | (counter << shift);
}
// the counters are cleared and s_LastCutUnitX/Z reset (-1)
void ClearTreeCutCounters(); // 0x005A30B4
// whether x, z is the unit s_LastCutUnitX/Z
bool IsLastCutUnit(s32 x, s32 z); // 0x005A30E0
// the unit of the last tree that was cut down (PlaceParam::isCutdown; -1: none)
extern s32 s_LastCutUnitX;
extern s32 s_LastCutUnitZ;
// the actions that wait start (Update)
void UpdatePlaceActions(); // 0x005A2EE8
// all actions end now
void FinishPlaceActions(); // 0x005A301C
} // namespace fgobj
