#pragma once

#include "decomp.h"

namespace nw {
namespace gfx {
// the model of the name in a graphics file (the game's helper; NULL: none)
const void* GetModel(const void* const* ppGraphicsFile, const char* name); // 0x004A7240 | libgarden [tier A]
} // namespace gfx
} // namespace nw
