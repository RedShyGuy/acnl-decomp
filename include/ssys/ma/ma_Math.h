#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector3.h"

// the math helpers of ssys::ma: rotations of matrices and vectors by 16-bit angles (0x10000 = a full turn), values that
// move towards a target step by step (all names are ours; the binary has no symbols for them)
namespace ssys {
namespace ma {
// (sets a 16-bit angle to 0)
void ClearAngle(s16* angle); // 0x00567F58

// mtx = mtx * rotation (nothing for angle 0)
void RotateX(nn::math::MTX34* mtx, s16 angle); // 0x00567F64
void RotateY(nn::math::MTX34* mtx, s16 angle); // 0x00568068
void RotateZ(nn::math::MTX34* mtx, s16 angle); // 0x005681A0
// mtx = rotation
void SetRotateX(nn::math::MTX34* mtx, s16 angle); // 0x00567FF8
void SetRotateY(nn::math::MTX34* mtx, s16 angle); // 0x005680FC
void SetRotateZ(nn::math::MTX34* mtx, s16 angle); // 0x00568234
// the three rotations in the order of the name
void RotateYXZ(nn::math::MTX34* mtx, s16 x, s16 y, s16 z); // 0x0056816C
void SetRotateZYX(nn::math::MTX34* mtx, s16 x, s16 y, s16 z); // 0x005682A4
void SetRotateYXZ(nn::math::MTX34* mtx, s16 x, s16 y, s16 z); // 0x00568334
// makes the axes (columns) axis0 and axis1 and the third one orthonormal, axis0 keeps its direction
void Orthonormalize(nn::math::MTX34* mtx, s32 axis0, s32 axis1); // 0x005683C4

// false if v is (almost) zero
bool Normalize(nn::math::VEC3* v); // 0x0056AC60
void NormalizeIfPossible(nn::math::VEC3* v); // 0x0056AE60
void RotateX(nn::math::VEC3* v, s16 angle); // 0x0056ACBC
void RotateY(nn::math::VEC3* v, s16 angle); // 0x0056AD0C
void RotateZ(nn::math::VEC3* v, s16 angle); // 0x0056AD5C
void CopyVec3(nn::math::VEC3* dst, const nn::math::VEC3* src); // 0x0056AEB4
void AssignVec3(nn::math::VEC3* dst, const nn::math::VEC3* src); // 0x0056AEC0
// divides by the largest component if it is above 1
void LimitToOne(nn::math::VEC3* v); // 0x0056B14C

// step towards target by step; true when it is reached
bool Chase(s16* value, s16 target, s16 step); // 0x0056BD04
bool Chase(s32* value, s32 target, s32 step); // 0x0056BCA0
bool Chase(u8* value, u8 target, s16 step); // 0x0056BC30
bool Chase(f32* value, f32 target, f32 step); // 0x0056BBA8
bool ChaseAngle(s16* value, s16 target, s16 step); // 0x0056B8CC
// step towards target by the distance / divisor, at most maxStep and at least minStep; the distance that is left
s32 ChaseDivide(s32* value, s32 target, s32 divisor, s32 maxStep, s32 minStep); // 0x0056B920
s16 ChaseDivide(s16* value, s16 target, s32 divisor, s16 maxStep, s16 minStep); // 0x0056BA14
void ChaseDivide(s16* value, s16 target, s32 divisor, s16 maxStep); // 0x0056B9C4
// step towards target by the distance * scale, at most maxStep and at least minStep; the distance that is left
f32 ChaseScale(f32* value, f32 target, f32 scale, f32 maxStep, f32 minStep); // 0x0056BD70
void ChaseScale(f32* value, f32 target, f32 scale, f32 maxStep); // 0x0056BE7C
// value -= value * scale (at most maxStep)
void Decay(f32* value, f32 scale, f32 maxStep); // 0x0056BE48

// whether value lies between a and b (in either order)
bool IsInRange(f32 value, f32 a, f32 b); // 0x0056BAD0
bool IsInRange(s32 value, s32 a, s32 b); // 0x0056BB20
// |a - b| as angles
s32 GetAngleDistance(s16 a, s16 b); // 0x0056BB54
// from at the same angle to to at the opposite one
f32 LerpByAngle(s16 a, s16 b, f32 from, f32 to); // 0x0056BB68

// moves a position to a target in frames, slowing down in the last ones
struct Mover
{
    // (inline) stopped
    Mover() : m_SlowFrames(0), m_FrameCount(-1) {}
    void Start(const nn::math::VEC3& from, const nn::math::VEC3& to, f32 speed, s32 maxFrames, s32 minFrames,
               s32 slowFrames); // 0x00568ACC
    // true when it has arrived
    bool Step(); // 0x00568A28
    void Stop(); // 0x00568A1C

    nn::math::VEC3 m_Position;  // 0x00
    nn::math::VEC3 m_Velocity;  // 0x0C
    s32 m_SlowFrames;           // 0x18
    s32 m_FrameCount;           // 0x1C frames left, -1 when stopped
};
ASSERT_SIZE(Mover, 0x20);

// a triangle with its normal (cross product of the edges p0->p1 and p1->p2)
struct Triangle
{
    void Set(const nn::math::VEC3& p0, const nn::math::VEC3& p1, const nn::math::VEC3& p2); // 0x0056ADA8

    u32 _0;                         // 0x00
    u32 _4;                         // 0x04
    nn::math::VEC3 m_Points[3];     // 0x08
    nn::math::VEC3 m_Normal;        // 0x2C
};
ASSERT_SIZE(Triangle, 0x38);
} // namespace ma
} // namespace ssys
