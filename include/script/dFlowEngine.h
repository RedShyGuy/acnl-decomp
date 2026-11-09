#pragma once

#include "decomp.h"
#include "libms/FlowNode.h"
#include "libms/libms.h"

namespace script {
// The runner of a flow chart of LibMessageStudio (MsgFlwBn): one node per call is handed to the slot of its type; the
// derived class sets the next node. All slots are pure (the vtable has __cxa_pure_virtual only); the slot and member
// names are ours.
// RTTI N6script10FlowEngineE @ 0x008D36F4
// vtable 0x009095F4 (vptr 0x009095FC), offset_to_top 0, 9 entries
class FlowEngine
{
public:
    // the types of the nodes (LMS_FlowNode::type)
    enum NodeType
    {
        NODE_MESSAGE = 1,
        NODE_BRANCH = 2,
        NODE_EVENT = 3,
        NODE_ENTRY = 4,
        NODE_JUMP = 5,
    };

    // (inline)
    FlowEngine() : m_Flow(), m_Index(-1), m_Next(-1), m_pNode(NULL), m_Count(0), m_IsEnd(0) {}
    virtual ~FlowEngine() = 0;
    // the nodes by their type (flow: m_Flow)
    virtual void OnMessage(libms::Flow* flow, const libms::FlowNode& node) = 0; // slot 0x08
    virtual void OnBranch(libms::Flow* flow, const libms::FlowNode& node) = 0; // slot 0x0C
    virtual void OnEvent(libms::Flow* flow, const libms::FlowNode& node) = 0; // slot 0x10
    virtual void OnEntry(libms::Flow* flow, const libms::FlowNode& node) = 0; // slot 0x14
    virtual void OnJump(libms::Flow* flow, const libms::FlowNode& node) = 0; // slot 0x18
    // the flow has moved to node
    virtual void OnNode(const libms::FlowNode& node) = 0; // slot 0x1C
    // after a node that is done (the flow moved on or ended); the result is not used
    virtual bool OnNodeDone() = 0; // slot 0x20

    // the current node is processed (a new one first goes to OnNode)
    void Process(); // 0x005D5F00

    libms::Flow m_Flow;           // 0x04
    s32 m_Index;                  // 0x0C the current node (-1: none)
    s32 m_Next;                   // 0x10 the next node
    const LMS_FlowNode* m_pNode;  // 0x14 the current node
    s32 m_Count;                  // 0x18 the calls on the current node
    u8 m_IsEnd;                   // 0x1C the flow has ended
};
ASSERT_SIZE(FlowEngine, 0x20);

// (inline; the vptr store in the derived destructors)
inline FlowEngine::~FlowEngine() {}
} // namespace script
