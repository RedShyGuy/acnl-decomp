#pragma once

#include "decomp.h"

namespace nw {
namespace gfx {
namespace res {
class ResLookupTable
{
public:
    void Cleanup(); // 0x0013F108 | nintendogs:callgraph [tier A]
    void Setup(); // 0x004A6B98 | nintendogs:bytes [tier A]
};
} // namespace res
} // namespace gfx
} // namespace nw
