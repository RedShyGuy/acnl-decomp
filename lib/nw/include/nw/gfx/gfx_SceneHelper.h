#pragma once

#include "decomp.h"

namespace nw {
namespace gfx {
class SceneHelper
{
public:
    void CalculateDepth(const nn::math::VEC3&, const nn::math::MTX34&, const nw::gfx::Camera&); // 0x0048DA5C | nintendogs:bytes [tier B]
};
} // namespace gfx
} // namespace nw
