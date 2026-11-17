// The contact tests of the collision world (all names are ours): one function for each pair of shape kinds in a table
// 5 x 5 (pairs in the other order use the same test with the shapes swapped and the normals turned around; the
// pairs AABB/AABB, AABB/Capsule, Capsule/Triangle and others have no test). A contact has the point, the normal from
// the second shape to the first and the depth.

#include "collision/dAABB.h"
#include "collision/dCapsule.h"
#include "collision/dCylinder.h"
#include "collision/dShape.h"
#include "collision/dSphere.h"
#include "collision/dTriangle.h"
#include "ssys/ma/ma_Math.h"

#include <cmath>

namespace collision {
// a test of a pair of shapes (the number of contacts)
typedef s32 (*CollideFunc)(Contact* out, u32 size, const Shape* a, const Shape* b);

// an entry of the table: the test and whether it takes the shapes in the other order
struct CollideEntry
{
    CollideFunc func; // 0x0
    bool isSwapped;   // 0x4
};

namespace {
inline f32 Dot(const nn::math::VEC3& a, const nn::math::VEC3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// (the segment test uses the squared distance where it means the dot product; kept as in the original)
inline f32 DistanceSquare(const nn::math::VEC3& a, const nn::math::VEC3& b)
{
    const nn::math::VEC3 d = a - b;
    return Dot(d, d);
}

inline bool IsAlmostZero(f32 value)
{
    return fabsf(value) <= 0x1p-23f;
}

inline f32 Clamp01(f32 value)
{
    if (value < 0.0f) {
        value = 0.0f;
    }
    if (value > 1.0f) {
        value = 1.0f;
    }
    return value;
}

// 0x006F7F04
DECOMP_NOINLINE void ClosestPointOnAABB(nn::math::VEC3* out, const nn::math::VEC3& point, const AABB* box)
{
    *out = point;
    if (out->x < box->m_Min.x) {
        out->x = box->m_Min.x;
    }
    if (out->x > box->m_Max.x) {
        out->x = box->m_Max.x;
    }
    if (out->y < box->m_Min.y) {
        out->y = box->m_Min.y;
    }
    if (out->y > box->m_Max.y) {
        out->y = box->m_Max.y;
    }
    if (out->z < box->m_Min.z) {
        out->z = box->m_Min.z;
    }
    if (out->z > box->m_Max.z) {
        out->z = box->m_Max.z;
    }
}

// 0x006F7FD4
DECOMP_NOINLINE void ClosestPointOnSegment(nn::math::VEC3* out, f32* pT, const nn::math::VEC3& point, const nn::math::VEC3& start,
                           const nn::math::VEC3& end)
{
    const nn::math::VEC3 segment = end - start;
    const nn::math::VEC3 toPoint = point - start;
    const f32 t = Clamp01(Dot(toPoint, segment) / Dot(segment, segment));
    if (pT != NULL) {
        *pT = t;
    }
    *out = start + segment * t;
}

// 0x006F8094 (the regions of the triangle in turn: the corners, the edges, the inside)
DECOMP_NOINLINE void ClosestPointOnTriangle(nn::math::VEC3* out, const nn::math::VEC3& point, const Triangle* triangle)
{
    const nn::math::VEC3& a = triangle->m_Vertices[0];
    const nn::math::VEC3& b = triangle->m_Vertices[1];
    const nn::math::VEC3& c = triangle->m_Vertices[2];
    const nn::math::VEC3 ab = b - a;
    const nn::math::VEC3 ac = c - a;
    const nn::math::VEC3 ap = point - a;
    const f32 d1 = Dot(ab, ap);
    const f32 d2 = Dot(ac, ap);
    if (d1 <= 0.0f && d2 <= 0.0f) {
        *out = a;
        return;
    }
    const nn::math::VEC3 bp = point - b;
    const f32 d3 = Dot(ab, bp);
    const f32 d4 = Dot(ac, bp);
    if (d3 >= 0.0f && d4 <= d3) {
        *out = b;
        return;
    }
    const f32 vc = d1 * d4 - d3 * d2;
    if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
        const f32 v = d1 / (d1 - d3);
        *out = a + ab * v;
        return;
    }
    const nn::math::VEC3 cp = point - c;
    const f32 d5 = Dot(ab, cp);
    const f32 d6 = Dot(ac, cp);
    if (d6 >= 0.0f && d5 <= d6) {
        *out = c;
        return;
    }
    const f32 vb = d5 * d2 - d1 * d6;
    if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
        const f32 w = d2 / (d2 - d6);
        *out = a + ac * w;
        return;
    }
    const f32 va = d3 * d6 - d5 * d4;
    if (va <= 0.0f) {
        const f32 e1 = d4 - d3;
        if (e1 >= 0.0f) {
            const f32 e2 = d5 - d6;
            if (e2 >= 0.0f) {
                const f32 w = e1 / (e1 + e2);
                *out = b + (c - b) * w;
                return;
            }
        }
    }
    const f32 denom = 1.0f / (va + vb + vc);
    const f32 v = vb * denom;
    const f32 w = vc * denom;
    const nn::math::VEC3 alongAC = ac * w;
    *out = a + ab * v + alongAC;
}

// 0x006F84AC (the closest points c1 and c2 of the segments p1-q1 and p2-q2 and their parameters)
DECOMP_NOINLINE DECOMP_NOIPA void ClosestPointsOfSegments(nn::math::VEC3* c1, nn::math::VEC3* c2, f32* pS, f32* pT, const nn::math::VEC3& p1,
                             const nn::math::VEC3& q1, const nn::math::VEC3& p2, const nn::math::VEC3& q2)
{
    const nn::math::VEC3 d1 = q1 - p1;
    const nn::math::VEC3 d2 = q2 - p2;
    const nn::math::VEC3 r = p1 - p2;
    const f32 a = Dot(d1, d1);
    const f32 e = Dot(d2, d2);
    const f32 f = DistanceSquare(d2, r);
    f32 s = 0.0f;
    f32 t = 0.0f;
    if (IsAlmostZero(a) && IsAlmostZero(e)) {
        if (pS != NULL) {
            *pS = s;
        }
        if (pT != NULL) {
            *pT = t;
        }
        *c1 = p1;
        *c2 = p2;
        return;
    }
    if (IsAlmostZero(a)) {
        t = Clamp01(f / e);
    } else {
        const f32 c = DistanceSquare(d1, r);
        if (IsAlmostZero(e)) {
            s = Clamp01(-c / a);
        } else {
            const f32 b = DistanceSquare(d1, d2);
            const f32 denom = a * e - b * b;
            if (!IsAlmostZero(denom)) {
                s = Clamp01((b * f - c * e) / denom);
            }
            t = (b * s + f) / e;
            if (t < 0.0f) {
                t = 0.0f;
                s = Clamp01(-c / a);
            } else if (t > 1.0f) {
                t = 1.0f;
                s = Clamp01((b - c) / a);
            }
        }
    }
    *c1 = p1 + d1 * s;
    *c2 = p2 + d2 * t;
    if (pS != NULL) {
        *pS = s;
    }
    if (pT != NULL) {
        *pT = t;
    }
}

// 0x006F724C
DECOMP_NOINLINE s32 CollideSphereAABB(Contact* out, u32 size, const Shape* a, const Shape* b)
{
    if (size < sizeof(Contact)) {
        return 0;
    }
    const Sphere* sphere = static_cast<const Sphere*>(a);
    const AABB* box = static_cast<const AABB*>(b);
    nn::math::VEC3 closest;
    ClosestPointOnAABB(&closest, sphere->m_Center, box);
    const nn::math::VEC3 d = closest - sphere->m_Center;
    const f32 distanceSquare = Dot(d, d);
    if (sphere->m_Radius * sphere->m_Radius < distanceSquare) {
        return 0;
    }
    out->normal = sphere->m_Center - closest;
    ssys::ma::NormalizeIfPossible(&out->normal);
    out->depth = sphere->m_Radius - sqrtf(distanceSquare);
    out->point = closest;
    return 1;
}

// 0x006F7404 (two contacts: at the height of the bottom and of the top of the cylinder)
DECOMP_NOINLINE s32 CollideCylinderAABB(Contact* out, u32 size, const Shape* a, const Shape* b)
{
    if (size < sizeof(Contact) * 2) {
        return 0;
    }
    const Cylinder* cylinder = static_cast<const Cylinder*>(a);
    const AABB* box = static_cast<const AABB*>(b);
    if (box->m_Max.y < cylinder->m_Center.y) {
        return 0;
    }
    if (cylinder->m_Center.y + cylinder->m_Height < box->m_Min.y) {
        return 0;
    }
    nn::math::VEC3 closest;
    ClosestPointOnAABB(&closest, cylinder->m_Center, box);
    const nn::math::VEC3 d = cylinder->m_Center - closest;
    const f32 distanceSquare = d.x * d.x + d.z * d.z;
    if (cylinder->m_Radius * cylinder->m_Radius < distanceSquare) {
        return 0;
    }
    out[0].normal = cylinder->m_Center - closest;
    out[0].normal.y = 0.0f;
    ssys::ma::NormalizeIfPossible(&out[0].normal);
    out[0].depth = cylinder->m_Radius - sqrtf(distanceSquare);
    out[0].point = closest;
    out[1].normal = out[0].normal;
    out[1].depth = out[0].depth;
    nn::math::VEC3 top = cylinder->m_Center;
    top.y += cylinder->m_Height;
    ClosestPointOnAABB(&out[1].point, top, box);
    return 2;
}

// 0x006F75EC
DECOMP_NOINLINE s32 CollideSphereSphere(Contact* out, u32 size, const Shape* a, const Shape* b)
{
    if (size < sizeof(Contact)) {
        return 0;
    }
    const Sphere* sphereA = static_cast<const Sphere*>(a);
    const Sphere* sphereB = static_cast<const Sphere*>(b);
    // the point of the surface of b towards a
    nn::math::VEC3 direction = sphereA->m_Center - sphereB->m_Center;
    ssys::ma::NormalizeIfPossible(&direction);
    const nn::math::VEC3 surface = sphereB->m_Center + direction * sphereB->m_Radius;
    const nn::math::VEC3 d = sphereA->m_Center - surface;
    const f32 distanceSquare = Dot(d, d);
    if (sphereA->m_Radius * sphereA->m_Radius < distanceSquare) {
        return 0;
    }
    out->normal = sphereA->m_Center - surface;
    ssys::ma::NormalizeIfPossible(&out->normal);
    out->depth = sphereA->m_Radius - sqrtf(distanceSquare);
    out->point = surface;
    return 1;
}

// 0x006F774C (the depth only counts the radius of the sphere)
DECOMP_NOINLINE s32 CollideSphereCapsule(Contact* out, u32 size, const Shape* a, const Shape* b)
{
    if (size < sizeof(Contact)) {
        return 0;
    }
    const Sphere* sphere = static_cast<const Sphere*>(a);
    const Capsule* capsule = static_cast<const Capsule*>(b);
    nn::math::VEC3 closest;
    ClosestPointOnSegment(&closest, NULL, sphere->m_Center, capsule->m_Start, capsule->m_End);
    const nn::math::VEC3 d = sphere->m_Center - closest;
    const f32 distanceSquare = Dot(d, d);
    const f32 radius = sphere->m_Radius + capsule->m_Radius;
    if (radius * radius < distanceSquare) {
        return 0;
    }
    out->normal = sphere->m_Center - closest;
    ssys::ma::NormalizeIfPossible(&out->normal);
    out->depth = sphere->m_Radius - sqrtf(distanceSquare);
    out->point = closest;
    return 1;
}

// 0x006F78A4 (unfinished in the original: the closest points are found, but it never reports a contact)
DECOMP_NOINLINE s32 CollideCapsuleCapsule(Contact*, u32 size, const Shape* a, const Shape* b)
{
    if (size < sizeof(Contact)) {
        return 0;
    }
    const Capsule* capsuleA = static_cast<const Capsule*>(a);
    const Capsule* capsuleB = static_cast<const Capsule*>(b);
    nn::math::VEC3 closestA;
    nn::math::VEC3 closestB;
    ClosestPointsOfSegments(&closestA, &closestB, NULL, NULL, capsuleA->m_Start, capsuleA->m_End, capsuleB->m_Start,
                            capsuleB->m_End);
    return 0;
}

// 0x006F7920
DECOMP_NOINLINE s32 CollideSphereCylinder(Contact* out, u32 size, const Shape* a, const Shape* b)
{
    if (size < sizeof(Contact)) {
        return 0;
    }
    const Sphere* sphere = static_cast<const Sphere*>(a);
    const Cylinder* cylinder = static_cast<const Cylinder*>(b);
    if (sphere->m_Center.y + sphere->m_Radius < cylinder->m_Center.y) {
        return 0;
    }
    if (cylinder->m_Center.y + cylinder->m_Height < sphere->m_Center.y - sphere->m_Radius) {
        return 0;
    }
    nn::math::VEC3 d = sphere->m_Center - cylinder->m_Center;
    d.y = 0.0f;
    const f32 radius = cylinder->m_Radius + sphere->m_Radius;
    if (radius * radius <= Dot(d, d)) {
        return 0;
    }
    out->normal = d;
    ssys::ma::NormalizeIfPossible(&out->normal);
    out->depth = radius - sqrtf(Dot(d, d));
    out->point = cylinder->m_Center + out->normal * cylinder->m_Radius;
    out->point.y = sphere->m_Center.y;
    return 1;
}

// 0x006F7ACC (only for a sphere in front of the triangle; also fills the normal and the depth of a second contact)
DECOMP_NOINLINE s32 CollideSphereTriangle(Contact* out, u32 size, const Shape* a, const Shape* b)
{
    if (size < sizeof(Contact)) {
        return 0;
    }
    const Sphere* sphere = static_cast<const Sphere*>(a);
    const Triangle* triangle = static_cast<const Triangle*>(b);
    if (Dot(triangle->m_Normal, sphere->m_Center) < Dot(triangle->m_Normal, triangle->m_Vertices[0])) {
        return 0;
    }
    nn::math::VEC3 closest;
    ClosestPointOnTriangle(&closest, sphere->m_Center, triangle);
    const nn::math::VEC3 d = closest - sphere->m_Center;
    const f32 distanceSquare = Dot(d, d);
    if (sphere->m_Radius * sphere->m_Radius < distanceSquare) {
        return 0;
    }
    out[0].normal = triangle->m_Normal;
    ssys::ma::NormalizeIfPossible(&out[0].normal);
    const f32 distance = sqrtf(distanceSquare);
    out[0].depth = sphere->m_Radius - distance;
    out[1].normal = sphere->m_Center - closest;
    ssys::ma::NormalizeIfPossible(&out[1].normal);
    out[1].depth = sphere->m_Radius - distance;
    out[0].point = closest;
    return 1;
}

// 0x006F7C98 (two contacts: on the side of b at the height of the bottom and of the top of a)
DECOMP_NOINLINE s32 CollideCylinderCylinder(Contact* out, u32 size, const Shape* a, const Shape* b)
{
    if (size < sizeof(Contact) * 2) {
        return 0;
    }
    const Cylinder* cylinderA = static_cast<const Cylinder*>(a);
    const Cylinder* cylinderB = static_cast<const Cylinder*>(b);
    if (cylinderB->m_Center.y + cylinderB->m_Height < cylinderA->m_Center.y) {
        return 0;
    }
    if (cylinderA->m_Center.y + cylinderA->m_Height < cylinderB->m_Center.y) {
        return 0;
    }
    nn::math::VEC3 normal = cylinderA->m_Center - cylinderB->m_Center;
    const f32 distanceSquare = normal.x * normal.x + normal.z * normal.z;
    const f32 radius = cylinderA->m_Radius + cylinderB->m_Radius;
    if (radius * radius < distanceSquare) {
        return 0;
    }
    normal.y = 0.0f;
    ssys::ma::NormalizeIfPossible(&normal);
    const f32 depth = cylinderA->m_Radius + cylinderB->m_Radius - sqrtf(distanceSquare);
    // the vertical line on the side of b
    const nn::math::VEC3 bottom = cylinderB->m_Center + normal * cylinderB->m_Radius;
    nn::math::VEC3 top = bottom;
    top.y += cylinderB->m_Height;
    out[0].normal = normal;
    out[0].depth = depth;
    nn::math::VEC3 closest;
    ClosestPointOnSegment(&closest, NULL, cylinderA->m_Center, bottom, top);
    out[0].point = closest;
    out[1].normal = normal;
    out[1].depth = depth;
    nn::math::VEC3 topA = cylinderA->m_Center;
    topA.y += cylinderA->m_Height;
    ClosestPointOnSegment(&closest, NULL, topA, bottom, top);
    out[1].point = closest;
    return 2;
}
} // namespace

// the tests for each pair of kinds (Sphere, AABB, Cylinder, Capsule, Triangle)
// 0x00952578
CollideEntry s_CollideTable[Shape::KIND_NUM][Shape::KIND_NUM] = {
    {{CollideSphereSphere, false}, {CollideSphereAABB, false}, {CollideSphereCylinder, false},
     {CollideSphereCapsule, false}, {CollideSphereTriangle, false}},
    {{CollideSphereAABB, true}, {NULL, false}, {CollideCylinderAABB, true}, {NULL, false}, {NULL, false}},
    {{CollideSphereCylinder, true}, {CollideCylinderAABB, false}, {CollideCylinderCylinder, false}, {NULL, false},
     {NULL, false}},
    {{CollideSphereCapsule, true}, {NULL, false}, {NULL, false}, {CollideCapsuleCapsule, false}, {NULL, false}},
    {{CollideSphereTriangle, true}, {NULL, false}, {NULL, false}, {NULL, false}, {NULL, false}},
};

// 0x006F8EB4 (name is ours)
collision::Contact::Contact()
{
    // empty (in the original too)
}

// 0x006F8E90 (name is ours)
collision::Contact& collision::Contact::operator=(const Contact& other)
{
    point = other.point;
    normal = other.normal;
    depth = other.depth;
    return *this;
}

// 0x006F8DA4 (name is ours)
s32 Collide(Contact* out, u32 size, const Shape* a, const Shape* b)
{
    const u32 kindA = a->m_Kind;
    const u32 kindB = b->m_Kind;
    if (kindA >= Shape::KIND_NUM || kindB >= Shape::KIND_NUM) {
        return 0;
    }
    const CollideEntry& entry = s_CollideTable[kindA][kindB];
    if (entry.func == NULL) {
        return 0;
    }
    if (!entry.isSwapped) {
        return entry.func(out, size, a, b);
    }
    const s32 num = entry.func(out, size, b, a);
    for (s32 i = 0; i < num; i++) {
        out[i].normal.x = -out[i].normal.x;
        out[i].normal.y = -out[i].normal.y;
        out[i].normal.z = -out[i].normal.z;
    }
    return num;
}
} // namespace collision
