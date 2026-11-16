#include "fgobj/dObjectStump.h"
#include "Sv/dSvFgName.h"
#include "fgobj/fgobj_Exe.h"
#include "fgobj/fgobj_Globals.h"
#include "math/dAngle16.h"
#include "nn/math/math_Api.h"

namespace fgobj {
namespace {
// the frames before the hole / of the flight / of the fade at its end
const u8 STUMP_DELAY = 30;
const u8 STUMP_FLIGHT_FRAMES = 15;
const u8 STUMP_FADE_FRAMES = 5;
const u8 STUMP_FADE_ALPHA_STEP = 50;
// the start speed upwards and the gravity of the flight
const f32 STUMP_START_SPEED_Y = 4.0f;
const f32 STUMP_GRAVITY = 0.5f;
} // namespace

// 0x00593178 (name is ours)
bool fgobj::ObjectStump::Update()
{
    if (m_Delay != 0) {
        m_Delay--;
        if (m_Delay == 0) {
            SvFgName hole(static_cast<SvFgName::ID>(SvFgName::NAME_HOLE));
            PlaceScale(hole, m_X, m_Z, false);
        }
        UpdateMatrix(false, 0.0f);
        return false;
    }
    if (m_FlightFrame == 0) {
        return true;
    }
    m_FlightFrame--;
    m_Velocity.y -= m_Gravity;
    m_Position.x += m_Velocity.x;
    m_Position.y += m_Velocity.y;
    m_Position.z += m_Velocity.z;
    UpdateMatrix(false, 0.0f);
    if (m_FlightFrame < STUMP_FADE_FRAMES) {
        m_Alpha = m_FlightFrame * STUMP_FADE_ALPHA_STEP;
        m_Flags |= FLAG_ALPHA_STATE;
        f32 opacity = m_Alpha * (1.0f / 255.0f);
        if (opacity < 0.0f) {
            opacity = 0.0f;
        } else if (opacity > 1.0f) {
            opacity = 1.0f;
        }
        m_Shadow.m_Opacity = opacity;
    }
    return false;
}

// 0x00593328 (name is ours)
fgobj::ObjectStump* fgobj::ObjectStump::Create(const SvFgName& item, const s32* x, const s32* z, const math::Angle16* angle)
{
    ObjectStump* stump = NewObject<ObjectStump>(s_SmallHeaps, SMALL_HEAP_NUM);
    if (stump == NULL) {
        return NULL;
    }
    stump->Setup(item, *x, *z, false);
    stump->m_Delay = STUMP_DELAY;
    stump->m_FlightFrame = STUMP_FLIGHT_FRAMES;
    stump->m_Velocity.x = nn::math::SinIdx(angle->value);
    stump->m_Velocity.z = nn::math::CosIdx(angle->value);
    stump->m_Velocity.y = STUMP_START_SPEED_Y;
    stump->m_Gravity = STUMP_GRAVITY;
    stump->m_Alpha = 255;
    stump->InitializeShadow();
    return stump;
}

// 0x00593438 slot 0x20 (name is ours)
s32 fgobj::ObjectStump::Execute()
{
    if (Update()) {
        return EXECUTE_FINISHED_KEEP_REQUEST;
    }
    return EXECUTE_CONTINUE;
}

// 0x0059347C slot 0x00
// 0x0059344C (deleting dtor)
fgobj::ObjectStump::~ObjectStump()
{
    // (only the base is destroyed)
}

} // namespace fgobj
