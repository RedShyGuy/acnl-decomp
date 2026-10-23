#include "nw/gfx/gfx_ParticleModel.h"

#include <new>

#include "nw/gfx/gfx_ParticleSet.h"
#include "nw/gfx/gfx_ParticleShape.h"

namespace nw {
namespace gfx {
// 0x0097F488
const TypeInfo nw::gfx::ParticleModel::s_TypeInfo(&Model::s_TypeInfo);

// sets s_TypeInfo
// 0x0079C1E8 (static initializer)

// 0x0049368C slot 0x24 | slot vf_0x24 of nw::gfx::SceneNode
u32 nw::gfx::ParticleModel::Initialize(os::IAllocator* pAllocator)
{
    u32 result = RESULT_SUCCESS;
    result |= Model::Initialize(pAllocator);
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }

    // the room for the sets and the shapes, all NULL
    if (m_MaxParticleSets == 0) {
        return result;
    }
    void* pBuffer = pAllocator->Alloc(m_MaxParticleSets * sizeof(ParticleSet*), 4);
    if (pBuffer == nullptr) {
        result |= RESULT_FAILURE_MASK;
    }
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }
    {
        ut::MoveArray<ParticleSet*> particleSets(pBuffer, m_MaxParticleSets, pAllocator);
        m_ParticleSets = particleSets;
    }
    m_ParticleSets.Resize(m_MaxParticleSets);
    for (int i = 0; i < m_ParticleSets.size(); ++i) {
        m_ParticleSets[i] = nullptr;
    }

    if (m_MaxParticleSets == 0) {
        return result;
    }
    pBuffer = pAllocator->Alloc(m_MaxParticleSets * sizeof(ParticleShape*), 4);
    if (pBuffer == nullptr) {
        result |= RESULT_FAILURE_MASK;
    }
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }
    {
        ut::MoveArray<ParticleShape*> particleShapes(pBuffer, m_MaxParticleSets, pAllocator);
        m_ParticleShapes = particleShapes;
    }
    m_ParticleShapes.Resize(m_MaxParticleSets);
    for (int i = 0; i < m_ParticleShapes.size(); ++i) {
        m_ParticleShapes[i] = nullptr;
    }
    return result;
}

// 0x00493AE0 | nintendogs:bytes [tier A]
void nw::gfx::ParticleModel::GetMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResParticleModel resource, const Description& description)
{
    pSize->Add(sizeof(ParticleModel));
    GetMemorySizeForInitialize(pSize, resource, description);
    const int count = resource.ptr()->particleSets.count;
    for (int i = 0; i < count; ++i) {
        res::ResParticleSet resSet(resource.ptr()->particleSets.GetData(i));
        ParticleShape::GetMemorySizeInternal(pSize, resSet.GetCollection().ptr()->capacity);
        ParticleSet::GetMemorySizeInternal(pSize, resSet, ParticleSet::Description());
    }
}

// 0x00493BD0 | nintendogs:callgraph [tier A]
void nw::gfx::ParticleModel::GetMemorySizeForInitialize(os::MemorySizeCalculator* pSize, res::ResParticleModel resource,
                                                        const Description& description)
{
    Model::GetMemorySizeForInitialize(pSize, resource, description);
    pSize->Add(description.maxParticleSets * sizeof(ParticleSet*));
    pSize->Add(description.maxParticleSets * sizeof(ParticleShape*));
}

// 0x004940AC | nintendogs:bytes [tier A]
void nw::gfx::ParticleModel::GetDeviceMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResParticleModel resource,
                                                         const Description& /*description*/)
{
    const int count = resource.ptr()->particleSets.count;
    for (int i = 0; i < count; ++i) {
        res::ResParticleSet resSet(resource.ptr()->particleSets.GetData(i));
        ParticleShape::GetDeviceMemorySizeInternal(pSize, resSet.GetCollection().ptr()->capacity);
        ParticleSet::GetDeviceMemorySizeInternal(pSize, resSet);
    }
}

// 0x00494148 slot 0x10 | nintendogs:bytes
void nw::gfx::ParticleModel::Accept(ISceneVisitor* pVisitor)
{
    pVisitor->VisitParticleModel(this);
    for (SceneNode** ppChild = m_Children.begin(); ppChild != m_Children.end(); ++ppChild) {
        (*ppChild)->Accept(pVisitor);
    }
}

// 0x0049419C | nintendogs:callseq [tier A]
nw::gfx::ParticleModel* nw::gfx::ParticleModel::Create(SceneNode* pParent, res::ResSceneObject resObject, const Description& description,
                                                       os::IAllocator* pAllocator, os::IAllocator* pDeviceAllocator)
{
    const res::ResParticleModelData* pResource = res::DynamicCastData<res::ResParticleModelData>(resObject.ptr(), res::ResParticleModelData::TYPE);
    void* pMemory = pAllocator->Alloc(sizeof(ParticleModel), 4);
    if (pMemory == nullptr) {
        return nullptr;
    }
    ParticleModel* pModel = new (pMemory) ParticleModel(pAllocator, res::ResParticleModel(pResource), description);
    if (pModel->Initialize(pAllocator) & RESULT_FAILURE_MASK) {
        SafeDestroy(pModel);
        return nullptr;
    }

    // a shape and a set per set of the resource, the sets children of the model
    bool isSuccess = true;
    for (int i = 0; i < pResource->particleSets.count; ++i) {
        res::ResParticleSet resSet(pResource->particleSets.GetData(i));
        res::ResSceneObject resShape(
            res::DynamicCastData<res::ResParticleShapeData>(res::ResModel(pResource).GetShape(i), res::ResParticleShapeData::TYPE));
        ParticleShape* pShape = ParticleShape::Create(resShape, resSet.GetCollection().ptr()->capacity, pAllocator, pDeviceAllocator);
        if (pShape == nullptr) {
            isSuccess = false;
            break;
        }
        if (pModel->m_ParticleShapeCount < pModel->m_ParticleShapes.size()) {
            pModel->m_ParticleShapes[pModel->m_ParticleSetCount] = pShape;
            ++pModel->m_ParticleShapeCount;
        }
        ParticleSet* pSet = ParticleSet::Create(pModel, resSet, ParticleSet::Description(), pAllocator, pDeviceAllocator, pShape);
        if (pSet == nullptr) {
            isSuccess = false;
            break;
        }
        if (pModel->m_ParticleSetCount < pModel->m_ParticleSets.size()) {
            pModel->m_ParticleSets[pModel->m_ParticleSetCount] = pSet;
            ++pModel->m_ParticleSetCount;
        }
    }
    if (!isSuccess) {
        for (u32 i = 0; i < pModel->m_MaxParticleSets; ++i) {
            SafeDestroy(pModel->m_ParticleSets[i]);
            pModel->m_ParticleSets[i] = nullptr;
        }
        SafeDestroy(pModel);
        return nullptr;
    }

    if (pParent != nullptr) {
        pParent->AttachChild(pModel);
    }
    return pModel;
}

// 0x004947E4 slot 0x00 | nintendogs:bytes
// 0x004947D8 (deleting dtor)
nw::gfx::ParticleModel::~ParticleModel()
{
    for (int i = 0; i < m_ParticleShapes.size(); ++i) {
        SafeDestroy(m_ParticleShapes[i]);
    }
}

} // namespace gfx
} // namespace nw
