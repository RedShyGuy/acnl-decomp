#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"

namespace nw {
namespace anim {
namespace res {
class ResAnim
{
public:
    void Cleanup(); // 0x001385DC | nintendogs:bytes [tier A]
    void Setup(nw::os::IAllocator*, nw::gfx::res::ResGraphicsFile); // 0x004D6124 | nintendogs:bytes [tier A]
};
} // namespace res
} // namespace anim
} // namespace nw
