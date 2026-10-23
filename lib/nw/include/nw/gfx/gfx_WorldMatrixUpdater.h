#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector3.h"
#include "nw/gfx/gfx_GfxObject.h"

namespace nw {
namespace gfx {
class CalculatedTransform;

// RTTI N2nw3gfx18WorldMatrixUpdaterE @ 0x008D06DC
// vtable 0x00902784 (vptr 0x0090278C), offset_to_top 0, 2 entries
// The world matrix (without the own scale) and the world scale of a node from its local transform
// and the world and local transforms of its parent, by the scaling rule of the skeleton (the class
// name and the functions are from the reference symbols).
class WorldMatrixUpdater : public ::nw::gfx::GfxObject
{
public:
    class Builder;

    // (inline)
    explicit WorldMatrixUpdater(os::IAllocator* pAllocator) : GfxObject(pAllocator) {}
    virtual ~WorldMatrixUpdater();

    // Softimage: the scale of the parent applies to the translation only
    void CalculateWorldXsi(nn::math::MTX34* pWorldMatrix, nn::math::VEC3* pWorldScale, const CalculatedTransform& transform,
                           const CalculatedTransform& parentWorldTransform, const CalculatedTransform& parentTransform) const; // 0x0073A8AC | nintendogs:bytes-fuzzy [tier A]
    // the scale of the parent applies to the whole matrix
    void CalculateWorldBasic(nn::math::MTX34* pWorldMatrix, nn::math::VEC3* pWorldScale, const CalculatedTransform& transform,
                             const CalculatedTransform& parentWorldTransform, const CalculatedTransform& parentTransform) const; // 0x0073AA44 | nintendogs:bytes [tier A]
    // Maya with segment scale compensation: the scale of the parent applies to the translation only
    void CalculateWorldMayaSsc(nn::math::MTX34* pWorldMatrix, nn::math::VEC3* pWorldScale, const CalculatedTransform& transform,
                               const CalculatedTransform& parentWorldTransform, const CalculatedTransform& parentTransform) const; // 0x0073ADC0 | nintendogs:bytes [tier A]
};
ASSERT_SIZE(WorldMatrixUpdater, 0x8);
} // namespace gfx
} // namespace nw
