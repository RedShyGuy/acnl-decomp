#pragma once

#include "decomp.h"
#include "libms/FlowNode.h"

namespace libms {

// RTTI N5libms15FlowNodeMessageE @ 0x008D2CD8
// vtable 0x00909148 (vptr 0x00909150), offset_to_top 0, 2 entries
class FlowNodeMessage : public FlowNode
{
public:
    explicit FlowNodeMessage(const FlowNode& node);
    virtual ~FlowNodeMessage();
};
ASSERT_SIZE(FlowNodeMessage, 0x8);

} // namespace libms
