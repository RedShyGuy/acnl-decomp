#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"

namespace nw {
namespace gfx {
namespace res {
class ResFog
{
public:
    void Cleanup(); // 0x001376C8 | nintendogs:bytes [tier A]
    void Setup(nw::os::IAllocator*, nw::gfx::res::ResGraphicsFile); // 0x004A9DEC | nintendogs:bytes [tier A]
};
} // namespace res
} // namespace gfx
} // namespace nw
