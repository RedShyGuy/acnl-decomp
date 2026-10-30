#include "libms/FlowNodeJump.h"

namespace libms {

// the node as its type (a copy of the generic node)
// 0x005B216C (name is ours)
libms::FlowNodeJump::FlowNodeJump(const FlowNode& node) : FlowNode(node)
{
}

// 0x005B2194
// 0x005B2190 (deleting dtor)
libms::FlowNodeJump::~FlowNodeJump()
{
    // empty (in the original too)
}

} // namespace libms
