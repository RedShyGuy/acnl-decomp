#pragma once

#include "decomp.h"

namespace nw {
namespace gfx {
namespace res {
class ResTextureSampler
{
public:
    void SetTextureMipmapCommand(); // 0x004A7D08 | nintendogs:bytes [tier B]
    void GetOwnerCommand() const; // 0x0073C788 | nintendogs:bytes [tier A]
};
} // namespace res
} // namespace gfx
} // namespace nw
