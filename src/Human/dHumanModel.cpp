#include "Human/dHumanModel.h"

#include "Bs/dBsStrcMgr.h"
#include "Human/dHumanFaceAnimControl.h"
#include "Human/human_Extern.h"
#include "Other/dSeadParticleHandle.h"
#include "Other/dWorldCurve.h"
#include "Sound/sound_Extern.h"
#include "camera/dCameraMgr.h"
#include "field/dFdInfo.h"
#include "g3d/dCalcRatio.h"
#include "g3d/dGroupTransformAnim.h"
#include "math/dAngle16.h"
#include "nn/math/math_Api.h"
#include "nw/gfx/gfx_SkeletalModel.h"
#include "nw/gfx/res/gfx_ResSkeleton.h"
#include "sead/seadMathCalcCommon.h"
#include "ssys/ma/ma_Math.h"
#include "stage/stage_Api.h"
#include "stage/stage_Extern.h"
#include <cmath>

extern const nn::math::VEC3 VEC3_ONE;
extern const math::Angle16Vec3 ANGLE16_VEC3_ZERO;
extern const nn::math::MTX34 MTX34_IDENTITY;

// the stage mode and the bone names of the humans
// 0x00953D28
u8 HumanModel::s_Mode;
// 0x00953D2C
const char* HumanModel::s_BoneNames[HumanModel::BONE_NAME_NUM] = {
    "root", "base", "head", "chest", "waist", "Lhand", "Rhand", "Larm1", "Rarm1", "Lfoot3", "Rfoot3", "feel", "mouth",
    "Lfoot1",
};
// 0x0086947C
extern const s16 HumanModel::s_HeadDivisors[2] = {12, 8};
// (read by game code at 0x0018E944 and 0x006059C4; meaning unknown)
// 0x00953D68
f32 g_Unknown953D68 = FdInfo::UNIT_WORLD_SIZE_XF * 4.0f;
// (only the static initializer writes it)
// 0x00A1914C
nn::math::VEC3 g_UnknownA1914C(0.0f, 0.0f, 0.0f);

namespace {
// the height of the platform of the train station (Func1ABA38)
const f32 STATION_PLATFORM_HEIGHT = 112.0f;

// the head turns at most this far up/down and to the sides, by at most HEAD_CHASE_STEP per frame (HeadCtrl)
const s16 HEAD_PITCH_MAX = 0x1800;
const s16 HEAD_YAW_MAX = 0x2000;
const s16 HEAD_CHASE_STEP = 0xC00;

// the local transform of a bone of a skeletal model
inline nw::gfx::CalculatedTransform* GetLocalTransform(g3d::SkeletalModel* model, s32 boneIdx)
{
    return &model->GetSkeletalModel()->GetSkeleton()->GetLocalTransforms()->begin()[boneIdx];
}

// the bit of slot idx in m_ReverseBits follows the sign of speed
inline void SetReverseBit(u8* bits, u32 idx, f32 speed)
{
    u8* byte = &bits[idx >> 3];
    const u8 bit = 1 << (idx & 7);
    *byte &= ~bit;
    if (speed < 0.0f) {
        *byte |= bit;
    }
}

// the face control plays the body animation again when the arm animation ends
inline void ResetFaceAnim(HumanModel* model, u32 idx)
{
    HumanFaceAnimControl* face = model->GetFaceCtrl();
    const u16 bodyAnim = model->m_AnimIds[HumanModel::ANIM_SLOT_BODY];
    if (face == NULL || idx != HumanModel::ANIM_SLOT_ARMS || bodyAnim >= HumanAnimName::COUNT) {
        return;
    }
    if (model->m_AnimIds[HumanModel::ANIM_SLOT_ARMS] >= HumanAnimName::COUNT &&
        face->m_SlotA.m_AnimId == HumanAnimName::COUNT && face->m_SlotB.m_AnimId == HumanAnimName::COUNT) {
        return;
    }
    face->PlayAnimA(bodyAnim, 1, false, 0.0f, 1.0f);
    face->PlayAnimB2(bodyAnim, 1, false, 0.0f, 1.0f);
}
} // namespace

// 0x001AB2E8
bool HumanModel::BindFaceCtrl()
{
    HumanFaceAnimControl* face = GetFaceCtrl();
    if (face == NULL) {
        return false;
    }
    face->m_Unknown18 = false;
    face->m_pHumanModel = this;
    return true;
}

// 0x001AB320
void HumanModel::SetAnimSpeed(u32 idx, f32 speed)
{
    if (idx >= ANIM_SLOT_NUM) {
        return;
    }
    m_Anim.m_Anims[idx].SetSpeed(speed);
    SetReverseBit(&m_ReverseBits, idx, speed);
}

// 0x001AB388
void HumanModel::SetAnimFrame(u32 idx, f32 frame)
{
    if (frame >= 0.0f && idx < ANIM_SLOT_NUM) {
        m_Anim.m_Anims[idx].SetFrame(frame);
    }
}

// 0x001AB3B8
void HumanModel::StopAnim(u32 idx, f32 duration)
{
    ResetFaceAnim(this, idx);
    m_AnimIds[idx] = HumanAnimName::COUNT;
    m_PrevAnimIds[idx] = HumanAnimName::COUNT;
    m_Anim.StartMorph(idx, duration);
}

// 0x001AB48C
void HumanModel::InitializeBones()
{
    m_BoneIndices[BONE_BASE] = m_Model.GetBoneIndex(s_BoneNames[BONE_NAME_BASE]);
    m_BoneIndices[BONE_ROOT] = m_Model.GetBoneIndex(s_BoneNames[BONE_NAME_ROOT]);
    m_BoneIndices[BONE_L_HAND] = m_Model.GetBoneIndex(s_BoneNames[BONE_NAME_L_HAND]);
    m_BoneIndices[BONE_R_HAND] = m_Model.GetBoneIndex(s_BoneNames[BONE_NAME_R_HAND]);
    m_BoneIndices[BONE_L_FOOT_3] = m_Model.GetBoneIndex(s_BoneNames[BONE_NAME_L_FOOT_3]);
    m_BoneIndices[BONE_R_FOOT_3] = m_Model.GetBoneIndex(s_BoneNames[BONE_NAME_R_FOOT_3]);
    m_BoneIndices[BONE_FEEL] = m_Model.GetBoneIndex(s_BoneNames[BONE_NAME_FEEL]);
    m_BoneIndices[BONE_HEAD] = m_Model.GetBoneIndex(s_BoneNames[BONE_NAME_HEAD]);
    m_BoneIndices[BONE_CHEST] = m_Model.GetBoneIndex(s_BoneNames[BONE_NAME_CHEST]);
    m_BoneIndices[BONE_WAIST] = m_Model.GetBoneIndex(s_BoneNames[BONE_NAME_WAIST]);
    m_BoneIndices[BONE_MOUTH] = m_Model.GetBoneIndex(s_BoneNames[BONE_NAME_MOUTH]);
    if (stage::IsKind(stage::KIND_VILLAGE, stage::GetCurrent()) || stage::GetCurrent() == stage::NAME_MUSEUM_FISH_0 ||
        stage::GetCurrent() == stage::NAME_MUSEUM_FISH_1 || stage::GetCurrent() == stage::NAME_MUSEUM_FISH_2) {
        s_Mode = 0;
    } else if (stage::IsKind(stage::KIND_TRAIN_STATION_ENVIRONMENT, stage::GetCurrent())) {
        s_Mode = 2;
    } else {
        s_Mode = 0;
    }
    m_IsVillage = stage::IsKind(stage::KIND_VILLAGE, stage::GetCurrent());
}

// 0x001AB5CC
void HumanModel::RootCtrl::Save(HumanModel*)
{
    if (m_pBaseTransform != NULL) {
        m_BaseTranslation.x = m_pBaseTransform->m_TransformMatrix.m[0][3];
        m_BaseTranslation.y = m_pBaseTransform->m_TransformMatrix.m[1][3];
        m_BaseTranslation.z = m_pBaseTransform->m_TransformMatrix.m[2][3];
    }
    if (m_pRootTransform != NULL) {
        m_RootTranslation.x = m_pRootTransform->m_TransformMatrix.m[0][3];
        m_RootTranslation.y = m_pRootTransform->m_TransformMatrix.m[1][3];
        m_RootTranslation.z = m_pRootTransform->m_TransformMatrix.m[2][3];
    }
}

// 0x001AB618
void HumanModel::RootCtrl::Reset()
{
    if (m_pRootTransform != NULL) {
        m_pRootTransform->SetTransform(nw::gfx::res::ResBone(m_pRootBone));
    }
}

namespace {
// a saved translation turned with the curve of the world (inline)
inline void ApplyCurveToTransform(nw::gfx::CalculatedTransform* transform, nn::math::VEC3* translation, s16 tilt,
                                  f32 distance, u16 rotateY)
{
    nn::math::MTX34 rotate;
    ssys::ma::SetRotateX(&rotate, tilt);
    const f32 s = nn::math::SinIdx(tilt);
    const f32 c = nn::math::CosIdx(tilt);
    nn::math::VEC3 offset;
    offset.x = 0.0f;
    offset.y = (c - 1.0f) * distance;
    offset.z = distance * s;
    sead::Vector3CalcCtr<f32>::add(*translation, *translation, offset);
    ssys::ma::RotateY(translation, -rotateY);
    nn::math::MTX34 local = transform->m_TransformMatrix;
    sead::Matrix34CalcCtr<f32>::multiply(rotate, local, rotate);
    rotate.m[0][3] = translation->x;
    rotate.m[1][3] = translation->y;
    rotate.m[2][3] = translation->z;
    transform->m_TransformMatrix = rotate;
    transform->m_Flags |= nw::gfx::CalculatedTransform::FLAG_IS_DIRTY;
    transform->m_Flags &= ~(nw::gfx::CalculatedTransform::FLAG_IS_ROTATE_ZERO |
                            nw::gfx::CalculatedTransform::FLAG_ROTATE_UNSET);
}
} // namespace

// 0x001AB634
void HumanModel::RootCtrl::ApplyWorldCurve(HumanModel* model, const nn::math::MTX34* mtx)
{
    if (m_pBaseTransform == NULL || m_pRootTransform == NULL) {
        return;
    }
    nn::math::VEC3 root = m_RootTranslation;
    ssys::ma::RotateY(&root, model->m_Rotation.y.value);
    const s16 rootTilt = Func52E760(&root);
    nn::math::VEC3 base = m_BaseTranslation;
    ssys::ma::RotateY(&base, model->m_Rotation.y.value);
    const s16 baseTilt = Func52E760(&base);
    if (rootTilt == 0 && baseTilt == 0) {
        return;
    }
    const f32 distance = std::sqrt(mtx->m[1][3] * mtx->m[1][3] + mtx->m[2][3] * mtx->m[2][3]);
    if (rootTilt != 0) {
        ApplyCurveToTransform(m_pRootTransform, &root, rootTilt, distance, model->m_Rotation.y.value);
    }
    if (baseTilt != 0) {
        base.z = 0.0f;
        ApplyCurveToTransform(m_pBaseTransform, &base, baseTilt, distance, model->m_Rotation.y.value);
    }
}

// 0x001AB8A4
void HumanModel::RootCtrl::AddTranslation(HumanModel* model, nn::math::VEC3* position, bool reset)
{
    if (position == NULL) {
        position = m_pPosition;
    }
    if (position == NULL) {
        return;
    }
    nn::math::VEC3 translation;
    sead::Vector3CalcCtr<f32>::add(translation, m_BaseTranslation, m_RootTranslation);
    ssys::ma::RotateY(&translation, model->m_Rotation.y.value);
    position->x += translation.x;
    position->z += translation.z;
    if (!reset) {
        return;
    }
    m_RootTranslation.z = 0.0f;
    m_RootTranslation.x = 0.0f;
    m_BaseTranslation.z = 0.0f;
    m_BaseTranslation.x = 0.0f;
    m_pBaseTransform->m_TransformMatrix.m[0][3] = 0.0f;
    m_pBaseTransform->m_TransformMatrix.m[1][3] = m_BaseTranslation.y;
    m_pBaseTransform->m_TransformMatrix.m[2][3] = 0.0f;
    m_pBaseTransform->m_Flags |= nw::gfx::CalculatedTransform::FLAG_IS_DIRTY;
    m_pRootTransform->m_TransformMatrix.m[1][3] = m_RootTranslation.y;
    m_pRootTransform->m_TransformMatrix.m[2][3] = m_RootTranslation.z;
    m_pRootTransform->m_TransformMatrix.m[0][3] = m_RootTranslation.x;
    m_pRootTransform->m_Flags |= nw::gfx::CalculatedTransform::FLAG_IS_DIRTY;
}

// 0x001AB97C
void HumanModel::RootCtrl::Bind(HumanModel* model)
{
    nw::gfx::Skeleton* skeleton = model->m_Model.GetSkeletalModel()->GetSkeleton();
    if (skeleton == NULL) {
        return;
    }
    const s32 base = model->m_BoneIndices[BONE_BASE];
    const s32 root = model->m_BoneIndices[BONE_ROOT];
    m_pBaseTransform = &skeleton->GetLocalTransforms()->begin()[base];
    m_pRootTransform = &skeleton->GetLocalTransforms()->begin()[root];
    const nw::gfx::res::ResSkeleton bones(skeleton->GetResSceneObject().ptr());
    m_pBaseBone = const_cast<nw::gfx::res::ResBoneData*>(bones.GetBone(base).ptr());
    m_pRootBone = const_cast<nw::gfx::res::ResBoneData*>(bones.GetBone(root).ptr());
}

// 0x001ABA38
f32 Func1ABA38(const nn::math::VEC3* position)
{
    if (!stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent())) {
        return 0.0f;
    }
    if (stage::IsKind(stage::KIND_VILLAGE, stage::GetCurrent()) && position->z < FdInfo::BLOCK_WORLD_SIZE_ZF) {
        // (the platform of the train station)
        if (position->y <= STATION_PLATFORM_HEIGHT) {
            return 1.0f;
        }
        return Func5D505C(position->y, FdInfo::UNIT_WORLD_SIZE_HALF_ZF + STATION_PLATFORM_HEIGHT,
                          STATION_PLATFORM_HEIGHT);
    }
    if (camera::Mgr::s_pInstance == NULL) {
        return 0.0f;
    }
    f32 height = STATION_PLATFORM_HEIGHT;
    const nn::math::VEC3* player = Func5C37B0(4, true);
    if (player != NULL) {
        height = player->y;
        if (height > STATION_PLATFORM_HEIGHT) {
            height = STATION_PLATFORM_HEIGHT;
        }
    }
    if (height > position->y) {
        return Func5D505C(height - position->y, s_HeightRange, 0.0f);
    }
    if (BsStrcMgr::s_InitState != BsStrcMgr::INIT_STATE_READY || BsStrcMgr::s_pInstance == NULL) {
        return 1.0f;
    }
    for (u32 i = 0; i < BsStrcMgr::BRIDGE_NUM; i++) {
        const nn::math::VEC3* bridge = BsStrcMgr::s_pInstance->GetBridgePosition(i);
        if (bridge == NULL) {
            continue;
        }
        const f32 dx = bridge->x - position->x;
        if (std::fabs(dx) > s_BridgeRangeFar) {
            continue;
        }
        const f32 dz = bridge->z - position->z;
        if (std::fabs(dz) > s_BridgeRangeFar) {
            continue;
        }
        return Func5D505C(std::sqrt(dx * dx + dz * dz), s_BridgeRangeNear, s_BridgeRangeFar);
    }
    return 1.0f;
}

// 0x001ABCC4
bool HumanModel::BuildAnim(u16 id, ssys::ma::Allocator* allocator, s32 maxMemberAnims)
{
    if (!m_Anim.Build(&m_Model, Func770970(&g_HumanAnimTable, id)->anim0, allocator, maxMemberAnims)) {
        return false;
    }
    m_Anim.m_Anims[ANIM_SLOT_BODY].SetPlayMode(g3d::FrameController::PLAY_MODE_LOOP);
    m_Model.SetSkeletalAnim(&m_Anim);
    return true;
}

// 0x001ABD20
void HumanModel::Func1ABD20()
{
    nn::math::VEC3 position;
    if (m_BoneIndices[BONE_FEEL] < 0 || !m_Model.GetBoneWorldPosition(&position, m_BoneIndices[BONE_FEEL])) {
        return;
    }
    Func52EA98(&position, &position);
    Func207B90(0x112, &position, &ANGLE16_VEC3_ZERO, &VEC3_ONE, true, 0);
    HumanFaceAnimControl* face = GetFaceCtrl();
    if (face != NULL && face->IsDefaultAnimA()) {
        face->PlayAnimA(HumanAnimName::MR_GAN_1, 0, false, 0.0f, 1.0f);
    }
}

// 0x001ABDDC
void HumanModel::SetAnimPlayMode(s32 playMode, u32 idx)
{
    if (idx < ANIM_SLOT_NUM) {
        m_Anim.m_Anims[idx].SetPlayMode(playMode);
    }
}

// 0x001ABDF8
bool HumanModel::BindFaceCtrlAnim()
{
    HumanFaceAnimControl* face = GetFaceCtrl();
    if (face == NULL) {
        return false;
    }
    // (the original tests the address of the member)
    if (&m_Anim == NULL) {
        return false;
    }
    face->m_Unknown18 = true;
    face->m_pGroupAnim = &m_Anim;
    return true;
}

// 0x001ABE3C
bool HumanModel::PlayArmAnim(u16 id, bool armsOnly, bool force, f32 duration)
{
    const u8 freeArms = Func6E6268(m_AnimIds[IsArmAnimPlaying() ? ANIM_SLOT_ARMS : ANIM_SLOT_BODY])->freeArms;
    u8 arms = Func6E6268(id)->armFlags & 15;
    if (!armsOnly) {
        arms &= freeArms;
    }
    if (!force && m_AnimIds[ANIM_SLOT_ARMS] == id && m_ArmFlags == arms) {
        return true;
    }
    StopArmAnim(duration);
    if (arms == 0) {
        return true;
    }
    m_PrevAnimIds[ANIM_SLOT_ARMS] = m_AnimIds[ANIM_SLOT_ARMS];
    m_AnimIds[ANIM_SLOT_ARMS] = id;
    m_Anim.ChangeAnim(ANIM_SLOT_ARMS, Func770970(&g_HumanAnimTable, id)->anim0, 0, duration);
    if (arms & ARM_FLAG_RIGHT) {
        m_Anim.AssignBones(ANIM_SLOT_ARMS, s_BoneNames[BONE_NAME_R_ARM_1]);
    }
    if (arms & ARM_FLAG_LEFT) {
        m_Anim.AssignBones(ANIM_SLOT_ARMS, s_BoneNames[BONE_NAME_L_ARM_1]);
    }
    m_ArmFlags = arms;
    return true;
}

// 0x001AC014
void Func1AC014(void* owner)
{
    reinterpret_cast<HumanModel*>(static_cast<u8*>(owner) + 0x1B4)->ClearRootTranslation();
}

// 0x001AC01C
void HumanModel::ClearRootTranslation()
{
    ::ClearRootTranslation(&m_Model);
}

// 0x001AC020
void ClearRootTranslation(g3d::SkeletalModel* model)
{
    nw::gfx::CalculatedTransform* transform = GetLocalTransform(model, 0);
    transform->m_TransformMatrix.m[0][3] = 0.0f;
    transform->m_TransformMatrix.m[1][3] = 0.0f;
    transform->m_TransformMatrix.m[2][3] = 0.0f;
    transform->m_Flags |= nw::gfx::CalculatedTransform::FLAG_IS_DIRTY;
}

// 0x001AC060
void HumanModel::StopArmAnim(f32 duration)
{
    if (m_ArmFlags != 0) {
        ResetFaceAnim(this, ANIM_SLOT_ARMS);
        m_AnimIds[ANIM_SLOT_ARMS] = HumanAnimName::COUNT;
        m_PrevAnimIds[ANIM_SLOT_ARMS] = HumanAnimName::COUNT;
        m_Anim.StartMorph(ANIM_SLOT_ARMS, duration);
    }
    m_ArmFlags = 0;
}

// 0x001AC134
void Func1AC134(void* owner)
{
    reinterpret_cast<HumanModel*>(static_cast<u8*>(owner) + 0x1B4)->ClearRootRotation();
}

// 0x001AC13C
void HumanModel::ClearRootRotation()
{
    ::ClearRootRotation(&m_Model);
}

// 0x001AC140
void ClearRootRotation(g3d::SkeletalModel* model)
{
    GetLocalTransform(model, 0)->SetRotateXYZ(0.0f, 0.0f, 0.0f);
}

// 0x001AC174
f32 Func1AC174(const nn::math::VEC3* position)
{
    const u8 mode = HumanModel::s_Mode;
    if (mode == 0 || mode == 1) {
        return 1.0f;
    }
    if (mode == 3) {
        return 1.0f;
    }
    const f32 z = position->z;
    if (s_StationRangeZ0 > z || z > s_StationRangeZ2) {
        return Func1E72C4();
    }
    if (z < s_StationRangeZ1) {
        const f32 factor = Func5D505C(z, s_StationRangeZ0, s_StationRangeZ1);
        const f32 value = Func1E72C4();
        return value + factor * (Func1E65C8() - Func1E72C4());
    }
    const f32 factor = Func5D505C(z, s_StationRangeZ1, s_StationRangeZ2);
    const f32 value = Func1E65C8();
    return value + factor * (Func1E72C4() - Func1E65C8());
}

// 0x001AC280
HumanFaceAnimControl* HumanModel::GetFaceCtrl()
{
    return NULL;
}

// 0x001AC288
void HumanModel::vf_0x04()
{
    // empty (in the original too)
}

// 0x001AC28C
bool HumanModel::FinalizeModel()
{
    for (u32 i = 0; i < ANIM_SLOT_NUM; i++) {
        m_PrevAnimIds[i] = HumanAnimName::COUNT;
        m_AnimIds[i] = HumanAnimName::COUNT;
    }
    if (m_Model.GetNode() != NULL) {
        m_Model.Finalize();
    }
    m_Anim.Finalize();
    return true;
}

// 0x001AC2DC
bool HumanModel::PlayAnimImpl(u16 id, u32 idx, u32 playMode, f32 frame, f32 speed, f32 duration, u32 flags,
                              bool* changed, u8 morphType)
{
    if (playMode == PLAY_MODE_OF_ANIM) {
        playMode = (Func6E6268(id)->flags >> 5) & 1;
    }
    bool dummy;
    if (changed == NULL) {
        changed = &dummy;
    }
    *changed = false;
    const nw::anim::res::ResAnim anim = Func770970(&g_HumanAnimTable, id)->anim0;
    if (!anim.IsValid()) {
        return false;
    }
    if (speed < 0.0f) {
        frame = anim.GetFrameSize() - frame - 1.0f;
    }
    if (m_AnimIds[idx] == id && (flags & PLAY_FLAG_RESTART) == 0) {
        if (flags & PLAY_FLAG_SPEED) {
            if (idx < ANIM_SLOT_NUM) {
                m_Anim.m_Anims[idx].SetSpeed(speed);
                SetReverseBit(&m_ReverseBits, idx, speed);
            }
            *changed = true;
        }
        if (flags & PLAY_FLAG_FRAME) {
            if (!(0.0f > frame) && idx < ANIM_SLOT_NUM) {
                m_Anim.m_Anims[idx].SetFrame(frame);
            }
            *changed = true;
        }
        if (flags & PLAY_FLAG_PLAY_MODE) {
            if (idx < ANIM_SLOT_NUM) {
                m_Anim.m_Anims[idx].SetPlayMode(playMode);
            }
            *changed = true;
        }
        return true;
    }
    if (id >= HumanAnimName::COUNT) {
        return false;
    }
    if (idx == ANIM_SLOT_BODY && m_AnimIds[ANIM_SLOT_BODY] == HumanAnimName::COUNT) {
        duration = 0.0f;
    }
    m_Anim.ChangeAnim(idx, anim, morphType, duration);
    if (!(0.0f > frame) && idx < ANIM_SLOT_NUM) {
        m_Anim.m_Anims[idx].SetFrame(frame);
    }
    if (idx < ANIM_SLOT_NUM) {
        m_Anim.m_Anims[idx].SetSpeed(speed);
        SetReverseBit(&m_ReverseBits, idx, speed);
    }
    if (idx < ANIM_SLOT_NUM) {
        m_Anim.m_Anims[idx].SetPlayMode(playMode);
    }
    m_PrevAnimIds[idx] = m_AnimIds[idx];
    m_AnimIds[idx] = id;
    HumanFaceAnimControl* face = GetFaceCtrl();
    if (face != NULL) {
        face->PlayAnimA(id, playMode, false, frame, speed);
        face->PlayAnimB(id, playMode, false, frame, speed);
    }
    *changed = true;
    return true;
}

// 0x001AC56C
bool HumanModel::PlayAnim(u16 id, u32 playMode, u32 flags, f32 frame, f32 speed, f32 duration, bool* changed,
                          u8 morphType)
{
    return PlayAnimImpl(id, ANIM_SLOT_BODY, playMode, frame, speed, duration, flags, changed, morphType);
}

// 0x001AC59C
bool HumanModel::FaceCtrl::Update()
{
    if (!m_Unknown18) {
        return static_cast<HumanFaceAnimControl*>(this)->UpdateTalk();
    }
    if (m_pGroupAnim == NULL) {
        return false;
    }
    const bool isFinished = m_pGroupAnim->m_Anims[FACE_ANIM_SLOT].IsFinished();
    if (m_Unknown16) {
        if (!m_pGroupAnim->m_pGroupAnim->IsEvaluatorEnabled(FACE_ANIM_SLOT) || isFinished) {
            const u16 id = m_Unknown14 == 1 ? HumanAnimName::NML_TALK_1 : HumanAnimName::NML_TALK_2;
            m_pGroupAnim->ChangeAnim(FACE_ANIM_SLOT, Func770970(&g_HumanAnimTable, id)->anim0, 0, 0.0f);
            m_pGroupAnim->AssignBones(FACE_ANIM_SLOT, "mouth");
            if (m_Unknown14 == 1) {
                m_Unknown14 = 0;
            } else {
                m_Unknown14 = 1;
            }
        }
        m_Unknown16 = false;
        return true;
    }
    if (isFinished && m_pGroupAnim->m_pGroupAnim->IsEvaluatorEnabled(FACE_ANIM_SLOT)) {
        m_pGroupAnim->StartMorph(FACE_ANIM_SLOT, 0.0f);
        m_Unknown14 = 1;
    }
    return false;
}

// 0x001AC6D0
bool HumanFaceAnimControl::UpdateTalk()
{
    const bool isFinished = IsSlotBFinished();
    if (m_Unknown16) {
        const bool hasAnimB = m_SlotB.m_AnimId != HumanAnimName::COUNT;
        const u16 idB = GetAnimIdB();
        if (!(hasAnimB && !isFinished && (m_AnimId10 == idB || m_AnimId12 == idB))) {
            PlayAnimB(m_Unknown14 == 1 ? m_AnimId10 : m_AnimId12, 2, false, 0.0f, 1.0f);
            if (m_Unknown14 == 1) {
                m_Unknown14 = 0;
            } else {
                m_Unknown14 = 1;
            }
        }
        m_Unknown16 = false;
        return true;
    }
    if (isFinished && m_pHumanModel != NULL) {
        HumanModel* model = m_pHumanModel;
        const u32 idx = model->IsArmAnimPlaying() ? HumanModel::ANIM_SLOT_ARMS : HumanModel::ANIM_SLOT_BODY;
        const u16 id = model->m_AnimIds[idx];
        const f32 frame = model->GetAnimFrame(idx);
        PlayAnimB(id, 2, false, frame, 1.0f);
    }
    return false;
}

// 0x001AC82C
void HumanModel::HeadCtrl::Restore()
{
    nw::gfx::CalculatedTransform* transform = GetLocalTransform(&m_pOwner->m_Model, m_pOwner->m_BoneIndices[BONE_HEAD]);
    transform->m_TransformMatrix = m_SavedMtx;
    transform->m_Flags |= nw::gfx::CalculatedTransform::FLAG_IS_DIRTY;
}

// 0x001AC984
void HumanModel::HeadCtrl::Update()
{
    const s32 head = m_pOwner->m_BoneIndices[BONE_HEAD];
    if (head >= 0 && m_pOwner->m_Model.GetBoneWorldPosition(&m_HeadPosition, head)) {
        Func52EA98(&m_HeadPosition, &m_HeadPosition);
    }
    const s16 turn = m_pOwner->m_Rotation.y.value - m_OwnerYaw;
    if (m_Timer != 0) {
        const f32 dx = m_Target.x - m_pOwner->m_Position.x;
        const f32 dz = m_Target.z - m_pOwner->m_Position.z;
        const f32 dy = m_Target.y + m_TargetOffsetY - m_HeadPosition.y;
        s32 pitch = math::Angle16::Atan2(dy, std::sqrt(dx * dx + dz * dz)).value;
        s16 yaw = math::Angle16::Atan2(dx, dz).value - m_pOwner->m_Rotation.y.value;
        if (m_IsHalfPitch) {
            pitch >>= 1;
        }
        s16 pitchTarget;
        if (pitch < -HEAD_PITCH_MAX) {
            pitchTarget = -HEAD_PITCH_MAX;
        } else if (pitch > HEAD_PITCH_MAX) {
            pitchTarget = HEAD_PITCH_MAX;
        } else {
            pitchTarget = pitch;
        }
        if (yaw < -HEAD_YAW_MAX) {
            yaw = -HEAD_YAW_MAX;
        } else if (yaw > HEAD_YAW_MAX) {
            yaw = HEAD_YAW_MAX;
        }
        // (the head keeps its direction in the world while the owner turns towards the target)
        if (turn * static_cast<s16>(yaw - m_Yaw) > 0 && m_Yaw * turn > 0) {
            m_Yaw = m_Yaw - turn;
        }
        ssys::ma::ChaseDivide(&m_Pitch, pitchTarget, m_Divisor, HEAD_CHASE_STEP);
        ssys::ma::ChaseDivide(&m_Yaw, yaw, m_Divisor, HEAD_CHASE_STEP);
        m_Timer--;
    } else {
        ssys::ma::ChaseDivide(&m_Pitch, 0, m_ReturnDivisor, HEAD_CHASE_STEP);
        ssys::ma::ChaseDivide(&m_Yaw, 0, m_ReturnDivisor, HEAD_CHASE_STEP);
    }
    if (m_Pitch == m_AppliedPitch && m_Yaw == m_AppliedYaw) {
        return;
    }
    nw::gfx::CalculatedTransform* transform = GetLocalTransform(&m_pOwner->m_Model, m_pOwner->m_BoneIndices[BONE_HEAD]);
    const f32 x = transform->m_TransformMatrix.m[0][3];
    const f32 y = transform->m_TransformMatrix.m[1][3];
    const f32 z = transform->m_TransformMatrix.m[2][3];
    m_SavedMtx = transform->m_TransformMatrix;
    nn::math::MTX34 world = transform->m_TransformMatrix;
    nn::math::MTX34 rotate;
    sead::Matrix34CalcCtr<f32>::makeIdentity(rotate);
    ssys::ma::SetRotateX(&rotate, m_Yaw);
    ssys::ma::RotateY(&rotate, m_Pitch);
    sead::Matrix34CalcCtr<f32>::multiply(world, rotate, world);
    transform->m_TransformMatrix = world;
    transform->m_TransformMatrix.m[0][3] = x;
    transform->m_TransformMatrix.m[1][3] = y;
    transform->m_Flags &= ~(nw::gfx::CalculatedTransform::FLAG_IS_ROTATE_ZERO |
                            nw::gfx::CalculatedTransform::FLAG_ROTATE_UNSET);
    transform->m_TransformMatrix.m[2][3] = z;
    transform->m_Flags |= nw::gfx::CalculatedTransform::FLAG_IS_DIRTY;
}

// 0x001ACC20
void HumanModel::HeadCtrl::SetTarget(const nn::math::VEC3* target, u16 frames, s16 divisor, s16 returnDivisor)
{
    m_Timer = frames;
    m_Target = *target;
    m_Divisor = divisor;
    m_ReturnDivisor = returnDivisor;
}

// 0x001ACC4C
void HumanModel::Func1ACC4C()
{
    HumanFaceAnimControl* face = GetFaceCtrl();
    if (face != NULL && face->IsDefaultAnimA()) {
        face->PlayAnimA(HumanAnimName::MR_GAN_1, 0, false, 0.0f, 1.0f);
    }
}

// 0x001ACCA0
void HumanModel::SetTransform(const nn::math::VEC3* position, const math::Angle16Vec3* rotation, f32 scale)
{
    m_HeadCtrl.m_OwnerYaw = m_Rotation.y.value;
    m_Position = *position;
    m_Rotation.x = rotation->x;
    m_Rotation.y = rotation->y;
    m_Rotation.z = rotation->z;
    nn::math::VEC3 curved;
    if (scale == 1.0f) {
        m_Tilt = GetWorldCurvePositionFromCamera(&curved, *position).value;
    } else if (scale == 0.0f) {
        m_Tilt = GetWorldCurvePosition(&curved, *position).value;
    } else {
        m_Tilt = Func52E794(&curved, *position, scale).value;
    }
    // 0x00A19140 (guard 0x00953D64)
    static const nn::math::VEC3 s_Up(0.0f, 1.0f, 0.0f);
    nn::math::MTX34 mtx;
    mtx.m[0][0] = 1.0f;
    mtx.m[0][1] = 0.0f;
    mtx.m[0][2] = 0.0f;
    mtx.m[0][3] = curved.x;
    mtx.m[1][0] = s_Up.x;
    mtx.m[1][1] = s_Up.y;
    mtx.m[1][2] = s_Up.z;
    mtx.m[1][3] = curved.y;
    mtx.m[2][0] = 0.0f;
    mtx.m[2][1] = 0.0f;
    mtx.m[2][2] = 1.0f;
    mtx.m[2][3] = curved.z;
    ssys::ma::RotateX(&mtx, m_Tilt);
    ssys::ma::RotateY(&mtx, rotation->y.value);
    SetMtx(&mtx);
}

// 0x001ACE0C
HumanModel::HumanModel(bool isNpc)
{
    m_HeadCtrl.m_SavedMtx = MTX34_IDENTITY;
    m_HeadCtrl.m_Divisor = s_HeadDivisors[0];
    m_HeadCtrl.m_ReturnDivisor = s_HeadDivisors[1];
    m_HeadCtrl.m_pOwner = this;
    m_HeadCtrl.m_Timer = 0;
    m_HeadCtrl.m_IsHalfPitch = false;
    m_HeadCtrl.m_HeadPosition.x = 0.0f;
    m_HeadCtrl.m_HeadPosition.y = 0.0f;
    m_HeadCtrl.m_HeadPosition.z = 0.0f;
    m_HeadCtrl.m_Target.x = 0.0f;
    m_HeadCtrl.m_Target.y = 0.0f;
    m_HeadCtrl.m_Target.z = 0.0f;
    m_HeadCtrl.m_TargetOffsetY = 0.0f;
    m_HeadCtrl.m_Pitch = 0;
    m_HeadCtrl.m_Yaw = 0;
    m_HeadCtrl.m_AppliedPitch = -1;
    m_HeadCtrl.m_AppliedYaw = -1;
    m_HeadCtrl.m_OwnerYaw = 0;
    m_Rotation.x.value = 0;
    m_Rotation.y.value = 0;
    m_Rotation.z.value = 0;
    m_Tilt = 0;
    m_Unknown98 = 0.0f;
    for (u32 i = 0; i < BONE_NUM; i++) {
        m_BoneIndices[i] = 0;
    }
    m_ReverseBits = 0;
    m_ArmFlags = 0;
    m_IsVillage = false;
    for (u32 i = 0; i < 8; i++) {
        m_BoneIndices[i] = -1;
    }
    m_IsNpc = isNpc;
    for (u32 i = 0; i < ANIM_SLOT_NUM; i++) {
        m_PrevAnimIds[i] = HumanAnimName::COUNT;
        m_AnimIds[i] = HumanAnimName::COUNT;
    }
}

// 0x001ACFCC
// 0x001ACF5C (deleting dtor)
HumanModel::~HumanModel()
{
    // (the derived destructors call FinalizeModel, the copy here has it inlined)
    FinalizeModel();
}

// 0x0070B694
bool HumanModel::GetBoneMatrix(nn::math::MTX34* out, s32 boneIdx, bool isCurved) const
{
    if (boneIdx < 0) {
        return false;
    }
    const nw::gfx::CalculatedTransform* world = m_Model.GetBoneWorldTransform(boneIdx);
    if (world == NULL) {
        return false;
    }
    if (!isCurved || !stage::HasWorldCurve(stage::GetCurrent())) {
        return m_Model.GetBoneWorldMatrix(out, boneIdx);
    }
    out->m[0][0] = 1.0f;
    out->m[0][1] = 0.0f;
    out->m[0][2] = 0.0f;
    out->m[1][0] = 0.0f;
    out->m[1][1] = 1.0f;
    out->m[1][2] = 0.0f;
    out->m[2][0] = 0.0f;
    out->m[2][1] = 0.0f;
    out->m[2][2] = 1.0f;
    out->m[0][3] = m_Position.x;
    out->m[1][3] = m_Position.y;
    out->m[2][3] = m_Position.z;
    ssys::ma::RotateYXZ(out, m_Rotation.x.value, m_Rotation.y.value, m_Rotation.z.value);
    nn::math::MTX34 transform = world->m_TransformMatrix;
    sead::Matrix34CalcCtr<f32>::multiply(*out, *out, transform);
    return true;
}

// 0x0070B7B8
bool HumanModel::GetBonePosition(nn::math::VEC3* out, s32 boneIdx, bool isCurved) const
{
    if (boneIdx < 0) {
        return false;
    }
    if (!m_Model.GetBoneWorldPosition(out, boneIdx)) {
        return false;
    }
    if (isCurved) {
        Func52EA98(out, out);
    }
    return true;
}

// 0x0070B860
f32 HumanModel::Func70B860() const
{
    f32 baseHeight;
    f32 footHeight = 0.0f;
    const nw::gfx::SkeletalModel* model = nw::gfx::DynamicCast<nw::gfx::SkeletalModel>(m_Model.GetNode());
    if (model != NULL) {
        const nw::gfx::res::ResBoneData* bone =
            model->GetSkeleton()->GetOriginalBones()->GetBone(m_BoneIndices[BONE_BASE]).ptr();
        if (bone != NULL) {
            baseHeight = bone->transform.translate.y;
        } else {
            baseHeight = footHeight;
        }
    } else {
        baseHeight = footHeight;
    }
    model = nw::gfx::DynamicCast<nw::gfx::SkeletalModel>(m_Model.GetNode());
    if (model != NULL) {
        const nw::gfx::res::ResSkeleton* bones = model->GetSkeleton()->GetOriginalBones();
        const nw::gfx::res::ResBoneData* bone =
            bones->GetBone(m_Model.GetBoneIndex(s_BoneNames[BONE_NAME_L_FOOT_1])).ptr();
        if (bone != NULL) {
            footHeight = bone->transform.translate.y;
        }
    }
    return (baseHeight - footHeight) * (2.0f / 27.0f);
}

// 0x0070B840
bool HumanModel::vf_0x28()
{
    return true;
}

// 0x0070B99C
bool HumanModel::IsAnimMorphing(u32 idx) const
{
    if (idx >= ANIM_SLOT_NUM) {
        return false;
    }
    const g3d::MorphTransformAnimEvaluator* evaluator = m_Anim.m_Anims[idx].GetMorphEvaluator();
    if (evaluator == NULL) {
        return false;
    }
    return !evaluator->m_CalcRatio.IsDone();
}

// the static initializer of the file sets g_Unknown953D68 and g_UnknownA1914C
// 0x00792390 (static initializer)
