#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_SkeletalModel.h"
#include "nw/gfx/res/gfx_ResSceneObject.h"

// The settings of a skeletal model (the class name and Create are from the reference symbols, the
// members are ours).
class nw::gfx::SkeletalModel::Builder
{
public:
    // a skeletal model under the parent: with the skeleton of another model, or a new one (NULL: no memory)
    SkeletalModel* Create(SceneNode* pParent, res::ResSceneObject resource, os::IAllocator* pAllocator); // 0x00497C5C | nintendogs:callseq [tier A]

    Model::Description m_Description;  // 0x00
    Skeleton* m_pSharedSkeleton;       // 0x1C (NULL: a skeleton of its own)
};
ASSERT_SIZE(nw::gfx::SkeletalModel::Builder, 0x20);
