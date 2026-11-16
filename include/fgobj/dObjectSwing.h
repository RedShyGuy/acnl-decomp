#pragma once

#include "decomp.h"
#include "Other/dSeadParticleHandle.h"
#include "fgobj/dObjectExe.h"
#include "g3d/dSkeletalAnim.h"
#include "nn/math/math_Vector3.h"

class SvFgName;

namespace fgobj {
// A tree or bush that is shaken or cut down: a skeletal animation of FgObjAnim.bcres with the effects of leaves and the
// sounds; a cut tree falls to the left or right and fades out (the class name is from the RTTI, the rest is ours).
// RTTI N5fgobj11ObjectSwingE @ 0x008D29A0
// vtable 0x00908BF4 (vptr 0x00908BFC), offset_to_top 0, 10 entries
class ObjectSwing : public ::fgobj::ObjectExe
{
public:
    // m_Kind
    enum Kind : u8
    {
        KIND_SHAKE_S = 0,
        KIND_SHAKE_L = 1,
        KIND_SHAKE_L_WAIT = 2,
        KIND_SHAKE_M = 3,
        KIND_CUT_LEFT = 4,      // falls to x - 1 (the animations cut*_L)
        KIND_CUT_RIGHT = 5,     // falls to x + 1 (cut*_R)
    };
    // m_Animation: the suffixes of the skeletal animations (s_AnimationSuffixes)
    enum Animation : u8
    {
        ANIMATION_SHAKE_S = 0,
        ANIMATION_SHAKE_M = 1,
        ANIMATION_SHAKE_L = 2,
        ANIMATION_SHAKE_L_WAIT = 3,
        ANIMATION_CUT0_L = 4,   // falls freely
        ANIMATION_CUT1_L = 5,   // something is in the way (IsFallBlocked)
        ANIMATION_CUT0_R = 6,
        ANIMATION_CUT1_R = 7,
        ANIMATION_NUM = 8,
    };
    // m_State: the function of Execute (s_ExecuteFunctions)
    enum State : u8
    {
        STATE_SHAKE = 0,
        STATE_CUT = 1,
    };

    typedef s32 (ObjectSwing::*ExecuteFunction)();

    // the object of item at the unit x, z (isFallAll: the leaves of a bush fall all at once, SE_OBJ_TREE_SWING_FALL_ALL)
    static ObjectSwing* Create(const SvFgName& item, s32 x, s32 z, u8 kind, bool isFallAll); // 0x00594654

    virtual ~ObjectSwing(); // 0x005948B0 slot 0x00
    // 0x0059485C (deleting dtor)
    bool vf_0x10() override; // 0x005936E4 slot 0x10
    // kills the effects, then ObjectBase::Finalize
    void Finalize() override; // 0x005947D8 slot 0x14
    s32 Execute() override; // 0x0059482C slot 0x20

    void Setup(const SvFgName& item, s32 x, s32 z, u8 kind); // 0x00593F58
    // the position of the effects: the bone "j_main" (cedars: the bone of their size), raised by the size
    bool GetEffectPosition(nn::math::VEC3* out) const; // 0x005936FC
    // whether the unit next to x, z in x (by dx) blocks the fall: higher by more than 32, of the attributes of
    // FdInfo::IsAttribute02_03_04_A2_A5_F8 or with a grown tree (only trees of size 3 and more, no bamboo)
    bool IsFallBlocked(s32 x, s32 z, s32 dx) const; // 0x00593A5C
    // the effect of the shaking or cutting with its scale, interval and rate (kind: Kind)
    void StartEffect(u8 kind); // 0x00593C64
    // the leaf effect 211 (when Func63061C is true)
    void StartLeafEffect(u8 kind); // 0x00593920
    // the emit interval of the effects by kind and size
    f32 GetEmitInterval(u8 kind) const; // 0x00594488
    void PlayShakeSound(const SvFgName* item, u8 kind); // 0x0059429C
    // the sounds of the fall: the swing after 8 frames, the landing 10 frames before the end of the fade
    void PlayFallSound(); // 0x005934A8
    // the shadow fades out from frame 10 to frame 22 of the animation
    void UpdateShadowOpacity(); // 0x00594210
    s32 ExecuteShake(); // 0x005941A8
    s32 ExecuteCut(); // 0x00593B50

    // the functions of Execute by m_State
    static const ExecuteFunction s_ExecuteFunctions[2];
    // the suffixes of the skeletal animations by Animation
    static const char* const s_AnimationSuffixes[ANIMATION_NUM];

    g3d::SkeletalAnim m_SkeletalAnim;   // 0x124
    SeadParticleHandle m_Effect;        // 0x13C
    SeadParticleHandle m_LeafEffect;    // 0x154
    u8 m_State;                         // 0x16C State
    u8 m_Kind;                          // 0x16D Kind
    u8 m_Animation;                     // 0x16E Animation
    u8 m_SoundFrame;                    // 0x16F
};
ASSERT_OFFSET(ObjectSwing, m_Effect, 0x13C);
ASSERT_OFFSET(ObjectSwing, m_State, 0x16C);
ASSERT_SIZE(ObjectSwing, 0x170);

// the model of the item of object in FgObjAnim.bcres (ObjectSwing, ObjectStumpAnim; some bushes have special models when
// Func63061C is true)
const void* GetAnimModel(const ObjectBase* object); // 0x005A3640
} // namespace fgobj
