#include "libms/FlowNode.h"

namespace libms {

// 0x005B22C8 (name is ours)
libms::FlowNode::FlowNode(const LMS_FlowNode* node) : m_pNode(node)
{
}

// 0x005B22E0
// 0x005B22DC (deleting dtor)
libms::FlowNode::~FlowNode()
{
    // empty (in the original too)
}

} // namespace libms
