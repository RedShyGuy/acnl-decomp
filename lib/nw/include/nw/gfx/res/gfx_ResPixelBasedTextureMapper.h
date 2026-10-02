#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResTexture.h"

namespace nw {
namespace gfx {
namespace res {
class ResPixelBasedTextureMapper
{
public:
    void ForceSetupTexture(nw::gfx::res::ResTexture); // 0x004A9630 | mk7dlp:bytes [tier B]
    void DestroyDynamic(); // 0x004A9CE0 | nintendogs:bytes [tier B]
};
} // namespace res
} // namespace gfx
} // namespace nw
