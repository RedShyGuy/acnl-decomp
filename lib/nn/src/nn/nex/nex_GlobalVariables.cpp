#include "nn/nex/nex_GlobalVariables.h"
#include "nn/nex/nex_ThreadVariableList.h"

namespace nn {
namespace nex {
// 0x0096C974 (name is ours)
GlobalVariables* GlobalVariables::s_pInstance;
// 0x00AE04EC (name is ours)
ForcedCriticalSection GlobalVariables::s_Lock(1);

// 0x0037765C | fefates:callseq [tier B]
void nn::nex::GlobalVariables::AcquireInstance()
{
    ScopedCS lock(s_Lock);
    if (s_pInstance != nullptr) {
        s_pInstance->AcquireRef();
        return;
    }
    ThreadVariableList* pList = new ThreadVariableList();
    if (pList != nullptr) {
        ThreadVariableList::s_IsCreated = true;
    }
    ThreadVariableList::s_pInstance = pList;
    s_pInstance = new GlobalVariables();
}

// 0x00377750 (name is ours)
void nn::nex::GlobalVariables::ReleaseInstance()
{
    ScopedCS lock(s_Lock);
    if (s_pInstance == nullptr) {
        return;
    }
    bool isLast = s_pInstance->m_RefCount == 1;
    s_pInstance->ReleaseRef();
    if (isLast) {
        s_pInstance = nullptr;
        delete ThreadVariableList::s_pInstance;
        ThreadVariableList::s_pInstance = nullptr;
    }
}

// 0x00377810 | fefates:callseq [tier B]
nn::nex::GlobalVariables::GlobalVariables()
    : m_LastError(0), m_LastErrorDetail(0), m_CurrentThread(nullptr), m_Unknown0x18C(true)
{
    CriticalSection::s_IsLockDisabled = false;
    m_BroadcastAddress = L"255.255.255.255";
}

// 0x00377B84 | fefates:callseq
// 0x00377B54 (deleting dtor)
nn::nex::GlobalVariables::~GlobalVariables()
{
    ScopedCS lock(s_Lock);
    s_pInstance = nullptr;
}

} // namespace nex
} // namespace nn
