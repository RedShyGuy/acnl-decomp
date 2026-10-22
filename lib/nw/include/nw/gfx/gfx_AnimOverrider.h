#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_AnimBlender.h"
#include "nw/gfx/gfx_AnimGroup.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx13AnimOverriderE @ 0x008D05C8
// vtable 0x00902494 (vptr 0x0090249C), offset_to_top 0, 9 entries
// A blender in which the later animation objects override the earlier ones: a member takes the
// value of the last object that has one; with a blend operation, the parts of the value come from
// the last object that has them.
class AnimOverrider : public AnimBlender
{
public:
    // (inline)
    explicit AnimOverrider(os::IAllocator* pAllocator) : AnimBlender(pAllocator, ANIM_TYPE_OVERRIDER) {}
    virtual ~AnimOverrider();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x00738898 slot 0x08
    virtual const anim::AnimResult* GetResult(void* pTarget, int memberIdx) const // 0x007388A4 slot 0x18 (name is ours)
    {
        const anim::AnimBlendOp* pBlendOp = m_pAnimGroup->GetBlendOperation(memberIdx);
        if (pBlendOp != nullptr && pBlendOp->HasBlend()) {
            anim::AnimResult result;
            result.m_EnabledFlags = 0;
            bool hasResult = false;
            for (int i = m_AnimObjects.size() - 1; i >= 0; --i) {
                const AnimObject* pAnimObject = m_AnimObjects[i];
                if (pAnimObject == nullptr) {
                    continue;
                }
                const anim::AnimResult* pResult = pAnimObject->GetResult(&result, memberIdx);
                if (pResult == nullptr) {
                    continue;
                }
                hasResult = true;
                if (pBlendOp->Override(static_cast<anim::AnimResult*>(pTarget), pResult)) {
                    return static_cast<const anim::AnimResult*>(pTarget);
                }
            }
            return hasResult ? static_cast<const anim::AnimResult*>(pTarget) : nullptr;
        }
        for (int i = m_AnimObjects.size() - 1; i >= 0; --i) {
            const AnimObject* pAnimObject = m_AnimObjects[i];
            if (pAnimObject != nullptr) {
                const anim::AnimResult* pResult = pAnimObject->GetResult(pTarget, memberIdx);
                if (pResult != nullptr) {
                    return pResult;
                }
            }
        }
        return nullptr;
    }

    static const TypeInfo s_TypeInfo;
};
ASSERT_SIZE(AnimOverrider, 0x20);
} // namespace gfx
} // namespace nw
