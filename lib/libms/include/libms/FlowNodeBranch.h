#pragma once

#include "decomp.h"
#include "libms/FlowNode.h"

namespace libms {

// RTTI N5libms14FlowNodeBranchE @ 0x008D2CCC
// vtable 0x00909138 (vptr 0x00909140), offset_to_top 0, 2 entries
class FlowNodeBranch : public FlowNode
{
public:
    explicit FlowNodeBranch(const FlowNode& node);
    virtual ~FlowNodeBranch();
};
ASSERT_SIZE(FlowNodeBranch, 0x8);

} // namespace libms
