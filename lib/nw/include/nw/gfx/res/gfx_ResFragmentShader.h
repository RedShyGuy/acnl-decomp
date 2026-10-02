#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"

namespace nw {
namespace gfx {
namespace res {
class ResFragmentShader
{
public:
    void Cleanup(); // 0x0013F170 | nintendogs:bytes-fuzzy [tier A]
    void CheckFragmentShader(nw::gfx::res::ResMaterialData*); // 0x004A7984 | nintendogs:bytes [tier B]
    void Setup(nw::os::IAllocator*, nw::gfx::res::ResGraphicsFile, nw::gfx::res::ResMaterialData*); // 0x004A7BF4 | nintendogs:bytes [tier B]
};
} // namespace res
} // namespace gfx
} // namespace nw
