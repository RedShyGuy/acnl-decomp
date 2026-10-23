#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector3.h"
#include "nw/gfx/gfx_GfxObject.h"
#include "nw/gfx/res/gfx_ResSkeleton.h"

namespace nw {
namespace gfx {
class CalculatedTransform;

// RTTI N2nw3gfx16BillboardUpdaterE @ 0x008D067C
// vtable 0x009026AC (vptr 0x009026B4), offset_to_top 0, 2 entries
// The local matrix of a billboard bone: its axes turned to the camera (the class name and the
// functions are from the reference symbols, parameter names are ours).
class BillboardUpdater : public ::nw::gfx::GfxObject
{
public:
    // (inline)
    explicit BillboardUpdater(os::IAllocator* pAllocator) : GfxObject(pAllocator) {}
    virtual ~BillboardUpdater();

    // NULL: no memory
    static BillboardUpdater* Create(os::IAllocator* pAllocator); // 0x0049BF3C | nintendogs:callgraph [tier A]
    // the axes with z along the direction (isZFixed: y follows, else z follows the y axis of the
    // transform), the translation of the transform
    void CalculateLocalMatrix(nn::math::MTX34* pMatrix, const CalculatedTransform& transform, nn::math::VEC3 direction,
                              bool isZFixed) const; // 0x00739568 | nintendogs:bytes [tier B]
    // the axes in view space (z along the direction, y from the up vector) turned back by the
    // inverse view matrix
    void CalculateScreenLocalMatrix(nn::math::MTX34* pMatrix, const CalculatedTransform& transform, const nn::math::MTX34& inverseViewMatrix,
                                    nn::math::VEC3 up, nn::math::VEC3& direction) const; // 0x00739780 | nintendogs:bytes [tier B]
    void Update(nn::math::MTX34* pMatrix, const nn::math::MTX34& viewMatrix, const nn::math::MTX34& inverseViewMatrix,
                const nn::math::VEC3& cameraPosition, const CalculatedTransform& worldTransform, const CalculatedTransform& localTransform,
                res::ResBone::BillboardMode mode) const; // 0x007399A8 | nintendogs:bytes-fuzzy [tier B]
};
ASSERT_SIZE(BillboardUpdater, 0x8);
} // namespace gfx
} // namespace nw
