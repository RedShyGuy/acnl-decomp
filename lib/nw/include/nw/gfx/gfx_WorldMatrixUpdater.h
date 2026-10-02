#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_GfxObject.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx18WorldMatrixUpdaterE @ 0x008D06DC
// vtable 0x00902784 (vptr 0x0090278C), offset_to_top 0, 2 entries
class WorldMatrixUpdater : public ::nw::gfx::GfxObject
{
public:
    class Builder;
    WorldMatrixUpdater(); // ctor candidate(s) 0x0049FA6C (unverified)
    virtual void vf_0x00(); // 0x0049FAA8 slot 0x00 | virtual slot, introduced by nw::gfx::WorldMatrixUpdater
    virtual void vf_0x04(); // 0x0049FAA4 slot 0x04 | virtual slot, introduced by nw::gfx::WorldMatrixUpdater
    void CalculateWorldXsi(nn::math::MTX34*, nn::math::VEC3*, const nw::gfx::CalculatedTransform&, const nw::gfx::CalculatedTransform&, const nw::gfx::CalculatedTransform&) const; // 0x0073A8AC | nintendogs:bytes-fuzzy [tier A]
    void CalculateWorldBasic(nn::math::MTX34*, nn::math::VEC3*, const nw::gfx::CalculatedTransform&, const nw::gfx::CalculatedTransform&, const nw::gfx::CalculatedTransform&) const; // 0x0073AA44 | nintendogs:bytes [tier A]
    void CalculateWorldMayaSsc(nn::math::MTX34*, nn::math::VEC3*, const nw::gfx::CalculatedTransform&, const nw::gfx::CalculatedTransform&, const nw::gfx::CalculatedTransform&) const; // 0x0073ADC0 | nintendogs:bytes [tier A]
};
} // namespace gfx
} // namespace nw
