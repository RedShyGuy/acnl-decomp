#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_GfxObject.h"

namespace nw {
namespace gfx {
class SceneNode;
class TransformNode;
class Model;
class SkeletalModel;
class Camera;
class Fog;
class Light;
class FragmentLight;
class AmbientLight;
class HemiSphereLight;
class ParticleSet;
class ParticleEmitter;
class ParticleModel;

// RTTI N2nw3gfx13ISceneVisitorE @ 0x008D05EC
// Visits the nodes of a scene (SceneNode::Accept); one function per node class (names after the
// reference symbols of SceneTraverser, the others are ours).
class ISceneVisitor : public ::nw::gfx::GfxObject
{
public:
    // (inline)
    explicit ISceneVisitor(os::IAllocator* pAllocator) : GfxObject(pAllocator) {}
    virtual ~ISceneVisitor() {}

    virtual const TypeInfo* GetRuntimeTypeInfo() const = 0; // slot 0x08
    virtual void VisitSceneNode(SceneNode* pNode) = 0; // slot 0x0C (name is ours)
    virtual void VisitTransformNode(TransformNode* pNode) = 0; // slot 0x10
    // (the node class of this slot is not known yet)
    virtual void VisitUnknown0x14(SceneNode* pNode) = 0; // slot 0x14
    virtual void VisitModel(Model* pModel) = 0; // slot 0x18
    virtual void VisitSkeletalModel(SkeletalModel* pModel) = 0; // slot 0x1C
    virtual void VisitCamera(Camera* pCamera) = 0; // slot 0x20
    virtual void VisitFog(Fog* pFog) = 0; // slot 0x24
    virtual void VisitLight(Light* pLight) = 0; // slot 0x28
    virtual void VisitFragmentLight(FragmentLight* pLight) = 0; // slot 0x2C
    virtual void VisitAmbientLight(AmbientLight* pLight) = 0; // slot 0x30
    virtual void VisitHemiSphereLight(HemiSphereLight* pLight) = 0; // slot 0x34
    virtual void VisitParticleSet(ParticleSet* pParticleSet) = 0; // slot 0x38 (name is ours)
    virtual void VisitParticleEmitter(ParticleEmitter* pEmitter) = 0; // slot 0x3C (name is ours)
    virtual void VisitParticleModel(ParticleModel* pModel) = 0; // slot 0x40 (name is ours)

    static const TypeInfo s_TypeInfo;
};
} // namespace gfx
} // namespace nw
