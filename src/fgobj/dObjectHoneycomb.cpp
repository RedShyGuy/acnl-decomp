#include "fgobj/dObjectHoneycomb.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "fgobj/fgobj_Extern.h"
#include "fgobj/fgobj_Globals.h"

namespace fgobj {
namespace {
// the start velocity and the gravity
const f32 HONEYCOMB_START_SPEED_Y = 1.5f;
const f32 HONEYCOMB_START_SPEED_Z = 2.0f;
const f32 HONEYCOMB_GRAVITY = 0.5f;
// the frames of a bounce
const s32 HONEYCOMB_BOUNCE_FRAMES = 8;
// the fade on the neighbour unit
const u8 FADE_FRAMES = 63;
const u8 FADE_ALPHA_STEP = 4;
} // namespace

// 0x00595C7C (name is ours)
fgobj::ObjectHoneycomb* fgobj::ObjectHoneycomb::Create(const SvFgName& item, s32 x, s32 z, const nn::math::VEC3* offset,
                                                       u8 direction)
{
    ObjectHoneycomb* honeycomb = NewObject<ObjectHoneycomb>(s_SmallHeaps, SMALL_HEAP_NUM);
    if (honeycomb == NULL) {
        return NULL;
    }
    honeycomb->Setup(item, x, z, false);
    honeycomb->m_Target = honeycomb->m_Position;
    honeycomb->m_Position.x += offset->x;
    honeycomb->m_Position.y += offset->y;
    honeycomb->m_Position.z += offset->z;
    honeycomb->m_Velocity.x = 0.0f;
    honeycomb->m_Velocity.y = HONEYCOMB_START_SPEED_Y;
    honeycomb->m_Velocity.z = HONEYCOMB_START_SPEED_Z;
    honeycomb->m_State = STATE_FALL;
    honeycomb->m_Direction = direction;
    honeycomb->m_Gravity = HONEYCOMB_GRAVITY;
    honeycomb->UpdateMatrix(false, 0.0f);
    honeycomb->m_FadeFrame = 0;
    SoundMgr::SPlaySharedSound(SE_OBJ_CMN_FALL_M, &honeycomb->m_Position, honeycomb, false);
    honeycomb->m_ShadowFrame = 0;
    return honeycomb;
}

// 0x00595DDC slot 0x20 (name is ours)
s32 fgobj::ObjectHoneycomb::Execute()
{
    if (m_FadeFrame != 0) {
        m_FadeFrame--;
        if (m_FadeFrame == 0) {
            return EXECUTE_FINISHED;
        }
        m_Alpha = m_FadeFrame * FADE_ALPHA_STEP;
        m_Flags |= FLAG_ALPHA_FROM_TEXTURE | FLAG_UNK_20000 | FLAG_ALPHA_STATE;
        return EXECUTE_CONTINUE;
    }
    m_Velocity.y -= m_Gravity;
    m_Position.x += m_Velocity.x;
    m_Position.y += m_Velocity.y;
    m_Position.z += m_Velocity.z;
    if (!(m_Position.y > m_Target.y)) {
        m_Position.y = m_Target.y;
        if (m_State == STATE_FALL) {
            SetVelocityToTarget(HONEYCOMB_BOUNCE_FRAMES, 1.0f);
            m_State = STATE_BOUNCE;
        } else if (m_State == STATE_BOUNCE) {
            PlayLandSound(false);
            Func2AAF80(&m_Position);
            Func5C61E4(&m_Position);
            return EXECUTE_FINISHED;
        } else if (m_State == STATE_FALL_TO_NEIGHBOUR) {
            SetVelocityToTarget(HONEYCOMB_BOUNCE_FRAMES, 1.0f);
            m_State = STATE_BOUNCE_TO_NEIGHBOUR;
        } else if (m_State == STATE_BOUNCE_TO_NEIGHBOUR) {
            m_Z = -1;
            m_X = -1;
            PlayLandSound(false);
            m_Position = m_Target;
            UpdateMatrix(false, 0.0f);
            m_FadeFrame = FADE_FRAMES;
            Func2AAF80(&m_Position);
            Func5C61E4(&m_Position);
            return EXECUTE_CONTINUE;
        } else {
            return EXECUTE_FINISHED;
        }
        PlayLandSound(true);
    }
    UpdateShadow();
    UpdateMatrix(false, 0.0f);
    return EXECUTE_CONTINUE;
}

// 0x00595FE4 slot 0x00
// 0x00595FB4 (deleting dtor)
fgobj::ObjectHoneycomb::~ObjectHoneycomb()
{
    // (only the base is destroyed)
}

} // namespace fgobj
