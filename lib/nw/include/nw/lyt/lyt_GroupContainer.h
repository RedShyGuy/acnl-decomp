#pragma once

#include "decomp.h"
#include "nw/lyt/lyt_Group.h"
#include "nw/ut/ut_LinkList.h"

namespace nw {
namespace lyt {
// The groups of a layout. Member names are ours.
class GroupContainer
{
public:
    typedef ut::LinkList<Group, 4> GroupList;

    // (inline)
    GroupContainer() {}
    // the groups are deleted (not the user allocated ones)
    ~GroupContainer(); // 0x004B56A0 | nintendogs:bytes [tier A]

    void AppendGroup(Group* pGroup); // 0x0013EEEC | libgarden [tier A]
    // NULL if there is none
    Group* FindGroupByName(const char* findName); // 0x004B5650 | nintendogs:bytes [tier A]

    GroupList m_GroupList; // 0x0
};
ASSERT_SIZE(GroupContainer, 0xC);
} // namespace lyt
} // namespace nw
