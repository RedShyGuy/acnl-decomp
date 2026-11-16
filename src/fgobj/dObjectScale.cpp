#include "fgobj/dObjectScale.h"
#include "Sv/dSvFgName.h"
#include "fgobj/fgobj_Globals.h"

namespace fgobj {
namespace {
const u8 SCALE_FRAME_NUM = 9;
// the scales of the frames
// 0x0084776C
const f32 s_ScaleScales[SCALE_FRAME_NUM] = {0.36f, 0.48f, 0.6f, 0.72f, 0.82f, 0.9f, 0.95f, 0.98f, 1.0f};
const f32 SCALE_START = 0.1f;
// the cylinder of a hole: radius at the full size, height, the values of the body
const f32 HOLE_CYLINDER_RADIUS = 15.0f;
const f32 HOLE_CYLINDER_START_RADIUS = 1.5f;
const f32 HOLE_CYLINDER_HEIGHT = 64.0f;
const u32 HOLE_BODY_UNK8 = 4;
const u32 HOLE_BODY_UNKC = 0x7FFFFFFF;
} // namespace

// 0x00591594 slot 0x14 (name is ours)
void fgobj::ObjectScale::Finalize()
{
    if (m_Item.IsName(SvFgName::NAME_HOLE)) {
        bgcheck::RemoveBody(&m_Cylinder);
    }
    ObjectBase::Finalize();
}

// 0x00592EE4 (name is ours)
fgobj::ObjectScale* fgobj::ObjectScale::Create(const SvFgName& item, s32 x, s32 z, bool keepRequest)
{
    ObjectScale* object = NewObject<ObjectScale>(s_SmallHeaps, SMALL_HEAP_NUM);
    if (object == NULL) {
        return NULL;
    }
    object->Setup(item, x, z, false);
    object->m_Frame = 0;
    object->m_KeepRequest = keepRequest;
    nn::math::VEC3 scale(SCALE_START, SCALE_START, SCALE_START);
    object->m_Model.SetScale(scale);
    object->m_Model.Update();
    if (object->m_Item.IsName(SvFgName::NAME_HOLE)) {
        object->m_Cylinder.Set(&object->m_Position, HOLE_CYLINDER_START_RADIUS, HOLE_CYLINDER_HEIGHT, HOLE_BODY_UNK8,
                               HOLE_BODY_UNKC);
        bgcheck::AddBody(&object->m_Cylinder);
    }
    return object;
}

// 0x0059302C slot 0x20 (name is ours)
s32 fgobj::ObjectScale::Execute()
{
    if (m_Frame >= SCALE_FRAME_NUM) {
        return m_KeepRequest == true ? EXECUTE_FINISHED_KEEP_REQUEST : EXECUTE_FINISHED;
    }
    f32 s = s_ScaleScales[m_Frame];
    nn::math::VEC3 scale(s, s, s);
    m_Model.SetScale(scale);
    m_Model.Update();
    m_Frame++;
    if (m_Item.IsName(SvFgName::NAME_HOLE)) {
        m_Cylinder.SetCylinder(&m_Position, s * HOLE_CYLINDER_RADIUS, HOLE_CYLINDER_HEIGHT);
    }
    return EXECUTE_CONTINUE;
}

// 0x00593138 slot 0x00
// 0x005930F4 (deleting dtor)
fgobj::ObjectScale::~ObjectScale()
{
    // (only the members are destroyed)
}

} // namespace fgobj
