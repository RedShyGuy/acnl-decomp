#pragma once

#include "decomp.h"

class SoundObjBase;

// The footsteps of a player or a character (a member of SoundObjPlayer and SoundObjNpc): each step plays the walk sound of
// the ground (SE_ACT_WALK_L_* and SE_ACT_WALK_R_* by turns), a jump or a landing the matching SE_ACT_JUMP_* or SE_ACT_LAND_*;
// on grass and soil the ground cover of the stage (SoundMgr::m_GroundCover) turns them into leaves or snow. The steps follow
// the volume and the pans of the owner (the class and all names are ours).
class SoundFootstep
{
public:
    // m_Size: the pitch of the steps
    enum Size
    {
        SIZE_SMALL = 0,
        SIZE_MEDIUM = 1,
        SIZE_LARGE = 2,
        SIZE_NUM = 3,
    };

    SoundFootstep(); // 0x002A6B9C

    void Init(SoundObjBase* owner); // 0x002A6574
    void Stop(); // 0x002A6B60
    // a step (walkSound: SE_ACT_WALK_L_*, -1: none); speed 0..1
    void Walk(f32 speed, u32 walkSound, u32 groundValue); // 0x002A67F8
    void Jump(u32 walkSound, u32 groundValue); // 0x002A6B74
    void Land(u32 walkSound, u32 groundValue); // 0x002A6B88

    static const f32 s_SizePitches[SIZE_NUM];

    SoundObjBase* m_pOwner; // 0x00
    s32 m_Variable;         // 0x04 the sequence variable 10 of the steps
    bool m_IsRightFoot;     // 0x08 the next step is SE_ACT_WALK_R_*
    u32 m_Size;             // 0x0C Size
    bool m_IsBell;          // 0x10 SE_ACT_WALK_BELL with each step
    bool m_IsHeel;          // 0x11 SE_ACT_WALK_*_HEEL with each step
    f32 m_Speed;            // 0x14 of the last step

private:
    // the walk sound of the ground: grass and soil (above 55, thin below 110) follow the ground cover
    DECOMP_ALWAYS_INLINE static u32 GetGroundSound(u32 walkSound, u32 groundValue);
    // a jump or a landing (baseSound: SE_ACT_JUMP_GRASS or SE_ACT_LAND_GRASS)
    void PlayOnce(u32 baseSound, u32 walkSound, u32 groundValue); // 0x002A6584
};
ASSERT_SIZE(SoundFootstep, 0x18);
