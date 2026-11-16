#include "fgobj/dObjectHanabi.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "fgobj/fgobj_Api.h"
#include "fgobj/fgobj_Extern.h"
#include "stage/stage_Api.h"
#include "stage/stage_Extern.h"

namespace fgobj {
namespace {
// the frames of the wait and of the burning, the effect
const s16 WAIT_FRAMES = 20;
const s16 BURN_FRAMES = 300;
const u32 BURN_EFFECT_ID = 530;
} // namespace

// 0x00595010 slot 0x14 (name is ours)
void fgobj::ObjectHanabi::Finalize()
{
    if (m_Effect.GetEmitterSet() != NULL) {
        m_Effect.Kill();
    }
    if (m_HoldsRequest == true) {
        FinishPlaceRequest(m_X, m_Z, stage::GetCurrent());
    }
    ObjectBase::Finalize();
}

// 0x00595068 slot 0x20 (name is ours)
s32 fgobj::ObjectHanabi::Execute()
{
    if (m_State == STATE_THROW) {
        if (Update()) {
            m_Frame = WAIT_FRAMES;
            m_State = STATE_WAIT;
            m_Position = m_Target;
            UpdateMatrix(false, 0.0f);
        }
        return EXECUTE_CONTINUE;
    }
    if (m_State == STATE_WAIT) {
        if (m_Frame > 0) {
            m_Frame--;
            return EXECUTE_CONTINUE;
        }
        m_Frame = BURN_FRAMES;
        m_State = STATE_BURN;
        m_Position = m_Target;
        UpdateMatrix(false, 0.0f);
        Func207998(&m_Effect, BURN_EFFECT_ID, &m_Position, &ANGLE16_VEC3_ZERO, &VEC3_ONE, false, 0);
        SoundMgr::SPlaySharedSound(SE_ACT_PUT_FIRE, &m_Position, this, false);
        return EXECUTE_CONTINUE;
    }
    s32 x = m_X;
    s32 z = m_Z;
    for (s32 dz = -1; dz <= 1; dz++) {
        for (s32 dx = -1; dx <= 1; dx++) {
            g_UnitBitsAAF736.Set(x + dx, z + dz);
        }
    }
    if (m_Frame > 0) {
        m_Frame--;
        return EXECUTE_CONTINUE;
    }
    m_HoldsRequest = false;
    return EXECUTE_FINISHED;
}

// 0x00595284 slot 0x00
// 0x00595240 (deleting dtor)
fgobj::ObjectHanabi::~ObjectHanabi()
{
    // (only the members are destroyed)
}

} // namespace fgobj
