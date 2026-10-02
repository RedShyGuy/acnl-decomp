#include "nw/gfx/gfx_GraphicsDevice.h"

namespace nw {
namespace gfx {
// 0x004998A4 | nintendogs:bytes [tier A]
void nw::gfx::GraphicsDevice::ActivateLookupTable(nw::gfx::res::ResImageLookupTable, nw::gfx::GraphicsDevice::LutTarget)
{
}

// 0x0049993C | nintendogs:bytes [tier A]
void nw::gfx::GraphicsDevice::InvalidateLookupTable(nw::gfx::res::ResImageLookupTable)
{
}

// 0x0049996C | nintendogs:bytes [tier A]
void nw::gfx::GraphicsDevice::InvalidateAllLookupTables()
{
}

// 0x00499994 | nintendogs:bytes [tier A]
void nw::gfx::GraphicsDevice::ActivateFragmentLightPosition(int, const nn::math::VEC4&)
{
}

} // namespace gfx
} // namespace nw
