#pragma once

#include "decomp.h"
#include "nw/anim/anim_AnimFrameController.h"
#include "nw/anim/res/anim_ResAnim.h"
#include "nw/gfx/gfx_AnimObject.h"
#include "nw/ut/ut_MoveArray.h"

namespace nw {
namespace gfx {
// the results of TryBind besides RESULT_SUCCESS (names are ours)
const u32 RESULT_BIND_PARTIAL = 0x1;    // some members of the animation are not in the group
const u32 RESULT_BIND_NONE = 0x80000002; // no member of the animation is in the group

// RTTI N2nw3gfx17BaseAnimEvaluatorE @ 0x008D0694
// vtable 0x009026F8 (vptr 0x00902700), offset_to_top 0, 13 entries
// Plays one animation resource on the members of the bound group with the same path. The function
// names are from the reference symbols (the cache slots 0x28-0x30 and the members are ours).
class BaseAnimEvaluator : public AnimObject
{
public:
    // (inline)
    BaseAnimEvaluator(os::IAllocator* pAllocator, int animType) : AnimObject(pAllocator, animType), m_IsCacheDirty(true), m_IsCacheExternal(false), m_UseCacheFrame(false) {}
    virtual ~BaseAnimEvaluator();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x0073A864 slot 0x08
    virtual u32 TryBind(AnimGroup* pAnimGroup);
    virtual void Release();
    virtual void UpdateFrame();
    // a new animation (bound again to the group)
    virtual u32 ChangeAnim(anim::res::ResAnim animData); // slot 0x24
    // the buffer of the values of the last frame (NULL: none)
    virtual void* GetCacheBuffer() const { return nullptr; } // 0x0073A85C slot 0x28
    virtual u32 GetCacheSize() const { return 0; } // 0x0073A870 slot 0x2C
    // a buffer of size bytes for the cache (not freed by the evaluator; NULL: none)
    virtual void SetCacheBuffer(void* pBuffer, u32 size); // slot 0x30

    // (inline)
    anim::res::ResAnim GetAnimData() const { return m_AnimData; }
    f32 GetFrame() const { return m_AnimFrameController.GetFrame(); }
    // the member animation of a group member (NO_MEMBER_ANIM: none)
    int GetMemberAnimIndex(int memberIdx) const { return m_BindIndices[memberIdx]; }

    // the animation of the resource without a binding
    void SetAnimData(anim::res::ResAnim animData)
    {
        m_AnimData = animData;
        m_AnimFrameController.m_Speed = 1.0f;
        m_AnimFrameController.m_StartFrame = 0.0f;
        m_AnimFrameController.m_EndFrame = animData.GetFrameSize();
        m_AnimFrameController.m_Frame.ResetFrame(0.0f);
        switch (animData.GetLoopMode()) {
        case anim::res::ResAnimData::LOOP_MODE_ONETIME:
            m_AnimFrameController.m_PlayPolicy = anim::PlayPolicy_Onetime;
            break;
        case anim::res::ResAnimData::LOOP_MODE_LOOP:
            m_AnimFrameController.m_PlayPolicy = anim::PlayPolicy_Loop;
            break;
        default:
            break;
        }
        m_IsCacheDirty = true;
    }

    static const TypeInfo s_TypeInfo;
    // the member animation index of an unbound group member
    static const int NO_MEMBER_ANIM;

    ut::MoveArray<int> m_BindIndices;                 // 0x10 per group member: its member animation
    ut::MoveArray<int> m_AnimMemberIndices;           // 0x20 per member animation: its group member
    anim::AnimFrameController m_AnimFrameController;  // 0x30
    anim::res::ResAnim m_AnimData;                    // 0x54
    bool m_IsCacheDirty;                              // 0x58 the frame changed since the last UpdateCache
    bool m_IsCacheExternal;                           // 0x59 the cache buffer is not ours (SetCacheBuffer)
    bool m_UseCacheFrame;                             // 0x5A (AnimEvaluator)
};
ASSERT_SIZE(BaseAnimEvaluator, 0x5C);
} // namespace gfx
} // namespace nw
