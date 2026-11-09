#include "script/dFlowEngine.h"

namespace script {
// 0x005D5F00 (name is ours)
void FlowEngine::Process()
{
    if (m_Index != m_Next) {
        m_Index = m_Next;
        m_pNode = m_Flow.GetNode(m_Next);
        libms::FlowNode node(m_pNode);
        OnNode(node);
        m_Count = 0;
    }
    if (m_pNode == NULL) {
        m_IsEnd = 1;
    } else {
        libms::FlowNode node(m_pNode);
        switch (node.GetType()) {
        case NODE_MESSAGE:
            OnMessage(&m_Flow, node);
            break;
        case NODE_BRANCH:
            OnBranch(&m_Flow, node);
            break;
        case NODE_EVENT:
            OnEvent(&m_Flow, node);
            break;
        case NODE_ENTRY:
            OnEntry(&m_Flow, node);
            break;
        case NODE_JUMP:
            OnJump(&m_Flow, node);
            break;
        default:
            m_IsEnd = 1;
            break;
        }
    }
    m_Count++;
    if (m_Index != m_Next || m_IsEnd) {
        OnNodeDone();
    }
}

} // namespace script
