#include "libms/FlowNodeBranch.h"

namespace libms {

// the node as its type (a copy of the generic node)
// 0x005B21F0 (name is ours)
libms::FlowNodeBranch::FlowNodeBranch(const FlowNode& node) : FlowNode(node)
{
}

// 0x005B2218
// 0x005B2214 (deleting dtor)
libms::FlowNodeBranch::~FlowNodeBranch()
{
    // empty (in the original too)
}

} // namespace libms
