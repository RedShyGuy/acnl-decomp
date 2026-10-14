#include "nn/nex/nex_PseudoGlobalVariableList.h"

namespace nn {
namespace nex {
// 0x0096C990 (name is ours)
PseudoGlobalVariableRoot* PseudoGlobalVariableList::s_pFirst;
// 0x0096C994 (name is ours)
u32 PseudoGlobalVariableList::s_Count;
// 0x0096CAA0 (name is ours)
PseudoGlobalVariableList PseudoGlobalVariableList::s_Instance;

// 0x003B5728 slot 0x00
// 0x003B5710 (deleting dtor)
nn::nex::PseudoGlobalVariableList::~PseudoGlobalVariableList()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003B55C0 | fefates:callgraph [tier C]
void nn::nex::PseudoGlobalVariableList::AddVariable(nn::nex::PseudoGlobalVariableRoot* pVariable)
{
    if (s_pFirst == nullptr) {
        s_pFirst = pVariable;
        s_Count = 1;
        return;
    }
    PseudoGlobalVariableRoot* pLast = s_pFirst;
    while (pLast->GetNext() != nullptr) {
        pLast = pLast->GetNext();
    }
    pLast->SetNext(pVariable);
    s_Count++;
}

// 0x003B564C (name is ours)
void nn::nex::PseudoGlobalVariableList::RemoveVariable(nn::nex::PseudoGlobalVariableRoot* pVariable)
{
    if (s_pFirst == pVariable) {
        s_pFirst = s_pFirst->GetNext();
        pVariable->SetNext(nullptr);
        s_Count--;
        return;
    }
    PseudoGlobalVariableRoot* pPrevious = nullptr;
    for (PseudoGlobalVariableRoot* p = s_pFirst; p != nullptr; p = p->GetNext()) {
        if (p == pVariable) {
            pPrevious->SetNext(p->GetNext());
            pVariable->SetNext(nullptr);
            s_Count--;
            return;
        }
        pPrevious = p;
    }
}

} // namespace nex
} // namespace nn
