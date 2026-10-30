#pragma once

#include "decomp.h"
#include "libms/FlowNode.h"

namespace libms {

// RTTI N5libms12FlowNodeJumpE @ 0x008D2CA8
// vtable 0x00909108 (vptr 0x00909110), offset_to_top 0, 2 entries
class FlowNodeJump : public FlowNode
{
public:
    explicit FlowNodeJump(const FlowNode& node);
    virtual ~FlowNodeJump();
};
ASSERT_SIZE(FlowNodeJump, 0x8);

} // namespace libms
