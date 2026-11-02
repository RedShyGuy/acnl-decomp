#include "ssys/ma/ma_Math.h"
#include "nn/math/math_Api.h"
#include "sead/seadMathCalcCommon.h"

#include <cmath>

namespace ssys {
namespace ma {
namespace {
// the rotation matrices (inline)
inline void MakeRotateX(nn::math::MTX34* mtx, s16 angle)
{
    f32 c = nn::math::CosIdx(angle);
    f32 s = nn::math::SinIdx(angle);
    mtx->m[0][0] = 1.0f;
    mtx->m[0][1] = 0.0f;
    mtx->m[0][2] = 0.0f;
    mtx->m[0][3] = 0.0f;
    mtx->m[1][0] = 0.0f;
    mtx->m[1][1] = c;
    mtx->m[1][2] = -s;
    mtx->m[1][3] = 0.0f;
    mtx->m[2][0] = 0.0f;
    mtx->m[2][1] = s;
    mtx->m[2][2] = c;
    mtx->m[2][3] = 0.0f;
}

inline void MakeRotateY(nn::math::MTX34* mtx, s16 angle)
{
    f32 c = nn::math::CosIdx(angle);
    f32 s = nn::math::SinIdx(angle);
    mtx->m[0][0] = c;
    mtx->m[0][1] = 0.0f;
    mtx->m[0][2] = s;
    mtx->m[0][3] = 0.0f;
    mtx->m[1][0] = 0.0f;
    mtx->m[1][1] = 1.0f;
    mtx->m[1][2] = 0.0f;
    mtx->m[1][3] = 0.0f;
    mtx->m[2][0] = -s;
    mtx->m[2][1] = 0.0f;
    mtx->m[2][2] = c;
    mtx->m[2][3] = 0.0f;
}

inline void MakeRotateZ(nn::math::MTX34* mtx, s16 angle)
{
    f32 c = nn::math::CosIdx(angle);
    f32 s = nn::math::SinIdx(angle);
    mtx->m[0][0] = c;
    mtx->m[0][1] = -s;
    mtx->m[0][2] = 0.0f;
    mtx->m[0][3] = 0.0f;
    mtx->m[1][0] = s;
    mtx->m[1][1] = c;
    mtx->m[1][2] = 0.0f;
    mtx->m[1][3] = 0.0f;
    mtx->m[2][0] = 0.0f;
    mtx->m[2][1] = 0.0f;
    mtx->m[2][2] = 1.0f;
    mtx->m[2][3] = 0.0f;
}
} // namespace

// 0x00567F58 (name is ours)
void ClearAngle(s16* angle)
{
    *angle = 0;
}

// 0x00567F64 (name is ours)
void RotateX(nn::math::MTX34* mtx, s16 angle)
{
    if (angle == 0) {
        return;
    }
    nn::math::MTX34 rotation;
    MakeRotateX(&rotation, angle);
    sead::Matrix34CalcCtr<f32>::multiply(*mtx, *mtx, rotation);
}

// 0x00567FF8 (name is ours)
void SetRotateX(nn::math::MTX34* mtx, s16 angle)
{
    MakeRotateX(mtx, angle);
}

// 0x00568068 (name is ours)
void RotateY(nn::math::MTX34* mtx, s16 angle)
{
    if (angle == 0) {
        return;
    }
    nn::math::MTX34 rotation;
    MakeRotateY(&rotation, angle);
    sead::Matrix34CalcCtr<f32>::multiply(*mtx, *mtx, rotation);
}

// 0x005680FC (name is ours)
void SetRotateY(nn::math::MTX34* mtx, s16 angle)
{
    MakeRotateY(mtx, angle);
}

// 0x0056816C (name is ours)
void RotateYXZ(nn::math::MTX34* mtx, s16 x, s16 y, s16 z)
{
    RotateY(mtx, y);
    RotateX(mtx, x);
    RotateZ(mtx, z);
}

// 0x005681A0 (name is ours)
void RotateZ(nn::math::MTX34* mtx, s16 angle)
{
    if (angle == 0) {
        return;
    }
    nn::math::MTX34 rotation;
    MakeRotateZ(&rotation, angle);
    sead::Matrix34CalcCtr<f32>::multiply(*mtx, *mtx, rotation);
}

// 0x00568234 (name is ours)
void SetRotateZ(nn::math::MTX34* mtx, s16 angle)
{
    MakeRotateZ(mtx, angle);
}

// 0x005682A4 (name is ours)
void SetRotateZYX(nn::math::MTX34* mtx, s16 x, s16 y, s16 z)
{
    MakeRotateZ(mtx, z);
    RotateY(mtx, y);
    RotateX(mtx, x);
}

// 0x00568334 (name is ours)
void SetRotateYXZ(nn::math::MTX34* mtx, s16 x, s16 y, s16 z)
{
    MakeRotateY(mtx, y);
    RotateX(mtx, x);
    RotateZ(mtx, z);
}

// 0x005683C4 (name is ours)
void Orthonormalize(nn::math::MTX34* mtx, s32 axis0, s32 axis1)
{
    f32* m = &mtx->m[0][0];
    nn::math::VEC3 v0(m[axis0], m[axis0 + 4], m[axis0 + 8]);
    nn::math::VEC3 v1(m[axis1], m[axis1 + 4], m[axis1 + 8]);
    sead::Vector3CalcCtr<f32>::normalize(v0);
    nn::math::VEC3 cross;
    sead::Vector3CalcCtr<f32>::cross(cross, v0, v1);
    nn::math::VEC3 v2 = cross;
    sead::Vector3CalcCtr<f32>::normalize(v2);
    sead::Vector3CalcCtr<f32>::cross(cross, v2, v0);
    v1 = cross;
    sead::Vector3CalcCtr<f32>::normalize(v1);
    s32 axis2 = 3 - axis0 - axis1;
    m[axis0] = v0.x;
    m[axis0 + 4] = v0.y;
    m[axis0 + 8] = v0.z;
    m[axis1] = v1.x;
    m[axis1 + 4] = v1.y;
    m[axis1 + 8] = v1.z;
    m[axis2] = v2.x;
    m[axis2 + 4] = v2.y;
    m[axis2 + 8] = v2.z;
}

// 0x00568A1C (name is ours)
void Mover::Stop()
{
    m_FrameCount = -1;
}

// 0x00568A28 (name is ours)
bool Mover::Step()
{
    if (m_FrameCount <= 0) {
        m_FrameCount = -1;
        return true;
    }
    if (m_FrameCount <= m_SlowFrames) {
        f32 factor = static_cast<f32>(m_FrameCount) / static_cast<f32>(m_FrameCount + 1);
        m_Velocity.x *= factor;
        m_Velocity.y *= factor;
        m_Velocity.z *= factor;
    }
    m_Position.x += m_Velocity.x;
    m_Position.y += m_Velocity.y;
    m_Position.z += m_Velocity.z;
    m_FrameCount--;
    return false;
}

// 0x00568ACC (name is ours)
void Mover::Start(const nn::math::VEC3& from, const nn::math::VEC3& to, f32 speed, s32 maxFrames, s32 minFrames,
                  s32 slowFrames)
{
    m_Position.x = from.x;
    m_Position.y = from.y;
    m_Position.z = from.z;
    m_SlowFrames = slowFrames;
    f32 dx = to.x - from.x;
    f32 dy = to.y - from.y;
    f32 dz = to.z - from.z;
    s32 frames = static_cast<s32>(sqrtf(dx * dx + dy * dy + dz * dz) / speed);
    if (frames > maxFrames) {
        frames = maxFrames;
    }
    if (frames < minFrames) {
        frames = minFrames;
    }
    m_FrameCount = frames;
    f32 sum = 0.0f;
    for (s32 i = 0; i < slowFrames; i++) {
        sum += static_cast<f32>(slowFrames - i);
    }
    f32 scale = 1.0f / (static_cast<f32>(frames - slowFrames) + sum / static_cast<f32>(slowFrames + 1));
    m_Velocity.x = dx * scale;
    m_Velocity.y = dy * scale;
    m_Velocity.z = dz * scale;
}

// 0x0056AC60 (name is ours)
bool Normalize(nn::math::VEC3* v)
{
    f32 x = v->x;
    f32 length = sqrtf(x * x + v->y * v->y + v->z * v->z);
    if (fabsf(length) <= 0x1p-23f) {
        return false;
    }
    f32 inverse = 1.0f / length;
    v->x = x * inverse;
    v->y = v->y * inverse;
    v->z = v->z * inverse;
    return true;
}

// 0x0056ACBC (name is ours)
void RotateX(nn::math::VEC3* v, s16 angle)
{
    f32 c = nn::math::CosIdx(angle);
    f32 s = nn::math::SinIdx(angle);
    f32 y = v->y;
    f32 z = v->z;
    v->y = c * y - s * z;
    v->z = s * y + c * z;
}

// 0x0056AD0C (name is ours)
void RotateY(nn::math::VEC3* v, s16 angle)
{
    f32 c = nn::math::CosIdx(angle);
    f32 s = nn::math::SinIdx(angle);
    f32 x = v->x;
    f32 z = v->z;
    v->x = c * x + s * z;
    v->z = c * z - s * x;
}

// 0x0056AD5C (name is ours)
void RotateZ(nn::math::VEC3* v, s16 angle)
{
    f32 c = nn::math::CosIdx(angle);
    f32 s = nn::math::SinIdx(angle);
    f32 x = v->x;
    f32 y = v->y;
    v->x = c * x - s * y;
    v->y = s * x + c * y;
}

// 0x0056ADA8 (name is ours)
void Triangle::Set(const nn::math::VEC3& p0, const nn::math::VEC3& p1, const nn::math::VEC3& p2)
{
    m_Points[0] = p0;
    m_Points[1] = p1;
    m_Points[2] = p2;
    f32 ax = m_Points[1].x - m_Points[0].x;
    f32 ay = m_Points[1].y - m_Points[0].y;
    f32 az = m_Points[1].z - m_Points[0].z;
    f32 bx = m_Points[2].x - m_Points[1].x;
    f32 by = m_Points[2].y - m_Points[1].y;
    f32 bz = m_Points[2].z - m_Points[1].z;
    m_Normal.x = ay * bz - az * by;
    m_Normal.y = az * bx - ax * bz;
    m_Normal.z = ax * by - ay * bx;
    NormalizeIfPossible(&m_Normal);
}

// 0x0056AE60 (name is ours)
void NormalizeIfPossible(nn::math::VEC3* v)
{
    f32 x = v->x;
    f32 length = sqrtf(x * x + v->y * v->y + v->z * v->z);
    if (fabsf(length) <= 0x1p-23f) {
        return;
    }
    f32 inverse = 1.0f / length;
    v->x = x * inverse;
    v->y = v->y * inverse;
    v->z = v->z * inverse;
}

// 0x0056AEB4 (name is ours)
void CopyVec3(nn::math::VEC3* dst, const nn::math::VEC3* src)
{
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
}

// (copies the bits)
// 0x0056AEC0 (name is ours)
void AssignVec3(nn::math::VEC3* dst, const nn::math::VEC3* src)
{
    const u32* from = reinterpret_cast<const u32*>(src);
    u32* to = reinterpret_cast<u32*>(dst);
    to[0] = from[0];
    to[1] = from[1];
    to[2] = from[2];
}

// 0x0056B14C (name is ours)
void LimitToOne(nn::math::VEC3* v)
{
    f32 x = v->x;
    f32 y = v->y;
    f32 max = x;
    if (y > max) {
        max = y;
    }
    f32 z = v->z;
    if (z > max) {
        max = z;
    }
    if (max > 1.0f) {
        v->x = x / max;
        v->y = y / max;
        v->z = z / max;
    }
}

// (the sign of (value - target) * step tells whether the target is passed, also across the wrap of the angle)
// 0x0056B8CC (name is ours)
bool ChaseAngle(s16* value, s16 target, s16 step)
{
    s16 current = *value;
    if (current == target) {
        return true;
    }
    if (step == 0) {
        return false;
    }
    if (static_cast<s16>(current - target) > 0) {
        step = -step;
    }
    s32 next = current + step;
    *value = next;
    if (static_cast<s16>(next - target) * step >= 0) {
        *value = target;
        return true;
    }
    return false;
}

// 0x0056B920 (name is ours)
s32 ChaseDivide(s32* value, s32 target, s32 divisor, s32 maxStep, s32 minStep)
{
    s32 current = *value;
    s32 distance = target - current;
    if (current != target) {
        s32 step = distance / divisor;
        if (step > minStep || step < -minStep) {
            if (step > maxStep) {
                step = maxStep;
            } else if (step < -maxStep) {
                step = -maxStep;
            }
            *value = step + current;
        } else if (distance >= 0) {
            current += minStep;
            *value = current;
            if (target - current <= 0) {
                *value = target;
            }
        } else {
            current -= minStep;
            *value = current;
            if (target - current >= 0) {
                *value = target;
            }
        }
    }
    return target - *value;
}

// 0x0056B9C4 (name is ours)
void ChaseDivide(s16* value, s16 target, s32 divisor, s16 maxStep)
{
    s16 current = *value;
    s16 step = static_cast<s16>(static_cast<s16>(target - current) / divisor);
    if (step > maxStep) {
        *value = current + maxStep;
    } else if (step >= -maxStep) {
        *value = step + current;
    } else {
        *value = current - maxStep;
    }
}

// 0x0056BA14 (name is ours)
s16 ChaseDivide(s16* value, s16 target, s32 divisor, s16 maxStep, s16 minStep)
{
    s16 current = *value;
    s16 distance = target - current;
    if (current != target) {
        s16 step = distance / divisor;
        if (step > minStep || step < -minStep) {
            if (step > maxStep) {
                step = maxStep;
            } else if (step < -maxStep) {
                step = -maxStep;
            }
            *value = step + current;
        } else if (distance >= 0) {
            s16 next = current + minStep;
            *value = next;
            if (static_cast<s16>(target - next) <= 0) {
                *value = target;
            }
        } else {
            s16 next = current - minStep;
            *value = next;
            if (static_cast<s16>(target - next) >= 0) {
                *value = target;
            }
        }
    }
    return target - *value;
}

// 0x0056BAD0 (name is ours)
bool IsInRange(f32 value, f32 a, f32 b)
{
    if (a < b) {
        return value >= a && value <= b;
    }
    return value >= b && value <= a;
}

// 0x0056BB20 (name is ours)
bool IsInRange(s32 value, s32 a, s32 b)
{
    if (a < b) {
        return value >= a && b >= value;
    }
    return value >= b && a >= value;
}

// 0x0056BB54 (name is ours)
s32 GetAngleDistance(s16 a, s16 b)
{
    s16 distance = a - b;
    return distance < 0 ? -distance : distance;
}

// 0x0056BB68 (name is ours)
f32 LerpByAngle(s16 a, s16 b, f32 from, f32 to)
{
    u16 distance = a - b;
    if (distance & 0x8000) {
        distance = -distance;
    }
    u16 weight = distance ^ 0x7FFF;
    return from + (to - from) * static_cast<f32>(weight) * (1.0f / 32768.0f);
}

// 0x0056BBA8 (name is ours)
bool Chase(f32* value, f32 target, f32 step)
{
    f32 current = *value;
    if (current == target) {
        return true;
    }
    if (step == 0.0f) {
        return false;
    }
    if (current > target) {
        step = -step;
    }
    current += step;
    *value = current;
    if (current == target || (current > target) == (step > 0.0f)) {
        *value = target;
        return true;
    }
    return false;
}

// 0x0056BC30 (name is ours)
bool Chase(u8* value, u8 target, s16 step)
{
    u8 current = *value;
    if (step == 0) {
        return current == target;
    }
    if (current > target) {
        step = -step;
    }
    s16 next = current + step;
    if (target != next && (next > target) != (step >= 0)) {
        *value = next;
        return false;
    }
    *value = target;
    return true;
}

// 0x0056BCA0 (name is ours)
bool Chase(s32* value, s32 target, s32 step)
{
    s32 current = *value;
    if (current == target) {
        return true;
    }
    if (step == 0) {
        return false;
    }
    if (current > target) {
        step = -step;
    }
    current += step;
    *value = current;
    if (current == target || (current > target) == (step > 0)) {
        *value = target;
        return true;
    }
    return false;
}

// 0x0056BD04 (name is ours)
bool Chase(s16* value, s16 target, s16 step)
{
    s16 current = *value;
    if (current == target) {
        return true;
    }
    if (step == 0) {
        return false;
    }
    if (current > target) {
        step = -step;
    }
    current += step;
    *value = current;
    if (current == target || (current > target) == (step > 0)) {
        *value = target;
        return true;
    }
    return false;
}

// 0x0056BD70 (name is ours)
f32 ChaseScale(f32* value, f32 target, f32 scale, f32 maxStep, f32 minStep)
{
    f32 current = *value;
    if (current != target) {
        f32 step = (target - current) * scale;
        if (step >= minStep || step <= -minStep) {
            if (step > maxStep) {
                step = maxStep;
            }
            if (step < -maxStep) {
                step = -maxStep;
            }
            *value = current + step;
        } else if (step > 0.0f) {
            if (step < minStep) {
                f32 next = current + minStep;
                *value = next;
                *value = next > target ? target : next;
            }
        } else if (step > -minStep) {
            f32 next = current - minStep;
            *value = next;
            *value = next > target ? next : target;
        }
    }
    f32 distance = target - *value;
    return distance > 0.0f ? distance : -distance;
}

// 0x0056BE48 (name is ours)
void Decay(f32* value, f32 scale, f32 maxStep)
{
    f32 current = *value;
    f32 step = current * scale;
    if (step > maxStep) {
        step = maxStep;
    } else if (step < -maxStep) {
        step = -maxStep;
    }
    *value = current - step;
}

// 0x0056BE7C (name is ours)
void ChaseScale(f32* value, f32 target, f32 scale, f32 maxStep)
{
    f32 current = *value;
    if (current == target) {
        return;
    }
    f32 step = (target - current) * scale;
    if (step > maxStep) {
        step = maxStep;
    } else if (step < -maxStep) {
        step = -maxStep;
    }
    *value = current + step;
}

} // namespace ma
} // namespace ssys
