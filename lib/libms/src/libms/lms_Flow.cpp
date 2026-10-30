#include "libms/lms_Api.h"
#include "lms_Internal.h"

extern "C" {

namespace {

// FLW3 starts with u16 nodeNum, u16 branchNum and padding to 16 bytes; the u16 branch tables follow the nodes
inline s32 GetFlowNodeNum(LMS_FlowBinary* flow)
{
    if (flow->flw3 == -1)
    {
        return LMS_ERROR_NO_FLOW_NODES;
    }
    return *reinterpret_cast<const u16*>(flow->common.blocks[flow->flw3].data);
}

} // namespace

// 0x00159F38 (name is ours)
LMS_FlowBinary* LMS_InitFlow(const void* data)
{
    LMS_FlowBinary* flow = static_cast<LMS_FlowBinary*>(LMSi_Allocate(sizeof(LMS_FlowBinary)));
    flow->common.data = static_cast<const u8*>(data);
    LMSi_AnalyzeMessageHeader(&flow->common, "MsgFlwBn");
    flow->flw3 = LMSi_SearchBlockByName(&flow->common, "FLW3");
    flow->fen1 = LMSi_SearchBlockByName(&flow->common, "FEN1");
    flow->ref1 = LMSi_SearchBlockByName(&flow->common, "REF1");
    return flow;
}

// 0x00159A5C (name is ours)
void LMS_CloseFlow(LMS_FlowBinary* flow)
{
    if (flow->common.blocks != NULL)
    {
        LMSi_Free(flow->common.blocks);
    }
    LMSi_Free(flow);
}

// 0x00159D6C (name is ours)
const LMS_FlowNode* LMS_GetFlowNode(LMS_FlowBinary* flow, s32 index)
{
    if (flow->flw3 == -1)
    {
        return NULL;
    }
    return reinterpret_cast<const LMS_FlowNode*>(flow->common.blocks[flow->flw3].data + 0x10) + index;
}

// the targets of the cases of a branch node
// 0x00159AA8 (name is ours)
const u16* LMS_GetBranchCases(LMS_FlowBinary* flow, s32 index)
{
    s32 nodeNum = GetFlowNodeNum(flow);
    const LMS_FlowNode* node = LMS_GetFlowNode(flow, index);
    if (node == NULL || node->type != LMS_FLOW_NODE_BRANCH)
    {
        return NULL;
    }
    const u8* data = flow->common.blocks[flow->flw3].data;
    return reinterpret_cast<const u16*>(data + 0x10 + nodeNum * sizeof(LMS_FlowNode)) + node->branchIndex;
}

// the node of an entry point by its label (FEN1)
// 0x00159BCC (name is ours)
s32 LMS_GetFlowEntryIndex(LMS_FlowBinary* flow, const char* label)
{
    if (flow->fen1 == -1)
    {
        return LMS_ERROR_NO_ENTRY_LABELS;
    }
    s32 length = LMSi_GetLength(label);
    return LMSi_FindLabel(&flow->common.blocks[flow->fen1], label, length);
}

// the data of FLW3 at a byte offset
// 0x00159CB0 (name is ours)
const void* LMS_GetFlowData(LMS_FlowBinary* flow, s32 offset)
{
    const LMS_BinaryBlock* block = &flow->common.blocks[flow->flw3];
    if (offset >= 0 && block->size > static_cast<u32>(offset))
    {
        return block->data + offset;
    }
    return NULL;
}

} // extern "C"
