#pragma once

#include "decomp.h"
#include "netgame/netgame_Api.h"
#include "stage/stage_Api.h"
#include "nn/math/math_Vector3.h"
#include "math/dAngle16.h"

// The field objects: the items on the field and what happens to them (picking up, shaking, cutting, digging, ...).
// Enumerators, structures and the names of the free functions after libgarden fgobj/*.hpp; other names are ours.
class SvFgName;
namespace fgobj {
class ObjectBase;
} // namespace fgobj

namespace g3d {
class ResourceLoader;
} // namespace g3d
namespace ssys {
namespace ma {
class FileData;
} // namespace ma
} // namespace ssys

namespace fgobj {

// what is done at a unit (libgarden: fgobj::PlaceType; the values without a name there are numbers)
enum PlaceType : u8
{
    PLACE_TYPE_NONE = 0x00,
    PLACE_TYPE_PICK_UP = 0x01,
    PLACE_TYPE_PICK_UP_FULL = 0x02,
    PLACE_TYPE_PICK_UP_SPECIAL = 0x03,
    PLACE_TYPE_PICK_UP_SPECIAL_FULL = 0x04,
    PLACE_TYPE_PICK_UP_SPECIAL_RELEASE = 0x05,
    PLACE_TYPE_PULL = 0x06,
    PLACE_TYPE_UNKNOWN_07 = 0x07, // a hole or a pattern (RequestUnknown07; the object is pressed away)
    PLACE_TYPE_SHAKE_TREE = 0x08,
    PLACE_TYPE_SHAKE_TREE_BEES = 0x09,
    PLACE_TYPE_THROW = 0x0A,
    PLACE_TYPE_BURY = 0x0B,
    PLACE_TYPE_PLANT = 0x0C,
    PLACE_TYPE_DISPLAY = 0x0D,
    PLACE_TYPE_CUT_TREE = 0x0E,
    PLACE_TYPE_CUT_TREE_BEES = 0x0F,
    PLACE_TYPE_HIT_ITEM = 0x10,
    PLACE_TYPE_HIT_COIN_STONE = 0x11,
    PLACE_TYPE_BREAK_ROCK = 0x12,
    PLACE_TYPE_DIG_HOLE = 0x13,
    PLACE_TYPE_DIG_ITEM = 0x14,
    PLACE_TYPE_UNKNOWN_15 = 0x15,
    PLACE_TYPE_UNKNOWN_16 = 0x16,
    PLACE_TYPE_UNKNOWN_17 = 0x17,
    PLACE_TYPE_UNKNOWN_18 = 0x18,
    PLACE_TYPE_UNKNOWN_19 = 0x19,
    PLACE_TYPE_UNKNOWN_1A = 0x1A,
    PLACE_TYPE_DIG_STUMP = 0x1B,
    PLACE_TYPE_FILL = 0x1C,
    PLACE_TYPE_UNKNOWN_1D = 0x1D, // stumps and other field objects (RequestUnknown1D)
    PLACE_TYPE_UNKNOWN_1E = 0x1E, // a field object (RequestUnknown1E)
    PLACE_TYPE_UNKNOWN_1F = 0x1F, // a hole (RequestUnknown1E)
    PLACE_TYPE_PITFALL = 0x20,    // a buried pitfall seed (RequestPitfall; name is ours)
    PLACE_TYPE_FALL_ITEM = 0x21,
    PLACE_TYPE_THROW_SWAP_AFTER_PICK_UP = 0x22,
    PLACE_TYPE_UNKNOWN_23 = 0x23,
    PLACE_TYPE_UNKNOWN_24 = 0x24,
};

// a unit position on the field (libgarden)
struct Position8 {
    u8 x;
    u8 y;
};

// the types whose request waits for a confirmation of the requesting console (the pick-ups with full pockets and the
// digs of items; UnitLock::CONFIRM_PENDING, ConfirmPendingRequest; name is ours)
inline bool IsPendingPlaceType(u32 type)
{
    switch (type) {
    case PLACE_TYPE_PICK_UP_FULL:
    case PLACE_TYPE_PICK_UP_SPECIAL_FULL:
    case PLACE_TYPE_PICK_UP_SPECIAL_RELEASE:
    case PLACE_TYPE_UNKNOWN_15:
    case PLACE_TYPE_UNKNOWN_17:
    case PLACE_TYPE_UNKNOWN_18:
    case PLACE_TYPE_UNKNOWN_19:
    case PLACE_TYPE_UNKNOWN_1A:
        return true;
    default:
        return false;
    }
}

// a locked unit while something happens at it (libgarden: fgobj::UnitLock); free if the stage is NAME_NONE
struct UnitLock {
    // the bits of m_Confirm (libgarden: unlockConfirm : 4, pending : 1)
    enum
    {
        CONFIRM_PLAYERS_MASK = 0xF, // the players that still have to confirm the unlock (bit 0 = this console)
        CONFIRM_PENDING = 0x10,
    };

    // every player in the game has to confirm the unlock (bit 0 for this console), the pending types also the pending
    // bit (name is ours)
    void ResetConfirm()
    {
        confirm = 1;
        for (u32 i = netgame::PLAYER_NO_1; i < netgame::PLAYER_NO_MAX; i++) {
            if (netgame::ExistsPlayer(static_cast<netgame::PlayerNo>(i))) {
                confirm |= 1 << i;
            }
        }
        if (IsPendingPlaceType(type)) {
            confirm |= CONFIRM_PENDING;
        }
    }

    /* 0x0 */ Position8 pos;
    /* 0x2 */ stage::Name stage;
    /* 0x3 */ u8 confirm;
    /* 0x4 */ PlaceType type;
};
ASSERT_SIZE(UnitLock, 0x5);

const u32 UNIT_LOCK_NUM = 32;
// 0x009B51C4
extern UnitLock s_UnitLocks[UNIT_LOCK_NUM];

// locks a unit and tells the other players (as host); the handle is the index, -1 if all locks are used
s32 LockUnit(PlaceType type, u8 x, u8 y, stage::Name stage, bool netgameSend); // 0x005A13C4 | libgarden [tier A]
// the lock of a unit (-1 without)
s32 GetUnitLockHandle(u8 x, u8 y, stage::Name stage); // 0x005A11BC | libgarden [tier A]
// one player confirmed the unlock (PLAYER_NO_TARGET_ALL: all); the last one frees the lock and tells the others (as
// host)
void UnlockUnit(u8 x, u8 y, stage::Name stage, netgame::PlayerNo player); // 0x005A1278 | libgarden [tier A]
// frees the lock of a unit of the current stage if its type is UNKNOWN_23 or UNKNOWN_24
void UnlockUnitOfType23Or24(u8 x, u8 y); // 0x005A1228
// all locks of UNKNOWN_23 are freed, those of UNKNOWN_24 also empty their unit (when the Proc ends; name is ours)
void FreeUnitLocksOfType23Or24(); // 0x005A15A0

// the lookup tables of the field objects (Fg/Bcres/FgLut.bcres; libgarden)
g3d::ResourceLoader* GetFgLut(); // 0x00595874 | libgarden [tier A]
// the parameters of the field objects (Fg/Param/Fg.bin: item::ParamFgEntry by the index of the field object) and the
// lookup tables, loaded on the current heap (false if a file fails; names are ours)
bool LoadResources(); // 0x0059FE44
extern ssys::ma::FileData* s_pFgParam;
extern g3d::ResourceLoader* s_pFgLut;
// what the unit is searched for (libgarden)
enum FindUnitForPlayerCheck : u8
{
    FIND_UNIT_FOR_PLAYER_CHECK_PUT,
    FIND_UNIT_FOR_PLAYER_CHECK_PLANT,
    FIND_UNIT_FOR_PLAYER_CHECK_DISPLAY,
};
// a unit around the player where an item can be put (layer 0: the ground, 1: on something indoors, Func4E3D48); the units
// are tried in the order of s_PlaceUnitOrders for the direction of the player (placeInFront: the unit in front first)
bool FindPlaceableUnitAroundPlayer(s32& x, s32& z, u8& layer, FindUnitForPlayerCheck check, bool placeInFront); // 0x005990AC | libgarden [tier A]
// (names are ours) the angles of the eight directions of the player, the units around the player ([0]: x, [1]: z; 0 is
// the unit of the player) and their order for each direction (the unit in front is the second)
const u32 PLACE_UNIT_NUM = 9;
extern const u16 s_PlayerDirectionAngles[8];
extern const s8 s_PlaceUnitOffsets[2][PLACE_UNIT_NUM];
extern const s32 s_PlaceUnitOrders[8][PLACE_UNIT_NUM];

// a model of the graphics file of the field objects (NULL indoors or without the Proc; name is ours)
const void* GetFgModel(const char* name); // 0x0059A99C

// the unit in direction from x, z: the low three bits give the step in x, bits 4..6 the one in z (index into
// s_DirectionOffsets: -3..3 for 1..7); false for 0 in either
DECOMP_NOINLINE bool GetUnitOfDirection(s32* outX, s32* outZ, s32 x, s32 z, u8 direction); // 0x00599D7C
// the steps of GetUnitOfDirection
extern const s32 s_DirectionOffsets[8];

// whether item is a grown tree (SvFgName::IsGrownTree) of a size (ParamFgEntry::size) of at least 2
DECOMP_NOINLINE bool IsGrownTreeOfSize2(const SvFgName* item); // 0x0059A950
// the skeletal animation "<name><suffix of animKind>" of FgObjAnim.bcres (NULL indoors, without the Proc or without it;
// the suffixes are ObjectSwing::s_AnimationSuffixes)
const void* GetFgSkeletalAnim(const char* name, u8 animKind); // 0x0059C180
// the model "<name>_t" (isSpecial: "<name>_s_t") of FgObjAnim.bcres (NULL indoors or without the Proc)
const void* GetFgAnimModel(const char* name, bool isSpecial); // 0x0059C360

// a thrown object (ObjectThrow, outdoors for NAME_FOUNTAIN_FIREWORK an ObjectHanabi) of item from from onto the unit x, z;
// false without the Proc, outside the field or without a free heap
DECOMP_NOINLINE bool PlaceThrow(const SvFgName& item, s32 x, s32 z, bool useObjectHeight, f32 scale, const nn::math::VEC3* from); // 0x0059BB68
// an item shown at position for the one of id: a running object that takes it (ObjectExe::Refresh) keeps it, else a new
// ObjectOther (the arguments of ObjectOther::Create); false without the Proc or without a free heap (name is ours)
DECOMP_NOINLINE bool PlaceOther(const SvFgName& item, const nn::math::VEC3* position, const nn::math::VEC3* scale, s32 id, f32 shadowSize,
                                bool faceCameraIndoors, bool hasShadow, s16 rotateY, bool withFlag40000); // 0x0059B598
// an item that falls from position onto the unit x, z (ObjectPresent of state; false without the Proc, outside the field or
// without a free heap; name is ours)
DECOMP_NOINLINE bool PlacePresent(const SvFgName& item, s32 x, s32 z, const nn::math::VEC3* position, u8 state); // 0x0059C2B0
// a wobble of angles (a swing over 8 or 12 frames, started by s_Unk94FE61): x and y stay 0, z swings with the amplitude
// s_Unk94FE64 and s_Unk94FE7C follows a sine of s_Unk94FE78; false when it is not running (meaning unknown; name is ours)
bool UpdateWobble(s16* x, s16* y, s16* z); // 0x0059CE4C
// functions of ModuleFtr with the Proc indoors (through import thunks; -1 / 0 without the Proc; meaning unknown; names
// after the addresses)
s32 Func59C590(s32 a, s32 b); // 0x0059C590
s32 Func59D290(s32 a, s32 b, s32 c); // 0x0059D290
// (set to 1; the Proc clears it when it draws; meaning unknown; name is ours)
void SetUnk94FE5C(); // 0x0059C34C
// an item thrown into its unit (ObjectThrowHole)
DECOMP_NOINLINE bool PlaceThrowHole(const SvFgName& item, s32 x, s32 z, const nn::math::VEC3* from, f32 scale); // 0x0059C6A0
// the position of item at the unit x, z as the objects have it (the centre of the unit, with useObjectHeight the height of
// Func4EA670; on some stages 16 further in x, outdoors the weeds and shells scattered)
DECOMP_NOINLINE void GetObjectPosition(nn::math::VEC3* out, const SvFgName& item, s32 x, s32 z, bool useObjectHeight); // 0x0059B494
// the angle from the player to the position of item at the unit x, z (0 without the actor of the player)
DECOMP_NOINLINE void GetAngleFromPlayer(math::Angle16* out, const SvFgName& item, s32 x, s32 z, netgame::PlayerNo player); // 0x0059CB78
// a stump (isBush: the bush of ObjectStumpAnim) dug out by player at the unit x, z; the running objects there end
DECOMP_NOINLINE bool PlaceStump(const SvFgName& item, s32 x, s32 z, netgame::PlayerNo player, bool isBush); // 0x0059C468
// an ObjectHoneycomb that falls from the left or right of the tree at x, z and then falls in fallDirection
DECOMP_NOINLINE bool PlaceHoneycomb(const SvFgName& item, s32 x, s32 z, u8 fallDirection, bool isRight); // 0x0059C5BC
// an item that falls from the left or right of the tree at x, z: an ObjectFruit (items with an icon), else an ObjectLeaf
DECOMP_NOINLINE bool PlaceFall(const SvFgName& item, s32 x, s32 z, u8 fallDirection, bool isRight); // 0x0059D6CC
// a rock that this player hit (ObjectCoinStone in the direction from the player)
DECOMP_NOINLINE bool PlaceCoinStone(const SvFgName& item, s32 x, s32 z); // 0x0059D80C
// a hole that is dug (ObjectDig with its cylinder in the collision world)
DECOMP_NOINLINE bool PlaceDig(const SvFgName& item, s32 x, s32 z); // 0x0059A9D0
// an item that is buried (ObjectBury)
DECOMP_NOINLINE bool PlaceBury(const SvFgName& item, s32 x, s32 z, u8 finishMode, u8 animation); // 0x0059ADC0
// a tree or bush that is shaken or cut (ObjectSwing of kind); the field objects and running objects at the unit end,
// an ObjectTimer with after (empty: the field object of the parameters) stays while the tree moves, a decorated cedar
// gets its ObjectDecoSwing, and the objects hanging in the tree fall in the directions of dropOffset (indices 0..2)
DECOMP_NOINLINE bool PlaceSwing(const SvFgName& item, const SvFgName& after, s32 x, s32 z, const u8* dropOffset, u8 kind, bool isFallAll); // 0x0059B77C
// the objects of the unit x, z are made new: with the bit of m_FieldBit2 it only moves to m_FieldBit, else the field
// objects there are removed (removeRunning: also the running objects) and the field bit is reset
DECOMP_NOINLINE void RefreshUnit(s32 x, s32 z, bool removeRunning); // 0x0059F144
// the item of a dig that is buried: item becomes its field object (buried when it has none) and sprite the object shown
// while it is buried (seeds, saplings: their sprites); true only for a flower (both unchanged items otherwise)
bool ConvertForBury(SvFgName* item, SvFgName* sprite); // 0x0059A788
// the unit x, z is marked as changed (FieldBit of the Proc)
DECOMP_NOINLINE void SetFieldBit(s32 x, s32 z); // 0x0059AEDC

// the place request at the unit x, z of stage ends (written with the place requests)
DECOMP_NOINLINE void FinishPlaceRequest(s32 x, s32 z, stage::Name stage); // 0x005A2D34

// the bury action at the unit x, z of the current stage ends with its object: an ObjectScale of a buried item, else an
// ObjectPlant of the planted one (PlaceAction::OnPlanted; ObjectBury::FINISH_MODE_TAKE_UP)
void PlaceBuryResult(s32 x, s32 z); // 0x00596D80
// (names are ours; not decompiled yet) the field object at the unit x, z sways for 10 frames (ObjectBase::m_Unk0x11C; only
// objects with ObjectBase::FLAG_UNK_8, the flowers and plants) / a bush / a tree swings without a request (ObjectSwing of
// kind; false with more than four running particle handles or when an object at the unit refuses)
void SwayFieldObject(s32 x, s32 z); // 0x0059D41C
bool SwingBushObject(const SvFgName* fg, s32 x, s32 z); // 0x0059D05C
bool SwingTreeObject(const SvFgName* fg, s32 x, s32 z, u8 kind); // 0x0059D18C

// (names are ours; callers not found, maybe in modules) arrays of 100 objects keyed by ObjectBase::m_Unk0x114: the
// object of id is taken out (NULL: none) / object goes to the first free place (false: full)
const u32 OBJECT_ARRAY_NUM = 100;
ObjectBase* TakeObjectOfArray(ObjectBase** objects, s32 id); // 0x00594E64
bool AddObjectToArray(ObjectBase** objects, ObjectBase* object); // 0x00594EA8
// the design textures are prepared (not decompiled yet)
struct MyDesignTextures;
void InitializeMyDesignTextures(MyDesignTextures* textures); // 0x005A3238
// the field bit of the unit x, z of the Proc is reset (its objects are created again; name is ours)
void ResetFieldBit(u8 x, u8 z); // 0x0059C0D8
// the entry of the model table of the field object item outdoors (NULL indoors, without the Proc, the table or a model;
// name is ours)
struct FgModel;
FgModel* GetFgModelOf(const SvFgName* item); // 0x0059C104
} // namespace fgobj
