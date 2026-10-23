#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_BillboardUpdater.h"
#include "nw/gfx/gfx_ISceneUpdater.h"
#include "nw/gfx/gfx_SkeletonUpdater.h"
#include "nw/gfx/gfx_WorldMatrixUpdater.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx12SceneUpdaterE @ 0x008D05B0
// vtable 0x0090242C (vptr 0x00902434), offset_to_top 0, 9 entries
// The scene updater of NintendoWare with its skeleton, world matrix and billboard updaters (the
// class and function names are from the reference symbols where they have a source, member names
// are ours).
class SceneUpdater : public ::nw::gfx::ISceneUpdater
{
public:
    class Builder;

    // (inline)
    SceneUpdater(os::IAllocator* pAllocator, GfxPtr<SkeletonUpdater> skeletonUpdater, GfxPtr<WorldMatrixUpdater> worldMatrixUpdater,
                 GfxPtr<BillboardUpdater> billboardUpdater)
        : ISceneUpdater(pAllocator), m_SkeletonUpdater(skeletonUpdater), m_WorldMatrixUpdater(worldMatrixUpdater),
          m_BillboardUpdater(billboardUpdater), m_DepthSortMode(DEPTH_SORT_MODE_ALL)
    {
    }
    virtual ~SceneUpdater();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x007386CC slot 0x08
    virtual DepthSortMode GetDepthSortMode() const { return m_DepthSortMode; } // 0x007386C4 slot 0x0C (name is ours)
    virtual void SetDepthSortMode(DepthSortMode mode) { m_DepthSortMode = mode; } // 0x0049185C slot 0x10
    virtual void UpdateAll(SceneContext* pSceneContext); // 0x00491B38 slot 0x14 (name is ours)
    virtual void SubmitView(RenderQueue* pQueue, SceneContext* pSceneContext, const Camera& camera, u8 layer,
                            RenderSortMode renderSortMode); // 0x00491314 slot 0x18 | nintendogs:bytes
    virtual void SubmitView(RenderQueue* pQueue, SceneContext* pSceneContext, const Camera& camera, u8 layer, u8 particleLayer,
                            RenderSortMode renderSortMode); // 0x00491368 slot 0x1C (name is ours)
    virtual void SubmitView(RenderQueue* pQueue, SceneContext* pSceneContext, const Camera& camera, u8 layer, u8 particleLayer,
                            Model::IsVisibleModelFunctor* pIsVisibleModelFunctor, RenderSortMode renderSortMode); // 0x004913BC slot 0x20 (name is ours)

    static const TypeInfo s_TypeInfo;

    GfxPtr<SkeletonUpdater> m_SkeletonUpdater;       // 0x08
    GfxPtr<WorldMatrixUpdater> m_WorldMatrixUpdater; // 0x10
    GfxPtr<BillboardUpdater> m_BillboardUpdater;     // 0x18
    DepthSortMode m_DepthSortMode;                   // 0x20
};
ASSERT_SIZE(SceneUpdater, 0x24);
} // namespace gfx
} // namespace nw
