#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The base of the pseudo global variables: a global with a copy of its value per context (see
// PseudoSingleton). The variables form a list (PseudoGlobalVariableList). The names of the functions
// and members are ours.
// RTTI N2nn3nex24PseudoGlobalVariableRootE @ 0x008CEE20
// vtable 0x008FE914 (vptr 0x008FE91C), offset_to_top 0, 7 entries
class PseudoGlobalVariableRoot : public ::nn::nex::RootObject
{
public:
    PseudoGlobalVariableRoot(); // 0x003B572C | fefates:callgraph [tier C]
    virtual ~PseudoGlobalVariableRoot(); // 0x003B5758 slot 0x00
    // 0x003B5740 slot 0x04 (deleting dtor)
    // the values of the contexts (copies of the default value)
    virtual void AllocateValues() = 0; // slot 0x08
    virtual void FreeValues() = 0; // slot 0x0C
    // the value of the context is the default value again
    virtual void ResetValue(u32 context) = 0; // slot 0x10
    virtual PseudoGlobalVariableRoot* GetNext() = 0; // slot 0x14
    virtual void SetNext(PseudoGlobalVariableRoot* pNext) = 0; // slot 0x18

    PseudoGlobalVariableRoot* m_pNext; // 0x04

    // the number of contexts
    static u32 s_ContextCount;
};
ASSERT_SIZE(PseudoGlobalVariableRoot, 0x8);
} // namespace nex
} // namespace nn
