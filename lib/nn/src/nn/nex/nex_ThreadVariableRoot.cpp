#include "nn/nex/nex_ThreadVariableRoot.h"
#include "nn/nex/nex_ThreadVariableList.h"
#include "nn/os/CTR/detail/detail_Api.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace nex {
// 0x00390694 (name is ours)
void nn::nex::ThreadVariableRoot::ClearThread(u32 threadId)
{
    // nothing to do in this build
}

// 0x00390698 (name is ours)
void nn::nex::ThreadVariableRoot::ClearCurrentThread()
{
    // nothing to do in this build
}

// 0x003906C4 (name is ours)
void nn::nex::ThreadVariableRoot::ClearAll()
{
    // nothing to do in this build
}

// 0x003906C8 | fefates:callgraph [tier C]
nn::nex::ThreadVariableRoot::ThreadVariableRoot() : m_pChainNext(nullptr), m_pChainPrev(nullptr)
{
    ThreadVariableList* pList = ThreadVariableList::s_pInstance;
    if (ThreadVariableList::s_IsCreated) {
        ScopedCS lock(pList->m_Lock);
        pList->m_Variables.push_back(this);
    }
}

// 0x003907AC
// 0x00390764 (deleting dtor)
nn::nex::ThreadVariableRoot::~ThreadVariableRoot()
{
    ThreadVariableList::s_pInstance->Remove(this);
}

} // namespace nex
} // namespace nn
