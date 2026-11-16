#include "fgobj/dObjectSwing.h"
#include "Item/item_Api.h"
#include "Other/dWorldCurve.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "Sound/sound_Extern.h"
#include "Sv/dSvFgName.h"
#include "field/dFdInfo.h"
#include "field/dField.h"
#include "fgobj/dProc.h"
#include "fgobj/fgobj_Api.h"
#include "fgobj/fgobj_Extern.h"
#include "fgobj/fgobj_Globals.h"
#include "math/dAngle16.h"
#include "stage/stage_Extern.h"

namespace fgobj {
namespace {
// the frames of the fall: the swing sound, the start of the fade (alpha 25 per frame)
const u8 SWING_SOUND_FRAME = 8;
const u8 FADE_START_FRAME = 10;
const u8 FADE_ALPHA_STEP = 25;
// the shadow fades between these frames of the animation
const f32 SHADOW_FADE_START = 10.0f;
const f32 SHADOW_FADE_END = 22.0f;
// the bushes: the effect position and the special models
const f32 BUSH_EFFECT_OFFSET_Y = 5.0f;
const f32 BUSH_EFFECT_OFFSET_Z = 10.0f;
// IsFallBlocked: the unit is too high
const f32 FALL_BLOCK_HEIGHT = 32.0f;
// the leaf effect and the default scale of its volume
const u32 LEAF_EFFECT_ID = 211;
const f32 LEAF_EFFECT_SCALE = 0.6f;
// the sizes of the sounds of the bamboo start at 1
const s16 BAMBOO_SOUND_VALUE_OFFSET = 1;
// the value of the sound of the leaves of a bush that fall all at once
const s16 FALL_ALL_SOUND_VALUE = 4;

// the bones of the effects
const char* const EFFECT_BONE_NAME = "j_main";
// the special models of the bushes (GetModel)
const char* const BUSH_B_MODEL_NAME = "fg_bushB_2";
const char* const BUSH_C_MODEL_NAME = "fg_bushC_2";
const char* const BUSH_D_MODEL_NAME = "fg_bushD_2";
// the animations of the bushes by size
const char* const BUSH_ANIM_NAME_1 = "fg_bush_1";
const char* const BUSH_ANIM_NAME_2 = "fg_bush_2";
} // namespace

// the bones of the effects of the cedars by size - 1
// 0x0094FFFC
static const char* const s_SwingCedarBones[4] = {"B_1_0", "B_2_1", "B_3_2", "B_4_2"};

// the effects (particle IDs) by the index of Func1E6444: bushes that lose all leaves (column 1), trees (column 1, column
// 2 for the trees with bit 2 of ParamFgEntry::unk_0xF), cedars (column 3), palms and bamboo (column 4)
// 0x0094FFB0
static const s32 s_SwingFallAllEffects[19] = {0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9,
                                              0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2};
// 0x0095000C
static const s32 s_SwingTreeEffects[19] = {0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95,
                                           0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E};
// 0x00950058
static const s32 s_SwingCedarEffects[11] = {0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9};
// 0x00950084
static const s32 s_SwingPalmEffects[11] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4};
// 0x009500B0
static const s32 s_SwingBambooEffects[11] = {0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF};

namespace {
// by ParamFgEntry::size (bamboo: 4 sizes): the height of the effects above the bone
// 0x008477C4
const f32 s_TreeEffectHeights[5] = {0.0f, 13.0f, 18.0f, 28.0f, 38.0f};
// 0x008477D8
const f32 s_CedarEffectHeights[5] = {0.0f, -10.0f, 0.0f, -10.0f, 0.0f};
// 0x008477EC
const f32 s_PalmEffectHeights[5] = {0.0f, 5.0f, 27.0f, 25.0f, 40.0f};
// 0x00847800
const f32 s_BambooEffectHeights[4] = {0.0f, 12.0f, 30.0f, 45.0f};
// the scale of the volume of the effect of StartEffect
// 0x00847810
const f32 s_TreeEffectScales[5] = {1.0f, 0.35f, 0.65f, 0.75f, 1.0f};
// 0x00847824
const f32 s_CedarEffectScales[5] = {1.0f, 0.4f, 0.5f, 0.75f, 1.0f};
// 0x00847838
const f32 s_PalmEffectScales[5] = {1.0f, 0.4f, 0.6f, 0.8f, 1.0f};
// 0x0084784C
const f32 s_BambooEffectScales[4] = {1.0f, 1.0f, 0.8f, 1.0f};
// the scale of the volume of the leaf effect
// 0x0084785C
const f32 s_CedarLeafScales[5] = {1.0f, 0.1f, 0.2f, 0.35f, 0.6f};
// 0x00847870
const f32 s_TreeLeafScales[5] = {1.0f, 0.35f, 0.3f, 0.4f, 0.6f};
// 0x00847884
const f32 s_PalmLeafScales[5] = {1.0f, 0.1f, 0.2f, 0.35f, 0.6f};
// 0x00847898
const f32 s_BambooLeafScales[4] = {1.0f, 0.3f, 0.4f, 0.6f};
// the emit rate of the effect when cut
// 0x008478A8
const f32 s_TreeCutRates[5] = {1.0f, 5.0f, 5.0f, 3.0f, 3.0f};
// 0x008478BC
const f32 s_CedarCutRates[5] = {1.0f, 5.0f, 5.0f, 3.0f, 5.0f};
// 0x008478D0
const f32 s_PalmCutRates[5] = {1.0f, 5.0f, 2.0f, 2.0f, 2.0f};
// 0x008478E4
const f32 s_BambooCutRates[4] = {1.0f, 1.0f, 2.0f, 2.0f};
// the emit interval by kind (shake L, shake L wait, shake M, the others)
// 0x008478F4
const f32 s_TreeIntervalsShakeL[5] = {1.0f, 2.0f, 1.0f, 0.7f, 0.25f};
// 0x00847908
const f32 s_TreeIntervalsShakeLWait[5] = {1.0f, 2.0f, 2.0f, 0.2f, 0.2f};
// 0x0084791C
const f32 s_TreeIntervalsShakeM[5] = {1.0f, 2.0f, 0.3f, 0.3f, 0.2f};
// 0x00847930
const f32 s_TreeIntervals[5] = {1.0f, 2.0f, 0.5f, 0.4f, 0.25f};
// 0x00847944
const f32 s_CedarIntervalsShakeL[5] = {1.0f, 2.0f, 1.0f, 0.75f, 0.25f};
// 0x00847958
const f32 s_CedarIntervalsShakeLWait[5] = {1.0f, 2.0f, 2.0f, 0.2f, 0.2f};
// 0x0084796C
const f32 s_CedarIntervalsShakeM[5] = {1.0f, 2.0f, 0.3f, 0.3f, 0.2f};
// 0x00847980
const f32 s_CedarIntervals[5] = {1.0f, 2.0f, 0.5f, 0.4f, 0.25f};
// 0x00847994
const f32 s_PalmIntervalsShakeL[5] = {1.0f, 2.0f, 1.0f, 0.7f, 0.5f};
// 0x008479A8
const f32 s_PalmIntervalsShakeLWait[5] = {1.0f, 1.0f, 1.0f, 1.0f, 0.2f};
// 0x008479BC
const f32 s_PalmIntervalsShakeM[5] = {1.0f, 2.0f, 1.0f, 0.4f, 0.2f};
// 0x008479D0
const f32 s_PalmIntervals[5] = {1.0f, 2.0f, 0.6f, 0.5f, 0.25f};
// 0x008479E4
const f32 s_BambooIntervalsShakeL[4] = {1.0f, 1.0f, 0.5f, 0.3f};
// 0x008479F4
const f32 s_BambooIntervalsShakeLWait[4] = {1.0f, 1.0f, 0.4f, 0.2f};
// 0x00847A04
const f32 s_BambooIntervalsShakeM[4] = {1.0f, 1.0f, 0.3f, 0.2f};
// 0x00847A14
const f32 s_BambooIntervals[4] = {1.0f, 1.0f, 0.4f, 0.3f};
// the bushes: the scale of the leaf effect and the emit interval by size
const f32 BUSH_LEAF_SCALE_SMALL = 0.25f;
const f32 BUSH_LEAF_SCALE_LARGE = 0.5f;
const f32 BUSH_INTERVAL_SMALL = 3.0f;
} // namespace

// (name is ours)
// 0x0094FFA0
const fgobj::ObjectSwing::ExecuteFunction fgobj::ObjectSwing::s_ExecuteFunctions[2] = {&ObjectSwing::ExecuteShake,
                                                                                      &ObjectSwing::ExecuteCut};

// (name is ours)
// 0x0094FF70
const char* const fgobj::ObjectSwing::s_AnimationSuffixes[ANIMATION_NUM] = {"shakeS", "shakeM", "shakeL", "shakeL_wait",
                                                                           "cut0_L", "cut1_L", "cut0_R", "cut1_R"};

// 0x005934A8 (name is ours)
void fgobj::ObjectSwing::PlayFallSound()
{
    if (m_SoundFrame < SWING_SOUND_FRAME) {
        m_SoundFrame++;
        if (m_SoundFrame == SWING_SOUND_FRAME) {
            u8 fgKind = m_Item.GetFgKind();
            const item::ParamFgEntry* param = item::GetParamFgEntry(m_Item);
            if (param != NULL) {
                s16 value = param->size;
                if (fgKind == SvFgName::FG_KIND_TREE) {
                    if (m_Item.IsName(SvFgName::NAME_DEAD_PERFECT_TREE)) {
                        SoundMgr::SPlaySharedSoundWithValue(SE_OBJ_TREE_DEAD_DOWN_SWING, &m_Position, this, value, false);
                    } else {
                        SoundMgr::SPlaySharedSoundWithValue(SE_OBJ_TREE_BROAD_DOWN_SWING, &m_Position, this, value, false);
                    }
                } else if (fgKind == SvFgName::FG_KIND_PALM_TREE) {
                    SoundMgr::SPlaySharedSoundWithValue(SE_OBJ_TREE_PALM_DOWN_SWING, &m_Position, this, value, false);
                } else if (fgKind == SvFgName::FG_KIND_CEDAR_TREE) {
                    SoundMgr::SPlaySharedSoundWithValue(SE_OBJ_TREE_NEEDLE_DOWN_SWING, &m_Position, this, value, false);
                } else if (fgKind == SvFgName::FG_KIND_BAMBOO) {
                    SoundMgr::SPlaySharedSoundWithValue(SE_OBJ_TREE_BAMBOO_DOWN_SWING, &m_Position, this,
                                                        value + BAMBOO_SOUND_VALUE_OFFSET, false);
                }
            }
        }
    }
    if (m_FadeFrame == FADE_START_FRAME) {
        u8 fgKind = m_Item.GetFgKind();
        const item::ParamFgEntry* param = item::GetParamFgEntry(m_Item);
        if (param != NULL) {
            s16 value = param->size;
            if (fgKind == SvFgName::FG_KIND_TREE) {
                if (m_Item.IsName(SvFgName::NAME_DEAD_PERFECT_TREE)) {
                    SoundMgr::SPlaySharedSoundWithValue(SE_OBJ_TREE_DEAD_DOWN_LAND, &m_Position, this, value, false);
                } else {
                    SoundMgr::SPlaySharedSoundWithValue(SE_OBJ_TREE_BROAD_DOWN_LAND, &m_Position, this, value, false);
                }
            } else if (fgKind == SvFgName::FG_KIND_PALM_TREE) {
                SoundMgr::SPlaySharedSoundWithValue(SE_OBJ_TREE_PALM_DOWN_LAND, &m_Position, this, value, false);
            } else if (fgKind == SvFgName::FG_KIND_CEDAR_TREE) {
                SoundMgr::SPlaySharedSoundWithValue(SE_OBJ_TREE_NEEDLE_DOWN_LAND, &m_Position, this, value, false);
            } else if (fgKind == SvFgName::FG_KIND_BAMBOO) {
                SoundMgr::SPlaySharedSoundWithValue(SE_OBJ_TREE_BAMBOO_DOWN_LAND, &m_Position, this,
                                                    value + BAMBOO_SOUND_VALUE_OFFSET, false);
            }
        }
    }
}

// 0x005936E4 slot 0x10
bool fgobj::ObjectSwing::vf_0x10()
{
    return m_Kind == KIND_SHAKE_S || m_Kind == KIND_SHAKE_L_WAIT;
}

// 0x005936FC (name is ours)
bool fgobj::ObjectSwing::GetEffectPosition(nn::math::VEC3* out) const
{
    u8 fgKind = m_Item.GetFgKind();
    u8 size = item::GetParamFgEntry(m_Item)->size;
    s32 bone;
    if (fgKind == SvFgName::FG_KIND_CEDAR_TREE) {
        bone = m_Model.GetBoneIndex(s_SwingCedarBones[size - 1]);
    } else {
        bone = m_Model.GetBoneIndex(EFFECT_BONE_NAME);
    }
    if (bone < 0) {
        return false;
    }
    nn::math::VEC3 position;
    if (!m_Model.GetBoneWorldPosition(&position, bone)) {
        return false;
    }
    Func52EA98(out, &position);
    if (m_Item.IsName(SvFgName::NAME_DEAD_PERFECT_TREE)) {
        return true;
    }
    if (fgKind == SvFgName::FG_KIND_TREE) {
        out->y += s_TreeEffectHeights[size];
    } else if (fgKind == SvFgName::FG_KIND_CEDAR_TREE) {
        out->y += s_CedarEffectHeights[size];
    } else if (fgKind == SvFgName::FG_KIND_PALM_TREE) {
        out->y += s_PalmEffectHeights[size];
    } else if (fgKind == SvFgName::FG_KIND_BAMBOO) {
        out->y += s_BambooEffectHeights[size];
    } else if (fgKind == SvFgName::FG_KIND_BUSH) {
        if (size == 1) {
            if (!m_Item.IsName(SvFgName::NAME_PINK_AZALEA_BUSH_GROWING_2) &&
                !m_Item.IsName(SvFgName::NAME_WHITE_AZALEA_BUSH_GROWING_2)) {
                out->y += BUSH_EFFECT_OFFSET_Y;
            }
        } else if (size == 2) {
            if (m_Item.IsName(SvFgName::NAME_PINK_AZALEA_BUSH) || m_Item.IsName(SvFgName::NAME_WHITE_AZALEA_BUSH)) {
                out->z += BUSH_EFFECT_OFFSET_Z;
            } else {
                out->y -= BUSH_EFFECT_OFFSET_Y;
            }
        }
    } else {
        return false;
    }
    return true;
}

// 0x00593920 (name is ours)
void fgobj::ObjectSwing::StartLeafEffect(u8 kind)
{
    f32 interval = GetEmitInterval(kind);
    nn::math::VEC3 position;
    if (!GetEffectPosition(&position)) {
        return;
    }
    Func207998(&m_LeafEffect, LEAF_EFFECT_ID, &position, &ANGLE16_VEC3_ZERO, &VEC3_ONE, true, 0);
    u8 fgKind = m_Item.GetFgKind();
    u8 size = item::GetParamFgEntry(m_Item)->size;
    f32 scale;
    if (fgKind == SvFgName::FG_KIND_CEDAR_TREE) {
        scale = s_CedarLeafScales[size];
    } else if (fgKind == SvFgName::FG_KIND_TREE) {
        scale = s_TreeLeafScales[size];
    } else if (fgKind == SvFgName::FG_KIND_PALM_TREE) {
        scale = s_PalmLeafScales[size];
    } else if (fgKind == SvFgName::FG_KIND_BAMBOO) {
        scale = s_BambooLeafScales[size];
    } else if (fgKind == SvFgName::FG_KIND_BUSH) {
        scale = size == 1 ? BUSH_LEAF_SCALE_SMALL : BUSH_LEAF_SCALE_LARGE;
    } else {
        scale = LEAF_EFFECT_SCALE;
    }
    nn::math::VEC3 scales(scale, scale, scale);
    m_LeafEffect.SetEmitterVolumeScale(&scales);
    m_LeafEffect.SetEmitInterval(interval);
}

// 0x00593A5C (name is ours)
bool fgobj::ObjectSwing::IsFallBlocked(s32 x, s32 z, s32 dx) const
{
    if (!m_Item.IsFg()) {
        return false;
    }
    if (m_Item.GetFgKind() == SvFgName::FG_KIND_BAMBOO || item::GetParamFgEntry(m_Item)->size < 3) {
        return false;
    }
    s32 nextX = x + dx;
    nn::math::VEC3 here;
    nn::math::VEC3 next;
    FdInfo::ToCenterWorldWithUnitHeight(&here, x, z);
    FdInfo::ToCenterWorldWithUnitHeight(&next, nextX, z);
    if (next.y > here.y + FALL_BLOCK_HEIGHT) {
        return true;
    }
    if (FdInfo::IsAttribute02_03_04_A2_A5_F8(nextX, z, FIELD_NAME_CURRENT)) {
        return true;
    }
    const SvFgName* fg = FdInfo::GetCurrentField()->GetFg(nextX, z, 0);
    if (fg != NULL && !IsGrownTreeOfSize2(fg)) {
        return false;
    }
    return true;
}

// 0x00593B50 (name is ours)
s32 fgobj::ObjectSwing::ExecuteCut()
{
    m_Model.UpdateAnims();
    UpdateMatrix(true, 0.0f);
    if (m_FadeFrame != 0) {
        m_FadeFrame--;
        if (m_FadeFrame == FADE_START_FRAME) {
            m_Flags |= FLAG_ALPHA_STATE;
            m_Alpha = m_FadeFrame * FADE_ALPHA_STEP;
        } else if (m_FadeFrame < FADE_START_FRAME) {
            m_Alpha = m_FadeFrame * FADE_ALPHA_STEP;
        }
    }
    PlayFallSound();
    UpdateShadowOpacity();
    if (m_Effect.GetEmitterSet() != NULL) {
        nn::math::VEC3 position;
        if (GetEffectPosition(&position)) {
            m_Effect.SetTransform(&position, &ANGLE16_VEC3_ZERO, true);
            if (m_LeafEffect.GetEmitterSet() != NULL) {
                m_LeafEffect.SetTransform(&position, &ANGLE16_VEC3_ZERO, true);
            }
        }
    }
    if (m_SkeletalAnim.IsFinished()) {
        return EXECUTE_FINISHED;
    }
    return EXECUTE_CONTINUE;
}

// 0x00593C64 (name is ours)
void fgobj::ObjectSwing::StartEffect(u8 kind)
{
    if (m_Item.IsName(SvFgName::NAME_DEAD_PERFECT_TREE)) {
        return;
    }
    if (kind == KIND_SHAKE_S) {
        if (Func63061C()) {
            StartLeafEffect(KIND_SHAKE_L);
        }
        return;
    }
    const s32* effects;
    s32 index;
    u8 kindOfFg = m_Item.GetFgKind();
    if (kindOfFg == SvFgName::FG_KIND_TREE) {
        index = Func1E6444((item::GetParamFgEntry(m_Item)->unk_0xF & 4) ? 2 : 1);
        effects = s_SwingTreeEffects;
    } else if (kindOfFg == SvFgName::FG_KIND_CEDAR_TREE) {
        index = Func1E6444(3);
        effects = s_SwingCedarEffects;
    } else if (kindOfFg == SvFgName::FG_KIND_PALM_TREE) {
        index = Func1E6444(4);
        effects = s_SwingPalmEffects;
    } else if (kindOfFg == SvFgName::FG_KIND_BAMBOO) {
        if (item::GetParamFgEntry(m_Item)->size < 2) {
            return;
        }
        index = Func1E6444(4);
        effects = s_SwingBambooEffects;
    } else {
        return;
    }
    s32 effect = effects[index];
    if (effect == -1) {
        return;
    }
    nn::math::VEC3 position;
    if (!GetEffectPosition(&position)) {
        return;
    }
    Func207998(&m_Effect, effect, &position, &ANGLE16_VEC3_ZERO, &VEC3_ONE, true, 0);
    u8 fgKind = m_Item.GetFgKind();
    u8 size = item::GetParamFgEntry(m_Item)->size;
    f32 scale;
    if (fgKind == SvFgName::FG_KIND_TREE) {
        scale = s_TreeEffectScales[size];
    } else if (fgKind == SvFgName::FG_KIND_CEDAR_TREE) {
        scale = s_CedarEffectScales[size];
    } else if (fgKind == SvFgName::FG_KIND_PALM_TREE) {
        scale = s_PalmEffectScales[size];
    } else if (fgKind == SvFgName::FG_KIND_BAMBOO) {
        scale = s_BambooEffectScales[size];
    } else {
        scale = 1.0f;
    }
    nn::math::VEC3 scales(scale, scale, scale);
    m_Effect.SetEmitterVolumeScale(&scales);
    m_Effect.SetEmitInterval(GetEmitInterval(kind));
    if (kind == KIND_CUT_LEFT || kind == KIND_CUT_RIGHT) {
        fgKind = m_Item.GetFgKind();
        const item::ParamFgEntry* param = item::GetParamFgEntry(m_Item);
        f32 rate = 1.0f;
        if (param != NULL) {
            u8 paramSize = param->size;
            if (fgKind == SvFgName::FG_KIND_TREE) {
                rate = s_TreeCutRates[paramSize];
            } else if (fgKind == SvFgName::FG_KIND_CEDAR_TREE) {
                rate = s_CedarCutRates[paramSize];
            } else if (fgKind == SvFgName::FG_KIND_PALM_TREE) {
                rate = s_PalmCutRates[paramSize];
            } else if (fgKind == SvFgName::FG_KIND_BAMBOO) {
                rate = s_BambooCutRates[paramSize];
            }
        }
        m_Effect.SetEmitRate(rate);
    }
    if (Func63061C()) {
        StartLeafEffect(kind);
    }
}

// 0x00593F58 (name is ours)
void fgobj::ObjectSwing::Setup(const SvFgName& item, s32 x, s32 z, u8 kind)
{
    m_State = STATE_SHAKE;
    m_Alpha = 255;
    m_Kind = kind;
    u8 animation = ANIMATION_SHAKE_L;
    m_Item = item;
    switch (m_Kind) {
    case KIND_SHAKE_S:
        animation = ANIMATION_SHAKE_S;
        break;
    case KIND_SHAKE_L_WAIT:
        animation = ANIMATION_SHAKE_L_WAIT;
        break;
    case KIND_SHAKE_M:
        animation = ANIMATION_SHAKE_M;
        break;
    case KIND_CUT_LEFT:
        animation = IsFallBlocked(x, z, -2) ? ANIMATION_CUT1_L : ANIMATION_CUT0_L;
        m_State = STATE_CUT;
        break;
    case KIND_CUT_RIGHT:
        animation = IsFallBlocked(x, z, 2) ? ANIMATION_CUT1_R : ANIMATION_CUT0_R;
        m_State = STATE_CUT;
        break;
    }
    const void* model = GetAnimModel(this);
    if (model == NULL) {
        SvFgName cedar(static_cast<SvFgName::ID>(SvFgName::NAME_CEDAR));
        m_Item = cedar;
        model = GetAnimModel(this);
    }
    Initialize(model, x, z, false, 0, true);
    UpdateMatrix(true, 0.0f);
    m_Animation = animation;
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
    const void* anim = GetFgSkeletalAnim(name, animation);
    m_SkeletalAnim.Build(&m_Model, nw::anim::res::ResAnim(anim), &m_Allocator, 0);
    m_Model.SetSkeletalAnim(&m_SkeletalAnim);
    m_SoundFrame = 0;
    m_FadeFrame = static_cast<u32>(m_SkeletalAnim.GetEndFrame());
    InitializeShadow();
    SetSortKey(z, 0, -1, false, true, true, false);
}

// 0x005941A8 (name is ours)
s32 fgobj::ObjectSwing::ExecuteShake()
{
    m_Model.UpdateAnims();
    UpdateMatrix(true, 0.0f);
    if (!m_SkeletalAnim.IsFinished()) {
        return EXECUTE_CONTINUE;
    }
    if (m_Kind == KIND_SHAKE_S) {
        return EXECUTE_FINISHED_KEEP_REQUEST;
    }
    if (m_Kind == KIND_SHAKE_L) {
        return EXECUTE_FINISHED;
    }
    if (m_Kind == KIND_SHAKE_L_WAIT) {
        return EXECUTE_FINISHED_KEEP_REQUEST;
    }
    if (m_Kind == KIND_SHAKE_M) {
        return EXECUTE_FINISHED;
    }
    return EXECUTE_FINISHED_KEEP_REQUEST;
}

// 0x00594210 (name is ours)
void fgobj::ObjectSwing::UpdateShadowOpacity()
{
    f32 frame = m_SkeletalAnim.GetFrame();
    f32 opacity;
    if (frame < SHADOW_FADE_START) {
        opacity = 1.0f;
    } else if (SHADOW_FADE_END < frame) {
        opacity = 0.0f;
    } else {
        f32 t = (SHADOW_FADE_END - frame) * (1.0f / 12.0f);
        if (t < 0.0f) {
            opacity = 0.0f;
        } else if (t > 1.0f) {
            opacity = 1.0f;
        } else {
            opacity = t;
        }
    }
    m_Shadow.m_Opacity = opacity;
}

// 0x0059429C (name is ours)
void fgobj::ObjectSwing::PlayShakeSound(const SvFgName* item, u8 kind)
{
    u8 fgKind = item->GetFgKind();
    const item::ParamFgEntry* param = item::GetParamFgEntry(*item);
    if (param == NULL) {
        return;
    }
    s16 value = param->size;
    if (kind == KIND_SHAKE_L) {
        if (fgKind == SvFgName::FG_KIND_TREE) {
            if (item->IsName(SvFgName::NAME_DEAD_PERFECT_TREE)) {
                SoundMgr::SPlaySharedSoundWithValue(SE_OBJ_TREE_DEAD_SWING_L, &m_Position, this, value, false);
            } else {
                SoundMgr::SPlaySharedSoundWithValue(SE_OBJ_TREE_BROAD_SWING_L, &m_Position, this, value, false);
            }
        } else if (fgKind == SvFgName::FG_KIND_PALM_TREE) {
            SoundMgr::SPlaySharedSoundWithValue(SE_OBJ_TREE_PALM_SWING_L, &m_Position, this, value, false);
        } else if (fgKind == SvFgName::FG_KIND_CEDAR_TREE) {
            SoundMgr::SPlaySharedSoundWithValue(SE_OBJ_TREE_NEEDLE_SWING_L, &m_Position, this, value, false);
        } else if (fgKind == SvFgName::FG_KIND_BAMBOO) {
            SoundMgr::SPlaySharedSoundWithValue(SE_OBJ_TREE_BAMBOO_SWING_L, &m_Position, this,
                                                value + BAMBOO_SOUND_VALUE_OFFSET, false);
        }
        return;
    }
    switch (fgKind) {
    case SvFgName::FG_KIND_CEDAR_TREE:
        SoundMgr::SPlaySharedSoundWithValue(SE_OBJ_TREE_NEEDLE_SWING_S, &m_Position, this, value, false);
        break;
    case SvFgName::FG_KIND_BUSH:
        SoundMgr::SPlaySharedSoundWithValue(SE_OBJ_TREE_LOW_SWING_S, &m_Position, this, value, false);
        break;
    case SvFgName::FG_KIND_BAMBOO:
        SoundMgr::SPlaySharedSoundWithValue(SE_OBJ_TREE_BAMBOO_SWING_S, &m_Position, this,
                                            value + BAMBOO_SOUND_VALUE_OFFSET, false);
        break;
    case SvFgName::FG_KIND_TREE:
        if (!item->IsName(SvFgName::NAME_DEAD_PERFECT_TREE)) {
            SoundMgr::SPlaySharedSoundWithValue(SE_OBJ_TREE_BROAD_SWING_S, &m_Position, this, value, false);
        }
        break;
    case SvFgName::FG_KIND_PALM_TREE:
        SoundMgr::SPlaySharedSoundWithValue(SE_OBJ_TREE_PALM_SWING_S, &m_Position, this, value, false);
        break;
    }
}

// 0x00594488 (name is ours)
f32 fgobj::ObjectSwing::GetEmitInterval(u8 kind) const
{
    u8 fgKind = m_Item.GetFgKind();
    const item::ParamFgEntry* param = item::GetParamFgEntry(m_Item);
    f32 interval = 1.0f;
    if (param == NULL) {
        return interval;
    }
    u8 size = param->size;
    if (fgKind == SvFgName::FG_KIND_TREE) {
        if (kind == KIND_SHAKE_L) {
            return s_TreeIntervalsShakeL[size];
        }
        if (kind == KIND_SHAKE_L_WAIT) {
            return s_TreeIntervalsShakeLWait[size];
        }
        return kind == KIND_SHAKE_M ? s_TreeIntervalsShakeM[size] : s_TreeIntervals[size];
    } else if (fgKind == SvFgName::FG_KIND_CEDAR_TREE) {
        if (kind == KIND_SHAKE_L) {
            return s_CedarIntervalsShakeL[size];
        }
        if (kind == KIND_SHAKE_L_WAIT) {
            return s_CedarIntervalsShakeLWait[size];
        }
        return kind == KIND_SHAKE_M ? s_CedarIntervalsShakeM[size] : s_CedarIntervals[size];
    } else if (fgKind == SvFgName::FG_KIND_PALM_TREE) {
        if (kind == KIND_SHAKE_L) {
            return s_PalmIntervalsShakeL[size];
        }
        if (kind == KIND_SHAKE_L_WAIT) {
            return s_PalmIntervalsShakeLWait[size];
        }
        return kind == KIND_SHAKE_M ? s_PalmIntervalsShakeM[size] : s_PalmIntervals[size];
    } else if (fgKind == SvFgName::FG_KIND_BAMBOO) {
        if (kind == KIND_SHAKE_L) {
            return s_BambooIntervalsShakeL[size];
        }
        if (kind == KIND_SHAKE_L_WAIT) {
            return s_BambooIntervalsShakeLWait[size];
        }
        return kind == KIND_SHAKE_M ? s_BambooIntervalsShakeM[size] : s_BambooIntervals[size];
    } else if (fgKind == SvFgName::FG_KIND_BUSH) {
        if (size == 1) {
            interval = BUSH_INTERVAL_SMALL;
        }
    }
    return interval;
}

// 0x00594654 (name is ours)
fgobj::ObjectSwing* fgobj::ObjectSwing::Create(const SvFgName& item, s32 x, s32 z, u8 kind, bool isFallAll)
{
    ObjectSwing* swing = NewObject<ObjectSwing>(s_LargeHeaps, LARGE_HEAP_NUM);
    if (swing == NULL) {
        return NULL;
    }
    swing->Setup(item, x, z, kind);
    if (isFallAll) {
        s32 index = Func1E6444(1);
        if (index < 19) {
            nn::math::VEC3 position;
            if (swing->GetEffectPosition(&position)) {
                Func207B90(s_SwingFallAllEffects[index], &position, &ANGLE16_VEC3_ZERO, &VEC3_ONE, true, 0);
            }
            SoundMgr::SPlaySharedSoundWithValue(SE_OBJ_TREE_SWING_FALL_ALL, &swing->m_Position, swing, FALL_ALL_SOUND_VALUE, false);
        }
    } else {
        swing->PlayShakeSound(&item, kind);
        swing->StartEffect(kind);
    }
    return swing;
}

// 0x005947D8 slot 0x14 (name is ours)
void fgobj::ObjectSwing::Finalize()
{
    if (m_Effect.GetEmitterSet() != NULL) {
        m_Effect.Kill();
    }
    if (m_LeafEffect.GetEmitterSet() != NULL) {
        m_LeafEffect.Kill();
    }
    ObjectBase::Finalize();
}

// 0x0059482C slot 0x20 (name is ours)
s32 fgobj::ObjectSwing::Execute()
{
    return (this->*s_ExecuteFunctions[m_State])();
}

// 0x005948B0 slot 0x00
// 0x0059485C (deleting dtor)
fgobj::ObjectSwing::~ObjectSwing()
{
    // (only the members are destroyed)
}

// 0x005A3640 (name is ours)
const void* GetAnimModel(const ObjectBase* object)
{
    const SvFgName& item = object->m_Item;
    u8 fgKind = item.GetFgKind();
    u32 index = item::GetParamFgEntry(item)->fgModel;
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
    bool isSpecial = false;
    if (SvFgName::IsBushFgKind(fgKind) && Func63061C()) {
        isSpecial = true;
        if (item.IsName(SvFgName::NAME_PINK_AZALEA_BUSH) || item.IsName(SvFgName::NAME_WHITE_AZALEA_BUSH)) {
            name = BUSH_B_MODEL_NAME;
        } else if (item.IsName(SvFgName::NAME_BLUE_HYDRANGEA_BUSH) || item.IsName(SvFgName::NAME_PINK_HYDRANGEA_BUSH)) {
            name = BUSH_C_MODEL_NAME;
        } else if (item.IsName(SvFgName::NAME_RED_HIBISCUS_BUSH) || item.IsName(SvFgName::NAME_YELLOW_HIBISCUS_BUSH)) {
            name = BUSH_D_MODEL_NAME;
        }
    }
    return GetFgAnimModel(name, isSpecial);
}

} // namespace fgobj
