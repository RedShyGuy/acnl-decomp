#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Transform.h"
#include "nw/anim/res/anim_ResAnimGroup.h"
#include "nw/ut/ut_Offset.h"
#include "nw/ut/ut_ResDicPatricia.h"

namespace nw {
namespace gfx {
namespace res {
// The resource of a scene object (only the fields known so far; names are ours).
struct ResSceneObjectData
{
    enum Flag
    {
        FLAG_IS_BRANCH_VISIBLE = 1 << 0,
    };

    u32 m_Unknown0x0[6];       // 0x0
    u32 flags;                 // 0x18 Flag
    u32 m_Unknown0x1C[3];      // 0x1C
    s32 animGroupCount;        // 0x28
    ut::Offset animGroupDic;   // 0x2C ResDicPatricia of the ResAnimGroupData
};
ASSERT_OFFSET(ResSceneObjectData, animGroupDic, 0x2C);

// the binary resource of a scene object (the class name is from the reference symbols)
class ResSceneObject
{
public:
    // (inline)
    explicit ResSceneObject(const void* pData = nullptr) : m_pData(static_cast<const ResSceneObjectData*>(pData)) {}
    const ResSceneObjectData* ptr() const { return m_pData; }
    bool IsValid() const { return m_pData != nullptr; }
    u32 GetFlags() const { return m_pData->flags; }
    s32 GetAnimGroupCount() const { return m_pData->animGroupCount; }
    anim::res::ResAnimGroup GetAnimGroup(int index) const
    {
        ut::ResDicPatricia dic = {m_pData->animGroupDic.To<ut::ResDicPatricia::Data>()};
        return anim::res::ResAnimGroup(dic.GetDataByIndex(index));
    }

    const ResSceneObjectData* m_pData; // 0x0
};
ASSERT_SIZE(ResSceneObject, 0x4);

// The resource of a transform node (only the fields known so far; names are ours).
struct ResTransformNodeData : public ResSceneObjectData
{
    nn::math::Transform3 transform;    // 0x30 the local transform
    nn::math::MTX34 m_Unknown0x54;     // 0x54
    nn::math::MTX34 worldMatrix;       // 0x84
};
ASSERT_OFFSET(ResTransformNodeData, worldMatrix, 0x84);

// the binary resource of a transform node (the class name is from the reference symbols)
class ResTransformNode : public ResSceneObject
{
public:
    // (inline)
    explicit ResTransformNode(const void* pData = nullptr) : ResSceneObject(pData) {}
    const ResTransformNodeData* ptr() const { return static_cast<const ResTransformNodeData*>(m_pData); }
    const nn::math::Transform3& GetTransform() const { return ptr()->transform; }
    const nn::math::MTX34& GetWorldMatrix() const { return ptr()->worldMatrix; }
};
ASSERT_SIZE(ResTransformNode, 0x4);
} // namespace res
} // namespace gfx
} // namespace nw
