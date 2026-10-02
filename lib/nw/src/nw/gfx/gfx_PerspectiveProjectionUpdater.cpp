#include "nw/gfx/gfx_CameraProjectionUpdater.h"
#include "nw/gfx/gfx_PerspectiveProjectionUpdater.h"

namespace nw {
namespace gfx {
// ctor candidate(s) 0x004A43A4, 0x004A44A0 (unverified)
nw::gfx::PerspectiveProjectionUpdater::PerspectiveProjectionUpdater()
{
}

// 0x004A4728 slot 0x00 | virtual slot, introduced by nw::gfx::PerspectiveProjectionUpdater
void nw::gfx::PerspectiveProjectionUpdater::vf_0x00()
{
}

// 0x004A46F4 slot 0x04 | virtual slot, introduced by nw::gfx::PerspectiveProjectionUpdater
void nw::gfx::PerspectiveProjectionUpdater::vf_0x04()
{
}

// 0x0073C540 slot 0x08 | virtual slot, introduced by nw::gfx::PerspectiveProjectionUpdater
void nw::gfx::PerspectiveProjectionUpdater::vf_0x08()
{
}

// 0x004A4508 slot 0x0C | nintendogs:bytes
void nw::gfx::PerspectiveProjectionUpdater::Update(nn::math::MTX44*, nn::math::MTX34*)
{
}

// 0x004A439C slot 0x10 | virtual slot, introduced by nw::gfx::PerspectiveProjectionUpdater
void nw::gfx::PerspectiveProjectionUpdater::vf_0x10()
{
}

// 0x0073C538 slot 0x14 | virtual slot, introduced by nw::gfx::PerspectiveProjectionUpdater
void nw::gfx::PerspectiveProjectionUpdater::vf_0x14()
{
}

// 0x0073C54C slot 0x18 | virtual slot, introduced by nw::gfx::PerspectiveProjectionUpdater
void nw::gfx::PerspectiveProjectionUpdater::vf_0x18()
{
}

// 0x004A43A4 | nintendogs:bytes [tier A]
void nw::gfx::PerspectiveProjectionUpdater::Create(nw::os::IAllocator*)
{
}

// 0x004A44A0 | nintendogs:callseq-callee [tier A]
void nw::gfx::PerspectiveProjectionUpdater::Create(nw::os::IAllocator*, nw::gfx::res::ResPerspectiveProjectionUpdater)
{
}

} // namespace gfx
} // namespace nw
