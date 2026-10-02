#include "nw/gfx/gfx_GfxObject.h"
#include "nw/gfx/gfx_BillboardUpdater.h"

namespace nw {
namespace gfx {
// ctor candidate(s) 0x0049BF3C (unverified)
nw::gfx::BillboardUpdater::BillboardUpdater()
{
}

// 0x0049BF78 slot 0x00 | virtual slot, introduced by nw::gfx::BillboardUpdater
void nw::gfx::BillboardUpdater::vf_0x00()
{
}

// 0x0049BF74 slot 0x04 | virtual slot, introduced by nw::gfx::BillboardUpdater
void nw::gfx::BillboardUpdater::vf_0x04()
{
}

// 0x0049BF3C | nintendogs:callgraph [tier A]
void nw::gfx::BillboardUpdater::Create(nw::os::IAllocator*)
{
}

// 0x00739568 | nintendogs:bytes [tier B]
void nw::gfx::BillboardUpdater::CalculateLocalMatrix(nn::math::MTX34*, const nw::gfx::CalculatedTransform&, nn::math::VEC3, bool) const
{
}

// 0x00739780 | nintendogs:bytes [tier B]
void nw::gfx::BillboardUpdater::CalculateScreenLocalMatrix(nn::math::MTX34*, const nw::gfx::CalculatedTransform&, const nn::math::MTX34&, nn::math::VEC3, nn::math::VEC3&) const
{
}

// 0x007399A8 | nintendogs:bytes-fuzzy [tier B]
void nw::gfx::BillboardUpdater::Update(nn::math::MTX34*, const nn::math::MTX34&, const nn::math::MTX34&, const nn::math::VEC3&, const nw::gfx::CalculatedTransform&, const nw::gfx::CalculatedTransform&, nw::gfx::res::ResBone::BillboardMode) const
{
}

} // namespace gfx
} // namespace nw
