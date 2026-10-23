#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_SceneObject.h"
#include "nw/gfx/res/gfx_ResSceneObject.h"
#include "nw/os/os_IAllocator.h"
#include "nw/os/os_MemorySizeCalculator.h"

namespace nw {
namespace gfx {
class Model;
class SceneNode;

// Builds the scene objects of a resource with their children, or adds up the memory they take
// (the class and function names are from the reference symbols, the members are ours). The
// settings go into the descriptions of the objects.
class SceneBuilder
{
public:
    // the object of m_Resource without its children (NULL: an unknown type or no memory)
    SceneObject* CreateObject(os::IAllocator* pAllocator, os::IAllocator* pDeviceAllocator); // 0x0048F670 | nintendogs:bytes [tier B]
    // the children of the resource: their memory or the objects under pParent
    void BuildChildren(os::MemorySizeCalculator* pSize, os::MemorySizeCalculator* pDeviceSize, SceneNode* pParent,
                       res::ResSceneObject resource, os::IAllocator* pAllocator, os::IAllocator* pDeviceAllocator,
                       bool isCalculatingSize) const; // 0x0073789C | nintendogs:bytes [tier A]
    // the object of the resource (with its children if isBuildingChildren), or with isCalculatingSize
    // only its memory (pSize) and device memory (pDeviceSize; each may be NULL)
    SceneObject* BuildSceneObject(os::MemorySizeCalculator* pSize, os::MemorySizeCalculator* pDeviceSize, SceneNode* pParent,
                                  res::ResSceneObject resource, os::IAllocator* pAllocator, os::IAllocator* pDeviceAllocator,
                                  bool isBuildingChildren, bool isCalculatingSize) const; // 0x007379B8 | nintendogs:callgraph [tier A]

    res::ResSceneObject m_Resource;          // 0x00
    bool m_IsFixedSizeMemory;                // 0x04
    s32 m_MaxChildren;                       // 0x08
    s32 m_MaxCallbacks;                      // 0x0C
    u32 m_BufferOption;                      // 0x10 of the models
    Model* m_pSharedMaterialModel;           // 0x14
    Model* m_pSharedMeshNodeVisibilityModel; // 0x18
    s32 m_MaxAnimObjectsPerGroup;            // 0x1C
    s32 m_ParticleSetMargin;                 // 0x20 particle sets a particle model takes beyond those of its resource
    bool m_IsAnimationEnabled;               // 0x24
};
ASSERT_OFFSET(SceneBuilder, m_IsAnimationEnabled, 0x24);
} // namespace gfx
} // namespace nw
