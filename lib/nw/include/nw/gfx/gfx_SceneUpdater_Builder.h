#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_SceneUpdater.h"

// The updaters of a scene updater; the missing ones are created (the class name is from the
// reference symbols, member names are ours).
class nw::gfx::SceneUpdater::Builder
{
public:
    // NULL: no memory (the updaters are moved into the scene updater)
    SceneUpdater* Create(nw::os::IAllocator* pAllocator); // 0x00491864 | nintendogs:bytes [tier A]

    GfxPtr<SkeletonUpdater> m_SkeletonUpdater;       // 0x00
    GfxPtr<WorldMatrixUpdater> m_WorldMatrixUpdater; // 0x08
    GfxPtr<BillboardUpdater> m_BillboardUpdater;     // 0x10
};
ASSERT_SIZE(nw::gfx::SceneUpdater::Builder, 0x18);
