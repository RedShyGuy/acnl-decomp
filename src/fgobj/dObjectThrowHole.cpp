#include "fgobj/dObjectThrowHole.h"
#include "fgobj/fgobj_Globals.h"

namespace fgobj {
namespace {
// the flight: frames, gravity, the growth of the scale per frame
const u8 THROW_HOLE_FRAMES = 15;
const f32 THROW_HOLE_GRAVITY = 0.5f;
const f32 THROW_HOLE_SCALE_STEP = 0.1f;
} // namespace

// 0x00596B0C (name is ours)
fgobj::ObjectThrowHole* fgobj::ObjectThrowHole::Create(const SvFgName& item, s32 x, s32 z, const nn::math::VEC3* from, f32 scale)
{
    ObjectThrowHole* object = NewObject<ObjectThrowHole>(s_SmallHeaps, SMALL_HEAP_NUM);
    if (object == NULL) {
        return NULL;
    }
    object->Setup(item, x, z, false);
    object->m_Target = object->m_Position;
    object->m_Position = *from;
    nn::math::VEC3 scales(scale, scale, scale);
    object->m_Model.SetScale(scales);
    object->m_Scale = scale;
    object->m_Frame = THROW_HOLE_FRAMES;
    object->m_Gravity = THROW_HOLE_GRAVITY;
    object->SetVelocityToTarget(THROW_HOLE_FRAMES, 1.0f);
    return object;
}

// 0x00596C58 slot 0x20 (name is ours)
s32 fgobj::ObjectThrowHole::Execute()
{
    if (m_Frame == 0) {
        return EXECUTE_FINISHED_KEEP_REQUEST;
    }
    m_Scale += THROW_HOLE_SCALE_STEP;
    if (m_Scale > 1.0f) {
        m_Scale = 1.0f;
    }
    nn::math::VEC3 scale(m_Scale, m_Scale, m_Scale);
    m_Model.SetScale(scale);
    m_Velocity.y -= m_Gravity;
    m_Position.x += m_Velocity.x;
    m_Position.y += m_Velocity.y;
    m_Position.z += m_Velocity.z;
    m_Frame--;
    UpdateMatrix(false, 0.0f);
    return EXECUTE_CONTINUE;
}

// 0x00596D54 slot 0x00
// 0x00596D24 (deleting dtor)
fgobj::ObjectThrowHole::~ObjectThrowHole()
{
    // (only the base is destroyed)
}

} // namespace fgobj
