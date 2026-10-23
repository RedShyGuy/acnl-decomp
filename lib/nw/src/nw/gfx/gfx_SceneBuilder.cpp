#include "nw/gfx/gfx_SceneBuilder.h"

#include "nw/gfx/gfx_AmbientLight.h"
#include "nw/gfx/gfx_Camera.h"
#include "nw/gfx/gfx_Fog.h"
#include "nw/gfx/gfx_FragmentLight.h"
#include "nw/gfx/gfx_HemiSphereLight.h"
#include "nw/gfx/gfx_Model.h"
#include "nw/gfx/gfx_ParticleEmitter.h"
#include "nw/gfx/gfx_ParticleModel.h"
#include "nw/gfx/gfx_SceneEnvironmentSetting.h"
#include "nw/gfx/gfx_SceneNode.h"
#include "nw/gfx/gfx_SkeletalModel.h"
#include "nw/gfx/gfx_SkeletalModel_Builder.h"
#include "nw/gfx/gfx_TransformNode.h"
#include "nw/gfx/res/gfx_ResCamera.h"
#include "nw/gfx/res/gfx_ResFog.h"
#include "nw/gfx/res/gfx_ResLight.h"
#include "nw/gfx/res/gfx_ResModel.h"
#include "nw/gfx/res/gfx_ResParticle.h"
#include "nw/gfx/res/gfx_ResSceneEnvironmentSetting.h"

namespace nw {
namespace gfx {
namespace {
// the scene object types the builder knows (the types of the resources; names are ours)
const u32 TYPE_SCENE_NODE = 0x40000000;
const u32 TYPE_TRANSFORM_NODE = 0x40000002;

// (inline) the values of a description before the builder sets it
inline void SetDefaults(SceneNode::Description* pDescription)
{
    pDescription->isFixedSizeMemory = true;
    pDescription->isAnimationEnabled = true;
    pDescription->maxCallbacks = 4;
    pDescription->maxChildren = 8;
    pDescription->maxAnimObjectsPerGroup = 1;
}
inline void SetDefaults(Model::Description* pDescription)
{
    SetDefaults(static_cast<SceneNode::Description*>(pDescription));
    pDescription->bufferOption = 0;
    pDescription->pSharedMaterialModel = nullptr;
    pDescription->pSharedMeshNodeVisibilityModel = nullptr;
}
} // namespace

// 0x0048F670 | nintendogs:bytes [tier B]
nw::gfx::SceneObject* nw::gfx::SceneBuilder::CreateObject(os::IAllocator* pAllocator, os::IAllocator* pDeviceAllocator)
{
    return BuildSceneObject(nullptr, nullptr, nullptr, m_Resource, pAllocator, pDeviceAllocator, false, false);
}

// 0x0073789C | nintendogs:bytes [tier A]
DECOMP_NOINLINE void nw::gfx::SceneBuilder::BuildChildren(os::MemorySizeCalculator* pSize, os::MemorySizeCalculator* pDeviceSize, SceneNode* pParent,
                                          res::ResSceneObject resource, os::IAllocator* pAllocator, os::IAllocator* pDeviceAllocator,
                                          bool isCalculatingSize) const
{
    if (!resource.IsValid()) {
        return;
    }
    for (int i = 0; i < resource.ptr()->childCount; ++i) {
        if (isCalculatingSize) {
            // (the original passes the resource of the parent here, not the one of the child)
            BuildSceneObject(pSize, pDeviceSize, nullptr, resource, nullptr, nullptr, true, true);
        } else {
            res::ResSceneObject child(resource.ptr()->children.To<ut::Offset>()[i].ToPtr());
            BuildSceneObject(nullptr, nullptr, pParent, child, pAllocator, pDeviceAllocator, true, false);
        }
    }
}

// 0x007379B8 | nintendogs:callgraph [tier A]
nw::gfx::SceneObject* nw::gfx::SceneBuilder::BuildSceneObject(os::MemorySizeCalculator* pSize, os::MemorySizeCalculator* pDeviceSize,
                                                              SceneNode* pParent, res::ResSceneObject resource, os::IAllocator* pAllocator,
                                                              os::IAllocator* pDeviceAllocator, bool isBuildingChildren,
                                                              bool isCalculatingSize) const
{
    if (!resource.IsValid()) {
        return nullptr;
    }

    SceneObject* pObject = nullptr;
    switch (resource.ptr()->typeInfo) {
    case TYPE_SCENE_NODE: {
        SceneNode::Description description;
        SetDefaults(&description);
        description.isFixedSizeMemory = m_IsFixedSizeMemory;
        description.maxCallbacks = m_MaxCallbacks;
        description.maxChildren = m_MaxChildren;
        description.maxAnimObjectsPerGroup = m_MaxAnimObjectsPerGroup;
        description.isAnimationEnabled = m_IsAnimationEnabled;
        if (isCalculatingSize) {
            if (pSize != nullptr) {
                pSize->Add(sizeof(SceneNode));
                SceneNode::GetMemorySizeForInitialize(pSize, resource, description);
            }
        } else {
            pObject = SceneNode::Create(pParent, resource, description, pAllocator);
        }
        break;
    }
    case TYPE_TRANSFORM_NODE: {
        TransformNode::Description description;
        SetDefaults(&description);
        description.isFixedSizeMemory = m_IsFixedSizeMemory;
        description.maxCallbacks = m_MaxCallbacks;
        description.maxChildren = m_MaxChildren;
        description.maxAnimObjectsPerGroup = m_MaxAnimObjectsPerGroup;
        description.isAnimationEnabled = m_IsAnimationEnabled;
        if (isCalculatingSize) {
            if (pSize != nullptr) {
                pSize->Add(sizeof(TransformNode));
                TransformNode::GetMemorySizeForInitialize(pSize, res::ResTransformNode(resource.ptr()), description);
            }
        } else {
            pObject = TransformNode::Create(pParent, res::ResTransformNode(resource.ptr()), description, pAllocator);
        }
        break;
    }
    case res::ResModelData::TYPE: {
        Model::Description description;
        SetDefaults(&description);
        description.isFixedSizeMemory = m_IsFixedSizeMemory;
        description.maxCallbacks = m_MaxCallbacks;
        description.maxChildren = m_MaxChildren;
        description.bufferOption = m_BufferOption;
        description.pSharedMaterialModel = m_pSharedMaterialModel;
        description.pSharedMeshNodeVisibilityModel = m_pSharedMeshNodeVisibilityModel;
        description.isAnimationEnabled = m_IsAnimationEnabled;
        description.maxAnimObjectsPerGroup = m_MaxAnimObjectsPerGroup;
        if (isCalculatingSize) {
            if (pSize != nullptr) {
                pSize->Add(sizeof(Model));
                Model::GetMemorySizeForInitialize(pSize, res::ResModel(resource.ptr()), description);
            }
        } else {
            pObject = Model::Create(pParent, resource, description, pAllocator);
        }
        break;
    }
    case res::ResSkeletalModelData::TYPE: {
        SkeletalModel::Builder builder;
        SetDefaults(&builder.m_Description);
        builder.m_pSharedSkeleton = nullptr;
        builder.m_Description.isFixedSizeMemory = m_IsFixedSizeMemory;
        builder.m_Description.maxCallbacks = m_MaxCallbacks;
        builder.m_Description.maxChildren = m_MaxChildren;
        builder.m_Description.bufferOption = m_BufferOption;
        builder.m_Description.pSharedMaterialModel = m_pSharedMaterialModel;
        builder.m_Description.pSharedMeshNodeVisibilityModel = m_pSharedMeshNodeVisibilityModel;
        builder.m_Description.maxAnimObjectsPerGroup = m_MaxAnimObjectsPerGroup;
        builder.m_Description.isAnimationEnabled = m_IsAnimationEnabled;
        if (isCalculatingSize) {
            if (pSize != nullptr) {
                SkeletalModel::GetMemorySizeInternal(pSize, res::ResModel(resource.ptr()), builder.m_Description);
            }
        } else {
            pObject = builder.Create(pParent, resource, pAllocator);
        }
        break;
    }
    case res::ResParticleModelData::TYPE: {
        const res::ResParticleModelData* pResource =
            res::DynamicCastData<res::ResParticleModelData>(resource.ptr(), res::ResParticleModelData::TYPE);
        ParticleModel::Description description;
        SetDefaults(&description);
        description.isFixedSizeMemory = m_IsFixedSizeMemory;
        description.maxCallbacks = m_MaxCallbacks;
        description.bufferOption = m_BufferOption;
        description.pSharedMaterialModel = m_pSharedMaterialModel;
        description.pSharedMeshNodeVisibilityModel = m_pSharedMeshNodeVisibilityModel;
        // the particle sets are children of the model
        description.maxChildren = pResource->childCount + pResource->particleSets.count + m_ParticleSetMargin;
        description.maxParticleSets = pResource->particleSets.count + m_ParticleSetMargin;
        description.isAnimationEnabled = m_IsAnimationEnabled;
        if (isCalculatingSize) {
            if (pSize != nullptr) {
                ParticleModel::GetMemorySizeInternal(pSize, res::ResParticleModel(pResource), description);
            }
            if (pDeviceSize != nullptr) {
                ParticleModel::GetDeviceMemorySizeInternal(pDeviceSize, res::ResParticleModel(pResource), description);
            }
        } else {
            pObject = ParticleModel::Create(pParent, resource, description, pAllocator, pDeviceAllocator);
        }
        break;
    }
    case res::ResParticleEmitterData::TYPE: {
        ParticleEmitter::Description description;
        SetDefaults(&description);
        description.isFixedSizeMemory = m_IsFixedSizeMemory;
        description.maxCallbacks = m_MaxCallbacks;
        description.maxChildren = m_MaxChildren;
        description.isAnimationEnabled = m_IsAnimationEnabled;
        if (isCalculatingSize) {
            if (pSize != nullptr) {
                ParticleEmitter::GetMemorySizeInternal(pSize, res::ResTransformNode(resource.ptr()), description);
            }
        } else {
            pObject = ParticleEmitter::Create(pParent, resource, description, pAllocator);
        }
        break;
    }
    case res::ResFragmentLightData::TYPE: {
        FragmentLight::Description description;
        SetDefaults(&description);
        description.isFixedSizeMemory = m_IsFixedSizeMemory;
        description.maxCallbacks = m_MaxCallbacks;
        description.maxChildren = m_MaxChildren;
        description.maxAnimObjectsPerGroup = m_MaxAnimObjectsPerGroup;
        description.isAnimationEnabled = m_IsAnimationEnabled;
        if (isCalculatingSize) {
            if (pSize != nullptr) {
                FragmentLight::GetMemorySizeInternal(pSize, res::ResFragmentLight(resource.ptr()), description);
            }
        } else {
            pObject = FragmentLight::Create(pParent, resource, description, pAllocator);
        }
        break;
    }
    case res::ResAmbientLightData::TYPE: {
        AmbientLight::Description description;
        SetDefaults(&description);
        description.isFixedSizeMemory = m_IsFixedSizeMemory;
        description.maxCallbacks = m_MaxCallbacks;
        description.maxChildren = m_MaxChildren;
        description.maxAnimObjectsPerGroup = m_MaxAnimObjectsPerGroup;
        description.isAnimationEnabled = m_IsAnimationEnabled;
        if (isCalculatingSize) {
            if (pSize != nullptr) {
                AmbientLight::GetMemorySizeInternal(pSize, res::ResAmbientLight(resource.ptr()), description);
            }
        } else {
            pObject = AmbientLight::Create(pParent, resource, description, pAllocator);
        }
        break;
    }
    case res::ResHemiSphereLightData::TYPE: {
        HemiSphereLight::Description description;
        SetDefaults(&description);
        description.isFixedSizeMemory = m_IsFixedSizeMemory;
        description.maxCallbacks = m_MaxCallbacks;
        description.maxChildren = m_MaxChildren;
        description.maxAnimObjectsPerGroup = m_MaxAnimObjectsPerGroup;
        description.isAnimationEnabled = m_IsAnimationEnabled;
        if (isCalculatingSize) {
            if (pSize != nullptr) {
                HemiSphereLight::GetMemorySizeInternal(pSize, res::ResHemiSphereLight(resource.ptr()), description);
            }
        } else {
            pObject = HemiSphereLight::Create(pParent, resource, description, pAllocator);
        }
        break;
    }
    case res::ResFogData::TYPE: {
        Fog::Description description;
        SetDefaults(&description);
        description.isFixedSizeMemory = m_IsFixedSizeMemory;
        description.maxCallbacks = m_MaxCallbacks;
        description.maxChildren = m_MaxChildren;
        description.isAnimationEnabled = m_IsAnimationEnabled;
        if (isCalculatingSize) {
            if (pSize != nullptr) {
                Fog::GetMemorySizeInternal(pSize, res::ResFog(resource.ptr()), description);
            }
        } else {
            pObject = Fog::Create(pParent, resource, description, pAllocator);
        }
        break;
    }
    case res::ResCameraData::TYPE: {
        Camera::Description description;
        SetDefaults(&description);
        description.isFixedSizeMemory = m_IsFixedSizeMemory;
        description.maxCallbacks = m_MaxCallbacks;
        description.maxChildren = m_MaxChildren;
        description.maxAnimObjectsPerGroup = m_MaxAnimObjectsPerGroup;
        description.isAnimationEnabled = m_IsAnimationEnabled;
        if (isCalculatingSize) {
            if (pSize != nullptr) {
                Camera::GetMemorySizeInternal(pSize, res::ResCamera(resource.ptr()), description);
            }
        } else {
            pObject = Camera::Create(pParent, resource, description, pAllocator);
        }
        break;
    }
    case res::ResSceneEnvironmentSettingData::TYPE: {
        // (the original passes an uninitialized description)
        SceneEnvironmentSetting::Description description = {};
        if (isCalculatingSize) {
            if (pSize != nullptr) {
                SceneEnvironmentSetting::GetMemorySizeInternal(pSize, res::ResSceneEnvironmentSetting(resource.ptr()), description);
            }
        } else {
            pObject = SceneEnvironmentSetting::Create(resource, description, pAllocator);
        }
        break;
    }
    default:
        break;
    }

    if (isCalculatingSize) {
        if (isBuildingChildren) {
            BuildChildren(pSize, pDeviceSize, nullptr, resource, nullptr, nullptr, true);
        }
    } else {
        SceneNode* pNode = DynamicCast<SceneNode>(pObject);
        if (pNode != nullptr && isBuildingChildren) {
            BuildChildren(nullptr, nullptr, pNode, resource, pAllocator, pDeviceAllocator, false);
        }
    }
    return pObject;
}

} // namespace gfx
} // namespace nw
