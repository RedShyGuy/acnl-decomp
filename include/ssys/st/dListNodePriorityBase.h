#pragma once

#include "decomp.h"
#include "ssys/st/dListNode.h"

namespace ssys {
namespace st {
// a node of a list sorted by a priority the derived class compares (ListPriority; no vtable of its own in the binary:
// it is only built as a base; function names are ours)
// RTTI N4ssys2st20ListNodePriorityBaseE @ 0x008D2530
class ListNodePriorityBase : public ::ssys::st::ListNode
{
public:
    // whether this node goes in front of other
    virtual bool IsBefore(const ListNode* other) const = 0;          // slot 0x08
    // the first node from start on that this node goes in front of (NULL: the end)
    virtual ListNode* FindInsertPosition(ListNode* start) const;    // 0x0056B88C slot 0x0C
};
} // namespace st
} // namespace ssys
