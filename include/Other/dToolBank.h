#pragma once

#include "decomp.h"
#include "Audio/dAudioObj1.h"
#include "Exchange/dExchangeModelBank.h"
#include "Item/dResLoader.h"
#include "Other/dSeadParticleHandle.h"
#include "Sv/dSvFgName.h"
#include "g3d/dMaterialAnim.h"
#include "g3d/dModel.h"
#include "g3d/dMorphSkeletalAnim.h"
#include "g3d/dSkeletalModel.h"
#include "g3d/dVisibilityAnim.h"
#include "math/dAngle16.h"
#include "nw/gfx/gfx_CalculatedTransform.h"

class Actor;
class AcPlayer;
class BalloonString;
class HumanModel;

// The bank of the tools in the hands of the humans (class name from the RTTI, layout size after libgarden; the members
// and the non-virtual functions are ours). Per slot it holds the model of the tool item (file and textures like the
// clothes banks) with a morph skeletal animation and material/visibility animations that follow the animation of the
// human (HumanAnimInfo: an animation of the tool per human animation), and the extras of some tools: the float of the
// fishing rod (a model and a state machine with ten states), three more models of the slingshot, the string of the
// balloon, the turning of the pinwheel, a sound per slot and particle effects.
// RTTI 8ToolBank @ 0x008CD5A8
// vtable 0x008F9F30 (vptr 0x008F9F38), offset_to_top 0, 11 entries
// vtable 0x008F9F64 (vptr 0x008F9F6C), offset_to_top -1200, 3 entries
class ToolBank : public ::ExchangeModelBank<24448u, BankVramConfig<736u, 4u, 3824u, 4u, false>, ExchangeBankKeywordMyDesignFgNameEx<SvFgName::NAME_EMPTY>, item::ResLoader, 2u, false>
{
public:
    typedef ExchangeBankKeywordMyDesignFgNameEx<SvFgName::NAME_EMPTY> Keyword;
    typedef ::ExchangeModelBank<24448u, BankVramConfig<736u, 4u, 3824u, 4u, false>,
                                ExchangeBankKeywordMyDesignFgNameEx<SvFgName::NAME_EMPTY>, item::ResLoader, 2u, false>
        ModelBank;

    // the kinds of the tools (Func311840 of the item: byte 0x0F of the item parameters; the items of a kind after romfs
    // Item/Param/Item.bin, the animations of HumanAnimName in brackets)
    enum ToolKind
    {
        TOOL_KIND_AXE = 2,             // the axes (a visibility animation: the damage of the axe, GetVisibilityFrame)
        TOOL_KIND_FISHING_ROD = 3,     // the fishing rods (TF_POLE_*; model 1 of the file is the float)
        TOOL_KIND_NET = 4,             // the nets (TF_NET_*)
        TOOL_KIND_SHOVEL = 5,          // the shovels (TF_NPC_SCOOP_*)
        TOOL_KIND_WATERING_CAN = 6,    // the watering cans (nothing of their own here)
        TOOL_KIND_DANDELION_PUFFS = 7, // (TF_DANDELION_*)
        TOOL_KIND_UMBRELLA = 8,        // the umbrellas and parasols (TF_UMB_*)
        TOOL_KIND_TOY_HAMMER = 9,      // (TS_HUMMER_*)
        TOOL_KIND_SLINGSHOT = 10,      // the slingshots (TF_SLING_*; three more models: the shots)
        TOOL_KIND_SPARKLER = 11,       // sparkler, roman candle (TF_FIRE_*)
        TOOL_KIND_PARTY_POPPER = 12,
        TOOL_KIND_BUBBLE_WAND = 13,
        TOOL_KIND_BALLOON = 14,        // the balloons (a BalloonString)
        TOOL_KIND_MEGAPHONE = 15,      // (TF_MEGAPHONE_*)
        TOOL_KIND_DRINK = 16,          // coffee, fizzy apple juice (TS_TAKEOUTCOFFEE_*)
        TOOL_KIND_PINWHEEL = 17,       // the pinwheels (TF_WINDMILL_*; bone 1 turns, a material frame per color)
        TOOL_KIND_BEANS = 18,
        TOOL_KIND_ICE_CREAM = 19,      // good-luck roll, soft serves, ice creams
        TOOL_KIND_TWEETER = 20,        // (TF_BLOWOUTS_*)
        TOOL_KIND_BAG = 21,            // Santa bag, mystery bag (TS_SANTA_BAG_*)
        TOOL_KIND_WAND = 22,           // the glow wands, flower fairy wand, Kiki and Lala wand
        TOOL_KIND_FRYING_PAN = 23,     // (TF_SKILLET_*)
    };

    // the states of the float of the fishing rod (m_FloatState; the functions of a state are in s_FloatStates)
    enum FloatState
    {
        FLOAT_STATE_HOLD = 0,     // at the tip of the rod (it follows it in m_FloatFollowFrames frames)
        FLOAT_STATE_CAST = 1,     // thrown in front of the actor (to 2 or 3 after m_FloatCastFrames)
        FLOAT_STATE_SWING = 2,    // swings back to the tip of the rod (to 0 when it is there)
        FLOAT_STATE_FALL = 3,     // falls to the target position (to 4 on the surface)
        FLOAT_STATE_FLOAT = 4,    // floats on the water, drifts with the flow of the field
        FLOAT_STATE_BITE = 5,     // a fish bites (the fish shadow AcFsFdShadow sets it): pulled under and wobbling, back
                                  // to 4 when m_FloatHandle is cleared
        FLOAT_STATE_HOOKED = 6,   // the player hooked the fish (with POLE_HIT_1): the fish pulls the float to
                                  // m_HookedTarget (set by the fish shadow), as far as its size allows
        FLOAT_STATE_REEL = 7,     // pulled back to the tip of the rod (to 0 when it is there)
        FLOAT_STATE_PULL_OUT = 8, // pulled out of the water during POLE_PUTBACK_1 / POLE_GET_1 (to 0 at the end)
        FLOAT_STATE_SINK = 9,     // falls (to 4 on the surface; for 60 frames)
        FLOAT_STATE_NUM = 10,
    };

    // the functions of a state of the float (actor: its position and rotation; matrix: the tip of the rod; the exit gets
    // the next state)
    typedef void (ToolBank::*FloatStateFunc)(const Actor* actor, const nn::math::MTX34* matrix);
    typedef void (ToolBank::*FloatStateExitFunc)(u32 nextState);
    struct FloatStateInfo
    {
        FloatStateFunc enter;     // 0x00
        FloatStateFunc update;    // 0x08
        FloatStateExitFunc exit;  // 0x10
        bool unknown18;           // 0x18 (not read)
        bool isClearHandle;       // 0x19 m_FloatHandle is cleared when the state starts
    };

    // a handle of a game object (cleared by the destructor)
    struct ObjectHandle
    {
        ObjectHandle() : m_Value(0) {}
        ~ObjectHandle() { m_Value = 0; }

        u32 m_Value; // 0x0 (0: none)
    };

    // three more models of a slot (the slingshot; a struct of its own: ARMCC builds the array with its constructor)
    struct SubModels
    {
        DECOMP_EMIT_COPY SubModels() {} // 0x006C2190
        DECOMP_EMIT_COPY ~SubModels() {} // 0x006C21A4

        g3d::SkeletalModel m_Models[3]; // 0x0
    };

    // the state of the sound of a slot (a struct of its own: ARMCC builds it with a constructor copy)
    struct SoundState
    {
        DECOMP_EMIT_COPY SoundState() { m_Raw = 0; } // 0x00151B64

        union {
            u8 m_Raw;
            struct
            {
                u8 m_IsStarted : 1;   // the sound object started (Initialize)
                u8 m_HasPosition : 1; // its position was set once (UpdateFrames)
            };
        };
    };

    ToolBank(); // 0x006C3E48
    virtual ~ToolBank(); // 0x006C43A8 slot 0x00

    virtual void OnRequest(const Keyword& keyword, u32 slot) override; // 0x006C1CB4 slot 0x0C
    virtual bool LoadResource(const Keyword& keyword, u32 slot) override; // 0x006BF4A0 slot 0x10
    virtual s32 LoadResourceSplit(const Keyword& keyword, u32 slot) override; // 0x006C1A08 slot 0x14
    virtual bool Initialize(const Keyword& keyword, u32 slot, bool isReady) override; // 0x006BFD24 slot 0x18
    virtual bool Finalize(const Keyword& keyword, u32 slot) override; // 0x006C0D04 slot 0x1C

    // the speed of the animations of the shown slot (morph and material animation)
    void SetAnimSpeed(f32 speed); // 0x006BEF04
    // an animation of the tool (HumanAnimName; playMode 2: the loop flag of the id; isKeep: only the speed (and the frame
    // at speed 0) if it already plays; true: it plays)
    bool PlayAnim(u16 id, s32 playMode, bool isKeep, f32 frame, f32 speed, f32 morphFrames); // 0x006BEF90
    // bone 1 of the pinwheel turns by m_BoneRotateX (the model evaluates its animations first)
    void ApplyBoneRotate(); // 0x006BF184
    void SetAnimFrame(f32 frame); // 0x006BF34C
    // the float dips into the water when a fish nibbles (the fish shadow calls it; a sound and the effect 235)
    void PlayFloatSplash(); // 0x006BF3D8
    // where the hooked fish pulls the float (the fish shadow sets it)
    void SetHookedTarget(const nn::math::VEC3* position); // 0x006BF628
    // adds the model of the shown slot and the extras to a scene
    void AddToScene(s32 sceneIdx); // 0x006BF6AC
    void AddExtrasToScene(s32 sceneIdx); // 0x006BF6D0
    // the pinwheel in the wind of a moving actor (from: the last, to: the current position, angle: its direction)
    void UpdateWindmill(const nn::math::VEC3* from, const nn::math::VEC3* to, const math::Angle16* angle); // 0x006C08F4
    // the sound position and the frames of the animations of the shown model
    void UpdateFrames(); // 0x006C0A98
    // the player hooked the fish: the float at position (FLOAT_STATE_HOOKED)
    void HookFloat(const Actor* actor, const nn::math::MTX34* matrix, const nn::math::VEC3* position); // 0x006C0BD4
    void ChangeFloatState(u32 state, const Actor* actor, const nn::math::MTX34* matrix); // 0x006C0C08
    // the float in the frame (actor and rod tip are remembered)
    void UpdateFloat(const Actor* actor, const nn::math::MTX34* matrix); // 0x006C1118
    // the animation of the tool for the animation humanAnimId of the human (unknown: m_Unknown7E4; true: an animation
    // started)
    bool PlayToolAnim(u16 humanAnimId, s32 playMode, bool isKeep, f32 frame, f32 speed, f32 morphFrames,
                      u8 unknown); // 0x006C1384
    void SetAnimPlayMode(s32 playMode); // 0x006C1988
    // the three models of the slingshot at the positions
    void SetSubModelPositions(const nn::math::VEC3* position0, const nn::math::VEC3* position1,
                              const nn::math::VEC3* position2); // 0x006C1BEC
    // a fish bites (the fish shadow calls it; FLOAT_STATE_BITE)
    void BiteFloat(const Actor* actor, const nn::math::MTX34* matrix); // 0x006C21C8
    void HoldFloat(const Actor* actor, const nn::math::MTX34* matrix, u16 followFrames); // 0x006C21D8
    // the balloon string of the shown slot is released in direction (false: no balloon)
    bool ReleaseBalloon(const math::Angle16* direction); // 0x006C2520
    // the float falls to landing (from position; the water flag is set)
    void ThrowFloatToWater(const Actor* actor, const nn::math::MTX34* matrix, const nn::math::VEC3* position,
                           const nn::math::VEC3* landing); // 0x006C25DC
    // adds the shown models of the slingshot to the scene of the tool
    void AddSubModelsToScene(bool isShown0, bool isShown1, bool isShown2); // 0x006C267C
    void ThrowFloat(const Actor* actor, const nn::math::MTX34* matrix, const nn::math::VEC3* position,
                    const nn::math::VEC3* landing); // 0x006C29F8
    // the model of the shown slot at the bone of parent (the balloon at its string)
    void SetBoneMatrix(g3d::SkeletalModel* parent, u32 bone); // 0x006C2C80
    void ReelFloat(const Actor* actor, const nn::math::MTX34* matrix, const nn::math::VEC3* position); // 0x006C2D84
    void FloatFloat(const Actor* actor, const nn::math::MTX34* matrix, const nn::math::VEC3* position); // 0x006C2DB8
    // the float is pulled out of the water with the human animation (POLE_PUTBACK_1 / POLE_GET_1; isReleaseHandle: for
    // POLE_PUTBACK_1, the handle is passed to Func27E23C)
    void PullOutFloat(const Actor* actor, const nn::math::MTX34* matrix, u16 humanAnimId, bool isReleaseHandle); // 0x006C2DEC
    void SetWindmillTargetSpeed(f32 speed); // 0x006C3C68
    // the player number of the owner (and whether it is the player of this machine)
    void SetPlayerNo(u32 playerNo); // 0x006C3E18

    // the kind of the field item at position (0: none, 1: kinds 0/2 of Func24066C, 2: other kinds)
    static s32 GetFieldItemKind(const nn::math::VEC3* position); // 0x006BF644
    // the position in front of the hand of model (always true)
    static bool GetNetPosition(nn::math::VEC3* out, const HumanModel* model); // 0x006BFBEC
    // the frame of the visibility animation of an axe: the head looks damaged by GetAxeDamage (only the item kind 14,
    // the axes)
    static f32 GetVisibilityFrame(const SvFgName* item); // 0x006BFCDC
    // the tip of the rod from its matrix
    static void GetRodTipPosition(nn::math::VEC3* out, const nn::math::MTX34* matrix); // 0x006C0B04
    // the first of seven points of the tool at the hand of the player under the ground (its index; -1: none)
    static s32 FindGroundHitOfPlayer(nn::math::VEC3* out, const AcPlayer* player); // 0x006C0E80
    static s32 FindGroundHit(nn::math::VEC3* out, const HumanModel* model, const nn::math::VEC3* position); // 0x006C0E94
    // four points of the tool at the hand of model (false: no hand matrix)
    static bool GetToolPoints(nn::math::VEC3* point0, nn::math::VEC3* point1, nn::math::VEC3* point2,
                              nn::math::VEC3* point3, const HumanModel* model); // 0x006C122C
    // the height of the ground at position (the beach of the island stages has its own rules)
    static f32 GetGroundHeight(const nn::math::VEC3* position); // 0x006C1CB8
    static f32 GetGroundHeightNear(const nn::math::VEC3* position, const nn::math::VEC3* other); // 0x006C1E24
    static bool GetScoopPosition(nn::math::VEC3* out, const HumanModel* model); // 0x006C2488

    // the states of the float
    void EnterFloatHold(const Actor* actor, const nn::math::MTX34* matrix); // 0x006C3734
    void UpdateFloatHold(const Actor* actor, const nn::math::MTX34* matrix); // 0x006C2994
    void ExitFloatHold(u32 nextState); // 0x006C2C7C
    void EnterFloatCast(const Actor* actor, const nn::math::MTX34* matrix); // 0x006C3D4C
    void UpdateFloatCast(const Actor* actor, const nn::math::MTX34* matrix); // 0x006C391C
    void ExitFloatCast(u32 nextState); // 0x006C3D2C
    void EnterFloatSwing(const Actor* actor, const nn::math::MTX34* matrix); // 0x006C3E14
    void UpdateFloatSwing(const Actor* actor, const nn::math::MTX34* matrix); // 0x006C3C74
    void ExitFloatSwing(u32 nextState); // 0x006C3D30
    void EnterFloatFall(const Actor* actor, const nn::math::MTX34* matrix); // 0x006C38B0
    void UpdateFloatFall(const Actor* actor, const nn::math::MTX34* matrix); // 0x006C2A98
    void ExitFloatFall(u32 nextState); // 0x006C2E34
    void EnterFloatFloat(const Actor* actor, const nn::math::MTX34* matrix); // 0x006C3A24
    void UpdateFloatFloat(const Actor* actor, const nn::math::MTX34* matrix); // 0x006C3078
    void ExitFloatFloat(u32 nextState); // 0x006C383C
    void EnterFloatBite(const Actor* actor, const nn::math::MTX34* matrix); // 0x006C3644
    void UpdateFloatBite(const Actor* actor, const nn::math::MTX34* matrix); // 0x006C2854
    void ExitFloatBite(u32 nextState); // 0x006C2C54
    void EnterFloatHooked(const Actor* actor, const nn::math::MTX34* matrix); // 0x006C2EEC
    void UpdateFloatHooked(const Actor* actor, const nn::math::MTX34* matrix); // 0x006C21F0
    void ExitFloatHooked(u32 nextState); // 0x006C29D0
    void EnterFloatReel(const Actor* actor, const nn::math::MTX34* matrix); // 0x006C3A20
    void UpdateFloatReel(const Actor* actor, const nn::math::MTX34* matrix); // 0x006C2FC0
    void ExitFloatReel(u32 nextState); // 0x006C3838
    void EnterFloatPullOut(const Actor* actor, const nn::math::MTX34* matrix); // 0x006C3A50
    void UpdateFloatPullOut(const Actor* actor, const nn::math::MTX34* matrix); // 0x006C3404
    void ExitFloatPullOut(u32 nextState); // 0x006C3868
    void EnterFloatSink(const Actor* actor, const nn::math::MTX34* matrix); // 0x006C3620
    void UpdateFloatSink(const Actor* actor, const nn::math::MTX34* matrix); // 0x006C2724
    void ExitFloatSink(u32 nextState); // 0x006C2C50

    // (inline) the shown model (the original calls the const slot of the base directly)
    const g3d::SkeletalModel* GetShownModel() const
    {
        return static_cast<const ModelBank*>(this)->ModelBank::GetCurrentModel();
    }
    // (inline) the animations of the shown slot (NULL without a shown model)
    g3d::MorphSkeletalAnim* GetMorphAnim()
    {
        return GetShownModel() != NULL ? &m_MorphAnims[GetCurrentSlot()] : NULL;
    }
    g3d::MaterialAnim* GetMaterialAnim()
    {
        return GetShownModel() != NULL ? &m_MaterialAnims[GetCurrentSlot()] : NULL;
    }
    // (inline) a counter of frames goes down to 0
    static void CountDown(u16* counter)
    {
        if (*counter != 0) {
            *counter = *counter >= 1 ? *counter - 1 : 0;
        }
    }

    u16 m_AnimIds[2];                                   // 0x4D0 HumanAnimName of the morph animation (COUNT: none)
    g3d::VisibilityAnim m_VisibilityAnims[2];           // 0x4D4
    g3d::MaterialAnim m_MaterialAnims[2];               // 0x504 the material animation of the tool animation
    g3d::MaterialAnim m_ItemMaterialAnims[2];           // 0x544 a frame per item (the pinwheel: item::GetIndex)
    BalloonString* m_pBalloonStrings[2];                // 0x584 (TOOL_KIND_BALLOON)
    u32 m_PlayerNo;                                     // 0x58C (4: none)
    bool m_IsMyPlayer;                                  // 0x590 m_PlayerNo is the player of this machine
    SubModels m_SubModels[2];                           // 0x594 (TOOL_KIND_SLINGSHOT)
    f32 m_WindmillTargetSpeed;                          // 0x5DC
    f32 m_WindmillSpeed;                                // 0x5E0
    math::Angle16 m_BoneRotateX;                        // 0x5E4 the angle of bone 1 of the pinwheel
    bool m_IsBoneRotated;                               // 0x5E6
    bool m_IsBoneTransformSaved;                        // 0x5E7
    nw::gfx::CalculatedTransform m_SavedBoneTransform;  // 0x5E8 the local transform of bone 1 without the angle
    g3d::Model m_FloatModels[2];                        // 0x628 (TOOL_KIND_FISHING_ROD: model 1 of the file)
    u8 m_FloatState;                                    // 0x640 FloatState
    nn::math::VEC3 m_FloatPosition;                     // 0x644
    nn::math::VEC3 m_FloatTarget;                       // 0x650 (y: the surface height there)
    math::Angle16Vec3 m_FloatRotation;                  // 0x65C
    math::Angle16Vec3 m_FloatTargetRotation;            // 0x662 (only x is used)
    ObjectHandle m_FloatHandle;                         // 0x668 (an object of the game at the float, probably the fish:
                                                        // Func71B550 gives the size class 2..7 for FLOAT_STATE_HOOKED)
    nn::math::VEC3 m_FloatCastOffset;                   // 0x66C the offset of the float to the actor when cast
    f32 m_FloatShadowScale;                             // 0x678
    nn::math::MTX34 m_RodMatrix;                        // 0x67C the matrix of the tip of the rod (UpdateFloat)
    nn::math::VEC3 m_ActorPosition;                     // 0x6AC
    math::Angle16 m_ActorAngleY;                        // 0x6B8
    bool m_IsFloatShown;                                // 0x6BA
    u16 m_FloatCastFrames;                              // 0x6BC
    nn::math::VEC3 m_FloatCastStart;                    // 0x6C0
    bool m_IsFloatInWater;                              // 0x6CC
    u8 m_Unknown6CD[3];                                 // 0x6CD
    u16 m_FloatFollowFrames;                            // 0x6D0
    f32 m_FloatSpeedY;                                  // 0x6D4
    u16 m_FloatFallFrames;                              // 0x6D8
    SeadParticleHandle m_LandEffect;                     // 0x6DC
    u16 m_PullOutFrames;                                 // 0x6F4
    SeadParticleHandle m_PullOutEffect;                     // 0x6F8
    u16 m_PullOutAnimId;                                 // 0x710 HumanAnimName
    nn::math::VEC3 m_PullOutTarget;                      // 0x714
    f32 m_PullOutSpeedY;                                 // 0x720
    nn::math::VEC3 m_PullOutStart;                       // 0x724
    nn::math::VEC3 m_HookedStart;                       // 0x730
    nn::math::VEC3 m_HookedTarget;                      // 0x73C
    f32 m_HookedMaxDistance;                            // 0x748
    f32 m_HookedDistance;                               // 0x74C
    SeadParticleHandle m_HookedEffect;                     // 0x750
    SeadParticleHandle m_DriftEffect;                     // 0x768
    bool m_IsDrifting;                                  // 0x780
    u8 m_DriftAttribute;                                // 0x781 (8: none)
    math::Angle16 m_DriftAngle;                         // 0x782
    u16 m_DriftFrames;                                  // 0x784
    u8 m_Unknown786[2];                                 // 0x786
    u16 m_SinkFrames;                                   // 0x788
    f32 m_SinkSpeed;                                    // 0x78C
    SeadParticleHandle m_BiteEffect;                     // 0x790
    u8 m_Unknown7A8;                                    // 0x7A8
    s32 m_EffectBoneIndex;                              // 0x7AC the bone "eff" of the model (-1: none)
    g3d::MorphSkeletalAnim m_MorphAnims[2];             // 0x7B0
    SoundState m_SoundStates[2];                        // 0x7E0
    s16 m_WindmillVolume;                               // 0x7E2
    u8 m_Unknown7E4;                                    // 0x7E4 (PlayToolAnim)
    AudioObj1 m_Sounds[2];                              // 0x7E8

    // the functions of the states of the float
    static const FloatStateInfo s_FloatStates[FLOAT_STATE_NUM];
    // the movement of the float (variables in .data)
    static f32 s_FloatGravity;    // per frame
    static s32 s_FloatFallFrames; // of FLOAT_STATE_FALL
    static f32 s_FloatDriftSpeed; // per frame
    // the points of the tools for FindGroundHit and GetToolPoints (factors of the unit size; the original reads them
    // from memory)
    static const f32 s_HitHeightMargin;  // added to the height of the ground
    static const f32 s_HitHeadEnd;       // the far end of the head
    static const f32 s_HitHeadHalfWidth;
    static const f32 s_HitHeadLength;
    static const f32 s_HitHandleX;
    static const f32 s_HitPoint0Z;
    static const f32 s_HitPoint0X;
    static const f32 s_HitGripZ;
    static const f32 s_HitGripX;
    // the step of the shadow scale of the float (times 4)
    static const f32 s_FloatShadowStep;
    // the frames of the visibility animation by GetAxeDamage
    static const f32 s_VisibilityFrames[8];
};
ASSERT_SIZE(ToolBank, 0x8A8);
ASSERT_OFFSET(ToolBank, m_AnimIds, 0x4D0);
ASSERT_OFFSET(ToolBank, m_SubModels, 0x594);
ASSERT_OFFSET(ToolBank, m_SavedBoneTransform, 0x5E8);
ASSERT_OFFSET(ToolBank, m_FloatModels, 0x628);
ASSERT_OFFSET(ToolBank, m_RodMatrix, 0x67C);
ASSERT_OFFSET(ToolBank, m_LandEffect, 0x6DC);
ASSERT_OFFSET(ToolBank, m_HookedEffect, 0x750);
ASSERT_OFFSET(ToolBank, m_BiteEffect, 0x790);
ASSERT_OFFSET(ToolBank, m_MorphAnims, 0x7B0);
ASSERT_OFFSET(ToolBank, m_Sounds, 0x7E8);
