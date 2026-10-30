#include "libms/FlowNodeEvent.h"

namespace libms {

// the node as its type (a copy of the generic node)
// 0x005B21C4 (name is ours)
libms::FlowNodeEvent::FlowNodeEvent(const FlowNode& node) : FlowNode(node)
{
}

// 0x005B21EC
// 0x005B21E8 (deleting dtor)
libms::FlowNodeEvent::~FlowNodeEvent()
{
    // empty (in the original too)
}

} // namespace libms
