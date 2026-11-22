#pragma once

#include "decomp.h"
#include "math/dAngle16.h"
#include "nn/math/math_Vector3.h"

class SvFgName;

namespace sead {
namespace ptcl {
class EmitterInstance;
class PtclInstance;
} // namespace ptcl
} // namespace sead

// Game functions that ToolBank calls (all names are ours, FuncXXXXXX after the address while the purpose is unknown).

// the field item at a unit (field: 1 in the known calls); the low byte of unknown8 is the kind of the item (>= 252:
// none)
struct FieldItemInfo
{
    u32 unknown0; // 0x0
    u32 unknown4; // 0x4
    u32 unknown8; // 0x8
    s32 unknownC; // 0xC
};
void Func240240(FieldItemInfo* out, s32 x, s32 z, u32 field); // 0x00240240
// a group of the field item kind (0..5; 6: none)
u32 Func24066C(u32 kind); // 0x0024066C

// the position at distance in direction angle from from (isCurved: on the curved world)
void Func64F96C(nn::math::VEC3* out, const nn::math::VEC3* from, math::Angle16 angle, f32 distance,
                bool isCurved); // 0x0064F96C
// a speed of the game (a constant of another file times 5.4)
f32 Func651E54(); // 0x00651E54
// the start speed of a throw from height from to height to in frames with gravity (per frame)
f32 Func5D50D8(f32 from, f32 to, f32 gravity, s32 frames); // 0x005D50D8
// the xz of value chases target (a part of the distance per call, between minStep and maxStep; the rest)
f32 Func566440(nn::math::VEC3* value, const nn::math::VEC3* target, f32 scale, f32 maxStep, f32 minStep); // 0x00566440
// the world matrix at position with rotation on the curved world
void Func52E9D8(nn::math::MTX34* out, const nn::math::VEC3* position, const math::Angle16Vec3* rotation); // 0x0052E9D8
// a shadow at position (drawn this frame)
void Func6EFAF4(const nn::math::VEC3* position, u32 unknown1, u32 unknown2, u32 unknown3, f32 scaleX, f32 scaleZ,
                f32 unknown4, f32 alpha, f32 unknown5); // 0x006EFAF4
// whether position is inside something of the field within radius
bool Func6F9D80(const nn::math::VEC3* position, f32 radius); // 0x006F9D80
// an entry of the field object of Func4E1338 at position (NULL: none)
const void* Func4EA078(void* object, const nn::math::VEC3* position, u32 unknown); // 0x004EA078
// the kind of the object of a handle (2..7)
s32 Func71B550(const u32* handle); // 0x0071B550
void Func27E23C(u32* handle, u32 unknown); // 0x0027E23C
// the particle callback of the tool effects
void Func6889B4(sead::ptcl::EmitterInstance* emitter, sead::ptcl::PtclInstance* ptcl); // 0x006889B4
// two flags of the game (index 0/1) that the tools switch on and off (Func4DEF14 sets, Func4DF040 clears)
void Func4DEF14(u32 index); // 0x004DEF14
void Func4DF040(u32 index); // 0x004DF040

// tests of the item
// the damage of an axe that breaks (AXE, SILVER_AXE: bits 0..3 of the flags, see SvFgName::UseBreakableAxe; 0 for
// the others)
u32 GetAxeDamage(const SvFgName* item); // 0x0076BE10
bool Func76D69C(const SvFgName* item); // 0x0076D69C
bool Func76D6C0(const SvFgName* item); // 0x0076D6C0
bool Func76D600(const SvFgName* item); // 0x0076D600

// the kind entry of an item (the table of the kinds of 0x00A193AC by byte 0x0F of the item parameters; NULL: none; its
// first halfword is the tool animation of the players, Func6E61CC)
const void* Func536114(const SvFgName* item); // 0x00536114
