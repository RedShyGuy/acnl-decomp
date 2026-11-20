#pragma once

#include "decomp.h"
#include "collision/dAABB.h"
#include "collision/dWorld.h"
#include "collision/dWorld_Body.h"
#include "math/dAngle16.h"
#include "nn/math/math_Vector3.h"

namespace bgcheck {
// a box of the background that actors cannot walk through (furniture, the walls of a camping car...): a body of the
// collision world 0x00987F10 while it is active. The movement test of the actors (0x0061553C) calls the slots of the
// bodies it touches (names are ours)
// RTTI N7bgcheck6MoveBgE @ 0x008D3EF4
// vtable 0x0090BC94 (vptr 0x0090BC9C), offset_to_top 0, 4 entries
class MoveBg : public ::collision::World::Body
{
public:
    MoveBg(); // 0x0061532C
    virtual ~MoveBg(); // 0x00615384 slot 0x00
    // 0x00615360 (deleting dtor)
    // the movement test touched a body of the group 8 / 0x100: the direction away from the box, the contact, the flags and
    // the object of the test (its type is not known yet)
    virtual void OnTouch(const nn::math::VEC3* direction, void* object, u32 flags, f32 radius); // 0x00615270 slot 0x08
    virtual void OnGuard(const nn::math::VEC3* contact, u32 flags, void* object); // 0x0061526C slot 0x0C

    // the box: width (x), height (y) and depth (z) of size scaled around base, turned by the quarter turn nearest to
    // angle (a turn by 90 or 270 degrees swaps width and depth); then the body joins the world
    void Set(const nn::math::VEC3& base, const nn::math::VEC3& size, const math::Angle16& angle, u32 group, u32 mask,
             u32 user, f32 scale); // 0x00615274
    // only the box: width (x), height (y) and depth (z) of size on base, unscaled and unturned
    void SetBox(const nn::math::VEC3& base, const nn::math::VEC3& size); // 0x00615458
    // (inline; the copies lie in the shared code)
    DECOMP_EMIT_COPY DECOMP_NOINLINE inline bool IsActive() const { return m_IsActive; } // 0x002F7364
    // leaves the world
    DECOMP_EMIT_COPY DECOMP_NOINLINE inline void Remove(); // 0x002F736C

    bool m_IsActive;                   // 0x10
    collision::AABB m_Box;             // 0x14
    nn::math::VEC3 m_Size;             // 0x34 width, height, depth after the turn
    math::Angle16 m_Angle;             // 0x40 the quarter turn
    u32 m_User;                        // 0x44
};
ASSERT_SIZE(MoveBg, 0x48);

// the collision world of the boxes (built by a static initializer that builds globals of several files)
extern collision::World g_MoveBgWorld;
// the body joins / leaves the world of the boxes (NULL: nothing; RemoveMoveBg is inline, the copy lies in the shared
// code)
void AddMoveBg(collision::World::Body* body); // 0x0061550C
DECOMP_EMIT_COPY DECOMP_NOINLINE inline void RemoveMoveBg(collision::World::Body* body); // 0x00134064

inline void RemoveMoveBg(collision::World::Body* body)
{
    if (body != NULL) {
        g_MoveBgWorld.Remove(body);
    }
}

inline void MoveBg::Remove()
{
    RemoveMoveBg(this);
    m_IsActive = false;
}
} // namespace bgcheck
