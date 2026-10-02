#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"

namespace nw {
namespace gfx {
namespace res {
class ResLight
{
public:
    void Cleanup(); // 0x001376F4 | nintendogs:bytes [tier A]
    void Setup(nw::os::IAllocator*, nw::gfx::res::ResGraphicsFile); // 0x004AA2E8 | nintendogs:bytes [tier A]
};
} // namespace res
} // namespace gfx
} // namespace nw
