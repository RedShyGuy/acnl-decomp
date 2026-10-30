#pragma once

#include "decomp.h"
#include "libms/lms_Api.h"

namespace libms {

// RTTI N5libms8FlowNodeE @ 0x008D2CE4
// vtable 0x00909158 (vptr 0x00909160), offset_to_top 0, 2 entries
// a node of a flow chart (FLW3) for the game's flow processing; the derived classes are the node types (all
// names except the class names are ours)
class FlowNode
{
public:
    explicit FlowNode(const LMS_FlowNode* node);
    virtual ~FlowNode();

protected:
    const LMS_FlowNode* m_pNode;  // +0x04
};
ASSERT_SIZE(FlowNode, 0x8);

} // namespace libms
