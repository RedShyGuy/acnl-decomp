#include "nw/gfx/gfx_ParticleEmitter.h"

#include <new>

#include "nn/nstd/nstd_String.h"

namespace nw {
namespace gfx {
// 0x0097F480
const TypeInfo nw::gfx::ParticleEmitter::s_TypeInfo(&TransformNode::s_TypeInfo);

// sets s_TypeInfo
// 0x0079D0FC (static initializer)

// 0x0049B418 (name is ours)
void nw::gfx::ParticleEmitter::GetMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResTransformNode resource, const Description& description)
{
    const res::ResParticleEmitterData* pResource = static_cast<const res::ResParticleEmitterData*>(resource.ptr());
    if (pResource->isParameterCopied) {
        pSize->Add(PARAMETER_SIZE);
    }
    const res::ResParticleVolumeData* pResVolume = pResource->volume.To<res::ResParticleVolumeData>();
    if (pResVolume != nullptr && pResVolume->isResourceCopied) {
        const u32 size = GetVolumeSize(pResVolume->typeInfo);
        if (size != 0) {
            pSize->Add(size);
        }
    }
    pSize->Add(sizeof(ParticleEmitter));
    TransformNode::GetMemorySizeForInitialize(pSize, resource, description);
}

// 0x0049B6FC slot 0x10 | nintendogs:bytes
void nw::gfx::ParticleEmitter::Accept(ISceneVisitor* pVisitor)
{
    pVisitor->VisitParticleEmitter(this);
    for (SceneNode** ppChild = m_Children.begin(); ppChild != m_Children.end(); ++ppChild) {
        (*ppChild)->Accept(pVisitor);
    }
}

// 0x0049B750 | nintendogs:bytes-fuzzy [tier A]
nw::gfx::ParticleEmitter* nw::gfx::ParticleEmitter::Create(SceneNode* pParent, res::ResSceneObject resObject, const Description& description,
                                                           os::IAllocator* pAllocator)
{
    const res::ResParticleEmitterData* pResource =
        res::DynamicCastData<res::ResParticleEmitterData>(resObject.ptr(), res::ResParticleEmitterData::TYPE);

    void* pParameters = nullptr;
    if (pResource->isParameterCopied) {
        pParameters = pAllocator->Alloc(PARAMETER_SIZE, 4);
        if (pParameters == nullptr) {
            return nullptr;
        }
        nnnstdMemCpy(pParameters, &pResource->isParameterCopied, PARAMETER_SIZE);
    }

    res::ResParticleVolumeData* pVolume = nullptr;
    const res::ResParticleVolumeData* pResVolume = pResource->volume.To<res::ResParticleVolumeData>();
    if (pResVolume != nullptr && pResVolume->isResourceCopied) {
        const u32 size = GetVolumeSize(pResVolume->typeInfo);
        if (size != 0) {
            pVolume = static_cast<res::ResParticleVolumeData*>(pAllocator->Alloc(size, 4));
        }
        if (pVolume == nullptr) {
            if (pParameters != nullptr) {
                pAllocator->Free(pParameters);
            }
            return nullptr;
        }
        nnnstdMemCpy(pVolume, pResVolume, size);
    }

    void* pMemory = pAllocator->Alloc(sizeof(ParticleEmitter), 4);
    if (pMemory == nullptr) {
        if (pParameters != nullptr) {
            pAllocator->Free(pParameters);
        }
        if (pVolume != nullptr) {
            pAllocator->Free(pVolume);
        }
        return nullptr;
    }
    ParticleEmitter* pEmitter = new (pMemory) ParticleEmitter(pAllocator, res::ResTransformNode(pResource), description, pParameters, pVolume);
    if (pEmitter->Initialize(pAllocator) & RESULT_FAILURE_MASK) {
        SafeDestroy(pEmitter);
        return nullptr;
    }
    if (pParent != nullptr) {
        pParent->AttachChild(pEmitter);
    }
    return pEmitter;
}

// 0x0049BBFC slot 0x00 | nintendogs:bytes
// 0x0049BBF8 (deleting dtor)
nw::gfx::ParticleEmitter::~ParticleEmitter()
{
    if (m_pParameters != nullptr) {
        GetAllocator()->Free(m_pParameters);
    }
    if (m_pVolume != nullptr) {
        GetAllocator()->Free(m_pVolume);
    }
}

} // namespace gfx
} // namespace nw
