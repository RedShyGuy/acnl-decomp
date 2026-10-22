#include "nw/lyt/lyt_GroupContainer.h"

#include "nw/lyt/internal/internal_Api.h"
#include "nw/lyt/lyt_Layout.h"

namespace nw {
namespace lyt {
// 0x004B56A0 | nintendogs:bytes [tier A]
nw::lyt::GroupContainer::~GroupContainer()
{
    for (GroupList::iterator it = m_GroupList.begin(); it != m_GroupList.end();) {
        GroupList::iterator currIt = it;
        ++it;
        m_GroupList.erase(currIt);
        Group* pGroup = GroupList::GetPointerFromNode(currIt.GetNode());
        if (!pGroup->IsUserAllocated()) {
            Layout::DeleteObj(pGroup);
        }
    }
}

// 0x0013EEEC | libgarden [tier A]
void nw::lyt::GroupContainer::AppendGroup(Group* pGroup)
{
    m_GroupList.PushBack(pGroup);
}

// 0x004B5650 | nintendogs:bytes [tier A]
Group* nw::lyt::GroupContainer::FindGroupByName(const char* findName)
{
    for (GroupList::iterator it = m_GroupList.begin(); it != m_GroupList.end(); ++it) {
        Group* pGroup = GroupList::GetPointerFromNode(it.GetNode());
        if (internal::EqualsResName(pGroup->GetName(), findName)) {
            return pGroup;
        }
    }
    return nullptr;
}

} // namespace lyt
} // namespace nw
