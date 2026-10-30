#pragma once

#include "decomp.h"
#include "libms/FlowNode.h"

namespace libms {

// RTTI N5libms13FlowNodeEventE @ 0x008D2CC0
// vtable 0x00909128 (vptr 0x00909130), offset_to_top 0, 2 entries
class FlowNodeEvent : public FlowNode
{
public:
    explicit FlowNodeEvent(const FlowNode& node);
    virtual ~FlowNodeEvent();
};
ASSERT_SIZE(FlowNodeEvent, 0x8);

} // namespace libms
