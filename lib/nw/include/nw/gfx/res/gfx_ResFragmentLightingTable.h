#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"

namespace nw {
namespace gfx {
namespace res {
class ResFragmentLightingTable
{
public:
    void Cleanup(); // 0x00140F80 | nintendogs:bytes-fuzzy [tier A]
    void Setup(nw::os::IAllocator*, nw::gfx::res::ResGraphicsFile); // 0x004A8874 | nintendogs:bytes [tier A]
};
} // namespace res
} // namespace gfx
} // namespace nw
