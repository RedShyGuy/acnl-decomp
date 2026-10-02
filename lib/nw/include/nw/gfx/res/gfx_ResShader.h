#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"

namespace nw {
namespace gfx {
namespace res {
class ResShader
{
public:
    void Cleanup(); // 0x001378BC | nintendogs:bytes-fuzzy [tier A]
    void Dereference(); // 0x004AA6C8 | nintendogs:bytes [tier A]
    void Setup(nw::os::IAllocator*, nw::gfx::res::ResGraphicsFile); // 0x004AA708 | nintendogs:bytes [tier B]
};
} // namespace res
} // namespace gfx
} // namespace nw
