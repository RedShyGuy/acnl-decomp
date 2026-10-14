#pragma once

#include "decomp.h"
#include "nn/nex/nex_PseudoGlobalVariableRoot.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The list of the pseudo global variables (the members are static; the names of the members and of
// the functions without a reference name are ours).
// RTTI N2nn3nex24PseudoGlobalVariableListE @ 0x008CEE14
// vtable 0x008FE904 (vptr 0x008FE90C), offset_to_top 0, 2 entries
class PseudoGlobalVariableList : public ::nn::nex::RootObject
{
public:
    // (inline)
    PseudoGlobalVariableList() {}
    virtual ~PseudoGlobalVariableList(); // 0x003B5728 slot 0x00
    // 0x003B5710 slot 0x04 (deleting dtor)
    // appended at the end
    void AddVariable(nn::nex::PseudoGlobalVariableRoot* pVariable); // 0x003B55C0 | fefates:callgraph [tier C]
    void RemoveVariable(nn::nex::PseudoGlobalVariableRoot* pVariable); // 0x003B564C

    static PseudoGlobalVariableRoot* s_pFirst;
    static u32 s_Count;
    // (the list of the variables)
    static PseudoGlobalVariableList s_Instance;
};
} // namespace nex
} // namespace nn
