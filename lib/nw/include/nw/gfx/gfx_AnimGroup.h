#pragma once

#include "decomp.h"
#include "nw/anim/anim_AnimBlendOp.h"
#include "nw/anim/res/anim_ResAnimGroup.h"
#include "nw/gfx/gfx_GfxObject.h"
#include "nw/os/os_MemorySizeCalculator.h"
#include "nw/ut/ut_MoveArray.h"

namespace nw {
namespace gfx {
class SceneNode;

// RTTI N2nw3gfx9AnimGroupE @ 0x008D0820
// vtable 0x00902AE8 (vptr 0x00902AF0), offset_to_top 0, 2 entries
// The members of an object that animations can change, after an animation group resource: per
// member its blend operation and pointers to the animated values (names of the functions from the
// reference symbols, the rest ours).
class AnimGroup : public GfxObject
{
public:
    // whether the members of an object are animated now (type and name are ours)
    typedef bool (*IsTargetEnabledFunc)(AnimGroup* pAnimGroup, void* pTargetObject);

    // the kind of blend operation of a member (the u32 array of the resource; names are ours)
    enum BlendOpType
    {
        BLEND_OP_BOOL = 0,
        BLEND_OP_INT = 1,
        BLEND_OP_FLOAT = 2,
        BLEND_OP_RGBA_COLOR = 3,
        BLEND_OP_TRANSFORM = 4, // AnimBlendOpTransform (in an AnimResult)
        BLEND_OP_VECTOR2 = 5,
        BLEND_OP_VECTOR3 = 6,
        BLEND_OP_TEXTURE = 7,
        BLEND_OP_TRANSFORM_STANDARD = 8,
        BLEND_OP_TRANSFORM_QUAT = 9,
        BLEND_OP_TRANSFORM_ACC_SCALE = 10,
        BLEND_OP_TRANSFORM_ACC_SCALE_QUAT = 11,
    };

    AnimGroup(anim::res::ResAnimGroup resAnimGroup, SceneNode* pSceneNode, os::IAllocator* pAllocator); // 0x004B3784 | nintendogs:callgraph [tier A]
    virtual ~AnimGroup();

    // the arrays (the original values only if useOriginalValue)
    u32 Initialize(bool useOriginalValue);
    // the memory Initialize allocates
    static void GetMemorySizeForInitialize(os::MemorySizeCalculator* pSize, anim::res::ResAnimGroup resAnimGroup, bool useOriginalValue);

    // (inline)
    anim::res::ResAnimGroup GetResAnimGroup() const { return m_ResAnimGroup; }
    s32 GetMemberCount() const { return m_ResAnimGroup.GetMemberCount(); }
    anim::res::ResAnimGroupMember GetResAnimGroupMember(int memberIdx) const { return m_ResAnimGroup.GetMember(memberIdx); }
    // the blend operation of a member (NULL: the values are written to the member directly)
    const anim::AnimBlendOp* GetBlendOperation(int memberIdx) const
    {
        return m_BlendOps[GetResAnimGroupMember(memberIdx).ptr()->blendOpIndex];
    }
    bool HasOriginalValue() const { return m_OriginalValues.size() != 0; }
    const void* GetOriginalValue(int memberIdx) const { return HasOriginalValue() ? m_OriginalValues[memberIdx] : nullptr; }
    SceneNode* GetSceneNode() const { return m_pSceneNode; }

    anim::res::ResAnimGroup m_ResAnimGroup;              // 0x8
    ut::MoveArray<const anim::AnimBlendOp*> m_BlendOps;  // 0xC per blend operation of the resource
    SceneNode* m_pSceneNode;                             // 0x1C
    ut::MoveArray<void*> m_TargetObjects;                // 0x20 per member: the object (for m_pIsTargetEnabled)
    ut::MoveArray<void*> m_Targets;                      // 0x30 per member: what SetValueForType writes to
    ut::MoveArray<void*> m_TargetValues;                 // 0x40 per member: the value the animations write
    ut::MoveArray<void*> m_OriginalValues;               // 0x50 per member (if useOriginalValue)
    IsTargetEnabledFunc m_pIsTargetEnabled;              // 0x60 (NULL: all members are)
    u8 m_Unknown0x64;                                    // 0x64
};
ASSERT_SIZE(AnimGroup, 0x68);
} // namespace gfx
} // namespace nw
