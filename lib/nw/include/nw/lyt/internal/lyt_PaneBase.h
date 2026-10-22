#pragma once

#include "decomp.h"
#include "nw/ut/ut_LinkList.h"

namespace nw {
namespace lyt {
namespace internal {
// RTTI N2nw3lyt8internal8PaneBaseE @ 0x008D08D0
// The link of a pane in the child list of its parent. Constructor and destructor are inline
// (the binary has no vtable of its own); member names are ours.
class PaneBase
{
public:
    // (inline)
    PaneBase() {}
    // (inline)
    virtual ~PaneBase() {}

    ut::LinkListNode m_Link; // 0x4
};
ASSERT_SIZE(PaneBase, 0xC);
} // namespace internal
} // namespace lyt
} // namespace nw
