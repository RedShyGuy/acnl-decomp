#pragma once

#include "decomp.h"

namespace nw {
namespace lyt {
class GroupContainer
{
public:
    void AppendGroup(nw::lyt::Group*); // 0x0013EEEC | libgarden [tier A]
    void FindGroupByName(const char*); // 0x004B5650 | nintendogs:bytes [tier A]
    ~GroupContainer(); // 0x004B56A0 | nintendogs:bytes [tier A]
};
} // namespace lyt
} // namespace nw
