#pragma once

#include "decomp.h"

namespace nw {
namespace gfx {
class GraphicsDevice
{
public:
    struct LutTarget { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void ActivateLookupTable(nw::gfx::res::ResImageLookupTable, nw::gfx::GraphicsDevice::LutTarget); // 0x004998A4 | nintendogs:bytes [tier A]
    void InvalidateLookupTable(nw::gfx::res::ResImageLookupTable); // 0x0049993C | nintendogs:bytes [tier A]
    void InvalidateAllLookupTables(); // 0x0049996C | nintendogs:bytes [tier A]
    void ActivateFragmentLightPosition(int, const nn::math::VEC4&); // 0x00499994 | nintendogs:bytes [tier A]
};
} // namespace gfx
} // namespace nw
