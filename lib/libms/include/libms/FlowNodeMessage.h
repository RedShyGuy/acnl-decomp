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

    DECOMP_EMIT_COPY DECOMP_NOINLINE bool HasNext() const { return m_pNode->next != NONE; } // 0x00756A24 (name is ours)
    DECOMP_EMIT_COPY DECOMP_NOINLINE u16 GetNext() const { return m_pNode->next; } // 0x00756A40 (name is ours)
    // the file of the message (an index into the names of the project)
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool HasFile() const { return m_pNode->value != NONE; } // 0x00756A08 (name is ours)
    DECOMP_EMIT_COPY DECOMP_NOINLINE u16 GetFile() const { return m_pNode->value; } // 0x007569F0 (name is ours)
    // the label of the message (the index of the text)
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool HasLabel() const { return m_pNode->value2 != NONE; } // 0x007569D4 (name is ours)
    DECOMP_EMIT_COPY DECOMP_NOINLINE u16 GetLabel() const { return m_pNode->value2; } // 0x007569FC (name is ours)
};
ASSERT_SIZE(FlowNodeMessage, 0x8);

} // namespace libms
