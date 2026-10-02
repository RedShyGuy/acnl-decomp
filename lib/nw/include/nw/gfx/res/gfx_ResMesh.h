#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResModel.h"

namespace nw {
namespace gfx {
namespace res {
class ResMesh
{
public:
    void Cleanup(); // 0x0013B920 | nintendogs:bytes [tier A]
    void Setup(nw::gfx::res::ResModel, nw::os::IAllocator*, nw::gfx::res::ResGraphicsFile); // 0x004A9E64 | nintendogs:callgraph [tier A]
};
} // namespace res
} // namespace gfx
} // namespace nw
