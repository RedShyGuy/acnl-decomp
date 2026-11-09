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
    // a missing value of the node
    static const u16 NONE = 0xFFFF;

    explicit FlowNode(const LMS_FlowNode* node);
    virtual ~FlowNode();

    // (the one-line getters are inline; the original has out-of-line copies of them per class)
    DECOMP_EMIT_COPY DECOMP_NOINLINE u8 GetType() const { return m_pNode->type; } // 0x00756A9C (name is ours)
    // (events, branches)
    DECOMP_EMIT_COPY DECOMP_NOINLINE u8 GetKind() const { return m_pNode->kind; } // 0x00756AA8 (name is ours)
    DECOMP_EMIT_COPY DECOMP_NOINLINE u32 GetParam() const { return m_pNode->param; } // 0x00756AB4 (name is ours)
    // 0x00756AD0 (copy)
    DECOMP_EMIT_COPY DECOMP_NOINLINE void GetParam(u32* param) const { *param = m_pNode->param; } // 0x00756AC0 (name is ours)
    // the two halves of the parameter
    DECOMP_EMIT_COPY DECOMP_NOINLINE void GetParams(u16* low, u16* high) const // 0x00756A84 (name is ours)
    {
        u32 param = m_pNode->param;
        *low = param;
        *high = param >> 16;
    }
    // 0x007569C8 (copy)
    DECOMP_EMIT_COPY DECOMP_NOINLINE u16 GetId() const { return m_pNode->value; } // 0x00756988 (name is ours)

protected:
    const LMS_FlowNode* m_pNode;  // +0x04
};
ASSERT_SIZE(FlowNode, 0x8);

} // namespace libms
