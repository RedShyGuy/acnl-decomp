#include "Other/dToolBank.h"

#include <cfloat>
#include <cmath>
#include "Ac/dAcPlayer.h"
#include "Bank/bank_Extern.h"
#include "Human/dHumanAnimName.h"
#include "Human/dHumanModel.h"
#include "Human/human_Extern.h"
#include "Item/item_Api.h"
#include "Other/dActor.h"
#include "Other/dBalloonString.h"
#include "Other/dToolBankExtern.h"
#include "Other/dWorldCurve.h"
#include "Player/dPlayerApi.h"
#include "fgobj/fgobj_Extern.h"
#include "field/dFdInfo.h"
#include "field/dFieldAttribute.h"
#include "field/dFieldTypes.h"
#include "g3d/g3d_System.h"
#include "netgame/dNetGameMgr.h"
#include "nn/math/math_Api.h"
#include "nw/gfx/gfx_SkeletalModel.h"
#include "nw/gfx/gfx_Skeleton.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResMaterial.h"
#include "nw/gfx/res/gfx_ResModel.h"
#include "sead/seadHeap.h"
#include "sead/seadMathCalcCommon.h"
#include "script/dScriptExtern.h"
#include "sound/sound_Extern.h"
#include "ssys/ma/ma_Math.h"
#include "stage/stage_Api.h"
#include "stage/stage_Extern.h"

namespace {
// the sounds of the tools
const u32 SE_FLOAT_SPLASH = 0x0100061E;
const u32 SE_FLOAT_LAND = 0x010005CD;
const u32 SE_FLOAT_BITE = 0x0100061F;
const u32 SE_FLOAT_PULL_OUT = 0x010005CE;

// the effects of the float
const u32 EFFECT_FLOAT_HOOKED = 230;
const u32 EFFECT_FLOAT_BITE = 231;
const u32 EFFECT_FLOAT_LAND = 232;
const u32 EFFECT_FLOAT_BITE_END = 233;
const u32 EFFECT_FLOAT_PULL_OUT = 234;
const u32 EFFECT_FLOAT_SPLASH = 235;
const u32 EFFECT_FLOAT_DRIFT = 236;

// the value of GetAttribute95E83FAt for no flow
const u8 FLOW_NONE = 8;

// (inline) the ResAnim of the material animation 0 of a graphics file, or of its visibility animation 0
inline nw::anim::res::ResAnim GetTextureAnim(const item::ResLoader* loader)
{
    return nw::anim::res::ResAnim(loader->GetGraphicsFile().GetTextureAnims(0));
}

// (inline) the end frame of the skeletal animation of id (0 without one)
inline f32 GetAnimFrameSize(u16 id)
{
    const nw::anim::res::ResAnim anim = Func770970(&g_HumanAnimTable, id)->anim0;
    return anim.ptr() != NULL ? anim.GetFrameSize() : 0.0f;
}

// (inline) a morph animation of the human animation table on model (Initialize)
DECOMP_ALWAYS_INLINE void BuildMorphAnim(g3d::MorphSkeletalAnim* anim, g3d::SkeletalModel* model, u16 id,
                                         ssys::ma::HeapAllocator* allocator, s32 playMode, f32 speed)
{
    anim->Build(model, Func770970(&g_HumanAnimTable, id)->anim0, allocator, 0);
    anim->SetPlayMode(playMode);
    anim->SetSpeed(speed);
}

// (inline) the body of ToolBank::GetFieldItemKind (GetGroundHeightNear has three copies of it)
DECOMP_ALWAYS_INLINE s32 GetFieldItemKindImpl(const nn::math::VEC3* position)
{
    s32 x;
    s32 z;
    FdInfo::ToUnit(x, z, *position);
    FieldItemInfo info;
    Func240240(&info, x, z, 1);
    if ((info.unknown8 & 0xFF) >= 252) {
        return 0;
    }
    const u32 group = Func24066C(info.unknown8 & 0xFF);
    if (group >= 6) {
        return 0;
    }
    return group == 0 || group == 2 ? 1 : 2;
}

// (inline) the height of the ground at unit position for GetGroundHeightNear: the field item there decides the margin
DECOMP_ALWAYS_INLINE f32 GetHeightWithFieldItem(const nn::math::VEC3* unitPosition, f32 base, f32 height, f32 lowMargin,
                                                f32 highMargin)
{
    const s32 kind = GetFieldItemKindImpl(unitPosition);
    if (kind == 0) {
        return base;
    }
    if (kind == 1) {
        const f32 low = base + lowMargin;
        return low >= height ? low : height;
    }
    const f32 high = base + highMargin;
    return height <= high ? high : height;
}
} // namespace

// 0x006BEF04 (name is ours)
void ToolBank::SetAnimSpeed(f32 speed)
{
    g3d::MorphSkeletalAnim* morph = GetMorphAnim();
    if (morph != NULL && morph->m_pAnimObject != NULL) {
        morph->SetSpeed(speed);
    }
    g3d::MaterialAnim* material = GetMaterialAnim();
    if (material != NULL && material->m_pAnimObject != NULL) {
        material->SetSpeed(speed);
    }
}

// 0x006BEF90 (name is ours)
bool ToolBank::PlayAnim(u16 id, s32 playMode, bool isKeep, f32 frame, f32 speed, f32 morphFrames)
{
    if (isKeep && m_AnimIds[GetCurrentSlot()] == id) {
        g3d::MorphSkeletalAnim* morph = GetMorphAnim();
        if (morph != NULL) {
            morph->SetSpeed(speed);
            if (speed == 0.0f) {
                morph->SetFrame(frame);
            }
        }
        return false;
    }
    const HumanAnimEntry* entry = Func770970(&g_HumanAnimTable, id);
    // (the result is not used)
    GetCurrentModel();
    g3d::MorphSkeletalAnim* morph = GetMorphAnim();
    bool isPlayed = false;
    if (playMode == 2) {
        playMode = (Func6E6268(id)->flags >> 5) & 1;
    }
    if (morph != NULL && entry->anim0.ptr() != NULL) {
        morph->ChangeAnim(entry->anim0, 0, morphFrames);
        morph->SetPlayMode(playMode);
        morph->SetSpeed(speed);
        morph->SetFrame(frame);
        isPlayed = true;
        m_AnimIds[GetCurrentSlot()] = id;
    }
    if (entry->anim3.ptr() != NULL) {
        g3d::MaterialAnim* material = GetMaterialAnim();
        if (material != NULL && material->m_pAnimObject != NULL) {
            material->ChangeAnim(entry->anim3);
            material->SetPlayMode(playMode);
            material->SetSpeed(speed);
            material->SetFrame(frame);
            isPlayed = true;
        }
    }
    return isPlayed;
}

// 0x006BF184 (name is ours)
void ToolBank::ApplyBoneRotate()
{
    g3d::SkeletalModel* model = GetCurrentModel();
    if (model == NULL) {
        return;
    }
    model->EvaluateAnims();
    if (!m_IsBoneRotated) {
        return;
    }
    SvFgName item(m_Keywords[GetCurrentSlot()].m_Name);
    if (Func311840(&item) != TOOL_KIND_PINWHEEL) {
        return;
    }
    nw::gfx::SkeletalModel* gfxModel = model->GetSkeletalModel();
    if (gfxModel == NULL) {
        return;
    }
    nw::gfx::CalculatedTransform* transform = gfxModel->GetSkeleton()->GetLocalTransforms()->begin() + 1;
    if (transform == NULL) {
        return;
    }
    if (!m_IsBoneTransformSaved) {
        transform->DisableFlags(nw::gfx::CalculatedTransform::FLAG_ROTATE_UNSET |
                                nw::gfx::CalculatedTransform::FLAG_IS_ROTATE_ZERO);
        m_SavedBoneTransform = *transform;
        m_IsBoneTransformSaved = true;
    }
    *transform = m_SavedBoneTransform;
    nn::math::MTX34 matrix = transform->m_TransformMatrix;
    const f32 translateX = matrix.m[0][3];
    const f32 translateY = matrix.m[1][3];
    const f32 translateZ = matrix.m[2][3];
    nn::math::MTX34 rotate;
    sead::Matrix34CalcCtr<f32>::makeIdentity(rotate);
    ssys::ma::RotateX(&rotate, static_cast<s16>(m_BoneRotateX.value));
    sead::Matrix34CalcCtr<f32>::multiply(matrix, rotate, matrix);
    nn::math::ARMv6::MTX34CopyAsm(&transform->m_TransformMatrix, &matrix);
    transform->m_TransformMatrix.m[0][3] = translateX;
    transform->m_TransformMatrix.m[1][3] = translateY;
    transform->m_TransformMatrix.m[2][3] = translateZ;
    transform->EnableFlags(nw::gfx::CalculatedTransform::FLAG_IS_DIRTY);
}

// 0x006BF34C (name is ours)
void ToolBank::SetAnimFrame(f32 frame)
{
    g3d::MorphSkeletalAnim* morph = GetMorphAnim();
    if (morph != NULL && morph->m_pAnimObject != NULL) {
        morph->SetFrame(frame);
    }
    g3d::MaterialAnim* material = GetMaterialAnim();
    if (material != NULL && material->m_pAnimObject != NULL) {
        material->SetFrame(frame);
    }
}

// 0x006BF3D8 (name is ours)
void ToolBank::PlayFloatSplash()
{
    if (FdInfo::IsPart95E93ENonZero(m_FloatPosition, FIELD_NAME_CURRENT)) {
        m_FloatPosition.y -= 1.5f;
    }
    const u32 slot = GetCurrentSlot();
    if (m_SoundStates[slot].m_HasPosition) {
        m_Sounds[slot].StartSound(SE_FLOAT_SPLASH);
    }
    nn::math::VEC3 position = m_FloatPosition;
    position.y = FdInfo::GetSurfaceHeightEx(position, FIELD_NAME_CURRENT);
    Func207B90(EFFECT_FLOAT_SPLASH, &position, &ANGLE16_VEC3_ZERO, &VEC3_ONE, true, 0);
}

// 0x006BF4A0 slot 0x10 (name after libgarden)
bool ToolBank::LoadResource(const Keyword& keyword, u32 slot)
{
    if (!keyword.m_Name.IsUsual()) {
        return false;
    }
    if (Func76BB44(&keyword.m_Name) != 7) {
        return false;
    }
    sead::Heap* heap = GetAllocator(slot)->m_pHeap;
    if (!GetLoader(slot, 0)->Load(&keyword.m_Name, heap, 0, 0, NULL, true, 0)) {
        return false;
    }
    if (!Func76BA5C(&keyword.m_Name)) {
        return true;
    }
    SvFgName design;
    Func76C8E0(&design, &keyword.m_Name);
    if (!design.IsEmpty()) {
        const void* data = keyword.GetDesignData();
        if (data != NULL && !GetLoader(slot, 1)->Load(&design, heap, 3, 1, data, false, 0)) {
            return false;
        }
    }
    return true;
}

// 0x006BF628 (name is ours)
void ToolBank::SetHookedTarget(const nn::math::VEC3* position)
{
    m_HookedTarget = *position;
}

// 0x006BF644 (name is ours)
s32 ToolBank::GetFieldItemKind(const nn::math::VEC3* position)
{
    return GetFieldItemKindImpl(position);
}

// 0x006BF6AC (name is ours)
void ToolBank::AddToScene(s32 sceneIdx)
{
    Func328698(this, sceneIdx);
    AddExtrasToScene(sceneIdx);
}

// 0x006BF6D0 (name is ours)
void ToolBank::AddExtrasToScene(s32 sceneIdx)
{
    const u32 slot = GetCurrentSlot();
    g3d::Model* floatModel = &m_FloatModels[slot];
    if (m_IsFloatShown && floatModel->m_pNode != NULL) {
        const f32 scale = unknown;
        floatModel->SetScale(nn::math::VEC3(scale, scale, scale));
        floatModel->AddToScene(sceneIdx);
    }
    BalloonString* balloon = m_pBalloonStrings[slot];
    if (balloon != NULL) {
        if (balloon->m_Color != 0xFF) {
            const nw::gfx::res::ResModelData* model =
                static_cast<const nw::gfx::res::ResModelData*>(GetLoader(slot, 0)->GetGraphicsFile().GetModels(0));
            nw::gfx::res::ResMaterialData* material =
                static_cast<nw::gfx::res::ResMaterialData*>(model->materials.GetData(0));
            material->translucencyKind = nw::gfx::res::ResMaterialData::TRANSLUCENCY_KIND_TRANSLUCENT;
            g3d::SetConstantColor(material, 0, -1, -1, -1, m_pBalloonStrings[slot]->m_Color);
        }
        m_pBalloonStrings[slot]->Func21348C();
    }
    if (!m_IsFloatShown) {
        m_FloatShadowScale = 0.0f;
        return;
    }
    const f32 step = s_FloatShadowStep * 4.0f;
    switch (m_FloatState) {
    case FLOAT_STATE_HOLD:
    case FLOAT_STATE_FLOAT:
    case FLOAT_STATE_BITE:
    case FLOAT_STATE_HOOKED:
    case FLOAT_STATE_SINK:
        if (m_FloatShadowScale != 0.0f) {
            ssys::ma::Chase(&m_FloatShadowScale, 0.0f, step);
        }
        break;
    default: {
        const f32 surface = FdInfo::GetSurfaceHeight(m_FloatPosition, FIELD_NAME_CURRENT);
        if (std::fabs(surface - m_FloatPosition.y) < 24.0f) {
            ssys::ma::Chase(&m_FloatShadowScale, 0.0f, step);
            break;
        }
        // the shadow is shown when the float is high above the ground around it and away from the tip of the rod
        f32 target = 1.0f;
        nn::math::VEC3 side;
        math::Angle16 angle;
        angle.value = m_ActorAngleY.value + 0xC000;
        Func64F96C(&side, &m_FloatPosition, angle, FdInfo::UNIT_WORLD_SIZE_ZF, true);
        side.y = FdInfo::GetHeightOrBgHeight(side, FIELD_NAME_CURRENT);
        bool isNear = m_FloatPosition.y < side.y + 10.0f;
        if (!isNear) {
            nn::math::VEC3 point;
            Func64F96C(&point, &side, m_ActorAngleY, FdInfo::UNIT_WORLD_SIZE_HALF_ZF * 1.2f, true);
            point.y = FdInfo::GetHeightOrBgHeight(point, FIELD_NAME_CURRENT);
            isNear = m_FloatPosition.y < point.y + 10.0f;
        }
        if (!isNear) {
            nn::math::VEC3 point;
            Func64F96C(&point, &side, m_ActorAngleY, FdInfo::UNIT_WORLD_SIZE_ZF * 1.2f, true);
            point.y = FdInfo::GetHeightOrBgHeight(point, FIELD_NAME_CURRENT);
            isNear = m_FloatPosition.y < point.y + 10.0f;
        }
        if (!isNear) {
            nn::math::VEC3 point;
            angle.value = -m_ActorAngleY.value;
            Func64F96C(&point, &side, angle, FdInfo::UNIT_WORLD_SIZE_ZF, true);
            point.y = FdInfo::GetHeightOrBgHeight(point, FIELD_NAME_CURRENT);
            isNear = m_FloatPosition.y < point.y + 10.0f;
        }
        if (!isNear) {
            nn::math::VEC3 point;
            angle.value = -m_ActorAngleY.value;
            Func64F96C(&point, &m_FloatPosition, angle, FdInfo::UNIT_WORLD_SIZE_ZF, true);
            point.y = FdInfo::GetHeightOrBgHeight(point, FIELD_NAME_CURRENT);
            isNear = m_FloatPosition.y < point.y + 10.0f;
        }
        if (isNear) {
            // 0x0095D594 (guard 0x0095D590)
            static const f32 s_RodDistance = FdInfo::UNIT_WORLD_SIZE_ZF * 0.9f;
            // 0x0095D598 (guard 0x0095D58C)
            static const f32 s_RodDistanceSq = s_RodDistance * s_RodDistance;
            const nn::math::VEC3 rodTip(m_RodMatrix.m[0][3], m_RodMatrix.m[1][3], m_RodMatrix.m[2][3]);
            nn::math::VEC3 curved;
            Func52EA98(&curved, &rodTip);
            const nn::math::VEC3 diff(m_FloatPosition.x - curved.x, m_FloatPosition.y - curved.y,
                                      m_FloatPosition.z - curved.z);
            if (diff.x * diff.x + diff.z * diff.z < s_RodDistanceSq) {
                target = 0.0f;
            }
        }
        if (m_FloatShadowScale != target) {
            ssys::ma::Chase(&m_FloatShadowScale, target, step);
        }
        break;
    }
    }
    const f32 scale = m_FloatShadowScale;
    if (scale == 0.0f) {
        return;
    }
    Func6EFAF4(&m_FloatPosition, 0, 1, 0, scale * 3.0f, scale * 3.0f, -1.0f, scale * 0.7f, 1.0f);
}

// 0x006BFBEC (name is ours)
bool ToolBank::GetNetPosition(nn::math::VEC3* out, const HumanModel* model)
{
    nn::math::VEC3 position;
    nn::math::MTX34 matrix;
    if (model->GetBoneMatrix(&matrix, model->m_BoneIndices[3], false)) {
        const nn::math::VEC3 local(9.0f, 0.0f, FdInfo::UNIT_WORLD_SIZE_XF * 0.77f);
        nn::math::VEC3 world;
        sead::Vector3CalcCtr<f32>::mul(world, matrix, local);
        nn::math::VEC3 curved;
        Func52EA98(&curved, &world);
        position = curved;
    }
    const f32 y = position.y;
    Func64F96C(&position, &position, model->m_Rotation.y, FdInfo::UNIT_WORLD_SIZE_XF * 0.2f, true);
    position.y = y;
    *out = position;
    return true;
}

// 0x006BFCDC (name is ours)
f32 ToolBank::GetVisibilityFrame(const SvFgName* item)
{
    if (item->GetKind() != 14) {
        return 0.0f;
    }
    const u32 index = GetAxeDamage(item);
    if (index >= 8) {
        return 2.0f;
    }
    return s_VisibilityFrames[index];
}

// 0x006BFD24 slot 0x18 (name after libgarden)
bool ToolBank::Initialize(const Keyword& keyword, u32 slot, bool isReady)
{
    if (!isReady) {
        if (!stage::IsKind(stage::KIND_INDOOR, stage::GetCurrent()) && m_IsMyPlayer) {
            Func4DF040(1);
            Func4DF040(0);
        }
        return false;
    }
    const stage::Name stageName = stage::GetCurrent();
    if (stageName == stage::NAME_MINI_GAME_1 || stageName == stage::NAME_AMIIBO_CAMERA) {
        m_IsFloatShown = false;
    }
    g3d::SkeletalModel* model = &m_Models[slot];
    item::ResLoader* loader = GetLoader(slot, 0);
    ssys::ma::HeapAllocator* allocator = GetAllocator(slot);
    const s32 kind = Func311840(&keyword.m_Name);
    loader->Read(0x10000, 0x30000, 0x20000);
    loader->SetHeaps(GetVramAHeap(slot), GetVramBHeap(slot), NULL);
    loader->InitializeCgfx(allocator, Func536700(), true);
    if (Func76BA5C(&keyword.m_Name)) {
        loader->InitializeCgfx(allocator, GetLoader(slot, 1), true);
    }
    loader->InitializeCgfx(allocator, NULL, true);
    g3d::SetLightSetIndex(static_cast<const nw::gfx::res::ResModelData*>(loader->GetGraphicsFile().GetModels(0)),
                          Func1E891C());
    if (!model->Create(nw::gfx::res::ResSceneObject(loader->GetGraphicsFile().GetModels(0)), allocator, allocator, 0,
                       true, 1)) {
        return false;
    }
    if (kind == TOOL_KIND_FISHING_ROD) {
        m_FloatPosition.x = 0.0f;
        m_FloatPosition.y = 0.0f;
        m_FloatPosition.z = 0.0f;
        if (!m_FloatModels[slot].Create(nw::gfx::res::ResSceneObject(loader->GetGraphicsFile().GetModels(1)),
                                        allocator, allocator, 0, true, 1)) {
            return false;
        }
    } else if (kind == TOOL_KIND_SLINGSHOT) {
        for (u32 i = 0; i < 3; i++) {
            if (!m_SubModels[slot].m_Models[i].Create(
                    nw::gfx::res::ResSceneObject(loader->GetGraphicsFile().GetModels(1)), allocator, allocator, 0, true,
                    1)) {
                return false;
            }
        }
    }
    m_EffectBoneIndex = -1;
    if (model->GetSkeletalModel() != NULL) {
        m_EffectBoneIndex = model->GetBoneIndex("eff");
    }
    m_WindmillTargetSpeed = 0.0f;
    m_WindmillSpeed = 0.0f;
    m_BoneRotateX.value = 0;
    m_IsBoneRotated = false;
    m_IsBoneTransformSaved = false;
    g3d::MorphSkeletalAnim* morph = &m_MorphAnims[slot];
    g3d::VisibilityAnim* visibility = &m_VisibilityAnims[slot];
    g3d::MaterialAnim* material = &m_MaterialAnims[slot];
    g3d::MaterialAnim* itemMaterial = &m_ItemMaterialAnims[slot];
    f32 frame = 0.0f;
    switch (kind) {
    case TOOL_KIND_AXE:
        if (Func76D69C(&keyword.m_Name)) {
            const nw::gfx::res::ResGraphicsFile file = loader->GetGraphicsFile();
            if (file.GetVisibilityAnimsCount() != 0) {
                visibility->Build(model, nw::anim::res::ResAnim(file.GetVisibilityAnims(0)), allocator);
                visibility->SetFrame(GetVisibilityFrame(&keyword.m_Name));
                visibility->SetSpeed(frame);
                visibility->SetPlayMode(0);
                model->SetVisibilityAnim(visibility);
            }
        }
        break;
    case TOOL_KIND_FISHING_ROD:
        BuildMorphAnim(morph, model, HumanAnimName::TF_POLE_WAIT_1, allocator, 1, 1.0f);
        model->SetSkeletalAnim(morph);
        break;
    case TOOL_KIND_NET:
        BuildMorphAnim(morph, model, HumanAnimName::TF_NET_WAIT_1, allocator, 1, 1.0f);
        model->SetSkeletalAnim(morph);
        break;
    case TOOL_KIND_SHOVEL:
        BuildMorphAnim(morph, model, HumanAnimName::TF_NPC_SCOOP_WAIT_1, allocator, 0, frame);
        model->SetSkeletalAnim(morph);
        break;
    case TOOL_KIND_DANDELION_PUFFS:
        BuildMorphAnim(morph, model, HumanAnimName::TF_DANDELION_1_SCALE_1, allocator, 0, frame);
        model->SetSkeletalAnim(morph);
        break;
    case TOOL_KIND_UMBRELLA:
        BuildMorphAnim(morph, model, HumanAnimName::TF_UMB_OPEN_1, allocator, 0, frame);
        morph->SetFrame(GetAnimFrameSize(HumanAnimName::TF_UMB_OPEN_1) - 1.0f);
        model->SetSkeletalAnim(morph);
        break;
    case TOOL_KIND_TOY_HAMMER:
        BuildMorphAnim(morph, model, HumanAnimName::TS_HUMMER_SWING_1, allocator, 0, frame);
        model->SetSkeletalAnim(morph);
        break;
    case TOOL_KIND_SLINGSHOT:
        BuildMorphAnim(morph, model, HumanAnimName::TF_SLING_WAIT_1, allocator, 0, frame);
        model->SetSkeletalAnim(morph);
        break;
    case TOOL_KIND_SPARKLER: {
        const HumanAnimEntry* entry = Func770970(&g_HumanAnimTable, HumanAnimName::TF_FIRE_SETOFF_2);
        morph->Build(model, entry->anim0, allocator, 0);
        morph->SetPlayMode(0);
        morph->SetSpeed(frame);
        model->SetSkeletalAnim(morph);
        material->Build(model, entry->anim3, allocator, false);
        material->SetPlayMode(0);
        material->SetSpeed(frame);
        model->SetMaterialAnim(material, 0);
        break;
    }
    case TOOL_KIND_BALLOON:
        m_pBalloonStrings[slot] = BalloonString::Create(allocator->m_pHeap);
        break;
    case TOOL_KIND_MEGAPHONE:
        BuildMorphAnim(morph, model, HumanAnimName::TF_MEGAPHONE_TALK_1, allocator, 1, frame);
        model->SetSkeletalAnim(morph);
        break;
    case TOOL_KIND_DRINK:
        BuildMorphAnim(morph, model, HumanAnimName::TS_TAKEOUTCOFFEE_TUMBLE_1, allocator, 0, frame);
        morph->SetFrame(frame);
        model->SetSkeletalAnim(morph);
        break;
    case TOOL_KIND_PINWHEEL:
        BuildMorphAnim(morph, model, HumanAnimName::TF_WINDMILL_WAIT_1, allocator, 1, 1.0f);
        model->SetSkeletalAnim(morph);
        itemMaterial->Build(model, GetTextureAnim(loader), allocator, false);
        itemMaterial->SetFrame(static_cast<f32>(item::GetIndex(keyword.m_Name)));
        itemMaterial->SetSpeed(frame);
        itemMaterial->SetPlayMode(0);
        model->SetMaterialAnim(itemMaterial, 0);
        model->EvaluateAnims();
        m_IsBoneRotated = true;
        break;
    case TOOL_KIND_TWEETER:
        BuildMorphAnim(morph, model, HumanAnimName::TF_BLOWOUTS_1, allocator, 0, frame);
        model->SetSkeletalAnim(morph);
        break;
    case TOOL_KIND_BAG:
        BuildMorphAnim(morph, model, HumanAnimName::TS_SANTA_BAG_EQUIP_1, allocator, 0, frame);
        morph->SetFrame(GetAnimFrameSize(HumanAnimName::TS_SANTA_BAG_EQUIP_1) - 1.0f);
        model->SetSkeletalAnim(morph);
        break;
    case TOOL_KIND_FRYING_PAN:
        BuildMorphAnim(morph, model, HumanAnimName::TF_SKILLET_WAIT_1, allocator, 1, 1.0f);
        model->SetSkeletalAnim(morph);
        break;
    }
    if (!stage::IsKind(stage::KIND_INDOOR, stage::GetCurrent()) && m_IsMyPlayer) {
        if (Func76D6C0(&keyword.m_Name)) {
            Func4DF040(1);
            Func4DEF14(0);
        } else if (Func76D600(&keyword.m_Name)) {
            Func4DF040(0);
            Func4DEF14(1);
        } else {
            Func4DF040(1);
            Func4DF040(0);
        }
    }
    SoundState& sound = m_SoundStates[slot];
    if (!sound.m_IsStarted) {
        AudioObj1& audio = m_Sounds[slot];
        audio.Start(&m_FloatPosition);
        audio.SetIsIgnoreMaster(m_IsMyPlayer);
        audio.m_Obj.SetIsUseHeightEntry(!m_IsMyPlayer);
        sound.m_IsStarted = true;
    }
    return true;
}

// 0x006C08F4 (name is ours)
void ToolBank::UpdateWindmill(const nn::math::VEC3* from, const nn::math::VEC3* to, const math::Angle16* angle)
{
    if (!m_IsBoneRotated) {
        m_WindmillVolume = 0;
        return;
    }
    const nn::math::VEC3 diff(to->x - from->x, to->y - from->y, to->z - from->z);
    const f32 distance = std::sqrt(diff.x * diff.x + diff.z * diff.z);
    if (distance * distance >= 0.01f) {
        const math::Angle16 direction = Actor::CalcAngleY(from, to);
        const s32 angleDistance =
            ssys::ma::GetAngleDistance(static_cast<s16>(direction.value), static_cast<s16>(angle->value));
        if (angleDistance <= 0x2000) {
            m_WindmillTargetSpeed =
                (1.0f - static_cast<f32>(angleDistance) * (1.0f / 8192.0f)) * (distance / Func651E54());
        }
    }
    if ((m_WindmillSpeed >= 0.0f && m_WindmillTargetSpeed > m_WindmillSpeed) ||
        (m_WindmillSpeed <= 0.0f && m_WindmillTargetSpeed < m_WindmillSpeed)) {
        ssys::ma::Chase(&m_WindmillSpeed, m_WindmillTargetSpeed, 0.1f);
        if (m_WindmillSpeed == m_WindmillTargetSpeed) {
            m_WindmillTargetSpeed = 0.0f;
        }
    } else {
        ssys::ma::Chase(&m_WindmillSpeed, 0.0f, 0.035f);
    }
    const f32 speed = m_WindmillSpeed;
    m_BoneRotateX.value = static_cast<s16>(m_BoneRotateX.value) + static_cast<s16>(static_cast<s32>(speed * 5120.0f));
    m_WindmillVolume = static_cast<s16>(std::fabs(speed) * 127.0f);
}

// 0x006C0A98 (name is ours)
void ToolBank::UpdateFrames()
{
    SoundState& sound = m_SoundStates[GetCurrentSlot()];
    if (sound.m_IsStarted) {
        m_Sounds[GetCurrentSlot()].SetPosition(&m_FloatPosition);
        sound.m_HasPosition = true;
    }
    g3d::SkeletalModel* model = GetCurrentModel();
    if (model != NULL) {
        model->UpdateAnimFrames();
    }
}

// 0x006C0B04 (name is ours)
void ToolBank::GetRodTipPosition(nn::math::VEC3* out, const nn::math::MTX34* matrix)
{
    nn::math::MTX34 tip(*matrix);
    ssys::ma::RotateZ(&tip, -15359);
    ssys::ma::RotateX(&tip, -10239);
    const nn::math::MTX34 offset(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -7.0f, 0.0f, 0.0f, 1.0f, 3.5f);
    sead::Matrix34CalcCtr<f32>::multiply(tip, tip, offset);
    const nn::math::VEC3 position(tip.m[0][3], tip.m[1][3], tip.m[2][3]);
    Func52EA98(out, &position);
}

// 0x006C0BD4 (name is ours)
void ToolBank::HookFloat(const Actor* actor, const nn::math::MTX34* matrix, const nn::math::VEC3* position)
{
    m_FloatPosition.x = position->x;
    m_FloatPosition.y = position->y;
    m_FloatPosition.z = position->z;
    ChangeFloatState(FLOAT_STATE_HOOKED, actor, matrix);
}

// 0x006C0C08 (name is ours)
void ToolBank::ChangeFloatState(u32 state, const Actor* actor, const nn::math::MTX34* matrix)
{
    sead::Matrix34CalcCtr<f32>::copy(m_RodMatrix, *matrix);
    if (actor != NULL) {
        m_ActorPosition.x = actor->m_Position.x;
        m_ActorPosition.y = actor->m_Position.y;
        m_ActorPosition.z = actor->m_Position.z;
        m_ActorAngleY = actor->m_Rotation.y;
    }
    if (m_FloatState < FLOAT_STATE_NUM) {
        (this->*s_FloatStates[m_FloatState].exit)(state);
    }
    m_FloatState = state;
    if (state < FLOAT_STATE_NUM) {
        const FloatStateInfo& info = s_FloatStates[state];
        if (info.isClearHandle) {
            m_FloatHandle.m_Value = 0;
        }
        (this->*info.enter)(actor, matrix);
    }
}

// 0x006C0D04 slot 0x1C (name after libgarden)
bool ToolBank::Finalize(const Keyword& keyword, u32 slot)
{
    // (the result is not used)
    Func311840(&keyword.m_Name);
    m_MorphAnims[slot].Finalize();
    m_VisibilityAnims[slot].Finalize();
    m_MaterialAnims[slot].Finalize();
    m_ItemMaterialAnims[slot].Finalize();
    m_AnimIds[slot] = HumanAnimName::COUNT;
    m_Models[slot].Finalize();
    if (m_FloatModels[slot].m_pNode != NULL) {
        m_FloatModels[slot].Finalize();
    }
    sead::Heap* heap = GetAllocator(slot)->m_pHeap;
    if (m_pBalloonStrings[slot] != NULL) {
        m_pBalloonStrings[slot]->Destroy();
        heap->free(m_pBalloonStrings[slot]);
        m_pBalloonStrings[slot] = NULL;
    }
    for (u32 i = 0; i < 3; i++) {
        g3d::SkeletalModel& subModel = m_SubModels[slot].m_Models[i];
        if (subModel.m_pNode != NULL) {
            subModel.g3d::Model::Finalize();
        }
    }
    GetLoader(slot, 0)->Release();
    GetLoader(slot, 1)->Release();
    SoundState& sound = m_SoundStates[slot];
    if (sound.m_IsStarted) {
        m_Sounds[slot].Stop();
        sound.m_IsStarted = false;
        sound.m_HasPosition = false;
    }
    return true;
}

// 0x006C0E80 (name is ours)
s32 ToolBank::FindGroundHitOfPlayer(nn::math::VEC3* out, const AcPlayer* player)
{
    return FindGroundHit(out, &player->m_Model, static_cast<const nn::math::VEC3*>(&player->m_Position));
}

// 0x006C0E94 (name is ours)
s32 ToolBank::FindGroundHit(nn::math::VEC3* out, const HumanModel* model, const nn::math::VEC3* position)
{
    const f32 unit = FdInfo::UNIT_WORLD_SIZE_XF;
    const f32 headEnd = unit * s_HitHeadEnd;
    const f32 headX = unit * s_HitHeadHalfWidth;
    const f32 headMiddle = (s_HitHeadEnd + s_HitHeadLength) * unit * 0.5f;
    const nn::math::VEC3 points[7] = {
        nn::math::VEC3(unit * s_HitPoint0X, 0.0f, unit * s_HitPoint0Z),
        nn::math::VEC3(headX, -4.0f, headEnd),
        nn::math::VEC3(headX, 4.0f, headEnd),
        nn::math::VEC3(headX, -4.0f, headMiddle),
        nn::math::VEC3(headX, 4.0f, headMiddle),
        nn::math::VEC3(FdInfo::UNIT_WORLD_SIZE_XF * s_HitHandleX, 0.0f, FdInfo::UNIT_WORLD_SIZE_XF * s_HitHeadLength),
        nn::math::VEC3(FdInfo::UNIT_WORLD_SIZE_XF * s_HitGripX, 0.0f, FdInfo::UNIT_WORLD_SIZE_XF * s_HitGripZ),
    };
    nn::math::MTX34 matrix;
    model->GetBoneMatrix(&matrix, model->m_BoneIndices[3], false);
    s32 index = 0;
    for (const nn::math::VEC3* point = points; point < &points[7]; point++, index++) {
        const bool isFirst = point == points;
        nn::math::VEC3 world;
        sead::Vector3CalcCtr<f32>::mul(world, matrix, *point);
        nn::math::VEC3 curved;
        Func52EA98(&curved, &world);
        f32 height;
        if (!isFirst) {
            height = GetGroundHeight(&curved);
            if (FdInfo::GetHeight(curved, FIELD_NAME_CURRENT) != height) {
                const f32 limit = FdInfo::GetHeightOrBgHeight(curved, FIELD_NAME_CURRENT) + s_HitHeightMargin;
                if (height > limit) {
                    height = limit;
                }
            }
        } else {
            height = GetGroundHeightNear(&curved, position);
        }
        if (curved.y <= height) {
            if (out != NULL) {
                *out = curved;
            }
            return index;
        }
    }
    return -1;
}

// 0x006C1118 (name is ours)
void ToolBank::UpdateFloat(const Actor* actor, const nn::math::MTX34* matrix)
{
    if (!m_IsFloatShown) {
        return;
    }
    sead::Matrix34CalcCtr<f32>::copy(m_RodMatrix, *matrix);
    if (actor != NULL) {
        m_ActorPosition.x = actor->m_Position.x;
        m_ActorPosition.y = actor->m_Position.y;
        m_ActorPosition.z = actor->m_Position.z;
        m_ActorAngleY = actor->m_Rotation.y;
    }
    if (m_FloatState < FLOAT_STATE_NUM) {
        (this->*s_FloatStates[m_FloatState].update)(actor, matrix);
    }
    g3d::Model* model = &m_FloatModels[GetCurrentSlot()];
    if (model->m_pNode != NULL) {
        nn::math::MTX34 world;
        Func52E9D8(&world, &m_FloatPosition, &m_FloatRotation);
        model->SetMatrix(world);
        model->UpdateAnimFrames();
        model->Update();
    }
}

// 0x006C122C (name is ours)
bool ToolBank::GetToolPoints(nn::math::VEC3* point0, nn::math::VEC3* point1, nn::math::VEC3* point2,
                             nn::math::VEC3* point3, const HumanModel* model)
{
    const f32 unit = FdInfo::UNIT_WORLD_SIZE_XF;
    const f32 x = unit * s_HitHeadHalfWidth;
    const f32 far = unit * 0.9f;
    const f32 near = unit * 0.65f;
    const nn::math::VEC3 farOuter(x, 0.0f, far);
    const nn::math::VEC3 farInner(x * 0.5f, 0.0f, far);
    const nn::math::VEC3 nearOuter(x, 0.0f, near);
    const nn::math::VEC3 nearInner(x * 0.5f, 0.0f, near);
    nn::math::MTX34 matrix;
    if (!model->GetBoneMatrix(&matrix, model->m_BoneIndices[3], false)) {
        return false;
    }
    nn::math::VEC3 worldA;
    nn::math::VEC3 worldB;
    sead::Vector3CalcCtr<f32>::mul(worldA, matrix, farInner);
    sead::Vector3CalcCtr<f32>::mul(worldB, matrix, farOuter);
    Func52EA98(point0, &worldA);
    Func52EA98(point1, &worldB);
    sead::Vector3CalcCtr<f32>::mul(worldA, matrix, nearInner);
    sead::Vector3CalcCtr<f32>::mul(worldB, matrix, nearOuter);
    Func52EA98(point2, &worldA);
    Func52EA98(point3, &worldB);
    return true;
}

// 0x006C1384 (name is ours)
bool ToolBank::PlayToolAnim(u16 humanAnimId, s32 playMode, bool isKeep, f32 frame, f32 speed, f32 morphFrames,
                            u8 unknown)
{
    m_Unknown7E4 = unknown;
    SvFgName item(m_Keywords[GetCurrentSlot()].m_Name);
    if (item.IsEmpty()) {
        return false;
    }
    const s32 kind = Func311840(&item);
    const HumanAnimInfo* info = Func6E6268(humanAnimId);
    u16 id;
    switch (kind) {
    case TOOL_KIND_FISHING_ROD:
        id = info->toolAnimFishingRod;
        if (id >= HumanAnimName::COUNT || id == 0) {
            return false;
        }
        return PlayAnim(id, playMode, isKeep, frame, speed, morphFrames);
    case TOOL_KIND_NET:
        id = info->toolAnimNet;
        if (id >= HumanAnimName::COUNT || id == 0) {
            return false;
        }
        return PlayAnim(id, playMode, isKeep, frame, speed, morphFrames);
    case TOOL_KIND_SHOVEL:
        if (m_Unknown7E4 == 2) {
            return false;
        }
        id = info->toolAnimShovel;
        if (id >= HumanAnimName::COUNT || id == 0) {
            return false;
        }
        return PlayAnim(id, playMode, isKeep, frame, speed, morphFrames);
    case TOOL_KIND_DANDELION_PUFFS:
        if (humanAnimId == HumanAnimName::HANDTOOL_FLY_1) {
            SetAnimFrame(0.0f);
            SetAnimSpeed(1.0f);
        } else {
            SetAnimFrame(0.0f);
            SetAnimSpeed(0.0f);
        }
        return false;
    case TOOL_KIND_UMBRELLA: {
        if (humanAnimId == HumanAnimName::UMB_OPEN_1) {
            return PlayAnim(HumanAnimName::TF_UMB_OPEN_1, playMode, false, frame, speed, morphFrames);
        }
        if (humanAnimId == HumanAnimName::UMB_ROTATE_1) {
            return PlayAnim(HumanAnimName::TF_UMB_ROTATE_1, playMode, false, frame, speed, 0.0f);
        }
        g3d::MorphSkeletalAnim* morph = GetMorphAnim();
        const f32 animSpeed = morph != NULL && morph->m_pAnimObject != NULL ? morph->GetSpeed() : 0.0f;
        if (animSpeed == 0.0f) {
            return false;
        }
        return PlayAnim(HumanAnimName::TF_UMB_OPEN_1, 0, false,
                        GetAnimFrameSize(HumanAnimName::TF_UMB_OPEN_1) - 1.0f, 0.0f, 0.0f);
    }
    case TOOL_KIND_TOY_HAMMER:
        id = info->toolAnimToyHammer;
        if (id < HumanAnimName::COUNT && id != 0) {
            return PlayAnim(id, 0, isKeep, frame, speed, morphFrames);
        }
        return PlayAnim(HumanAnimName::TS_HUMMER_SWING_1, 0, isKeep,
                        GetAnimFrameSize(HumanAnimName::TS_HUMMER_SWING_1) - 1.0f, 0.0f, morphFrames);
    case TOOL_KIND_SLINGSHOT:
        id = info->toolAnimSlingshot;
        if (id >= HumanAnimName::COUNT || id == 0) {
            return false;
        }
        return PlayAnim(id, playMode, isKeep, frame, speed, morphFrames);
    case TOOL_KIND_SPARKLER:
        if (humanAnimId == HumanAnimName::FIRE_SETOFF_1) {
            return PlayAnim(HumanAnimName::TF_FIRE_SETOFF_2, 0, false, 0.0f, speed, 0.0f);
        }
        SetAnimFrame(0.0f);
        SetAnimSpeed(0.0f);
        return false;
    case TOOL_KIND_MEGAPHONE:
        if (humanAnimId == HumanAnimName::MEGAPHONE_TALK_1) {
            SetAnimSpeed(1.0f);
        } else {
            SetAnimFrame(0.0f);
            SetAnimSpeed(0.0f);
        }
        return false;
    case TOOL_KIND_PINWHEEL:
        if (humanAnimId == HumanAnimName::HANDTOOL_FLY_1) {
            return PlayAnim(HumanAnimName::TF_WINDMILL_TURN_1, 0, true, 0.0f, 1.0f, 2.0f);
        }
        return PlayAnim(HumanAnimName::TF_WINDMILL_WAIT_1, 1, true, 0.0f, 1.0f, 10.0f);
    case TOOL_KIND_TWEETER:
        if (humanAnimId == HumanAnimName::HANDTOOL_BLOW_1) {
            SetAnimFrame(0.0f);
            SetAnimSpeed(1.0f);
        }
        return false;
    case TOOL_KIND_BAG:
        if (humanAnimId == 20 || humanAnimId == 19) {
            PlayAnim(HumanAnimName::TS_SANTA_BAG_EQUIP_1, 0, false,
                     GetAnimFrameSize(HumanAnimName::TS_SANTA_BAG_EQUIP_1) - 1.0f, -1.0f, 0.0f);
        } else if (humanAnimId == 21) {
            if (speed > 0.0f) {
                PlayAnim(HumanAnimName::TS_SANTA_BAG_PUTAWAY_1, 0, false, 0.0f, 1.0f, 0.0f);
            } else {
                PlayAnim(HumanAnimName::TS_SANTA_BAG_PUTAWAY_1, 0, false,
                         GetAnimFrameSize(HumanAnimName::TS_SANTA_BAG_PUTAWAY_1) - 1.0f, -1.0f, 0.0f);
            }
        }
        return false;
    case TOOL_KIND_FRYING_PAN:
        if (m_Unknown7E4 == 1) {
            id = info->toolAnimFryingPan1;
            if (id >= HumanAnimName::COUNT) {
                return false;
            }
        } else {
            id = info->toolAnimFryingPan;
            if (id >= HumanAnimName::COUNT) {
                return false;
            }
        }
        if (id == 0) {
            return false;
        }
        return PlayAnim(id, playMode, isKeep, frame, speed, morphFrames);
    default:
        return false;
    }
}

// 0x006C1988 (name is ours)
void ToolBank::SetAnimPlayMode(s32 playMode)
{
    g3d::MorphSkeletalAnim* morph = GetMorphAnim();
    if (morph != NULL && morph->m_pAnimObject != NULL) {
        morph->SetPlayMode(playMode);
    }
    g3d::MaterialAnim* material = GetMaterialAnim();
    if (material != NULL && material->m_pAnimObject != NULL) {
        material->SetPlayMode(playMode);
    }
}

// 0x006C1A08 slot 0x14 (name after libgarden)
s32 ToolBank::LoadResourceSplit(const Keyword& keyword, u32 slot)
{
    if (!keyword.m_Name.IsUsual() || Func76BB44(&keyword.m_Name) != 7) {
        return 2;
    }
    sead::Heap* heap = GetAllocator(slot)->m_pHeap;
    item::ResLoader* model = GetLoader(slot, 0);
    bool isLoaded = model->Func536B74(&keyword.m_Name, heap, 0, 0, NULL, 1, 0);
    item::ResLoader* textures = GetLoader(slot, 1);
    if (Func76BA5C(&keyword.m_Name)) {
        SvFgName design;
        Func76C8E0(&design, &keyword.m_Name);
        if (!design.IsEmpty()) {
            const void* data = keyword.GetDesignData();
            if (data != NULL) {
                isLoaded &= textures->Func536B74(&design, heap, 3, 1, data, 0, 0);
            }
        }
    }
    if (!isLoaded) {
        return 0;
    }
    if (model->IsLoadFailed() || textures->IsLoadFailed()) {
        model->Release();
        textures->Release();
        return 2;
    }
    return 1;
}

// 0x006C1BEC (name is ours)
void ToolBank::SetSubModelPositions(const nn::math::VEC3* position0, const nn::math::VEC3* position1,
                                    const nn::math::VEC3* position2)
{
    const nn::math::VEC3 positions[3] = {*position0, *position1, *position2};
    SubModels& subModels = m_SubModels[GetCurrentSlot()];
    for (u32 i = 0; i < 3; i++) {
        g3d::SkeletalModel* model = &subModels.m_Models[i];
        if (model->m_pNode != NULL) {
            nn::math::MTX34 matrix;
            math::Angle16 angle;
            angle.value = 0;
            MakeWorldCurveMatrix(&matrix, &positions[i], angle);
            model->SetMatrix(matrix);
            model->UpdateAnimFrames();
            model->Update();
        }
    }
}

// 0x006C1CB4 slot 0x0C
void ToolBank::OnRequest(const Keyword& keyword, u32 slot)
{
    // empty (in the original too)
}

// 0x006C1CB8 (name is ours)
f32 ToolBank::GetGroundHeight(const nn::math::VEC3* position)
{
    if (!stage::IsKind(stage::KIND_AUTO_CAMP, stage::GetCurrent())) {
        return FdInfo::Func6C8C4C(*position, FIELD_NAME_CURRENT);
    }
    const f32 height = FdInfo::Func6C8C4C(*position, FIELD_NAME_CURRENT);
    const bool isLow = height == 112.0f;
    if (position->x < 0.0f) {
        return isLow ? 112.0f : 140.0f;
    }
    if (0.0f > position->z) {
        return 140.0f;
    }
    s32 kindX;
    s32 kindZ;
    Func307340(&kindX, &kindZ, position);
    if (kindX != 0) {
        return isLow ? 112.0f : 140.0f;
    }
    if (kindZ != 0) {
        return 140.0f;
    }
    f32 result = height;
    const f32 other = FdInfo::Func6C880C(*position, FIELD_NAME_CURRENT);
    if (result < other) {
        result = other;
    }
    const void* entry = Func4EA078(Func4E1338(), position, 0);
    if (entry != NULL) {
        const f32 add = Func7529D0(entry) ? 13.0f : 32.0f;
        if (result < height + add) {
            result = height + add;
        }
    }
    if (result == 120.0f) {
        result = 140.0f;
    }
    return result;
}

// 0x006C1E24 (name is ours)
f32 ToolBank::GetGroundHeightNear(const nn::math::VEC3* position, const nn::math::VEC3* other)
{
    const f32 height = GetGroundHeight(position);
    // 0x0095D584 (guard 0x0095D580)
    static const f32 s_HighMargin = FdInfo::UNIT_WORLD_SIZE_ZF * 0.55f;
    // 0x0095D588 (guard 0x0095D57C)
    static const f32 s_LowMargin = FdInfo::UNIT_WORLD_SIZE_ZF * 0.3f;
    bool isPart = false;
    bool isAttribute = false;
    f32 base = 0.0f;
    if (FdInfo::HasPart95E93E(*position, FIELD_NAME_CURRENT)) {
        isPart = true;
        if (other != NULL) {
            const u32 attribute = FdInfo::GetPartAttribute(*other, FIELD_NAME_CURRENT);
            if (attribute == 23 || attribute == 24) {
                base = other->y;
                return GetHeightWithFieldItem(other, base, height, s_LowMargin, s_HighMargin);
            }
        }
    } else {
        const u32 attribute = FdInfo::GetPartAttribute(*position, FIELD_NAME_CURRENT);
        if (attribute == 23 || attribute == 24) {
            base = GetGroundHeight(position);
            isAttribute = true;
        }
    }
    if (!isPart && !isAttribute) {
        return height;
    }
    const f32 radius = FdInfo::UNIT_WORLD_SIZE_XF * 0.6f;
    for (s32 angle = 0; angle < 0x10000; angle += 0x1000) {
        nn::math::VEC3 point = *position;
        point.x += radius * nn::math::CosIdx(static_cast<u16>(angle));
        point.z += radius * nn::math::SinIdx(static_cast<u16>(angle));
        if (isPart) {
            const u32 attribute = FdInfo::GetPartAttribute(point, FIELD_NAME_CURRENT);
            if (attribute == 23 || attribute == 24) {
                base = FdInfo::GetHeight(point, FIELD_NAME_CURRENT);
                return GetHeightWithFieldItem(&point, base, height, s_LowMargin, s_HighMargin);
            }
        } else if (FdInfo::HasPart95E93E(point, FIELD_NAME_CURRENT)) {
            return GetHeightWithFieldItem(position, base, height, s_LowMargin, s_HighMargin);
        }
    }
    return height;
}

// 0x006C21C8 (name is ours)
void ToolBank::BiteFloat(const Actor* actor, const nn::math::MTX34* matrix)
{
    ChangeFloatState(FLOAT_STATE_BITE, actor, matrix);
}

// 0x006C21D8 (name is ours)
void ToolBank::HoldFloat(const Actor* actor, const nn::math::MTX34* matrix, u16 followFrames)
{
    m_FloatFollowFrames = followFrames;
    ChangeFloatState(FLOAT_STATE_HOLD, actor, matrix);
}

// 0x006C21F0 (name is ours)
void ToolBank::UpdateFloatHooked(const Actor* actor, const nn::math::MTX34* matrix)
{
    nn::math::VEC3 position = m_HookedTarget;
    position.y = FdInfo::GetSurfaceHeightEx(position, FIELD_NAME_CURRENT);
    if (m_HookedEffect.GetEmitterSet() != NULL) {
        m_HookedEffect.SetTransform(&position, &ANGLE16_VEC3_ZERO, true);
    } else {
        Func207998(&m_HookedEffect, EFFECT_FLOAT_HOOKED, &position, &ANGLE16_VEC3_ZERO, &VEC3_ONE, true, 0);
        m_HookedEffect.SetTransform(&position, &ANGLE16_VEC3_ZERO, true);
        m_HookedEffect.SetPtclCallback(Func6889B4, -1);
    }
    if (m_HookedEffect.GetEmitterSet() != NULL) {
        if (Func6F9D80(&position, FdInfo::UNIT_WORLD_SIZE_HALF_XF)) {
            m_HookedEffect.SetEmitRate(1.0f);
        } else {
            m_HookedEffect.SetEmitRate(0.0f);
        }
    }
    m_HookedDistance += 0.5f;
    if (m_HookedDistance > m_HookedMaxDistance) {
        m_HookedDistance = m_HookedMaxDistance;
    }
    const math::Angle16 angle = Actor::CalcAngleY(&m_HookedStart, &m_HookedTarget);
    nn::math::VEC3 offset(0.0f, 0.0f, m_HookedDistance);
    ssys::ma::RotateY(&offset, static_cast<s16>(angle.value));
    m_FloatPosition = nn::math::VEC3(m_HookedStart.x + offset.x, m_HookedStart.y + offset.y,
                                     m_HookedStart.z + offset.z);
    if (FdInfo::IsPart95E93ENonZero(m_FloatPosition, FIELD_NAME_CURRENT)) {
        const f32 surface = FdInfo::GetSurfaceHeightEx(m_FloatPosition, FIELD_NAME_CURRENT);
        if (!ssys::ma::Chase(&m_FloatPosition.y, surface - 1.0f, 0.2f)) {
            m_FloatPosition.y = surface - 2.0f;
        }
    }
    m_FloatRotation.y.value = angle.value + 0x4000;
    if (ssys::ma::ChaseDivide(reinterpret_cast<s16*>(&m_FloatRotation.x.value),
                              static_cast<s16>(m_FloatTargetRotation.x.value), 10, 0x3000, 1000) == 0) {
        m_FloatTargetRotation.x.value = -2047 - GetRandom(1000);
    }
}

// 0x006C2488 (name is ours)
bool ToolBank::GetScoopPosition(nn::math::VEC3* out, const HumanModel* model)
{
    nn::math::MTX34 matrix;
    if (!model->GetBoneMatrix(&matrix, model->m_BoneIndices[3], false)) {
        return false;
    }
    const nn::math::VEC3 local(9.0f, 0.0f, FdInfo::UNIT_WORLD_SIZE_XF * 1.15f);
    nn::math::VEC3 world;
    sead::Vector3CalcCtr<f32>::mul(world, matrix, local);
    nn::math::VEC3 curved;
    Func52EA98(&curved, &world);
    *out = curved;
    return true;
}

// 0x006C2520 (name is ours)
bool ToolBank::ReleaseBalloon(const math::Angle16* direction)
{
    SvFgName item(m_Keywords[GetCurrentSlot()].m_Name);
    if (Func311840(&item) != TOOL_KIND_BALLOON) {
        return false;
    }
    BalloonString* balloon = m_pBalloonStrings[GetCurrentSlot()];
    if (balloon != NULL && balloon->IsReleased()) {
        return true;
    }
    if (balloon != NULL && !(balloon->m_IsReleased && balloon->m_ReleaseFrames == 0)) {
        balloon->Release(*direction);
        return true;
    }
    return true;
}

// 0x006C25DC (name is ours)
void ToolBank::ThrowFloatToWater(const Actor* actor, const nn::math::MTX34* matrix, const nn::math::VEC3* position,
                                 const nn::math::VEC3* landing)
{
    if (std::fabs(m_FloatPosition.x) <= FLT_EPSILON && std::fabs(m_FloatPosition.z) <= FLT_EPSILON) {
        m_FloatPosition = *position;
    }
    m_FloatTarget = *landing;
    m_FloatTarget.y = FdInfo::GetSurfaceHeight(m_FloatTarget, FIELD_NAME_CURRENT);
    m_IsFloatInWater = true;
    ChangeFloatState(FLOAT_STATE_CAST, actor, matrix);
}

// 0x006C267C (name is ours)
void ToolBank::AddSubModelsToScene(bool isShown0, bool isShown1, bool isShown2)
{
    if (!isShown0 && !isShown1 && !isShown2) {
        return;
    }
    const bool isShown[3] = {isShown0, isShown1, isShown2};
    const f32 scale = unknown;
    SubModels& subModels = m_SubModels[GetCurrentSlot()];
    for (u32 i = 0; i < 3; i++) {
        if (isShown[i]) {
            g3d::SkeletalModel* model = &subModels.m_Models[i];
            if (model->m_pNode != NULL) {
                model->SetScale(nn::math::VEC3(scale, scale, scale));
                model->AddToScene(0);
            }
        }
    }
}

// 0x006C2724 (name is ours)
void ToolBank::UpdateFloatSink(const Actor* actor, const nn::math::MTX34* matrix)
{
    CountDown(&m_SinkFrames);
    const u8 flow = GetAttribute95E83FAt(m_FloatPosition, FIELD_NAME_CURRENT);
    if (flow != FLOW_NONE) {
        const u16 angle = flow << 13;
        const f32 sin = nn::math::SinIdx(angle);
        const f32 cos = nn::math::CosIdx(angle);
        m_FloatPosition.x += s_FloatDriftSpeed * sin;
        m_FloatPosition.z += s_FloatDriftSpeed * cos;
    }
    if (m_SinkFrames == 0) {
        m_SinkSpeed += s_FloatGravity;
        m_FloatPosition.y += m_SinkSpeed;
        const f32 surface = FdInfo::GetSurfaceHeightEx(m_FloatPosition, FIELD_NAME_CURRENT);
        if (m_FloatPosition.y < surface) {
            m_FloatPosition = nn::math::VEC3(m_FloatPosition.x, surface, m_FloatPosition.z);
            ChangeFloatState(FLOAT_STATE_FLOAT, actor, matrix);
        }
    }
}

// 0x006C2854 (name is ours)
void ToolBank::UpdateFloatBite(const Actor* actor, const nn::math::MTX34* matrix)
{
    if (m_FloatHandle.m_Value != 0) {
        if (FdInfo::IsPart95E93ENonZero(m_FloatPosition, FIELD_NAME_CURRENT)) {
            ssys::ma::Chase(&m_FloatPosition.y, FdInfo::GetSurfaceHeightEx(m_FloatPosition, FIELD_NAME_CURRENT) - 3.5f, 2.0f);
        }
        m_FloatRotation.y.value += 800;
        if (ssys::ma::ChaseDivide(reinterpret_cast<s16*>(&m_FloatRotation.x.value),
                                  static_cast<s16>(m_FloatTargetRotation.x.value), 20, 0x2000, 600) == 0) {
            m_FloatTargetRotation.x.value = -m_FloatTargetRotation.x.value;
            s16 rotate = static_cast<s16>(GetRandom(400) + 1024);
            if (static_cast<s16>(m_FloatTargetRotation.x.value) <= 0) {
                rotate = -rotate;
            }
            m_FloatTargetRotation.x.value = rotate;
        }
    } else {
        nn::math::VEC3 position = m_FloatPosition;
        position.y = FdInfo::GetSurfaceHeightEx(position, FIELD_NAME_CURRENT);
        Func207B90(EFFECT_FLOAT_BITE_END, &position, &ANGLE16_VEC3_ZERO, &VEC3_ONE, true, 0);
        ChangeFloatState(FLOAT_STATE_FLOAT, actor, matrix);
    }
}

// 0x006C2994 (name is ours)
void ToolBank::UpdateFloatHold(const Actor* actor, const nn::math::MTX34* matrix)
{
    EnterFloatHold(actor, matrix);
    m_FloatTargetRotation.x.value = 0;
    ssys::ma::ChaseDivide(reinterpret_cast<s16*>(&m_FloatRotation.x.value),
                          static_cast<s16>(m_FloatTargetRotation.x.value), 20, 0x2000, 600);
}

// 0x006C29D0 (name is ours)
void ToolBank::ExitFloatHooked(u32 nextState)
{
    if (m_HookedEffect.GetEmitterSet() != NULL) {
        m_HookedEffect.Kill();
    }
}

// 0x006C29F8 (name is ours)
void ToolBank::ThrowFloat(const Actor* actor, const nn::math::MTX34* matrix, const nn::math::VEC3* position,
                          const nn::math::VEC3* landing)
{
    if (std::fabs(m_FloatPosition.x) <= FLT_EPSILON && std::fabs(m_FloatPosition.z) <= FLT_EPSILON) {
        m_FloatPosition = *position;
    }
    m_FloatTarget = *landing;
    m_FloatTarget.y = FdInfo::GetSurfaceHeight(m_FloatTarget, FIELD_NAME_CURRENT);
    m_IsFloatInWater = false;
    ChangeFloatState(FLOAT_STATE_CAST, actor, matrix);
}

// 0x006C2A98 (name is ours)
void ToolBank::UpdateFloatFall(const Actor* actor, const nn::math::MTX34* matrix)
{
    const f32 lastY = m_FloatPosition.y;
    CountDown(&m_FloatFallFrames);
    m_FloatSpeedY += s_FloatGravity;
    m_FloatPosition.y += m_FloatSpeedY;
    if (m_FloatSpeedY > 0.0f) {
        Func566440(&m_FloatPosition, &m_FloatTarget, 0.08f, 12.0f, 2.0f);
    } else if (m_FloatFallFrames != 0) {
        const f32 frames = static_cast<f32>(m_FloatFallFrames);
        m_FloatPosition.x += (m_FloatTarget.x - m_FloatPosition.x) / frames;
        m_FloatPosition.z += (m_FloatTarget.z - m_FloatPosition.z) / frames;
    } else {
        m_FloatPosition.x = m_FloatTarget.x;
        m_FloatPosition.z = m_FloatTarget.z;
    }
    if (m_FloatPosition.y > m_FloatTarget.y || lastY <= m_FloatTarget.y) {
        return;
    }
    const u32 slot = GetCurrentSlot();
    if (m_SoundStates[slot].m_HasPosition) {
        m_Sounds[slot].StartSound(SE_FLOAT_LAND);
    }
    ChangeFloatState(FLOAT_STATE_FLOAT, actor, matrix);
    if (m_PlayerNo < 4) {
        AcPlayer* player = PlayerGetActor(static_cast<netgame::PlayerNo>(m_PlayerNo & 0xFF), false);
        if (player != NULL) {
            player->Func67867C(&m_FloatPosition);
        }
    }
}

// 0x006C2C50 (name is ours)
void ToolBank::ExitFloatSink(u32 nextState)
{
    // empty (in the original too)
}

// 0x006C2C54 (name is ours)
void ToolBank::ExitFloatBite(u32 nextState)
{
    if (m_BiteEffect.GetEmitterSet() != NULL) {
        m_BiteEffect.Kill();
    }
}

// 0x006C2C7C (name is ours)
void ToolBank::ExitFloatHold(u32 nextState)
{
    // empty (in the original too)
}

// 0x006C2C80 (name is ours)
void ToolBank::SetBoneMatrix(g3d::SkeletalModel* parent, u32 bone)
{
    SvFgName item(m_Keywords[GetCurrentSlot()].m_Name);
    if (Func311840(&item) != TOOL_KIND_BALLOON) {
        Func3286CC(this, parent, bone);
        return;
    }
    g3d::SkeletalModel* model = GetCurrentModel();
    if (model == NULL) {
        return;
    }
    const nn::math::MTX34* boneMatrix = Func7258B0(this, parent, bone, NULL);
    if (boneMatrix == NULL) {
        return;
    }
    nn::math::MTX34 hand;
    hand = *boneMatrix;
    sead::Matrix34CalcCtr<f32>::scaleBases(hand, unknown, unknown, unknown);
    const nn::math::MTX34 balloon(*m_pBalloonStrings[GetCurrentSlot()]->vf_0x0C(&hand));
    model->SetMatrix(balloon);
    model->Update();
}

// 0x006C2D84 (name is ours)
void ToolBank::ReelFloat(const Actor* actor, const nn::math::MTX34* matrix, const nn::math::VEC3* position)
{
    m_FloatPosition.x = position->x;
    m_FloatPosition.y = position->y;
    m_FloatPosition.z = position->z;
    ChangeFloatState(FLOAT_STATE_REEL, actor, matrix);
}

// 0x006C2DB8 (name is ours)
void ToolBank::FloatFloat(const Actor* actor, const nn::math::MTX34* matrix, const nn::math::VEC3* position)
{
    m_FloatPosition.x = position->x;
    m_FloatPosition.y = position->y;
    m_FloatPosition.z = position->z;
    ChangeFloatState(FLOAT_STATE_FLOAT, actor, matrix);
}

// 0x006C2DEC (name is ours)
void ToolBank::PullOutFloat(const Actor* actor, const nn::math::MTX34* matrix, u16 humanAnimId, bool isReleaseHandle)
{
    m_PullOutAnimId = humanAnimId;
    ChangeFloatState(FLOAT_STATE_PULL_OUT, actor, matrix);
    if (isReleaseHandle) {
        Func27E23C(&m_FloatHandle.m_Value, 0);
    }
}

// 0x006C2E34 (name is ours)
void ToolBank::ExitFloatFall(u32 nextState)
{
    m_FloatPosition = m_FloatTarget;
    if (nextState != FLOAT_STATE_FLOAT) {
        return;
    }
    nn::math::VEC3 position = m_FloatPosition;
    position.y = FdInfo::GetSurfaceHeightEx(position, FIELD_NAME_CURRENT);
    Func207998(&m_LandEffect, EFFECT_FLOAT_LAND, &position, &ANGLE16_VEC3_ZERO, &VEC3_ONE, true, 0);
    m_LandEffect.SetTransform(&position, &ANGLE16_VEC3_ZERO, true);
    m_LandEffect.SetPtclCallback(Func6889B4, -1);
}

// 0x006C2EEC (name is ours)
void ToolBank::EnterFloatHooked(const Actor* actor, const nn::math::MTX34* matrix)
{
    m_FloatRotation.x.value = 0;
    m_FloatTargetRotation.x.value = -2047;
    m_HookedStart = m_FloatPosition;
    m_HookedTarget = m_FloatPosition;
    m_HookedMaxDistance = 2.5f;
    m_HookedDistance = 0.0f;
    if (m_FloatHandle.m_Value != 0) {
        switch (Func71B550(&m_FloatHandle.m_Value)) {
        case 2:
        case 3:
        case 6:
            m_HookedMaxDistance = 3.5f;
            break;
        case 4:
        case 5:
        case 7:
            m_HookedMaxDistance = 5.5f;
            break;
        }
    }
}

// 0x006C2FC0 (name is ours)
void ToolBank::UpdateFloatReel(const Actor* actor, const nn::math::MTX34* matrix)
{
    nn::math::VEC3 tip;
    GetRodTipPosition(&tip, matrix);
    const f32 restXZ = Func566440(&m_FloatPosition, &tip, 0.08f, 10.0f, 0.6f);
    const f32 restY = ssys::ma::ChaseScale(&m_FloatPosition.y, tip.y, 0.08f, 10.0f, 0.2f);
    if (restXZ == 0.0f && restY == 0.0f) {
        ChangeFloatState(FLOAT_STATE_HOLD, actor, matrix);
    }
}

// 0x006C3078 (name is ours)
void ToolBank::UpdateFloatFloat(const Actor* actor, const nn::math::MTX34* matrix)
{
    const f32 dx = m_FloatPosition.x - actor->m_Position.x;
    const f32 dz = m_FloatPosition.z - actor->m_Position.z;
    const f32 distanceSq = dx * dx + dz * dz;
    const f32 surface = FdInfo::GetSurfaceHeightEx(m_FloatPosition, FIELD_NAME_CURRENT);
    if (distanceSq > 20000.0f) {
        // too far from the actor: pulled back to it
        const math::Angle16 angle = Actor::CalcAngleY(&m_FloatPosition, &actor->m_Position);
        const f32 speed = s_FloatDriftSpeed;
        m_FloatPosition.x += speed * nn::math::SinIdx(angle.value);
        m_FloatPosition.z += speed * nn::math::CosIdx(angle.value);
        if (m_FloatPosition.y <= surface) {
            m_FloatPosition.y += 0.5f;
        }
        m_IsDrifting = true;
    } else {
        const u8 flow = GetAttribute95E83FAt(m_FloatPosition, FIELD_NAME_CURRENT);
        if (flow != FLOW_NONE) {
            const u16 angle = flow << 13;
            const f32 sin = nn::math::SinIdx(angle);
            const f32 cos = nn::math::CosIdx(angle);
            nn::math::VEC3 ahead(m_FloatPosition.x + FdInfo::UNIT_WORLD_SIZE_HALF_XF * sin, m_FloatPosition.y,
                                 m_FloatPosition.z + FdInfo::UNIT_WORLD_SIZE_HALF_XF * cos);
            if (FdInfo::IsPart95E93ENonZero(ahead, FIELD_NAME_CURRENT)) {
                const nn::math::VEC3 next(m_FloatPosition.x + s_FloatDriftSpeed * sin, m_FloatPosition.y,
                                          m_FloatPosition.z + s_FloatDriftSpeed * cos);
                if (m_FloatPosition.y - FdInfo::GetSurfaceHeightEx(next, FIELD_NAME_CURRENT) > FdInfo::UNIT_WORLD_SIZE_HALF_XF) {
                    m_FloatPosition = next;
                    ChangeFloatState(FLOAT_STATE_SINK, actor, matrix);
                }
                m_FloatPosition = next;
            } else if (m_FloatPosition.y <= surface) {
                m_FloatPosition.y += 0.5f;
            }
        } else if (m_FloatPosition.y <= surface) {
            m_FloatPosition.y += 0.5f;
        }
    }
    if (m_IsDrifting) {
        const u8 flow = GetAttribute95E83FAt(m_FloatPosition, FIELD_NAME_CURRENT);
        if (flow != FLOW_NONE) {
            const s16 angle = static_cast<s16>(flow << 13);
            if (m_DriftAttribute == FLOW_NONE) {
                m_DriftAngle.value = angle;
            }
            m_DriftAttribute = flow;
            ssys::ma::ChaseDivide(reinterpret_cast<s16*>(&m_DriftAngle.value), angle, 9, 0x1000, 0x100);
            if (m_DriftFrames == 0 || --m_DriftFrames == 0) {
                nn::math::VEC3 direction(0.0f, 0.0f, 1.0f);
                ssys::ma::RotateY(&direction, static_cast<s16>(m_DriftAngle.value));
                nn::math::VEC3 position = m_FloatPosition;
                position.y = FdInfo::GetSurfaceHeightEx(position, FIELD_NAME_CURRENT);
                if (m_DriftEffect.GetEmitterSet() != NULL) {
                    m_DriftEffect.SetTransform(&position, &ANGLE16_VEC3_ZERO, true);
                } else {
                    Func207998(&m_DriftEffect, EFFECT_FLOAT_DRIFT, &position, &ANGLE16_VEC3_ZERO, &VEC3_ONE, true, 0);
                }
                m_DriftEffect.Func2E6550(&direction);
            }
        }
    }
    ssys::ma::Chase(&m_FloatPosition.y, surface, 0.1f);
}

// 0x006C3404 (name is ours)
void ToolBank::UpdateFloatPullOut(const Actor* actor, const nn::math::MTX34* matrix)
{
    CountDown(&m_PullOutFrames);
    m_PullOutSpeedY += s_FloatGravity;
    const f32 dx = m_FloatPosition.x - m_PullOutStart.x;
    const f32 dz = m_FloatPosition.z - m_PullOutStart.z;
    const f32 distance = std::sqrt(dx * dx + dz * dz);
    if (m_PullOutFrames != 0) {
        if (distance > FdInfo::UNIT_WORLD_SIZE_XF * 1.5f) {
            const f32 scale = 1.0f / static_cast<f32>(m_PullOutFrames);
            const nn::math::VEC3 step((m_PullOutTarget.x - m_FloatPosition.x) * scale,
                                      (m_PullOutTarget.y - m_FloatPosition.y) * scale,
                                      (m_PullOutTarget.z - m_FloatPosition.z) * scale);
            m_FloatPosition.x += step.x;
            m_FloatPosition.z += step.z;
            m_FloatPosition.y += m_PullOutSpeedY;
        } else {
            Func566440(&m_FloatPosition, &m_PullOutTarget, 0.097f, 14.5f, 2.0f);
            m_FloatPosition.y += m_PullOutSpeedY;
        }
    } else {
        m_FloatPosition = m_PullOutTarget;
    }
    m_FloatTargetRotation.x.value = 0;
    ssys::ma::ChaseDivide(reinterpret_cast<s16*>(&m_FloatRotation.x.value),
                          static_cast<s16>(m_FloatTargetRotation.x.value), 20, 0x2000, 600);
    if (m_PullOutFrames == 0) {
        m_FloatFollowFrames = 0;
        ChangeFloatState(FLOAT_STATE_HOLD, actor, matrix);
    }
}

// 0x006C3620 (name is ours)
void ToolBank::EnterFloatSink(const Actor* actor, const nn::math::MTX34* matrix)
{
    m_SinkFrames = 60;
    m_SinkSpeed = 0.0f;
}

// 0x006C3644 (name is ours)
void ToolBank::EnterFloatBite(const Actor* actor, const nn::math::MTX34* matrix)
{
    m_FloatRotation.x.value = 0;
    m_FloatTargetRotation.x.value = 1024;
    nn::math::VEC3 position = m_FloatPosition;
    position.y = FdInfo::GetSurfaceHeightEx(position, FIELD_NAME_CURRENT);
    Func207998(&m_BiteEffect, EFFECT_FLOAT_BITE, &position, &ANGLE16_VEC3_ZERO, &VEC3_ONE, true, 0);
    m_BiteEffect.SetTransform(&position, &ANGLE16_VEC3_ZERO, true);
    m_BiteEffect.SetPtclCallback(Func6889B4, -1);
    const u32 slot = GetCurrentSlot();
    if (m_SoundStates[slot].m_HasPosition) {
        m_Sounds[slot].StartSound(SE_FLOAT_BITE);
    }
}

// 0x006C3734 (name is ours)
void ToolBank::EnterFloatHold(const Actor* actor, const nn::math::MTX34* matrix)
{
    if (m_FloatFollowFrames != 0) {
        nn::math::VEC3 tip;
        GetRodTipPosition(&tip, matrix);
        nn::math::VEC3 step(tip.x - m_FloatPosition.x, tip.y - m_FloatPosition.y, tip.z - m_FloatPosition.z);
        const f32 scale = 1.0f / static_cast<f32>(m_FloatFollowFrames);
        step.x *= scale;
        step.y *= scale;
        step.z *= scale;
        m_FloatPosition.x += step.x;
        m_FloatPosition.y += step.y;
        m_FloatPosition.z += step.z;
    } else {
        GetRodTipPosition(&m_FloatPosition, matrix);
    }
    CountDown(&m_FloatFollowFrames);
}

// 0x006C3838 (name is ours)
void ToolBank::ExitFloatReel(u32 nextState)
{
    // empty (in the original too)
}

// 0x006C383C (name is ours)
void ToolBank::ExitFloatFloat(u32 nextState)
{
    if (m_DriftEffect.GetEmitterSet() != NULL) {
        m_DriftEffect.Kill();
    }
}

// 0x006C3868 (name is ours)
void ToolBank::ExitFloatPullOut(u32 nextState)
{
    m_FloatHandle.m_Value = 0;
    if (m_PullOutEffect.GetEmitterSet() != NULL) {
        m_PullOutEffect.Kill();
    }
}

// 0x006C38B0 (name is ours)
void ToolBank::EnterFloatFall(const Actor* actor, const nn::math::MTX34* matrix)
{
    m_FloatSpeedY = Func5D50D8(m_FloatPosition.y, m_FloatTarget.y, s_FloatGravity, s_FloatFallFrames);
    m_FloatFallFrames = s_FloatFallFrames;
    if (m_LandEffect.GetEmitterSet() != NULL) {
        m_LandEffect.Kill();
    }
}

// 0x006C391C (name is ours)
void ToolBank::UpdateFloatCast(const Actor* actor, const nn::math::MTX34* matrix)
{
    ssys::ma::ChaseScale(&m_FloatPosition.y, m_FloatCastStart.y + 15.0f, 0.05f, 10.0f, 0.1f);
    m_FloatRotation.y.value -= 1700;
    m_FloatPosition.x += nn::math::SinIdx(m_FloatRotation.y.value) * 2.2f;
    m_FloatPosition.z += nn::math::CosIdx(m_FloatRotation.y.value) * 2.2f;
    if (m_FloatCastFrames != 0) {
        CountDown(&m_FloatCastFrames);
        return;
    }
    if (m_IsFloatInWater) {
        ChangeFloatState(FLOAT_STATE_FALL, actor, matrix);
    } else {
        ChangeFloatState(FLOAT_STATE_SWING, actor, matrix);
    }
}

// 0x006C3A20 (name is ours)
void ToolBank::EnterFloatReel(const Actor* actor, const nn::math::MTX34* matrix)
{
    // empty (in the original too)
}

// 0x006C3A24 (name is ours)
void ToolBank::EnterFloatFloat(const Actor* actor, const nn::math::MTX34* matrix)
{
    m_IsDrifting = false;
    m_DriftAttribute = FLOW_NONE;
    m_DriftAngle.value = 0;
    m_DriftFrames = 7;
}

// 0x006C3A50 (name is ours)
void ToolBank::EnterFloatPullOut(const Actor* actor, const nn::math::MTX34* matrix)
{
    const u32 frames = static_cast<u32>(GetAnimFrameSize(m_PullOutAnimId));
    nn::math::VEC3 offset;
    if (m_PullOutAnimId != HumanAnimName::POLE_PUTBACK_1) {
        offset = nn::math::VEC3(-14.4110374f, 27.9303284f, -16.5936108f);
    } else {
        offset = nn::math::VEC3(-9.57147884f, 27.2388f, 1.53687406f);
    }
    ssys::ma::RotateY(&offset, static_cast<s16>(actor->m_Rotation.y.value));
    m_PullOutTarget = nn::math::VEC3(actor->m_Position.x + offset.x, actor->m_Position.y + offset.y,
                                    actor->m_Position.z + offset.z);
    m_PullOutStart = m_FloatPosition;
    m_PullOutSpeedY = Func5D50D8(m_FloatPosition.y, m_PullOutTarget.y, s_FloatGravity, frames);
    m_PullOutFrames = frames;
    nn::math::VEC3 position = m_FloatPosition;
    position.y = FdInfo::GetSurfaceHeightEx(position, FIELD_NAME_CURRENT);
    Func207998(&m_PullOutEffect, EFFECT_FLOAT_PULL_OUT, &position, &ANGLE16_VEC3_ZERO, &VEC3_ONE, true, 0);
    m_PullOutEffect.SetTransform(&position, &ANGLE16_VEC3_ZERO, true);
    m_PullOutEffect.SetPtclCallback(Func6889B4, -1);
    const u32 slot = GetCurrentSlot();
    if (m_SoundStates[slot].m_HasPosition) {
        m_Sounds[slot].StartSound(SE_FLOAT_PULL_OUT);
    }
}

// 0x006C3C68 (name is ours)
void ToolBank::SetWindmillTargetSpeed(f32 speed)
{
    m_WindmillTargetSpeed = speed;
}

// 0x006C3C74 (name is ours)
void ToolBank::UpdateFloatSwing(const Actor* actor, const nn::math::MTX34* matrix)
{
    nn::math::VEC3 tip;
    GetRodTipPosition(&tip, matrix);
    const f32 restXZ = Func566440(&m_FloatPosition, &tip, 0.22f, 10.0f, 0.6f);
    const f32 restY = ssys::ma::ChaseScale(&m_FloatPosition.y, tip.y, 0.2f, 10.0f, 0.6f);
    if (restXZ == 0.0f && restY == 0.0f) {
        ChangeFloatState(FLOAT_STATE_HOLD, actor, matrix);
    }
}

// 0x006C3D2C (name is ours)
void ToolBank::ExitFloatCast(u32 nextState)
{
    // empty (in the original too)
}

// 0x006C3D30 (name is ours)
void ToolBank::ExitFloatSwing(u32 nextState)
{
    m_FloatPosition = m_FloatTarget;
}

// 0x006C3D4C (name is ours)
void ToolBank::EnterFloatCast(const Actor* actor, const nn::math::MTX34* matrix)
{
    nn::math::VEC3 offset(m_FloatPosition.x - actor->m_Position.x, m_FloatPosition.y - actor->m_Position.y,
                          m_FloatPosition.z - actor->m_Position.z);
    ssys::ma::RotateY(&offset, static_cast<s16>(-actor->m_Rotation.y.value));
    m_FloatCastOffset = offset;
    GetRodTipPosition(&m_FloatCastStart, matrix);
    m_FloatPosition = m_FloatCastStart;
    m_FloatCastFrames = 23;
    m_FloatRotation.y.value = Actor::CalcAngleY(&m_FloatCastStart, &m_FloatTarget).value - 0x4000;
}

// 0x006C3E14 (name is ours)
void ToolBank::EnterFloatSwing(const Actor* actor, const nn::math::MTX34* matrix)
{
    // empty (in the original too)
}

// 0x006C3E18 (name is ours)
void ToolBank::SetPlayerNo(u32 playerNo)
{
    m_PlayerNo = playerNo;
    m_IsMyPlayer = netgame::GetMyPlayerNo() == m_PlayerNo;
}

// 0x006C3E48 (name is ours)
ToolBank::ToolBank()
    : m_PlayerNo(4), m_IsMyPlayer(false), m_WindmillTargetSpeed(0.0f), m_WindmillSpeed(0.0f), m_IsBoneRotated(false),
      m_IsBoneTransformSaved(false), m_FloatState(FLOAT_STATE_HOLD), m_FloatPosition(VEC3_ZERO),
      m_FloatTarget(VEC3_ZERO), m_FloatCastOffset(VEC3_ZERO), m_FloatShadowScale(0.0f), m_ActorPosition(VEC3_ZERO),
      m_IsFloatShown(true), m_FloatCastFrames(0), m_IsFloatInWater(false), m_FloatFollowFrames(0), m_FloatSpeedY(0.0f), m_FloatFallFrames(0),
      m_PullOutFrames(0), m_PullOutAnimId(0), m_PullOutSpeedY(0.0f), m_HookedMaxDistance(0.1f), m_HookedDistance(0.0f),
      m_IsDrifting(false), m_DriftAttribute(0), m_DriftFrames(0), m_SinkFrames(0), m_SinkSpeed(0.0f), m_Unknown7A8(0),
      m_EffectBoneIndex(-1), m_WindmillVolume(0), m_Unknown7E4(0)
{
    m_BoneRotateX.value = 0;
    m_FloatRotation.x.value = 0;
    m_FloatRotation.y.value = 0;
    m_FloatRotation.z.value = 0;
    m_FloatTargetRotation.x.value = 0;
    m_FloatTargetRotation.y.value = 0;
    m_FloatTargetRotation.z.value = 0;
    m_ActorAngleY.value = 0;
    m_DriftAngle.value = 0;
    m_AnimIds[0] = HumanAnimName::COUNT;
    m_pBalloonStrings[0] = NULL;
    m_AnimIds[1] = HumanAnimName::COUNT;
    m_pBalloonStrings[1] = NULL;
    // (the float is reset)
    m_FloatState = FLOAT_STATE_HOLD;
    m_FloatPosition.x = 0.0f;
    m_FloatPosition.y = 0.0f;
    m_FloatPosition.z = 0.0f;
    m_FloatRotation.x.value = 0;
    m_FloatRotation.y.value = 0;
    m_FloatRotation.z.value = 0;
    m_FloatShadowScale = 0.0f;
}

// 0x006C43A8 slot 0x00
// 0x006C4398 (deleting dtor)
ToolBank::~ToolBank()
{
    // (only the members and the bases are destroyed)
}

// 0x0076F254 (name is ours)
bool ToolBank::GetBonePosition(nn::math::VEC3* out, s32 bone, bool isCurved) const
{
    if (bone < 0) {
        return false;
    }
    const g3d::SkeletalModel* model = GetShownModel();
    if (model == NULL || !model->GetBoneWorldPosition(out, bone)) {
        return false;
    }
    if (isCurved) {
        Func52EA98(out, out);
    }
    return true;
}

// 0x0076F2F4 (name is ours)
bool ToolBank::TransformByModel(nn::math::VEC3* out, const nn::math::VEC3* position, bool isCurved) const
{
    const g3d::SkeletalModel* model = GetShownModel();
    if (model == NULL) {
        return false;
    }
    sead::Vector3CalcCtr<f32>::mul(*out, model->GetLocalTransform().m_TransformMatrix, *position);
    if (isCurved) {
        Func52EA98(out, out);
    }
    return true;
}

// 0x0076F454 (name is ours)
bool ToolBank::IsBalloonReleased() const
{
    const BalloonString* string = m_pBalloonStrings[GetCurrentSlot()];
    return string != NULL ? string->IsReleased() : false;
}

// 0x0076F474 (name is ours)
bool ToolBank::IsBalloonGone() const
{
    const BalloonString* string = m_pBalloonStrings[GetCurrentSlot()];
    return string == NULL || (string->IsReleased() && string->m_ReleaseFrames == 0);
}

// 0x0076F4B8 (name is ours)
bool ToolBank::HasKindFlag() const
{
    const u8* entry = static_cast<const u8*>(Func536114(&m_Keywords[GetCurrentSlot()].m_Name));
    return entry != NULL && (entry[13] & 4) != 0;
}

// 0x0076F540 (name is ours)
bool ToolBank::IsBalloon() const
{
    SvFgName item(m_Keywords[GetCurrentSlot()].m_Name);
    return Func311840(&item) == TOOL_KIND_BALLOON;
}
