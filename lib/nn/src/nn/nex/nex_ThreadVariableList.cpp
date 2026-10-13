#include "nn/nex/nex_ThreadVariableList.h"

namespace nn {
namespace nex {
// 0x0096C915 (name is ours)
bool ThreadVariableList::s_IsCreated;
// 0x0096CA3C (name is ours)
ThreadVariableList* ThreadVariableList::s_pInstance;

// 0x00390468 (name is ours)
void nn::nex::ThreadVariableList::ClearThread(u32 threadId)
{
    if (!s_IsCreated) {
        return;
    }
    ScopedCS lock(m_Lock);
    for (ThreadVariableRoot* p = m_Variables.m_pFirst; p != m_Variables.m_pEnd; p = p->m_pChainNext) {
        p->ClearThread(threadId);
    }
}

// 0x003904E8 (name is ours)
void nn::nex::ThreadVariableList::Remove(ThreadVariableRoot* pVariable)
{
    if (!s_IsCreated) {
        return;
    }
    ScopedCS lock(m_Lock);
    m_Variables.remove(pVariable);
}

// 0x003905E4
// 0x003905B4 (deleting dtor)
nn::nex::ThreadVariableList::~ThreadVariableList()
{
    if (s_IsCreated) {
        ScopedCS lock(m_Lock);
        for (ThreadVariableRoot* p = m_Variables.m_pFirst; p != m_Variables.m_pEnd; p = p->m_pChainNext) {
            p->ClearAll();
        }
    }
    s_IsCreated = false;
}

// 0x007E4470
// 0x007E4440 (deleting dtor)
template class qChain<ThreadVariableRoot*, DefaultChainPolicy<ThreadVariableRoot*> >;

} // namespace nex
} // namespace nn
