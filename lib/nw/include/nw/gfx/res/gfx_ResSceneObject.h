#pragma once

#include <cstring>

#include "decomp.h"
#include "nn/math/math_ARMv6.h"
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
        FLAG_INHERIT_DIRECTION = 1 << 3, // a light direction turns with the parent (name is ours)
    };

    u32 typeInfo;              // 0x00
    u32 signature;             // 0x04
    u32 m_Unknown0x8;          // 0x08 (a revision?)
    ut::Offset name;           // 0x0C
    s32 userDataCount;         // 0x10
    ut::Offset userDataDic;    // 0x14
    u32 flags;                 // 0x18 Flag
    u32 m_Unknown0x1C;         // 0x1C
    u32 m_Unknown0x20;         // 0x20
    u32 m_Unknown0x24;         // 0x24
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

// (inline) the copy of the transform node part of a resource (the matrices with MTX34CopyAsm, as the
// copy constructor of MTX34 does in the original; the offsets are copied as they are; name is ours)
inline void CopyResTransformNodeData(ResTransformNodeData* pOut, const ResTransformNodeData& resource)
{
    std::memcpy(static_cast<ResSceneObjectData*>(pOut), static_cast<const ResSceneObjectData*>(&resource), sizeof(ResSceneObjectData));
    pOut->transform = resource.transform;
    nn::math::ARMv6::MTX34CopyAsm(&pOut->m_Unknown0x54, &resource.m_Unknown0x54);
    nn::math::ARMv6::MTX34CopyAsm(&pOut->worldMatrix, &resource.worldMatrix);
}
} // namespace res
} // namespace gfx
} // namespace nw
