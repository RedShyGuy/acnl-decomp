#include "libms/FlowNodeEntry.h"

namespace libms {

// the node as its type (a copy of the generic node)
// 0x005B2198 (name is ours)
libms::FlowNodeEntry::FlowNodeEntry(const FlowNode& node) : FlowNode(node)
{
}

// 0x005B21C0
// 0x005B21BC (deleting dtor)
libms::FlowNodeEntry::~FlowNodeEntry()
{
    // empty (in the original too)
}

} // namespace libms
