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

    DECOMP_EMIT_COPY DECOMP_NOINLINE bool HasNext() const { return m_pNode->next != NONE; } // 0x00756938 (name is ours)
    // the next node, or the entry node in the other file
    DECOMP_EMIT_COPY DECOMP_NOINLINE u16 GetNext() const { return m_pNode->next; } // 0x00756954 (name is ours)
    // a jump in this flow (no other file)
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsLocal() const { return m_pNode->value == NONE; } // 0x0075691C (name is ours)
    DECOMP_EMIT_COPY DECOMP_NOINLINE u16 GetFile() const { return m_pNode->value; } // 0x00756910 (name is ours)
};
ASSERT_SIZE(FlowNodeJump, 0x8);

} // namespace libms
