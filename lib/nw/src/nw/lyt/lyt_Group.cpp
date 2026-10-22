#include "nw/lyt/lyt_Group.h"

#include "nw/lyt/lyt_Layout.h"
#include "nw/lyt/lyt_Pane.h"
#include "nw/lyt/lyt_ResourceFormat.h"

namespace nw {
namespace lyt {
// 0x004B8550 | nintendogs:bytes [tier A]
nw::lyt::Group::Group(const res::Group* pResGroup, Pane* pRootPane) : m_UserAllocated(false)
{
    internal::CopyString(m_Name, sizeof(m_Name), pResGroup->name);

    const char (*paneNames)[res::RESOURCE_NAME_LENGTH] =
        reinterpret_cast<const char (*)[res::RESOURCE_NAME_LENGTH]>(pResGroup + 1);
    for (int i = 0; i < pResGroup->paneNum; ++i) {
        Pane* pFindPane = pRootPane->FindPaneByName(paneNames[i], true);
        if (pFindPane != nullptr) {
            void* pMem = Layout::AllocMemory(sizeof(PaneLink));
            if (pMem != nullptr) {
                PaneLink* pPaneLink = new (pMem) PaneLink;
                pPaneLink->m_pTarget = pFindPane;
                m_PaneLinkList.PushBack(pPaneLink);
            }
        }
    }
}

// 0x004B8698 slot 0x00 | nintendogs:bytes
// 0x004B8640 (deleting dtor)
nw::lyt::Group::~Group()
{
    for (PaneLinkList::iterator it = m_PaneLinkList.begin(); it != m_PaneLinkList.end();) {
        PaneLinkList::iterator currIt = it;
        ++it;
        m_PaneLinkList.erase(currIt);
        Layout::DeleteObj(PaneLinkList::GetPointerFromNode(currIt.GetNode()));
    }
}

// 0x004B5230 (name is ours)
void BindAnimation(Group* pGroup, AnimTransform* pAnimTrans, bool bRecursive, bool bDisable)
{
    Group::PaneLinkList& paneList = pGroup->GetPaneList();
    for (Group::PaneLinkList::iterator it = paneList.begin(); it != paneList.end(); ++it) {
        Group::PaneLinkList::GetPointerFromNode(it.GetNode())->m_pTarget->BindAnimation(pAnimTrans, bRecursive, bDisable);
    }
}

} // namespace lyt
} // namespace nw
