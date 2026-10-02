#pragma once

#include "decomp.h"

namespace nw {
namespace gfx {
class SceneEnvironment
{
public:
    void SetActiveLightSet(int); // 0x0049C040 | nintendogs:bytes [tier B]
    void Reset(); // 0x0049C138 | nintendogs:bytes [tier B]
};
} // namespace gfx
} // namespace nw
