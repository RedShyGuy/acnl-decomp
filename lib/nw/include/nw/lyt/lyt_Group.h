#pragma once

#include "decomp.h"
#include "nw/ut/ut_LinkList.h"

namespace nw {
namespace lyt {
class AnimTransform;
class Pane;

namespace res {
struct Group;
} // namespace res

// a pane of a group
struct PaneLink
{
    ut::LinkListNode m_Link;    // 0x0
    Pane* m_pTarget;            // 0x8
};
ASSERT_SIZE(PaneLink, 0xC);

// RTTI N2nw3lyt5GroupE @ 0x008D0874
// vtable 0x00902C10 (vptr 0x00902C18), offset_to_top 0, 2 entries
//
// A named set of panes of a layout (the panes of the names in the resource, found from the root
// pane). Member names are ours.
class Group
{
public:
    typedef ut::LinkList<PaneLink, 0> PaneLinkList;

    Group(const res::Group* pResGroup, Pane* pRootPane); // 0x004B8550 | nintendogs:bytes [tier A]
    virtual ~Group(); // 0x004B8698 slot 0x00 | nintendogs:bytes
    // 0x004B8640 (deleting dtor)

    // (inline)
    const char* GetName() const { return m_Name; }
    PaneLinkList& GetPaneList() { return m_PaneLinkList; }
    bool IsUserAllocated() const { return m_UserAllocated; }

    ut::LinkListNode m_Link;        // 0x04 in the GroupContainer
    PaneLinkList m_PaneLinkList;    // 0x0C
    char m_Name[17];                // 0x18
    bool m_UserAllocated;           // 0x29 not deleted with the container
};
ASSERT_SIZE(Group, 0x2C);

// the animation is bound to the panes of the group (name is ours)
void BindAnimation(Group* pGroup, AnimTransform* pAnimTrans, bool bRecursive, bool bDisable); // 0x004B5230 (name is ours)
} // namespace lyt
} // namespace nw
