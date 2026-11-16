#include "fgobj/dObjectStumpAnim.h"
#include "Item/item_Api.h"
#include "Other/dWorldCurve.h"
#include "Sound/sound_Extern.h"
#include "Sv/dSvFgName.h"
#include "fgobj/dObjectSwing.h"
#include "fgobj/dProc.h"
#include "fgobj/fgobj_Api.h"
#include "fgobj/fgobj_Extern.h"
#include "fgobj/fgobj_Globals.h"
#include "math/dAngle16.h"
#include "nn/math/math_Api.h"
#include "stage/stage_Extern.h"

namespace fgobj {
namespace {
// the flight: the delay before the hole, its frames, the start speed upwards and the gravity
const u8 STUMP_ANIM_DELAY = 30;
const u8 STUMP_ANIM_FLIGHT_FRAMES = 15;
const f32 STUMP_ANIM_START_SPEED_Y = 4.0f;
const f32 STUMP_ANIM_GRAVITY = 0.5f;
// the bone of the effects and the offsets of the bushes by size
const char* const EFFECT_BONE_NAME = "j_main";
const f32 SMALL_BUSH_EFFECT_OFFSET_Y = 15.0f;
const f32 BUSH_EFFECT_OFFSET_Z = 10.0f;
const f32 BUSH_EFFECT_OFFSET_Y = 5.0f;
// the effects: scales and intervals of the small bushes, the emit rate, the leaf effect
const f32 SMALL_BUSH_SCALE = 0.5f;
const f32 SMALL_AZALEA_SCALE = 0.7f;
const f32 SMALL_BUSH_INTERVAL = 3.0f;
const f32 EFFECT_RATE = 3.0f;
const u32 LEAF_EFFECT_ID = 211;
const f32 LEAF_EFFECT_SCALE = 0.5f;
// the animations of the bushes by size
const char* const BUSH_ANIM_NAME_1 = "fg_bush_1";
const char* const BUSH_ANIM_NAME_2 = "fg_bush_2";
// the flower effects (particle IDs) and the season parts (Func1E6444(5)) in which the bushes bloom
const u32 FLOWER_EFFECT_HOLLY = 137;
const u32 FLOWER_EFFECT_PINK_AZALEA = 42;
const u32 FLOWER_EFFECT_WHITE_AZALEA = 43;
const u32 FLOWER_EFFECT_BLUE_HYDRANGEA = 138;
const u32 FLOWER_EFFECT_PINK_HYDRANGEA = 139;
const u32 FLOWER_EFFECT_RED_HIBISCUS = 135;
const u32 FLOWER_EFFECT_YELLOW_HIBISCUS = 136;
const u32 FLOWER_EFFECT_SWEET_OLIVE = 134;
const s32 HOLLY_SEASON_END = 14;
const u32 AZALEA_SEASON_START = 3;
const u32 AZALEA_SEASON_LENGTH = 2;
const u32 HYDRANGEA_SEASON_START = 6;
const u32 HYDRANGEA_SEASON_LENGTH = 1;
const u32 HIBISCUS_SEASON_START = 8;
const u32 HIBISCUS_SEASON_LENGTH = 2;
const u32 SWEET_OLIVE_SEASON_START = 11;
const u32 SWEET_OLIVE_SEASON_LENGTH = 2;
} // namespace

// (name is ours)
// 0x009500DC
const s32 fgobj::ObjectStumpAnim::s_LeafEffects[5][18] = {
    {44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61},
    {62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79},
    {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97},
    {98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115},
    {116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133},
};

// 0x00596010 (name is ours)
bool fgobj::ObjectStumpAnim::GetEffectPosition(nn::math::VEC3* out) const
{
    s32 bone = m_Model.GetBoneIndex(EFFECT_BONE_NAME);
    if (bone < 0) {
        return false;
    }
    nn::math::VEC3 position;
    if (!m_Model.GetBoneWorldPosition(&position, bone)) {
        return false;
    }
    Func52EA98(out, &position);
    u8 size = item::GetParamFgEntry(m_Item)->size;
    if (size == 1) {
        if (!m_Item.IsName(SvFgName::NAME_PINK_AZALEA_BUSH_GROWING_2) &&
            !m_Item.IsName(SvFgName::NAME_WHITE_AZALEA_BUSH_GROWING_2)) {
            out->y += SMALL_BUSH_EFFECT_OFFSET_Y;
        }
    } else if (size == 2) {
        if (m_Item.IsName(SvFgName::NAME_PINK_AZALEA_BUSH) || m_Item.IsName(SvFgName::NAME_WHITE_AZALEA_BUSH)) {
            out->z += BUSH_EFFECT_OFFSET_Z;
        } else {
            out->y -= BUSH_EFFECT_OFFSET_Y;
        }
    }
    return true;
}

// 0x00596128 (name is ours)
void fgobj::ObjectStumpAnim::StartEffects()
{
    s32 effect = GetEffectId();
    if (effect == -1) {
        return;
    }
    nn::math::VEC3 position;
    if (!GetEffectPosition(&position)) {
        return;
    }
    Func207998(&m_Effect, effect, &position, &ANGLE16_VEC3_ZERO, &VEC3_ONE, true, 0);
    f32 scale;
    if (item::GetParamFgEntry(m_Item)->size == 1) {
        if (m_Item.IsName(SvFgName::NAME_PINK_AZALEA_BUSH_GROWING_2) ||
            m_Item.IsName(SvFgName::NAME_WHITE_AZALEA_BUSH_GROWING_2)) {
            scale = SMALL_AZALEA_SCALE;
        } else {
            scale = SMALL_BUSH_SCALE;
        }
    } else {
        scale = 1.0f;
    }
    nn::math::VEC3 scales(scale, scale, scale);
    m_Effect.SetEmitterVolumeScale(&scales);
    f32 interval = 1.0f;
    if (item::GetParamFgEntry(m_Item)->size == 1) {
        interval = SMALL_BUSH_INTERVAL;
    }
    m_Effect.SetEmitInterval(interval);
    m_Effect.SetEmitRate(EFFECT_RATE);
    if (Func63061C()) {
        Func207998(&m_LeafEffect, LEAF_EFFECT_ID, &position, &ANGLE16_VEC3_ZERO, &VEC3_ONE, true, 0);
        // 0x0098B470 (guard 0x0094FE90)
        static const nn::math::VEC3 s_LeafEffectScale(LEAF_EFFECT_SCALE, LEAF_EFFECT_SCALE, LEAF_EFFECT_SCALE);
        m_LeafEffect.SetEmitterVolumeScale(&s_LeafEffectScale);
        m_LeafEffect.SetEmitInterval(interval);
    }
    u32 season = Func1E6444(5);
    u32 flower;
    if (m_Item.IsName(SvFgName::NAME_HOLLY_BUSH)) {
        if (season != 0 && static_cast<s32>(season) < HOLLY_SEASON_END) {
            return;
        }
        flower = FLOWER_EFFECT_HOLLY;
    } else if (m_Item.IsName(SvFgName::NAME_PINK_AZALEA_BUSH)) {
        if (season - AZALEA_SEASON_START > AZALEA_SEASON_LENGTH) {
            return;
        }
        flower = FLOWER_EFFECT_PINK_AZALEA;
    } else if (m_Item.IsName(SvFgName::NAME_WHITE_AZALEA_BUSH)) {
        if (season - AZALEA_SEASON_START > AZALEA_SEASON_LENGTH) {
            return;
        }
        flower = FLOWER_EFFECT_WHITE_AZALEA;
    } else if (m_Item.IsName(SvFgName::NAME_BLUE_HYDRANGEA_BUSH)) {
        if (season - HYDRANGEA_SEASON_START > HYDRANGEA_SEASON_LENGTH) {
            return;
        }
        flower = FLOWER_EFFECT_BLUE_HYDRANGEA;
    } else if (m_Item.IsName(SvFgName::NAME_PINK_HYDRANGEA_BUSH)) {
        if (season - HYDRANGEA_SEASON_START > HYDRANGEA_SEASON_LENGTH) {
            return;
        }
        flower = FLOWER_EFFECT_PINK_HYDRANGEA;
    } else if (m_Item.IsName(SvFgName::NAME_RED_HIBISCUS_BUSH)) {
        if (season - HIBISCUS_SEASON_START > HIBISCUS_SEASON_LENGTH) {
            return;
        }
        flower = FLOWER_EFFECT_RED_HIBISCUS;
    } else if (m_Item.IsName(SvFgName::NAME_YELLOW_HIBISCUS_BUSH)) {
        if (season - HIBISCUS_SEASON_START > HIBISCUS_SEASON_LENGTH) {
            return;
        }
        flower = FLOWER_EFFECT_YELLOW_HIBISCUS;
    } else if (m_Item.IsName(SvFgName::NAME_SWEET_OLIVE_BUSH)) {
        if (season - SWEET_OLIVE_SEASON_START > SWEET_OLIVE_SEASON_LENGTH) {
            return;
        }
        flower = FLOWER_EFFECT_SWEET_OLIVE;
    } else {
        return;
    }
    Func207998(&m_FlowerEffect, flower, &position, &ANGLE16_VEC3_ZERO, &VEC3_ONE, true, 0);
    m_FlowerEffect.SetEmitterVolumeScale(&scales);
    m_FlowerEffect.SetEmitInterval(interval);
}

// 0x005964C8 (name is ours)
void fgobj::ObjectStumpAnim::Setup(const SvFgName& item, const s32* x, const s32* z)
{
    m_Item = item;
    const void* model = GetAnimModel(this);
    Initialize(model, *x, *z, false, 0, true);
    const item::ParamFgEntry* param = item::GetParamFgEntry(m_Item);
    u32 index = param->fgModel;
    FgModel* fgModel;
    if (index >= Proc::FG_MODEL_NUM) {
        index = 0;
    }
    if (s_IsOutdoor && s_pProc != NULL && s_pProc->GetFgModels() != NULL && index < Proc::FG_MODEL_NUM) {
        fgModel = &s_pProc->GetFgModels()[index];
    } else {
        fgModel = NULL;
    }
    const char* name = fgModel->name;
    fgModel->nameEnd = 0;
    if (m_Item.IsBush()) {
        name = param->size >= 2 ? BUSH_ANIM_NAME_2 : BUSH_ANIM_NAME_1;
    }
    const void* anim = GetFgSkeletalAnim(name, ObjectSwing::ANIMATION_SHAKE_M);
    m_SkeletalAnim.Build(&m_Model, nw::anim::res::ResAnim(anim), &m_Allocator, 0);
    m_Model.SetSkeletalAnim(&m_SkeletalAnim);
    SetSortKey(*z, 0, -1, false, true, true, false);
}

// 0x00596600 (name is ours)
s32 fgobj::ObjectStumpAnim::GetEffectId() const
{
    s32 season = Func1E6444(5);
    if (m_Item.IsName(SvFgName::NAME_HOLLY_BUSH) || m_Item.IsName(SvFgName::NAME_HOLLY_BUSH_GROWING_2)) {
        return s_LeafEffects[0][season];
    }
    if (m_Item.IsName(SvFgName::NAME_PINK_AZALEA_BUSH) || m_Item.IsName(SvFgName::NAME_WHITE_AZALEA_BUSH) ||
        m_Item.IsName(SvFgName::NAME_PINK_AZALEA_BUSH_GROWING_2) || m_Item.IsName(SvFgName::NAME_WHITE_AZALEA_BUSH_GROWING_2)) {
        return s_LeafEffects[1][season];
    }
    if (m_Item.IsName(SvFgName::NAME_PINK_HYDRANGEA_BUSH) || m_Item.IsName(SvFgName::NAME_BLUE_HYDRANGEA_BUSH) ||
        m_Item.IsName(SvFgName::NAME_PINK_HYDRANGEA_BUSH_GROWING_2) ||
        m_Item.IsName(SvFgName::NAME_BLUE_HYDRANGEA_BUSH_GROWING_2)) {
        return s_LeafEffects[2][season];
    }
    if (m_Item.IsName(SvFgName::NAME_RED_HIBISCUS_BUSH) || m_Item.IsName(SvFgName::NAME_YELLOW_HIBISCUS_BUSH) ||
        m_Item.IsName(SvFgName::NAME_RED_HIBISCUS_BUSH_GROWING_2) || m_Item.IsName(SvFgName::NAME_YELLOW_HIBISCUS_BUSH_GROWING_2)) {
        return s_LeafEffects[3][season];
    }
    if (m_Item.IsName(SvFgName::NAME_SWEET_OLIVE_BUSH) || m_Item.IsName(SvFgName::NAME_SWEET_OLIVE_BUSH_GROWING_2)) {
        return s_LeafEffects[4][season];
    }
    return -1;
}

// 0x005967D0 (name is ours)
fgobj::ObjectStumpAnim* fgobj::ObjectStumpAnim::Create(const SvFgName& item, const s32* x, const s32* z, const math::Angle16* angle)
{
    ObjectStumpAnim* stump = NewObject<ObjectStumpAnim>(s_LargeHeaps, LARGE_HEAP_NUM);
    if (stump == NULL) {
        return NULL;
    }
    stump->Setup(item, x, z);
    stump->m_Delay = STUMP_ANIM_DELAY;
    stump->m_FlightFrame = STUMP_ANIM_FLIGHT_FRAMES;
    stump->m_Velocity.x = nn::math::SinIdx(angle->value);
    stump->m_Velocity.z = nn::math::CosIdx(angle->value);
    stump->m_Velocity.y = STUMP_ANIM_START_SPEED_Y;
    stump->m_Gravity = STUMP_ANIM_GRAVITY;
    stump->m_Alpha = 255;
    stump->InitializeShadow();
    stump->StartEffects();
    return stump;
}

// 0x00596908 slot 0x14 (name is ours)
void fgobj::ObjectStumpAnim::Finalize()
{
    if (m_Effect.GetEmitterSet() != NULL) {
        m_Effect.Kill();
    }
    if (m_LeafEffect.GetEmitterSet() != NULL) {
        m_LeafEffect.Kill();
    }
    if (m_FlowerEffect.GetEmitterSet() != NULL) {
        m_FlowerEffect.Kill();
    }
    ObjectBase::Finalize();
}

// 0x0059697C slot 0x20 (name is ours)
s32 fgobj::ObjectStumpAnim::Execute()
{
    m_Model.UpdateAnims();
    if (Update()) {
        return EXECUTE_FINISHED_KEEP_REQUEST;
    }
    if (m_Effect.GetEmitterSet() != NULL) {
        nn::math::VEC3 position;
        if (GetEffectPosition(&position)) {
            m_Effect.SetTransform(&position, &ANGLE16_VEC3_ZERO, true);
            if (m_LeafEffect.GetEmitterSet() != NULL) {
                m_LeafEffect.SetTransform(&position, &ANGLE16_VEC3_ZERO, true);
            }
            if (m_FlowerEffect.GetEmitterSet() != NULL) {
                m_FlowerEffect.SetTransform(&position, &ANGLE16_VEC3_ZERO, true);
            }
        }
    }
    return EXECUTE_CONTINUE;
}

// 0x00596AB4 slot 0x00
// 0x00596A58 (deleting dtor)
fgobj::ObjectStumpAnim::~ObjectStumpAnim()
{
    // (only the members are destroyed)
}

} // namespace fgobj
