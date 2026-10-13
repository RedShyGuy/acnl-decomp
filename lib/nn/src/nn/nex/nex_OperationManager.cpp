#include "nn/nex/nex_OperationManager.h"

namespace nn {
namespace nex {
// 0x00382F20 | mk7dlp:bytes [tier A]
void nn::nex::OperationManager::OperationEnds(nn::nex::Operation* pOperation)
{
    for (qList<OperationCallback*>::Node* pNode = m_Callbacks.GetFirst(); !m_Callbacks.IsEnd(pNode); pNode = pNode->m_pNext) {
        OperationCallback* pCallback = pNode->m_Value;
        if (pCallback->m_Priority < PRIORITY_END_FIRST) {
            continue;
        }
        if (pCallback->m_Priority > PRIORITY_END_LAST) {
            break;
        }
        pCallback->CallMethod(pOperation);
    }
    m_Operations.pop_back();
}

// 0x00382FB4 | mk7dlp:bytes [tier A]
void nn::nex::OperationManager::OperationBegins(nn::nex::Operation* pOperation)
{
    m_Operations.push_back(pOperation);
    for (qList<OperationCallback*>::Node* pNode = m_Callbacks.GetFirst(); !m_Callbacks.IsEnd(pNode); pNode = pNode->m_pNext) {
        OperationCallback* pCallback = pNode->m_Value;
        if (pCallback->m_Priority < PRIORITY_BEGIN_FIRST) {
            continue;
        }
        if (pCallback->m_Priority > PRIORITY_BEGIN_LAST) {
            break;
        }
        pCallback->CallMethod(pOperation);
    }
}

// 0x003830CC | fefates:bytes [tier B]
nn::nex::OperationManager::OperationManager()
{
}

// 0x003831E8 | fefates:bytes [tier B]
// 0x003831B8 (deleting dtor)
nn::nex::OperationManager::~OperationManager()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0072B75C | mk7dlp:bytes [tier A]
nn::nex::Operation* nn::nex::OperationManager::GetCurrentOperation() const
{
    if (m_Operations.GetSize() == 0) {
        return nullptr;
    }
    return m_Operations.back();
}

} // namespace nex
} // namespace nn
