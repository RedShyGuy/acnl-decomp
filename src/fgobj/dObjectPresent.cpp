#include "fgobj/dObjectPresent.h"
#include "Other/dSeadParticleHandle.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "field/dFdInfo.h"
#include "fgobj/fgobj_Extern.h"
#include "fgobj/fgobj_Globals.h"
#include "stage/stage_Extern.h"

namespace fgobj {
namespace {
// the start speed upwards and the gravity
const f32 PRESENT_START_SPEED_Y = 1.5f;
const f32 PRESENT_GRAVITY = 0.5f;
// the frames of the bounce (higher than 10 above the unit: longer)
const s16 BOUNCE_FRAMES = 8;
const s16 BOUNCE_FRAMES_HIGH = 24;
const f32 BOUNCE_HIGH = 10.0f;
// the splash on the water
const u32 SPLASH_EFFECT_ID = 582;
} // namespace

// 0x005952C4 (name is ours)
void fgobj::ObjectPresent::StartSplashEffect()
{
    nn::math::VEC3 position = m_Position;
    position.y = FdInfo::GetSurfaceHeight(position, FIELD_NAME_CURRENT);
    s8 index = 0;
    while (true) {
        SeadParticleHandle* free = &s_pParticleHandles[index];
        if (free->GetEmitterSet() == NULL || !free->IsAlive()) {
            break;
        }
        index++;
        if (static_cast<u32>(index) >= PARTICLE_HANDLE_NUM) {
            return;
        }
    }
    if (index == -1) {
        return;
    }
    SeadParticleHandle* handle;
    if (index >= 0 && index < static_cast<s32>(PARTICLE_HANDLE_NUM)) {
        handle = &s_pParticleHandles[index];
    } else {
        handle = NULL;
    }
    if (Func207998(handle, SPLASH_EFFECT_ID, &position, &ANGLE16_VEC3_ZERO, &VEC3_ONE, true, 0)) {
        handle->SetPtclCallback(Func2080B0, -1);
    }
}

// 0x005953D4 (name is ours)
fgobj::ObjectPresent* fgobj::ObjectPresent::Create(const SvFgName& item, s32 x, s32 z, const nn::math::VEC3* position, u8 state)
{
    ObjectPresent* present = NewObject<ObjectPresent>(s_SmallHeaps, SMALL_HEAP_NUM);
    if (present == NULL) {
        return NULL;
    }
    present->Setup(item, x, z, false);
    present->m_Target = present->m_Position;
    present->m_Position = *position;
    present->m_Velocity.x = 0.0f;
    present->m_Velocity.y = PRESENT_START_SPEED_Y;
    present->m_Velocity.z = 0.0f;
    present->m_Gravity = PRESENT_GRAVITY;
    present->m_State = state;
    present->UpdateMatrix(false, 0.0f);
    present->m_FloorHeight = FdInfo::GetHeight(*position, FIELD_NAME_CURRENT);
    present->m_BounceFrames = present->m_FloorHeight - present->m_Target.y > BOUNCE_HIGH ? BOUNCE_FRAMES_HIGH : BOUNCE_FRAMES;
    return present;
}

// 0x00595530 slot 0x20 (name is ours)
s32 fgobj::ObjectPresent::Execute()
{
    m_Velocity.y -= m_Gravity;
    m_Position.x += m_Velocity.x;
    m_Position.y += m_Velocity.y;
    m_Position.z += m_Velocity.z;
    if (m_Position.y > m_FloorHeight) {
        if (m_State == STATE_FALL || m_State == STATE_FALL_TO_NEIGHBOUR || m_State == STATE_FALL_INTO_WATER) {
            SoundMgr::SHoldSharedSound(SE_OBJ_BALLOON_HIT_FALL, &m_Position, this);
        }
    } else {
        m_Position.y = m_FloorHeight;
        m_FloorHeight = m_Target.y;
        switch (m_State) {
        case STATE_FALL:
            SetVelocityToTarget(m_BounceFrames, 1.0f);
            m_State = STATE_BOUNCE;
            break;
        case STATE_BOUNCE:
            PlayLandSound(false);
            Func67D0F8();
            return EXECUTE_FINISHED;
        case STATE_FALL_TO_NEIGHBOUR:
            SetVelocityToTarget(BOUNCE_FRAMES, 1.0f);
            m_State = STATE_BOUNCE_TO_NEIGHBOUR;
            break;
        case STATE_BOUNCE_TO_NEIGHBOUR:
            m_Z = -1;
            m_X = -1;
            PlayLandSound(false);
            Func5C8834();
            return EXECUTE_FINISHED;
        case STATE_FALL_INTO_WATER:
            m_Z = -1;
            m_X = -1;
            Func5C8834();
            StartSplashEffect();
            SoundMgr::SPlaySharedSound(SE_OBJ_CMN_SPLASH_DOWN_M, &m_Position, this, false);
            return EXECUTE_FINISHED;
        default:
            return EXECUTE_FINISHED;
        }
        SoundMgr::SPlaySharedSound(SE_OBJ_BALLOON_HIT_GROUND, &m_Position, this, false);
    }
    UpdateMatrix(false, 0.0f);
    return EXECUTE_CONTINUE;
}

// 0x00595710 slot 0x00
// 0x005956E0 (deleting dtor)
fgobj::ObjectPresent::~ObjectPresent()
{
    // (only the base is destroyed)
}

} // namespace fgobj
