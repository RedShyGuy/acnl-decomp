#include "ssys/st/dListNodePriorityBase.h"

namespace ssys {
namespace st {
// 0x0056B88C (name is ours)
ssys::st::ListNode* ssys::st::ListNodePriorityBase::FindInsertPosition(ListNode* start) const
{
    ListNode* node = start;
    while (node != NULL) {
        if (IsBefore(node)) {
            break;
        }
        node = node->m_pNext;
    }
    return node;
}

} // namespace st
} // namespace ssys
