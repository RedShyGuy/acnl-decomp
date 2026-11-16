#include "fgobj/dObjectDig.h"

namespace fgobj {
namespace {
const u8 DIG_FRAME_NUM = 4;
// the radii of the cylinder in the frames (times DIG_CYLINDER_RADIUS)
// 0x00847790
const f32 s_DigScales[DIG_FRAME_NUM] = {0.48f, 0.6f, 0.72f, 0.82f};
const f32 DIG_CYLINDER_RADIUS = 15.0f;
const f32 DIG_CYLINDER_HEIGHT = 64.0f;
} // namespace

// 0x005A3544 slot 0x14 (name is ours)
void fgobj::ObjectDig::Finalize()
{
    bgcheck::RemoveBody(&m_Cylinder);
    ObjectBase::Finalize();
}

// 0x005A3560 slot 0x20 (name is ours)
s32 fgobj::ObjectDig::Execute()
{
    if (m_Frame >= DIG_FRAME_NUM) {
        return EXECUTE_FINISHED;
    }
    f32 scale = s_DigScales[m_Frame];
    m_Frame++;
    m_Cylinder.SetCylinder(&m_Position, scale * DIG_CYLINDER_RADIUS, DIG_CYLINDER_HEIGHT);
    return EXECUTE_CONTINUE;
}

// 0x005A35F8 slot 0x00
// 0x005A35B4 (deleting dtor)
fgobj::ObjectDig::~ObjectDig()
{
    // (only the members are destroyed)
}

} // namespace fgobj
