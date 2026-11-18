#pragma once

#include "decomp.h"
#include "nw/anim/res/anim_ResAnim.h"

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
    u8 unknownE[2];       // 0x0E
    u8 flags;             // 0x10 (bit 4: a one-time face animation stays at its end; bit 5: loop)
};
// the information of id
const HumanAnimInfo* Func6E6268(u32 id); // 0x006E6268

// the material animation of a face slot of id and its clone in allocator (the getters of the first and of the second
// slot; the face control takes them as function pointers)
const nw::anim::res::ResAnim* Func6E5778(u32 id); // 0x006E5778
nw::anim::res::ResAnim Func6E57C0(u32 id, nw::os::IAllocator* allocator); // 0x006E57C0
const nw::anim::res::ResAnim* Func6E5864(u32 id); // 0x006E5864
nw::anim::res::ResAnim Func6E58AC(u32 id, nw::os::IAllocator* allocator); // 0x006E58AC
