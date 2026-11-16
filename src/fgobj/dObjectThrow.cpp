#include "fgobj/dObjectThrow.h"
#include "Item/item_Api.h"
#include "fgobj/fgobj_Extern.h"
#include "fgobj/fgobj_Globals.h"
#include "netgame/netgame_Api.h"

namespace fgobj {
namespace {
// the flight: the frames, the part of the way per frame, the gravity
const s16 THROW_FRAMES = 15;
const f32 THROW_RATIO = 0.8125f;
const f32 THROW_GRAVITY = 0.5f;
// the bounce
const s16 BOUNCE_FRAMES = 5;
// the scale grows by this per frame
const f32 THROW_SCALE_STEP = 0.1f;
// the shadow: at the start, landed, and it grows in the last frames of the flight
const f32 SHADOW_START_SIZE = 4.0f;
const f32 SHADOW_SIZE = 8.0f;
const s16 SHADOW_FRAMES = 10;
} // namespace

// 0x00594900 (name is ours)
bool fgobj::ObjectThrow::Update()
{
    m_Scale += THROW_SCALE_STEP;
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
    if (m_Frame == 0) {
        if (m_IsBounced) {
            PlayLandSound(false);
            return true;
        }
        m_Frame = BOUNCE_FRAMES;
        SetVelocityToTarget(BOUNCE_FRAMES, 1.0f);
        m_IsBounced = true;
        m_ShadowSize = SHADOW_SIZE;
        m_Shadow.m_Opacity = 1.0f;
        PlayLandSound(true);
    }
    if (!m_IsBounced && m_Frame < SHADOW_FRAMES) {
        f32 t = m_Frame * THROW_SCALE_STEP;
        m_ShadowSize = (1.0f - t * 0.5f) * SHADOW_SIZE;
        f32 opacity = 1.0f - t;
        if (opacity < 0.0f) {
            opacity = 0.0f;
        } else if (opacity > 1.0f) {
            opacity = 1.0f;
        }
        m_Shadow.m_Opacity = opacity;
    }
    UpdateMatrix(false, 0.0f);
    return false;
}

// 0x00594AA4 (name is ours)
void fgobj::ObjectThrow::StartThrow(const nn::math::VEC3* from, f32 scale)
{
    m_Target = m_Position;
    m_Position = *from;
    m_Scale = scale;
    nn::math::VEC3 scales(scale, scale, scale);
    m_Model.SetScale(scales);
    m_Frame = THROW_FRAMES;
    m_Gravity = THROW_GRAVITY;
    SetVelocityToTarget(THROW_FRAMES, THROW_RATIO);
    m_IsBounced = false;
    InitializeShadow();
    m_ShadowSize = SHADOW_START_SIZE;
    m_Shadow.m_Opacity = 0.0f;
}

// 0x00594B58 slot 0x20 (name is ours)
s32 fgobj::ObjectThrow::Execute()
{
    if (!Update()) {
        return EXECUTE_CONTINUE;
    }
    if (s_IsOutdoor && !netgame::IsMultiplePlayer()) {
        u32 kind = m_Item.GetKind();
        if (kind == item::KIND_CANDY || kind == item::KIND_LOLLIPOP || kind == item::KIND_BAD_FRUIT) {
            if (!s_Unk950530) {
                s_Unk950530 = true;
            }
        } else if (kind == item::KIND_DUST) {
            s_Unk950531 = true;
        } else if (kind == item::KIND_BAD_KABU) {
            if (!s_Unk950530) {
                s_Unk950530 = true;
            }
            s_Unk950531 = true;
        }
    }
    return EXECUTE_FINISHED;
}

// 0x00594C20 slot 0x00
// 0x00594BF0 (deleting dtor)
fgobj::ObjectThrow::~ObjectThrow()
{
    // (only the base is destroyed)
}

} // namespace fgobj
