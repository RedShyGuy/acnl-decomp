#pragma once

#include "decomp.h"

namespace nw {
namespace gfx {
namespace res {
class ResGraphicsFile
{
public:
    void Cleanup(); // 0x00131E5C | nintendogs:bytes [tier A]
    void Setup(nw::os::IAllocator*, nw::gfx::res::ResGraphicsFile); // 0x004A6D94 | nintendogs:callseq [tier A]
};
} // namespace res
} // namespace gfx
} // namespace nw
