#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"

namespace nw {
namespace gfx {
namespace res {
class ResTexture
{
public:
    void Cleanup(); // 0x001375CC | nintendogs:bytes-fuzzy [tier A]
    void Setup(nw::os::IAllocator*, nw::gfx::res::ResGraphicsFile); // 0x004A5AB8 | nintendogs:bytes [tier A]
};
} // namespace res
} // namespace gfx
} // namespace nw
