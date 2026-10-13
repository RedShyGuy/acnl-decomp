#pragma once

#include "decomp.h"
#include "nn/nex/nex_ForcedCriticalSection.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_ThreadVariableRoot.h"
#include "nn/nex/nex_qChain.h"

namespace nn {
namespace nex {
// All ThreadVariables (made with the GlobalVariables). The names of the members and of the
// functions are ours.
// RTTI N2nn3nex18ThreadVariableListE @ 0x008CE754
// vtable 0x008FD578 (vptr 0x008FD580), offset_to_top 0, 2 entries
class ThreadVariableList : public ::nn::nex::RootObject
{
public:
    // (inline in GlobalVariables::AcquireInstance)
    ThreadVariableList() : m_Lock(1) {}
    virtual ~ThreadVariableList(); // 0x003905E4 slot 0x00
    // 0x003905B4 slot 0x04 (deleting dtor)

    // the values of a thread go away
    void ClearThread(u32 threadId); // 0x00390468
    void Remove(ThreadVariableRoot* pVariable); // 0x003904E8

    qChain<ThreadVariableRoot*, DefaultChainPolicy<ThreadVariableRoot*> > m_Variables; // 0x04
    ForcedCriticalSection m_Lock;                                                      // 0x18

    static bool s_IsCreated;
    static ThreadVariableList* s_pInstance;
};
ASSERT_SIZE(ThreadVariableList, 0x40);
} // namespace nex
} // namespace nn
