#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_GfxObject.h"

namespace nw {
namespace gfx {
class BillboardUpdater;
class Camera;
class Skeleton;
class WorldMatrixUpdater;

// RTTI N2nw3gfx15SkeletonUpdaterE @ 0x008D0664
// vtable 0x00902688 (vptr 0x00902690), offset_to_top 0, 2 entries
// Calculates the world transforms and matrices of the bones of a skeleton (the class name and
// UpdateWorld are from the reference symbols).
class SkeletonUpdater : public ::nw::gfx::GfxObject
{
public:
    class Builder;

    // (inline)
    explicit SkeletonUpdater(os::IAllocator* pAllocator) : GfxObject(pAllocator) {}
    virtual ~SkeletonUpdater();

    // the bones in their order (parents first), with the callbacks of the skeleton around each one
    void UpdateWorld(Skeleton* pSkeleton, const WorldMatrixUpdater& worldMatrixUpdater) const; // 0x00738E08 | mk7dlp:callseq
    // the matrices that depend on the camera: billboard bones turned to it, the skinning matrices of
    // the bones that have one (name is ours)
    void UpdateView(Skeleton* pSkeleton, const BillboardUpdater& billboardUpdater, const Camera& camera) const; // 0x00738BD8 (name is ours)
};
ASSERT_SIZE(SkeletonUpdater, 0x8);
} // namespace gfx
} // namespace nw
