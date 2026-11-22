#pragma once

#include "decomp.h"
#include "g3d/dGroupSkeletalAnim.h"
#include "g3d/dSkeletalModel.h"
#include "math/dAngle16.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector3.h"

namespace nw {
namespace gfx {
class CalculatedTransform;
namespace res {
struct ResBoneData;
} // namespace res
} // namespace gfx
} // namespace nw

namespace ssys {
namespace ma {
class Allocator;
} // namespace ma
} // namespace ssys

class HumanFaceAnimControl;

// The model of a human (players and villagers): a skeletal model with four animation slots in a GroupSkeletalAnim
// (slot 0 the whole body, slot 1 the arms, slot 3 the mouth of the face control), the bones the game needs, the head
// that turns towards a target and the face control of the derived classes (class name from the RTTI, the layout from
// the constructor 0x001ACE0C; all other names are ours).
// RTTI 10HumanModel @ 0x008CB0E8
// vtable 0x008EC19C (vptr 0x008EC1A4), offset_to_top 0, 11 entries
class HumanModel
{
public:
    class FaceCtrl;

    // the animation slots (GroupSkeletalAnim::ANIM_COUNT)
    static const u32 ANIM_SLOT_NUM = 4;
    static const u32 ANIM_SLOT_BODY = 0;
    static const u32 ANIM_SLOT_ARMS = 1;

    // the flags of PlayAnim: only speed, frame or play mode change when the animation already plays (FLAG_RESTART:
    // the animation starts again)
    enum PlayFlag
    {
        PLAY_FLAG_SPEED = 1 << 0,
        PLAY_FLAG_FRAME = 1 << 1,
        PLAY_FLAG_PLAY_MODE = 1 << 2,
        PLAY_FLAG_RESTART = 1 << 3,
    };
    // the play mode of PlayAnim that takes the loop flag of the animation (HumanAnimInfo::flags bit 5)
    static const u32 PLAY_MODE_OF_ANIM = 2;

    // the bones of m_BoneIndices (the names in s_BoneNames)
    enum Bone
    {
        BONE_BASE = 0,
        BONE_ROOT = 1,
        BONE_L_HAND = 2,
        BONE_R_HAND = 3,
        BONE_L_FOOT_3 = 4,
        BONE_R_FOOT_3 = 5,
        BONE_FEEL = 6,
        BONE_HEAD = 7,
        BONE_CHEST = 8,
        BONE_WAIST = 9,
        BONE_MOUTH = 10,
        BONE_NUM = 11,
    };
    // the bone names of s_BoneNames
    enum BoneName
    {
        BONE_NAME_ROOT = 0,
        BONE_NAME_BASE = 1,
        BONE_NAME_HEAD = 2,
        BONE_NAME_CHEST = 3,
        BONE_NAME_WAIST = 4,
        BONE_NAME_L_HAND = 5,
        BONE_NAME_R_HAND = 6,
        BONE_NAME_L_ARM_1 = 7,
        BONE_NAME_R_ARM_1 = 8,
        BONE_NAME_L_FOOT_3 = 9,
        BONE_NAME_R_FOOT_3 = 10,
        BONE_NAME_FEEL = 11,
        BONE_NAME_MOUTH = 12,
        BONE_NAME_L_FOOT_1 = 13,
        BONE_NAME_NUM = 14,
    };
    // the arms of the arm animation (m_ArmFlags; the bones below Larm1/Rarm1 take slot 1)
    enum ArmFlag
    {
        ARM_FLAG_LEFT = 1 << 0,
        ARM_FLAG_RIGHT = 1 << 1,
    };

    // the head turns towards a target for some frames and back afterwards (the rotation is put on the local matrix of
    // the head bone; the saved matrix restores it)
    struct HeadCtrl
    {
        // the head in this frame
        void Update(); // 0x001AC984
        // the local matrix of the head bone becomes the saved one again
        void Restore(); // 0x001AC82C
        // looking at target for frames frames (the divisors of the chase towards the target and back)
        void SetTarget(const nn::math::VEC3* target, u16 frames, s16 divisor, s16 returnDivisor); // 0x001ACC20

        nn::math::MTX34 m_SavedMtx;       // 0x00 the local matrix of the head bone before the rotation
        s16 m_Divisor;                    // 0x30
        s16 m_ReturnDivisor;              // 0x32
        HumanModel* m_pOwner;             // 0x34
        u16 m_Timer;                      // 0x38 the frames left to look at the target
        bool m_IsHalfPitch;               // 0x3A the head turns up and down only half as far
        nn::math::VEC3 m_HeadPosition;    // 0x3C (in the curved world)
        nn::math::VEC3 m_Target;          // 0x48
        f32 m_TargetOffsetY;              // 0x54
        s16 m_Pitch;                      // 0x58
        s16 m_Yaw;                        // 0x5A
        s16 m_AppliedPitch;               // 0x5C (-1 at first; not written here)
        s16 m_AppliedYaw;                 // 0x5E
        u16 m_OwnerYaw;                   // 0x60 the rotation of the owner in the last SetTransform
    };

    // the translations of the bones root and base of a model are saved and restored around the animation (a helper of
    // the derived classes)
    struct RootCtrl
    {
        // the local transforms and the resource bones of root and base of model
        void Bind(HumanModel* model); // 0x001AB97C
        // the translations of the world transforms are saved
        void Save(HumanModel* model); // 0x001AB5CC
        // the transform of root becomes the one of its resource bone
        void Reset(); // 0x001AB618
        // the saved translations are turned with the curve of the world at their places (mtx: the world matrix of the
        // model)
        void ApplyWorldCurve(HumanModel* model, const nn::math::MTX34* mtx); // 0x001AB634
        // the saved translation of root (rotated with the model) is added to position (NULL: m_pPosition); with reset
        // the translations become zero
        void AddTranslation(HumanModel* model, nn::math::VEC3* position, bool reset); // 0x001AB8A4

        nn::math::VEC3 m_RootTranslation;               // 0x00
        nn::math::VEC3 m_BaseTranslation;               // 0x0C
        nn::math::VEC3* m_pPosition;                    // 0x18
        nw::gfx::CalculatedTransform* m_pRootTransform; // 0x1C
        nw::gfx::CalculatedTransform* m_pBaseTransform; // 0x20
        nw::gfx::res::ResBoneData* m_pRootBone;         // 0x24
        nw::gfx::res::ResBoneData* m_pBaseBone;         // 0x28
    };

    HumanModel(bool isNpc); // 0x001ACE0C
    // plays animation id in slot 0 (see PlayAnimImpl)
    virtual bool PlayAnim(u16 id, u32 playMode, u32 flags, f32 frame, f32 speed, f32 duration, bool* changed,
                          u8 morphType); // 0x001AC56C slot 0x00
    virtual void vf_0x04(); // 0x001AC288 slot 0x04 (empty)
    virtual void vf_0x08() = 0; // slot 0x08
    virtual void vf_0x0C() = 0; // slot 0x0C
    virtual void vf_0x10() = 0; // slot 0x10
    virtual void vf_0x14() = 0; // slot 0x14
    // the face control of the derived classes (NULL: none)
    virtual HumanFaceAnimControl* GetFaceCtrl(); // 0x001AC280 slot 0x18
    virtual ~HumanModel(); // 0x001ACFCC slot 0x1C
    // 0x001ACF5C slot 0x20 (deleting dtor)
    // the world matrix of the model
    virtual void SetMtx(const nn::math::MTX34* mtx) = 0; // slot 0x24
    virtual bool vf_0x28(); // 0x0070B840 slot 0x28

    // animation id in slot idx: when it plays already only what flags names changes (unless PLAY_FLAG_RESTART);
    // playMode PLAY_MODE_OF_ANIM takes the loop flag of the animation; frame < 0: no frame; a negative speed plays it
    // backwards from frame; slot 0 starts without a morph when it had no animation; changed (optional): something
    // changed; false: no such animation
    bool PlayAnimImpl(u16 id, u32 idx, u32 playMode, f32 frame, f32 speed, f32 duration, u32 flags, bool* changed,
                      u8 morphType); // 0x001AC2DC
    // the animation of slot idx ends (morphing into the other slots over duration frames)
    void StopAnim(u32 idx, f32 duration); // 0x001AB3B8
    // the speed / the frame / the play mode of the animation of slot idx
    void SetAnimSpeed(u32 idx, f32 speed); // 0x001AB320
    void SetAnimFrame(u32 idx, f32 frame); // 0x001AB388
    void SetAnimPlayMode(s32 playMode, u32 idx); // 0x001ABDDC
    // animation id on the arms (slot 1; ARM_FLAG_* from the animation table, both when !armsOnly), without force not
    // when it plays already
    bool PlayArmAnim(u16 id, bool armsOnly, bool force, f32 duration); // 0x001ABE3C
    // the arm animation ends
    void StopArmAnim(f32 duration); // 0x001AC060
    // the four animation slots for the model with animation id (looped) in allocator
    bool BuildAnim(u16 id, ssys::ma::Allocator* allocator, s32 maxMemberAnims); // 0x001ABCC4
    // the animations end and the model is finalized (the destructors of the derived classes)
    bool FinalizeModel(); // 0x001AC28C
    // the indices of the bones and the mode of the stage
    void InitializeBones(); // 0x001AB48C
    // the face control gets this model / the animation group of the model
    bool BindFaceCtrl(); // 0x001AB2E8
    bool BindFaceCtrlAnim(); // 0x001ABDF8
    // (the effect 0x112 at the bone feel and the face animation MR_GAN_1; meaning unknown)
    void Func1ABD20(); // 0x001ABD20
    // (the face animation MR_GAN_1 when the face control allows it; meaning unknown)
    void Func1ACC4C(); // 0x001ACC4C
    // the position, the rotation and the tilt from scale (1: from the curve of the world seen from the camera, 0: from
    // the curve of the world; meaning of the other values unknown) set the world matrix (slot 0x24)
    void SetTransform(const nn::math::VEC3* position, const math::Angle16Vec3* rotation, f32 scale); // 0x001ACCA0
    // the translation / the rotation of the root bone becomes zero
    DECOMP_NOINLINE void ClearRootTranslation(); // 0x001AC01C
    DECOMP_NOINLINE void ClearRootRotation(); // 0x001AC13C

    // (inline; copies for the game)
    DECOMP_EMIT_COPY bool IsAnimFinished(u32 idx) // 0x0070B674
    {
        if (idx >= ANIM_SLOT_NUM) {
            return false;
        }
        return m_Anim.m_Anims[idx].IsFinished();
    }
    DECOMP_EMIT_COPY f32 GetAnimSpeed(u32 idx) // 0x0070B794
    {
        if (idx >= ANIM_SLOT_NUM) {
            return 0.0f;
        }
        return m_Anim.m_Anims[idx].GetSpeed();
    }
    DECOMP_EMIT_COPY f32 GetAnimFrame(u32 idx) // 0x0070B7FC
    {
        if (idx >= ANIM_SLOT_NUM) {
            return 0.0f;
        }
        return m_Anim.m_Anims[idx].GetFrame();
    }
    // the last frame step of slot idx passed frame (see g3d::BaseAnim::IsFramePassed)
    DECOMP_EMIT_COPY bool IsAnimFramePassed(u32 idx, f32 frame) // 0x0070B820
    {
        if (idx >= ANIM_SLOT_NUM) {
            return false;
        }
        return m_Anim.m_Anims[idx].IsFramePassed(frame);
    }
    // the play mode of slot idx (g3d::FrameController::PlayMode)
    DECOMP_EMIT_COPY u8 GetAnimPlayMode(u32 idx) // 0x0070B848
    {
        if (idx >= ANIM_SLOT_NUM) {
            return 0;
        }
        return m_Anim.m_Anims[idx].m_PlayMode;
    }
    // (the height of the bone base over the bone Lfoot1 in the resource times 2/27; meaning unknown)
    f32 Func70B860() const; // 0x0070B860
    // the world matrix of a bone (isCurved: in the curved world on the stages with a world curve; false: no bone)
    bool GetBoneMatrix(nn::math::MTX34* out, s32 boneIdx, bool isCurved) const; // 0x0070B694
    // the world position of a bone (isCurved: in the curved world)
    bool GetBonePosition(nn::math::VEC3* out, s32 boneIdx, bool isCurved) const; // 0x0070B7B8
    // the arm animation plays
    DECOMP_EMIT_COPY bool IsArmAnimPlaying() const // 0x0070B970
    {
        return m_ArmFlags == 0 && m_AnimIds[ANIM_SLOT_ARMS] < ANIM_COUNT;
    }
    // the animation of slot idx still morphs
    bool IsAnimMorphing(u32 idx) const; // 0x0070B99C

    // the mode of the stage (2 on the stages of stage::KIND_TRAIN_STATION_ENVIRONMENT, else 0) and the bone names
    static u8 s_Mode;
    static const char* s_BoneNames[BONE_NAME_NUM];
    // the divisors of the head (HeadCtrl::m_Divisor/m_ReturnDivisor; PlayerModel reads them too)
    static const s16 s_HeadDivisors[2];

    HeadCtrl m_HeadCtrl;                   // 0x04
    u16 m_PrevAnimIds[ANIM_SLOT_NUM];      // 0x68 HumanAnimName
    u16 m_AnimIds[ANIM_SLOT_NUM];          // 0x70 HumanAnimName (COUNT: none)
    g3d::SkeletalModel m_Model;            // 0x78
    nn::math::VEC3 m_Position;             // 0x84
    math::Angle16Vec3 m_Rotation;          // 0x90
    s16 m_Tilt;                            // 0x96 (from SetTransform)
    f32 m_Unknown98;                       // 0x98
    s32 m_BoneIndices[BONE_NUM];           // 0x9C (-1: none)
    u8 m_ReverseBits;                      // 0xC8 bit idx: slot idx plays backwards
    g3d::GroupSkeletalAnim m_Anim;         // 0xCC
    u8 m_ArmFlags;                         // 0x140 ARM_FLAG_* of the arm animation
    bool m_IsVillage;                      // 0x141 the stage has stage::KIND_VILLAGE
    bool m_IsNpc;                          // 0x142 (NpcModelBase: true, PlayerModel: false)

private:
    // the animations of HumanAnimName (COUNT = none)
    static const u16 ANIM_COUNT = 630;
};
ASSERT_OFFSET(HumanModel, m_HeadCtrl, 0x04);
ASSERT_OFFSET(HumanModel, m_PrevAnimIds, 0x68);
ASSERT_OFFSET(HumanModel, m_Model, 0x78);
ASSERT_OFFSET(HumanModel, m_BoneIndices, 0x9C);
ASSERT_OFFSET(HumanModel, m_Anim, 0xCC);
ASSERT_OFFSET(HumanModel, m_IsNpc, 0x142);
ASSERT_SIZE(HumanModel, 0x144);

// the translation / the rotation of the local transform of the first bone of a skeletal model becomes zero (in the
// file of HumanModel; names are ours)
DECOMP_NOINLINE void ClearRootTranslation(g3d::SkeletalModel* model); // 0x001AC020
DECOMP_NOINLINE void ClearRootRotation(g3d::SkeletalModel* model); // 0x001AC140
// the same for the HumanModel at +0x1B4 of a game object of the player (game code that falls into the functions above)
void Func1AC014(void* owner); // 0x001AC014
void Func1AC134(void* owner); // 0x001AC134
// (a factor of a position by the stage: 1 outside the mode 2 of HumanModel, else from the depth; meaning unknown)
f32 Func1AC174(const nn::math::VEC3* position); // 0x001AC174
// (a factor of a position by the train station platform, the height under the player and the bridges; meaning unknown)
f32 Func1ABA38(const nn::math::VEC3* position); // 0x001ABA38
