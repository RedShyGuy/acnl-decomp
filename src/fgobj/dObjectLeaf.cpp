#include "fgobj/dObjectLeaf.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "fgobj/fgobj_Globals.h"
#include "nn/math/math_Api.h"

namespace fgobj {
namespace {
// the frames of the fall
const u8 LEAF_FRAME_NUM = 50;
// the angle step of the sway per frame and its amplitude in x
const u16 LEAF_SWAY_ANGLE_STEP = 0xA3D;
const f32 LEAF_SWAY_AMPLITUDE = 4.0f;
} // namespace

// 0x00592238 (name is ours)
fgobj::ObjectLeaf* fgobj::ObjectLeaf::Create(const SvFgName& item, s32 x, s32 z, const nn::math::VEC3* offset, u8 direction,
                                             u8 fallDirection)
{
    ObjectLeaf* leaf = NewObject<ObjectLeaf>(s_SmallHeaps, SMALL_HEAP_NUM);
    if (leaf == NULL) {
        return NULL;
    }
    leaf->Setup(item, x, z, false);
    leaf->m_Position.x += offset->x;
    leaf->m_Position.y += offset->y;
    leaf->m_Position.z += offset->z;
    leaf->StartFall(fallDirection);
    leaf->m_Direction = direction;
    leaf->m_Gravity = 0.0f;
    leaf->m_Velocity = (leaf->m_Target - leaf->m_Position) * 0.02f;
    leaf->m_Frame = LEAF_FRAME_NUM;
    leaf->m_PathPosition = leaf->m_Position;
    leaf->UpdateMatrix(false, 0.0f);
    SoundMgr::SPlaySharedSound(SE_OBJ_CMN_FALL_L, &leaf->m_Position, leaf, false);
    return leaf;
}

// 0x00592534 slot 0x00
// 0x00592504 (deleting dtor)
fgobj::ObjectLeaf::~ObjectLeaf()
{
    // (only the base is destroyed)
}

// 0x00592410 slot 0x20 (name is ours)
s32 fgobj::ObjectLeaf::Execute()
{
    m_Velocity.y -= m_Gravity;
    m_PathPosition.x += m_Velocity.x;
    m_PathPosition.y += m_Velocity.y;
    m_PathPosition.z += m_Velocity.z;
    m_Position = m_PathPosition;
    m_Position.x += nn::math::SinIdx(static_cast<u16>((LEAF_FRAME_NUM - m_Frame) * LEAF_SWAY_ANGLE_STEP)) * LEAF_SWAY_AMPLITUDE;
    if (m_Frame != 0) {
        m_Frame--;
        UpdateMatrix(false, 0.0f);
        return EXECUTE_CONTINUE;
    }
    m_Position = m_Target;
    UpdateMatrix(false, 0.0f);
    return EXECUTE_FINISHED;
}

} // namespace fgobj
