#pragma once

#include "decomp.h"
#include "ssys/st/dList.h"
#include "ssys/st/dListNodePriorityBase.h"

namespace ssys {
namespace st {
// a list of ListNodePriorityBase nodes kept in their order (no vtable of its own in the binary; function names are
// ours)
// RTTI N4ssys2st12ListPriorityE @ 0x008D2524
class ListPriority : public ::ssys::st::List
{
public:
    // links node in front of the first node it goes before (at the end if there is none)
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool InsertByPriority(ListNodePriorityBase* node) // 0x001263BC
    {
        if (m_pFirst != NULL && node->IsBefore(m_pLast)) {
            ListNode* position = node->FindInsertPosition(m_pFirst);
            if (position != NULL) {
                Insert(position, node);
                return true;
            }
        }
        PushBack(node);
        return true;
    }
};
} // namespace st
} // namespace ssys
