#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_AnimGroup.h"
#include "nw/gfx/gfx_GfxObject.h"
#include "nw/gfx/gfx_Model.h"
#include "nw/gfx/gfx_Skeleton.h"
#include "nw/gfx/gfx_StandardSkeleton.h"
#include "nw/gfx/res/gfx_ResSkeleton.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx13SkeletalModelE @ 0x008D0628
// vtable 0x00902564 (vptr 0x0090256C), offset_to_top 0, 11 entries
// A model with a skeleton (its own, or the one of another model) and the animation group of the
// bones. Member names are ours.
class SkeletalModel : public Model
{
public:
    class Builder;

    // (inline in Builder::Create)
    SkeletalModel(os::IAllocator* pAllocator, res::ResModel resource, const Description& description, GfxPtr<Skeleton> skeleton,
                  bool isSharingSkeleton)
        : Model(pAllocator, resource, description), m_Skeleton(skeleton), m_pSkeletalAnimGroup(nullptr), m_SkeletalAnimGroupIndex(-1),
          m_IsSharingSkeleton(isSharingSkeleton), m_IsWorldMatrixAnimation(false)
    {
        if (!isSharingSkeleton) {
            m_Skeleton->m_pOwner = this;
        }
    }
    virtual ~SkeletalModel(); // 0x00498360 slot 0x00 | nintendogs:bytes

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x00738B74 slot 0x08
    virtual void Accept(ISceneVisitor* pVisitor); // 0x00497C08 slot 0x10 | nintendogs:bytes
    virtual u32 Initialize(os::IAllocator* pAllocator); // 0x00497808 slot 0x24 | nintendogs:bytes

    Skeleton* GetSkeleton() const { return m_Skeleton.Get(); }

    // (inline) the memory Builder::Create takes with a skeleton of its own: the local transforms,
    // the skeleton, the model and the animation group of the bones
    DECOMP_ALWAYS_INLINE static void GetMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResModel resource, const Description& description)
    {
        const res::ResSkeletonData* pResSkeleton =
            static_cast<const res::ResSkeletalModelData*>(resource.ptr())->skeleton.To<res::ResSkeletonData>();
        const u32 alignment = pSize->GetAlignment();
        pSize->SetAlignment(32);
        pSize->Add(pResSkeleton->boneCount * sizeof(CalculatedTransform));
        pSize->SetAlignment(alignment);
        StandardSkeleton::GetMemorySizeInternal(pSize, res::ResSkeleton(pResSkeleton), description.maxCallbacks);

        pSize->Add(sizeof(SkeletalModel));
        Model::GetMemorySizeForInitialize(pSize, resource, description);
        if (description.isAnimationEnabled) {
            const int groupCount = resource.GetAnimGroupCount();
            for (int i = 0; i < groupCount; ++i) {
                anim::res::ResAnimGroup resAnimGroup = resource.GetAnimGroup(i);
                if ((resAnimGroup.GetFlags() & anim::res::ResAnimGroupData::FLAG_DIRECT_RESULT) &&
                    resAnimGroup.GetTargetType() == anim::res::ResAnimGroupData::TARGET_TYPE_SKELETON) {
                    pSize->Add(sizeof(AnimGroup));
                    AnimGroup::GetMemorySizeForInitialize(pSize, resAnimGroup, true);
                    break;
                }
            }
        }
    }

    static const TypeInfo s_TypeInfo;

    // the animation group of the bones (if the resource has one)
    u32 CreateSkeletalAnimGroup(os::IAllocator* pAllocator); // 0x0049785C | nintendogs:bytes-fuzzy [tier A]
    // the animated values of the group: the local transforms of the bones, or their world matrices
    void SetupAnimGroup(AnimGroup* pAnimGroup, bool isWorldMatrixAnimation) const; // 0x00738A3C | nintendogs:bytes [tier A]

    GfxPtr<Skeleton> m_Skeleton;          // 0x220
    AnimGroup* m_pSkeletalAnimGroup;      // 0x228
    s32 m_SkeletalAnimGroupIndex;         // 0x22C
    bool m_IsSharingSkeleton;             // 0x230
    bool m_IsWorldMatrixAnimation;        // 0x231
};
ASSERT_SIZE(SkeletalModel, 0x234);
} // namespace gfx
} // namespace nw
