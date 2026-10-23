#include "nw/gfx/gfx_ParticleSet.h"

#include <algorithm>
#include <new>

#include "nw/gfx/gfx_ParticleCollection.h"
#include "nw/gfx/gfx_ParticleShape.h"
#include "nw/gfx/gfx_ParticleUtil.h"
#include "nw/ut/ut_Api.h"

namespace nw {
namespace gfx {
// 0x009826EC
const TypeInfo nw::gfx::ParticleSet::s_TypeInfo(&SceneNode::s_TypeInfo);

// sets s_TypeInfo
// 0x0079A254 (static initializer)

// 0x0048C5E0 | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::ParticleSet::GetMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResParticleSet resource, const Description& description)
{
    const res::ResParticleSetData* pData = resource.ptr();
    const s32 maxInitializers = std::max(description.maxInitializers, pData->initializers.count);
    const s32 maxUpdaters = std::max(description.maxUpdaters, pData->updaters.count);

    const u32 alignment = pSize->GetAlignment();
    pSize->SetAlignment(4);
    pSize->Add(sizeof(ParticleSet));
    pSize->SetAlignment(alignment);
    pSize->SetAlignment(4);
    pSize->Add(maxInitializers * sizeof(Initializer));
    pSize->SetAlignment(alignment);
    pSize->SetAlignment(4);
    pSize->Add(maxUpdaters * sizeof(Updater));
    pSize->SetAlignment(alignment);

    // the copies of the resources
    for (int i = 0; i < pData->initializers.count; ++i) {
        res::ResParticleInitializer initializer(pData->initializers.GetData(i));
        if (initializer.ptr()->isResourceCopied) {
            ParticleUtil::GetMemorySizeForDuplicateResParticleInitializerInternal(pSize, &initializer);
        }
    }
    for (int i = 0; i < pData->updaters.count; ++i) {
        res::ResParticleUpdater updater(pData->updaters.GetData(i));
        if (updater.ptr()->isResourceCopied) {
            ParticleUtil::GetMemorySizeForDuplicateResParticleUpdaterInternal(pSize, &updater);
        }
    }

    SceneNode::GetMemorySizeForInitialize(pSize, resource, description);
    ParticleCollection::GetMemorySizeInternal(pSize, resource.GetCollection());
}

// 0x0048C914 | nintendogs:bytes
void nw::gfx::ParticleSet::Accept(ISceneVisitor* pVisitor)
{
    pVisitor->VisitParticleSet(this);
    for (SceneNode** ppChild = m_Children.begin(); ppChild != m_Children.end(); ++ppChild) {
        (*ppChild)->Accept(pVisitor);
    }
}

// 0x0048C968 | nintendogs:callseq [tier A]
nw::gfx::ParticleSet* nw::gfx::ParticleSet::Create(SceneNode* pParent, res::ResSceneObject resObject, const Description& description,
                                                   os::IAllocator* pAllocator, os::IAllocator* pDeviceAllocator, ParticleShape* pShape)
{
    const res::ResParticleSetData* pResource = res::DynamicCastData<res::ResParticleSetData>(resObject.ptr(), res::ResParticleSetData::TYPE);
    const s32 maxInitializers = std::max(description.maxInitializers, pResource->initializers.count);
    const s32 maxUpdaters = std::max(description.maxUpdaters, pResource->updaters.count);

    u8* pMemory = static_cast<u8*>(pAllocator->Alloc(
        ut::RoundUp(sizeof(ParticleSet) + maxInitializers * sizeof(Initializer), 4) + maxUpdaters * sizeof(Updater), 4));
    if (pMemory == nullptr) {
        return nullptr;
    }
    ParticleSet* pSet = new (pMemory) ParticleSet(pAllocator, res::ResParticleSet(pResource), description);
    u8* pBuffer = pMemory + sizeof(ParticleSet);
    if (pSet->Initialize(pAllocator) & RESULT_FAILURE_MASK) {
        SafeDestroy(pSet);
        return nullptr;
    }
    if (pParent != nullptr) {
        pParent->AttachChild(pSet);
    }

    // the initializers and updaters in the memory behind the set
    bool isSuccess = true;
    pBuffer = ut::RoundUp(pBuffer, 4);
    {
        ut::MoveArray<Initializer> initializers(pBuffer, maxInitializers);
        pSet->m_Initializers = initializers;
    }
    pBuffer += maxInitializers * sizeof(Initializer);
    for (int i = 0; i < pResource->initializers.count; ++i) {
        res::ResParticleInitializer initializer(pResource->initializers.GetData(i));
        bool isResourceOwner;
        const res::ResParticleInitializerData* pUsed;
        if (initializer.ptr()->isResourceCopied) {
            isResourceOwner = true;
            pUsed = ParticleUtil::DuplicateResParticleInitializer(&initializer, pAllocator);
            if (pUsed == nullptr) {
                isSuccess = false;
            }
        } else {
            isResourceOwner = false;
            pUsed = initializer.ptr();
        }
        pSet->m_Initializers.push_back(Initializer(isResourceOwner, pUsed, initializer.ptr()->typeInfo));
    }

    pBuffer = ut::RoundUp(pBuffer, 4);
    {
        ut::MoveArray<Updater> updaters(pBuffer, maxUpdaters);
        pSet->m_Updaters = updaters;
    }
    for (int i = 0; i < pResource->updaters.count; ++i) {
        res::ResParticleUpdater updater(pResource->updaters.GetData(i));
        bool isResourceOwner;
        const res::ResParticleUpdaterData* pUsed;
        if (updater.ptr()->isResourceCopied) {
            isResourceOwner = true;
            pUsed = ParticleUtil::DuplicateResParticleUpdater(&updater, pAllocator);
            if (pUsed == nullptr) {
                isSuccess = false;
            }
        } else {
            isResourceOwner = false;
            pUsed = updater.ptr();
        }

        const u32 typeInfo = updater.ptr()->typeInfo;
        bool hasAnimationValues = false;
        if (typeInfo == res::PARTICLE_UPDATER_ANIMATED_1 || typeInfo == res::PARTICLE_UPDATER_ANIMATED_0 ||
            typeInfo == res::PARTICLE_UPDATER_ANIMATED_2) {
            const res::ResParticleAnimationData* pAnimation = updater.ptr()->animation.To<res::ResParticleAnimationData>();
            if (pAnimation != nullptr && pAnimation->values.ToPtr() != nullptr) {
                hasAnimationValues = true;
            }
        }
        if (typeInfo == res::ResParticleFlagUpdaterData::TYPE &&
            static_cast<const res::ResParticleFlagUpdaterData*>(updater.ptr())->flag == 1) {
            pSet->m_UpdaterFlag = true;
        }
        pSet->m_Updaters.push_back(Updater(isResourceOwner, pUsed, typeInfo, hasAnimationValues));
    }

    if (!isSuccess) {
        SafeDestroy(pSet);
        return nullptr;
    }
    ParticleCollection* pCollection =
        ParticleCollection::Create(pSet, res::ResParticleSet(pResource).GetCollection(), pAllocator, pDeviceAllocator, pShape);
    if (pCollection == nullptr) {
        SafeDestroy(pSet);
        return nullptr;
    }
    pShape->CreateCommandCache(pSet);

    // the buffers of the attributes the initializers and updaters work on
    for (Initializer* pInitializer = pSet->m_Initializers.begin(); pInitializer != pSet->m_Initializers.end(); ++pInitializer) {
        const int usage = pInitializer->pResource->targetUsage;
        if (usage < 0) {
            pInitializer->pTargetBuffers[0] = nullptr;
            pInitializer->pTargetBuffers[1] = nullptr;
        } else {
            pInitializer->pTargetBuffers[0] = pCollection->GetStreamBuffer(usage, pCollection->m_IsBufferSwapped ? 1 : 0);
            pInitializer->pTargetBuffers[1] = pCollection->GetStreamBuffer(usage, pCollection->m_IsBufferSwapped ? 0 : 1);
        }
    }
    for (Updater* pUpdater = pSet->m_Updaters.begin(); pUpdater != pSet->m_Updaters.end(); ++pUpdater) {
        const int usage = pUpdater->pResource->targetUsage;
        if (usage < 0) {
            pUpdater->pTargetBuffers[0] = nullptr;
            pUpdater->pTargetBuffers[1] = nullptr;
        } else {
            pUpdater->pTargetBuffers[0] = pCollection->GetStreamBuffer(usage, pCollection->m_IsBufferSwapped ? 1 : 0);
            pUpdater->pTargetBuffers[1] = pCollection->GetStreamBuffer(usage, pCollection->m_IsBufferSwapped ? 0 : 1);
        }
    }
    return pSet;
}

// 0x0048D57C | nintendogs:callseq-callee [tier A]
nw::gfx::ParticleSet::ParticleSet(os::IAllocator* pAllocator, res::ResParticleSet resource, const Description& description)
    : SceneNode(pAllocator, resource, description), m_pParticleCollection(nullptr), m_Unknown0x50(0), m_Unknown0x74(1.0f, 1.0f, 1.0f),
      m_Unknown0x80(0.0f, 0.0f, 0.0f), m_UpdaterFlag(false)
{
}

// 0x0048D65C slot 0x00 | nintendogs:bytes
// 0x0048D658 (deleting dtor)
nw::gfx::ParticleSet::~ParticleSet()
{
    for (Initializer* pInitializer = m_Initializers.begin(); pInitializer != m_Initializers.end(); ++pInitializer) {
        if (pInitializer->isResourceOwner) {
            GetAllocator()->Free(const_cast<res::ResParticleInitializerData*>(pInitializer->pResource));
            pInitializer->pResource = nullptr;
        }
    }
    for (Updater* pUpdater = m_Updaters.begin(); pUpdater != m_Updaters.end(); ++pUpdater) {
        if (pUpdater->isResourceOwner) {
            GetAllocator()->Free(const_cast<res::ResParticleUpdaterData*>(pUpdater->pResource));
            pUpdater->pResource = nullptr;
        }
    }
    SafeDestroy(m_pParticleCollection);
}

// 0x0049EC90 | nintendogs:bytes [tier A]
void nw::gfx::ParticleSet::GetDeviceMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResParticleSet resource)
{
    ParticleCollection::GetDeviceMemorySizeInternal(pSize, resource.GetCollection());
}

} // namespace gfx
} // namespace nw
