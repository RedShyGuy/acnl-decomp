#pragma once

#include "decomp.h"
#include "math/dAngle16.h"
#include "nn/math/math_Vector3.h"
#include "nw/anim/res/anim_ResAnim.h"

namespace sead {
class Heap;
} // namespace sead

namespace nw {
namespace os {
class IAllocator;
} // namespace os
} // namespace nw

// Game functions and objects of the animation table of the humans that the face control uses (the class of the table
// is not written yet; all names are ours, FuncXXXXXX after the address while the purpose is unknown).

// the animations of an id of the table (HumanAnimName)
struct HumanAnimEntry
{
    nw::anim::res::ResAnim anim0; // 0x0 (the skeletal animation)
    nw::anim::res::ResAnim anim1; // 0x4 (the material animation of the first face slot)
    nw::anim::res::ResAnim anim2; // 0x8 (the material animation of the second face slot)
    nw::anim::res::ResAnim anim3; // 0xC (the material animation of a tool; ToolBank)
};

// the animation table (object 0x00A15B5C; placeholder)
struct HumanAnimTable
{
    u32 _unknown;
};
extern HumanAnimTable g_HumanAnimTable;
// (game code; names after the addresses) loading of the animations of the table for the stage (true when loaded) and
// their release (ScStage)
bool Func6E59D0(HumanAnimTable* table, sead::Heap* heap); // 0x006E59D0
void Func6E57E4(HumanAnimTable* table); // 0x006E57E4

// the entry of id (an empty one for an invalid id)
HumanAnimEntry* Func770970(HumanAnimTable* table, u32 id); // 0x00770970
// the information of an animation id (HumanAnimName): the animations of the tools in the hand for it
// (HumanAnimName, 0: none) and flags
struct HumanAnimInfo
{
    u16 toolAnimNet;      // 0x00 (ToolBank::TOOL_KIND_NET)
    u16 toolAnimFishingRod;     // 0x02 (ToolBank::TOOL_KIND_FISHING_ROD)
    u16 toolAnimSlingshot;    // 0x04 (ToolBank::TOOL_KIND_SLINGSHOT)
    u16 toolAnimShovel;    // 0x06 (ToolBank::TOOL_KIND_SHOVEL)
    u16 toolAnimFryingPan;  // 0x08 (ToolBank::TOOL_KIND_FRYING_PAN)
    u16 toolAnimFryingPan1; // 0x0A (ToolBank::TOOL_KIND_FRYING_PAN with the value 1 of PlayToolAnim)
    u16 toolAnimToyHammer;   // 0x0C (ToolBank::TOOL_KIND_TOY_HAMMER)
    u8 freeArms;          // 0x0E the arms an arm animation may take during this animation (HumanModel::ArmFlag)
    u8 armFlags;          // 0x0F the arms of this animation as an arm animation (low four bits)
    u8 flags;             // 0x10 (bit 0: PlayerModel may play it in slot 1; bit 3: on the curve of the world; bit 4: a
                          // one-time face animation stays at its end; bit 5: loop; bit 6: a tool animation without a
                          // tool; meanings of the others unknown)
    u8 flags2;            // 0x11 (bit 7: PlayerModel may play animations in slot 1 during this one)
};
// the information of id
const HumanAnimInfo* Func6E6268(u32 id); // 0x006E6268
class SvFgName;
// the animation of the tool of an item (HumanAnimName; 0: none)
u16 Func6E6050(const SvFgName* item); // 0x006E6050

// (functions of the game that HumanModel calls; names after the addresses while the purpose is unknown)
// the position of value between from and to as a factor 0..1 (1 when from == to)
f32 Func5D505C(f32 value, f32 from, f32 to); // 0x005D505C
// (two values of the object 0x009530CC)
f32 Func1E65C8(); // 0x001E65C8
f32 Func1E72C4(); // 0x001E72C4
// the tilt of the curve of the world at a position (0 on stages without one)
s16 Func52E760(const nn::math::VEC3* position); // 0x0052E760
// the position on the curve of the world between the two curves (factor between 0 and 1)
math::Angle16 Func52E794(nn::math::VEC3* curvedPosition, const nn::math::VEC3& position, f32 factor); // 0x0052E794

// the constants of HumanModel (in a file of their own, else GCC folds them): the range of the platform of the train
// station in z (Func1AC174), the height under the player and the distances to a bridge (Func1ABA38)
extern const f32 s_StationRangeZ0;
extern const f32 s_StationRangeZ1;
extern const f32 s_StationRangeZ2;
extern const f32 s_HeightRange;
extern const f32 s_BridgeRangeNear;
extern const f32 s_BridgeRangeFar;
// the constants of NpcModelBase (in the same group): the height of a chair, the unit of the height of the bone base
// and the offset of the bone Lfoot1
extern const f32 s_NpcChairHeight;
extern const f32 s_NpcBaseHeightUnit;
extern const f32 s_NpcFootOffset;
// the maximum number of member animations of the skeletal animations of the humans (a global of the game)
s32 Func6E6258(); // 0x006E6258

// the material animation of a face slot of id and its clone in allocator (the getters of the first and of the second
// slot; the face control takes them as function pointers)
const nw::anim::res::ResAnim* Func6E5778(u32 id); // 0x006E5778
nw::anim::res::ResAnim Func6E57C0(u32 id, nw::os::IAllocator* allocator); // 0x006E57C0
const nw::anim::res::ResAnim* Func6E5864(u32 id); // 0x006E5864
nw::anim::res::ResAnim Func6E58AC(u32 id, nw::os::IAllocator* allocator); // 0x006E58AC
