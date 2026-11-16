#include "fgobj/dObjectFruit.h"
#include "Item/item_Api.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "fgobj/fgobj_Globals.h"
#include "math/dAngle16.h"

namespace fgobj {
namespace {
// the start velocity (z: the directions to z + 1 go faster) and the gravity
const f32 FRUIT_START_SPEED_Y = 1.5f;
const f32 FRUIT_START_SPEED_Z = 0.15f;
const f32 FRUIT_START_SPEED_Z_FORWARD = 2.0f;
const f32 FRUIT_GRAVITY = 0.5f;
// the frames of a bounce
const s32 FRUIT_BOUNCE_FRAMES = 8;
} // namespace

// 0x00592664 (name is ours)
fgobj::ObjectFruit* fgobj::ObjectFruit::Create(const SvFgName& item, s32 x, s32 z, const nn::math::VEC3* offset, f32 angle,
                                               u8 direction, bool isMirrored)
{
    ObjectFruit* fruit = NewObject<ObjectFruit>(s_SmallHeaps, SMALL_HEAP_NUM);
    if (fruit == NULL) {
        return NULL;
    }
    fruit->Setup(item, x, z, false);
    fruit->m_Target = fruit->m_Position;
    fruit->m_Position.x += offset->x;
    fruit->m_Position.y += offset->y;
    fruit->m_Position.z += offset->z;
    fruit->m_Velocity.x = 0.0f;
    fruit->m_Velocity.y = FRUIT_START_SPEED_Y;
    if (direction == DIRECTION_Z_PLUS || direction == DIRECTION_X_MINUS_Z_PLUS || direction == DIRECTION_X_PLUS_Z_PLUS) {
        fruit->m_Velocity.z = FRUIT_START_SPEED_Z_FORWARD;
    } else {
        fruit->m_Velocity.z = FRUIT_START_SPEED_Z;
    }
    fruit->m_Gravity = FRUIT_GRAVITY;
    fruit->m_Direction = direction;
    fruit->m_State = STATE_FALL;
    if (isMirrored) {
        nn::math::VEC3 scale(-1.0f, 1.0f, 1.0f);
        fruit->m_Model.SetScale(scale);
    }
    fruit->m_RotateZ = static_cast<s32>(angle * math::DEG_TO_ANGLE16);
    fruit->UpdateMatrix(false, 0.0f);
    if (direction == DIRECTION_X_PLUS || direction == DIRECTION_X_MINUS_Z_PLUS || direction == DIRECTION_X_PLUS_Z_PLUS) {
        fruit->m_IsFallSoundRequested = true;
    } else {
        fruit->m_IsFallSoundRequested = false;
    }
    fruit->m_ShadowFrame = 0;
    return fruit;
}

// 0x00592A44 slot 0x00
// 0x00592A14 (deleting dtor)
fgobj::ObjectFruit::~ObjectFruit()
{
    // (only the base is destroyed)
}

// 0x00592860 slot 0x20 (name is ours)
s32 fgobj::ObjectFruit::Execute()
{
    if (m_IsFallSoundRequested) {
        u32 kind = m_Item.GetKind();
        if (kind == item::KIND_FRUIT || kind == item::KIND_GOOD_FRUIT) {
            SoundMgr::SPlaySharedSound(SE_OBJ_CMN_FALL_M, &m_Position, this, false);
        } else {
            SoundMgr::SPlaySharedSound(SE_OBJ_CMN_FALL_S, &m_Position, this, false);
        }
        m_IsFallSoundRequested = false;
    }
    m_Velocity.y -= m_Gravity;
    m_Position.x += m_Velocity.x;
    m_Position.y += m_Velocity.y;
    m_Position.z += m_Velocity.z;
    if (!(m_Position.y > m_Target.y)) {
        m_Position.y = m_Target.y;
        switch (m_State) {
        case STATE_FALL:
            SetVelocityToTarget(FRUIT_BOUNCE_FRAMES, 1.0f);
            m_State = STATE_BOUNCE;
            break;
        case STATE_BOUNCE:
            PlayLandSound(false);
            return EXECUTE_FINISHED;
        case STATE_FALL_TO_NEIGHBOUR:
            SetVelocityToTarget(FRUIT_BOUNCE_FRAMES, 1.0f);
            m_State = STATE_BOUNCE_TO_NEIGHBOUR;
            break;
        case STATE_BOUNCE_TO_NEIGHBOUR:
            m_Z = -1;
            m_X = -1;
            PlayLandSound(false);
            return EXECUTE_FINISHED;
        default:
            return EXECUTE_FINISHED;
        }
        PlayLandSound(true);
        nn::math::VEC3 scale(1.0f, 1.0f, 1.0f);
        m_Model.SetScale(scale);
        m_RotateZ = 0;
    }
    UpdateShadow();
    UpdateMatrix(false, 0.0f);
    return EXECUTE_CONTINUE;
}

} // namespace fgobj
