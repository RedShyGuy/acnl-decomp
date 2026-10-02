#include "nw/gfx/gfx_CameraProjectionUpdater.h"
#include "nw/gfx/gfx_OrthoProjectionUpdater.h"

namespace nw {
namespace gfx {
// ctor candidate(s) 0x004A0E08, 0x004A0F24 (unverified)
nw::gfx::OrthoProjectionUpdater::OrthoProjectionUpdater()
{
}

// 0x004A11BC slot 0x00 | virtual slot, introduced by nw::gfx::OrthoProjectionUpdater
void nw::gfx::OrthoProjectionUpdater::vf_0x00()
{
}

// 0x004A1188 slot 0x04 | virtual slot, introduced by nw::gfx::OrthoProjectionUpdater
void nw::gfx::OrthoProjectionUpdater::vf_0x04()
{
}

// 0x0073B9DC slot 0x08 | virtual slot, introduced by nw::gfx::OrthoProjectionUpdater
void nw::gfx::OrthoProjectionUpdater::vf_0x08()
{
}

// 0x004A0F8C slot 0x0C | nintendogs:bytes
void nw::gfx::OrthoProjectionUpdater::Update(nn::math::MTX44*, nn::math::MTX34*)
{
}

// 0x004A0E00 slot 0x10 | virtual slot, introduced by nw::gfx::OrthoProjectionUpdater
void nw::gfx::OrthoProjectionUpdater::vf_0x10()
{
}

// 0x0073B9D4 slot 0x14 | virtual slot, introduced by nw::gfx::OrthoProjectionUpdater
void nw::gfx::OrthoProjectionUpdater::vf_0x14()
{
}

// 0x0073B9E8 slot 0x18 | virtual slot, introduced by nw::gfx::OrthoProjectionUpdater
void nw::gfx::OrthoProjectionUpdater::vf_0x18()
{
}

// 0x004A0E08 | nintendogs:bytes [tier B]
void nw::gfx::OrthoProjectionUpdater::Create(nw::os::IAllocator*)
{
}

// 0x004A0F24 | nintendogs:callseq-callee [tier A]
void nw::gfx::OrthoProjectionUpdater::Create(nw::os::IAllocator*, nw::gfx::res::ResOrthoProjectionUpdater)
{
}

} // namespace gfx
} // namespace nw
