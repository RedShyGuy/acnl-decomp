#pragma once

#include "decomp.h"

namespace nw {
namespace lyt {
class TexMap
{
public:
    void Update(); // 0x004BAB50 | libgarden [tier A]
    TexMap(nw::lyt::TextureInfo const&); // 0x004BACDC | libgarden [tier A]
    TexMap(); // 0x004BAD50 | nintendogs:bytes [tier A]
};
} // namespace lyt
} // namespace nw
