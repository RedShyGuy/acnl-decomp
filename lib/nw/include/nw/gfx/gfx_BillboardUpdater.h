#pragma once

#include "decomp.h"
#include "nn/math/math_Vector3.h"
#include "nw/gfx/gfx_GfxObject.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx16BillboardUpdaterE @ 0x008D067C
// vtable 0x009026AC (vptr 0x009026B4), offset_to_top 0, 2 entries
class BillboardUpdater : public ::nw::gfx::GfxObject
{
public:
    BillboardUpdater(); // ctor candidate(s) 0x0049BF3C (unverified)
    virtual void vf_0x00(); // 0x0049BF78 slot 0x00 | virtual slot, introduced by nw::gfx::BillboardUpdater
    virtual void vf_0x04(); // 0x0049BF74 slot 0x04 | virtual slot, introduced by nw::gfx::BillboardUpdater
    void Create(nw::os::IAllocator*); // 0x0049BF3C | nintendogs:callgraph [tier A]
    void CalculateLocalMatrix(nn::math::MTX34*, const nw::gfx::CalculatedTransform&, nn::math::VEC3, bool) const; // 0x00739568 | nintendogs:bytes [tier B]
    void CalculateScreenLocalMatrix(nn::math::MTX34*, const nw::gfx::CalculatedTransform&, const nn::math::MTX34&, nn::math::VEC3, nn::math::VEC3&) const; // 0x00739780 | nintendogs:bytes [tier B]
    void Update(nn::math::MTX34*, const nn::math::MTX34&, const nn::math::MTX34&, const nn::math::VEC3&, const nw::gfx::CalculatedTransform&, const nw::gfx::CalculatedTransform&, nw::gfx::res::ResBone::BillboardMode) const; // 0x007399A8 | nintendogs:bytes-fuzzy [tier B]
};
} // namespace gfx
} // namespace nw
