#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"

namespace nw {
namespace gfx {
namespace res {
class ResLookupTableSet
{
public:
    void Cleanup(); // 0x00137638 | nintendogs:bytes [tier A]
    void Setup(nw::os::IAllocator*, nw::gfx::res::ResGraphicsFile); // 0x004A7C5C | nintendogs:bytes [tier A]
};
} // namespace res
} // namespace gfx
} // namespace nw
