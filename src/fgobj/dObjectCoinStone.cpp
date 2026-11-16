#include "fgobj/dObjectCoinStone.h"
#include "nn/math/math_Api.h"

namespace fgobj {
namespace {
// the step of the phase and the amplitude of the shake
const s16 SHAKE_PHASE_STEP = 0x1000;
const f32 SHAKE_AMPLITUDE = 8.0f;
} // namespace

// 0x00595930 slot 0x20 (name is ours)
s32 fgobj::ObjectCoinStone::Execute()
{
    m_Phase += SHAKE_PHASE_STEP;
    if (m_Phase < 0) {
        return EXECUTE_FINISHED;
    }
    f32 offset = nn::math::SinIdx(m_Phase) * SHAKE_AMPLITUDE;
    m_Position.x = m_UnitPosition.x + offset * nn::math::SinIdx(m_Direction);
    m_Position.z = m_UnitPosition.z + offset * nn::math::CosIdx(m_Direction);
    UpdateMatrix(false, 0.0f);
    return EXECUTE_CONTINUE;
}

// 0x005959F8 slot 0x00
// 0x005959C8 (deleting dtor)
fgobj::ObjectCoinStone::~ObjectCoinStone()
{
    // (only the base is destroyed)
}

} // namespace fgobj
