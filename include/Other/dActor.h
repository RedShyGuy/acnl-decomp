#pragma once

#include "decomp.h"
#include "math/dAngle16.h"
#include "nn/math/math_Vector3.h"
#include "Other/dBase.h"
#include "Other/dProcMgr.h"

// The base of the actors of the game: a process with a position and a rotation (class, field and GetPositionTo names
// after libgarden, the rest is ours). Actor::Create passes the start position and rotation to the constructor through
// s_CreateArg.
// RTTI 5Actor @ 0x008CD298
// vtable 0x008F89B0 (vptr 0x008F89B8), offset_to_top 0, 17 entries
class Actor : public ::Base
{
public:
    // the start position and rotation of the next actor (set by Create; NULL: none)
    struct CreateArg
    {
        const nn::math::VEC3_* pPosition;        // 0x0
        const math::Angle16Vec3* pRotation;      // 0x4
    };

    Actor(); // 0x0057C550
    virtual ~Actor(); // 0x0057C654 slot 0x00
    virtual bool CanInitialize() const; // 0x0057C528 slot 0x08
    virtual void HandleInitializationResult(oml::framework::Result result); // 0x0057C470 slot 0x10
    virtual bool CanFinalize() const; // 0x0057C53C slot 0x14
    virtual void HandleFinalizationResult(oml::framework::Result result); // 0x0057C474 slot 0x1C
    // the position and the rotation of the last frame are kept
    virtual bool CanCalc() const; // 0x0057C478 slot 0x20
    virtual void HandleCalcResult(oml::framework::Result result); // 0x0057C50C slot 0x28
    virtual bool CanDraw() const; // 0x0057C514 slot 0x2C
    virtual void ProcessDrawResult(oml::framework::Result result); // 0x005220E4 slot 0x34
    virtual u32 GetTypeMask(); // 0x007537F0 slot 0x3C (name is ours)
    virtual void vf_0x40(); // 0x0057C510 slot 0x40

    // the position at distance from the actor in the direction angle (with withHeight on the ground of field 0)
    void GetPositionTo(nn::math::VEC3& out, u16 angle, f32 distance, bool withHeight) const; // 0x0076330C | libgarden [tier A]

    // an actor of the process class as a child of parent at position with rotation (NULL for a parent of NULL)
    static oml::framework::Process* Create(ProcName name, oml::framework::Process* parent, u32 arg,
                                           const nn::math::VEC3_* position,
                                           const math::Angle16Vec3* rotation); // 0x0057C4C4
    // the angle around y from from to to
    static math::Angle16 CalcAngleY(const nn::math::VEC3_* from, const nn::math::VEC3_* to); // 0x0057C4F0

    static CreateArg s_CreateArg;

    // (names after libgarden)
    nn::math::VEC3_ m_Position;         // 0x14
    nn::math::VEC3_ m_BackupPosition;   // 0x20 the position of the last frame
    math::Angle16Vec3 m_Rotation;       // 0x2C
    math::Angle16Vec3 m_BackupRotation; // 0x32 the rotation of the last frame
    math::Angle16Vec3 m_PrevRotation;   // 0x38
    f32 m_Speed;                        // 0x40
    u8 m_Unk;                           // 0x44
};
ASSERT_SIZE(Actor, 0x48);

// the position at distance from from in the direction angle (y stays; out NULL: nothing; game code that is not
// decompiled yet, name after the address)
void Func5D4EC4(nn::math::VEC3_* out, const nn::math::VEC3_* from, u16 angle, f32 distance); // 0x005D4EC4
