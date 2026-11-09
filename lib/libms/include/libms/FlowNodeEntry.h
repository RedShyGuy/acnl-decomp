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

    DECOMP_EMIT_COPY DECOMP_NOINLINE bool HasNext() const { return m_pNode->next != NONE; } // 0x00756960 (name is ours)
    DECOMP_EMIT_COPY DECOMP_NOINLINE u16 GetNext() const { return m_pNode->next; } // 0x0075697C (name is ours)
};
ASSERT_SIZE(FlowNodeEntry, 0x8);

} // namespace libms
