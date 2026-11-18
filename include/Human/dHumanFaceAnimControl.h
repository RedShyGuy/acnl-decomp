#pragma once

#include "decomp.h"
#include "Human/dHumanModel_FaceCtrl.h"
#include "g3d/dMaterialAnim.h"
#include "nw/anim/res/anim_ResAnim.h"

class HumanResTextureAnimBank;

namespace nw {
namespace os {
class IAllocator;
} // namespace os
} // namespace nw

// The material animations of the face of a human model in two slots (class name from the RTTI, size after libgarden;
// the members and function names are ours). Each slot plays a clone of the material animation of an animation id
// (HumanAnimName) in the heap of its HumanResTextureAnimBank and returns to its default animation (m_DefaultAnimIdA/B)
// when a one-time animation ends; the default animation of the first slot restarts after 20..79 frames.
// RTTI 20HumanFaceAnimControl @ 0x008CCB7C
// vtable 0x008F5838 (vptr 0x008F5840), offset_to_top 0, 2 entries
class HumanFaceAnimControl : public ::HumanModel::FaceCtrl
{
public:
    // the getters of the animation table for a slot (the material animation of an id; its clone in an allocator)
    typedef const nw::anim::res::ResAnim* (*GetAnimFunc)(u32 id);
    typedef nw::anim::res::ResAnim (*CloneAnimFunc)(u32 id, nw::os::IAllocator* allocator);

    // the flags of PlayAnim
    enum PlayFlag
    {
        PLAY_FLAG_DEFAULT_IF_MISSING = 1 << 0, // the default animation of the first slot if id has none
        PLAY_FLAG_RESTART = 1 << 1,            // from the start (frame 0, speed 1, not looped); the timer starts
    };

    // a slot of the face
    struct AnimSlot
    {
        AnimSlot()
        {
            m_pBank = NULL;
            m_AnimId = HumanAnimName::INVALID;
            m_ResAnim = nw::anim::res::ResAnim();
            m_Timer = 0;
            m_IsHold = false;
            m_Unknown31 = false;
        }

        HumanResTextureAnimBank* m_pBank;  // 0x00 the heap of the clone
        u16 m_AnimId;                      // 0x04 HumanAnimName
        nw::anim::res::ResAnim m_ResAnim;  // 0x08 the clone
        g3d::MaterialAnim m_Anim;          // 0x0C
        s32 m_Timer;                       // 0x2C frames until the default animation restarts
        bool m_IsHold;                     // 0x30 the one-time animation stays at its end (no return to the default)
        bool m_Unknown31;                  // 0x31 (only cleared)
    };

    HumanFaceAnimControl(); // 0x0031E710
    virtual ~HumanFaceAnimControl();

    // an animation id in the first slot (playMode 2: the loop flag of the id; without useSpeed only the sign of the
    // speed is kept)
    DECOMP_NOINLINE void PlayAnimA(u16 id, s32 playMode, bool useSpeed, f32 frame, f32 speed); // 0x0031DDE0
    // the second slot returns to its default animation if it is missing (or the slot had none)
    DECOMP_NOINLINE void PlayAnimB(u16 id, s32 playMode, bool useSpeed, f32 frame, f32 speed); // 0x0031DF3C
    // the fallback animation of the second slot ends (evaluate: the model evaluates its animations first, only with a
    // fallback unless force)
    void EndFallbackB(bool evaluate, bool force); // 0x0031DEE4
    // the slots in the frame (updateFrames: the model updates the frames of its animations)
    void Update(bool updateFrames); // 0x0031E078
    // like PlayAnimB; the default animation also when the second slot had none
    void PlayAnimB2(u16 id, s32 playMode, bool useSpeed, f32 frame, f32 speed); // 0x0031E1C0
    // (true: the animation plays)
    DECOMP_NOINLINE bool PlayAnim(AnimSlot* slot, GetAnimFunc getAnim, CloneAnimFunc cloneAnim, f32 frame, f32 speed, s32 objectIdx,
                  u16 id, s32 playMode, u32 flags); // 0x0031E2E0
    DECOMP_NOINLINE void StopAnim(AnimSlot* slot, s32 objectIdx); // 0x0031E588
    // the slots for model (the clones in the heaps of bankA/bankB, the graphics file of resourceSet); both play NML_WAIT_1
    void Initialize(g3d::Model* model, HumanResTextureAnimBank* bankA, HumanResTextureAnimBank* bankB, void* unused,
                    g3d::ResourceSet* resourceSet, bool unknown24); // 0x0031E5E4
    void Finalize(); // 0x0031E6C4

    g3d::ResourceSet* m_pResourceSet20; // 0x20 (the same as FaceCtrl::m_pResourceSet)
    bool m_Unknown24;                   // 0x24 (false: the clones unbind member 1 of the animation)
    u16 m_DefaultAnimIdA;               // 0x26 ANIM_F7
    u16 m_DefaultAnimIdB;               // 0x28 ANIM_FA
    bool m_IsFallbackB;                 // 0x2A the second slot plays its default animation in place of a missing one
    AnimSlot m_SlotA;                   // 0x2C
    AnimSlot m_SlotB;                   // 0x60
};
ASSERT_SIZE(HumanFaceAnimControl, 0x94);
