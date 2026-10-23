#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_BasicRenderKeyFactory.h"
#include "nw/gfx/gfx_BasicRenderQueue.h"
#include "nw/gfx/gfx_GfxObject.h"
#include "nw/gfx/gfx_Model.h"
#include "nw/ut/ut_MoveArray.h"

namespace nw {
namespace gfx {
class Camera;
class SceneContext;

// the render queue of the scene updaters (names after the reference symbols)
typedef BasicRenderElement<u64> RenderElement;
typedef BasicRenderKeyFactory<u64> RenderKeyFactory;
typedef BasicRenderQueue<RenderElement, ut::MoveArray<RenderElement>, RenderKeyFactory> RenderQueue;

// RTTI N2nw3gfx13ISceneUpdaterE @ 0x008D05E0
// Updates the nodes of a scene context and puts the meshes of its models into render queues (the
// class, enum and SubmitView/SetDepthSortMode names are from the reference symbols, the rest is ours).
class ISceneUpdater : public ::nw::gfx::GfxObject
{
public:
    // which meshes are sorted by their own depth
    enum DepthSortMode : u8
    {
        DEPTH_SORT_MODE_ALL = 0,              // every mesh
        DEPTH_SORT_MODE_TRANSLUCENT_ONLY = 1, // only translucent meshes (the others at depth 0)
    };
    // how the meshes are sorted
    enum RenderSortMode : u8
    {
        RENDER_SORT_MODE_MESH = 0,
        // the meshes that are not opaque at the depth of their model (they stay together)
        RENDER_SORT_MODE_TRANSLUCENT_BY_MODEL = 1,
    };

    // (inline)
    explicit ISceneUpdater(os::IAllocator* pAllocator) : GfxObject(pAllocator) {}
    virtual ~ISceneUpdater() {}

    virtual const TypeInfo* GetRuntimeTypeInfo() const = 0; // slot 0x08
    virtual DepthSortMode GetDepthSortMode() const = 0; // slot 0x0C
    virtual void SetDepthSortMode(DepthSortMode mode) = 0; // slot 0x10
    // the animations, callbacks and world matrices of the nodes of the context
    virtual void UpdateAll(SceneContext* pSceneContext) = 0; // slot 0x14
    // the visible meshes of the models of the context for the camera into the queue (the models of
    // particles too at the layer)
    virtual void SubmitView(RenderQueue* pQueue, SceneContext* pSceneContext, const Camera& camera, u8 layer, RenderSortMode renderSortMode) = 0; // slot 0x18
    virtual void SubmitView(RenderQueue* pQueue, SceneContext* pSceneContext, const Camera& camera, u8 layer, u8 particleLayer,
                            RenderSortMode renderSortMode) = 0; // slot 0x1C
    virtual void SubmitView(RenderQueue* pQueue, SceneContext* pSceneContext, const Camera& camera, u8 layer, u8 particleLayer,
                            Model::IsVisibleModelFunctor* pIsVisibleModelFunctor, RenderSortMode renderSortMode) = 0; // slot 0x20

    static const TypeInfo s_TypeInfo;
};
} // namespace gfx
} // namespace nw
