#include "libms/FlowNodeMessage.h"

namespace libms {

// the node as its type (a copy of the generic node)
// 0x005B221C (name is ours)
libms::FlowNodeMessage::FlowNodeMessage(const FlowNode& node) : FlowNode(node)
{
}

// 0x005B2244
// 0x005B2240 (deleting dtor)
libms::FlowNodeMessage::~FlowNodeMessage()
{
    // empty (in the original too)
}

} // namespace libms
