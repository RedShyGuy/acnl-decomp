#include "Other/dActor.h"

#include "field/dFdInfo.h"

// 0x0094EFE8
Actor::CreateArg Actor::s_CreateArg;

// 0x0057C550
Actor::Actor()
{
    m_Rotation.x.value = 0;
    m_Rotation.y.value = 0;
    m_Rotation.z.value = 0;
    m_BackupRotation.x.value = 0;
    m_BackupRotation.y.value = 0;
    m_BackupRotation.z.value = 0;
    m_PrevRotation.x.value = 0;
    m_PrevRotation.y.value = 0;
    m_PrevRotation.z.value = 0;
    m_Speed = 0.0f;
    m_Unk = 0;
    if (s_CreateArg.pPosition != NULL) {
        m_Position = *s_CreateArg.pPosition;
        m_BackupPosition = m_Position;
    }
    if (s_CreateArg.pRotation != NULL) {
        m_Rotation = *s_CreateArg.pRotation;
        m_BackupRotation = m_Rotation;
        m_PrevRotation = m_Rotation;
    }
}

// 0x0057C654 slot 0x00
// 0x0057C640 (deleting dtor)
Actor::~Actor()
{
    // (only the base is destroyed)
}

// 0x0057C528 slot 0x08
bool Actor::CanInitialize() const
{
    if (!Base::CanInitialize()) {
        return false;
    }
    return true;
}

// 0x0057C470 slot 0x10
void Actor::HandleInitializationResult(oml::framework::Result result)
{
    Base::HandleInitializationResult(result);
}

// 0x0057C53C slot 0x14
bool Actor::CanFinalize() const
{
    if (!Base::CanFinalize()) {
        return false;
    }
    return true;
}

// 0x0057C474 slot 0x1C
void Actor::HandleFinalizationResult(oml::framework::Result result)
{
    Base::HandleFinalizationResult(result);
}

// 0x0057C478 slot 0x20
bool Actor::CanCalc() const
{
    if (!Base::CanCalc()) {
        return false;
    }
    // (the position and the rotation of the last frame are kept; the slot is const)
    Actor* self = const_cast<Actor*>(this);
    self->m_BackupPosition = m_Position;
    self->m_BackupRotation = m_Rotation;
    return true;
}

// 0x0057C50C slot 0x28
void Actor::HandleCalcResult(oml::framework::Result result)
{
    Base::HandleCalcResult(result);
}

// 0x0057C514 slot 0x2C
bool Actor::CanDraw() const
{
    if (!Base::CanDraw()) {
        return false;
    }
    return true;
}

// 0x005220E4 slot 0x34
void Actor::ProcessDrawResult(oml::framework::Result result)
{
    Base::ProcessDrawResult(result);
}

// 0x007537F0 slot 0x3C (name is ours)
u32 Actor::GetTypeMask()
{
    return TYPE_MASK_ACTOR;
}

// 0x0057C510 slot 0x40
void Actor::vf_0x40()
{
    // empty (in the original too)
}

// 0x0076330C | libgarden [tier A]
void Actor::GetPositionTo(nn::math::VEC3& out, u16 angle, f32 distance, bool withHeight) const
{
    Func5D4EC4(&out, &m_Position, angle, distance);
    if (withHeight) {
        out.y = FdInfo::GetHeight(out, static_cast<FieldName>(0));
    }
}

// 0x0057C4C4 (name is ours)
oml::framework::Process* Actor::Create(ProcName name, oml::framework::Process* parent, u32 arg,
                                       const nn::math::VEC3_* position, const math::Angle16Vec3* rotation)
{
    if (parent == NULL) {
        return NULL;
    }
    s_CreateArg.pPosition = position;
    s_CreateArg.pRotation = rotation;
    return ProcMgr::Create(name, parent, arg, PROC_TYPE_ACTOR);
}

// 0x0057C4F0 (name is ours)
math::Angle16 Actor::CalcAngleY(const nn::math::VEC3_* from, const nn::math::VEC3_* to)
{
    return math::Angle16::Atan2(to->x - from->x, to->z - from->z);
}
