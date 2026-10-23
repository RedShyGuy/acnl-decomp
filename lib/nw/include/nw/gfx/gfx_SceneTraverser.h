#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_ISceneVisitor.h"

namespace nw {
namespace gfx {
class SceneContext;

// RTTI N2nw3gfx14SceneTraverserE @ 0x008D0640
// vtable 0x009025CC (vptr 0x009025D4), offset_to_top 0, 17 entries
// Collects the nodes it visits into the arrays of a scene context (the class name, Begin and the
// Visit names with sources are from the reference symbols, the rest is ours).
class SceneTraverser : public ::nw::gfx::ISceneVisitor
{
public:
    class Builder;

    // (inline)
    explicit SceneTraverser(os::IAllocator* pAllocator) : ISceneVisitor(pAllocator), m_pSceneContext(nullptr) {}
    virtual ~SceneTraverser();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x00738BA4 slot 0x08
    virtual void VisitSceneNode(SceneNode* pNode); // 0x00499CCC slot 0x0C (name is ours)
    virtual void VisitTransformNode(TransformNode* pNode); // 0x0049A3A0 slot 0x10 (name is ours)
    virtual void VisitUnknown0x14(SceneNode* pNode); // 0x0049A3BC slot 0x14 (name is ours)
    virtual void VisitModel(Model* pModel); // 0x00499B20 slot 0x18 | nintendogs:bytes
    virtual void VisitSkeletalModel(SkeletalModel* pModel); // 0x0049A240 slot 0x1C (name is ours)
    virtual void VisitCamera(Camera* pCamera); // 0x00499B80 slot 0x20 (name is ours)
    virtual void VisitFog(Fog* pFog); // 0x0049A838 slot 0x24 (name is ours)
    virtual void VisitLight(Light* pLight); // 0x00499AC0 slot 0x28 | nintendogs:bytes
    virtual void VisitFragmentLight(FragmentLight* pLight); // 0x00499F80 slot 0x2C (name is ours)
    virtual void VisitAmbientLight(AmbientLight* pLight); // 0x00499E20 slot 0x30 (name is ours)
    virtual void VisitHemiSphereLight(HemiSphereLight* pLight); // 0x0049A4E4 slot 0x34 (name is ours)
    virtual void VisitParticleSet(ParticleSet* pParticleSet); // 0x00499CE8 slot 0x38 (name is ours)
    virtual void VisitParticleEmitter(ParticleEmitter* pEmitter); // 0x0049A644 slot 0x3C (name is ours)
    virtual void VisitParticleModel(ParticleModel* pModel); // 0x0049A0E0 slot 0x40 (name is ours)

    // the context emptied; the visited nodes are added to it
    void Begin(SceneContext* pSceneContext); // 0x0049A778 | nintendogs:bytes [tier B]
    void End(); // 0x0049A76C (name is ours)

    static const TypeInfo s_TypeInfo;

    SceneContext* m_pSceneContext; // 0x8
};
ASSERT_SIZE(SceneTraverser, 0xC);
} // namespace gfx
} // namespace nw
