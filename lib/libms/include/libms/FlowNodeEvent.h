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
    // the kinds of the events
    static const u8 KIND_CALL = 5;  // a call of the entry in the parameter (name is ours)

    virtual ~FlowNodeEvent();

    DECOMP_EMIT_COPY DECOMP_NOINLINE bool HasNext() const { return m_pNode->next != NONE; } // 0x00756994 (name is ours)
    DECOMP_EMIT_COPY DECOMP_NOINLINE u16 GetNext() const { return m_pNode->next; } // 0x007569B0 (name is ours)
};
ASSERT_SIZE(FlowNodeEvent, 0x8);

} // namespace libms
