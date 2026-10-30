#pragma once

#include "decomp.h"
#include "libms/FlowNode.h"

namespace libms {

// RTTI N5libms13FlowNodeEntryE @ 0x008D2CB4
// vtable 0x00909118 (vptr 0x00909120), offset_to_top 0, 2 entries
class FlowNodeEntry : public FlowNode
{
public:
    explicit FlowNodeEntry(const FlowNode& node);
    virtual ~FlowNodeEntry();
};
ASSERT_SIZE(FlowNodeEntry, 0x8);

} // namespace libms
