#pragma once

#include "decomp.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_InstantiationContext.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
// The instances of all contexts: context 0 is the default one, the others are in m_pContexts.
// s_Instance is constructed by the static initializer of the core of nex. The names of the
// functions other than the class and of the members are ours.
// RTTI N2nn3nex13InstanceTableE @ 0x008CE218
// vtable 0x008FC7D0 (vptr 0x008FC7D8), offset_to_top 0, 2 entries
class InstanceTable : public ::nn::nex::RootObject
{
public:
    InstanceTable() : m_pContexts(nullptr) {} // (inline)
    virtual ~InstanceTable(); // 0x00361D80 slot 0x00
    // 0x00361D04 slot 0x04 (deleting dtor)

    // the instance of a kind in a context (-1 if one of them is invalid; inline in the original
    // with a copy out of line; the checks are those of InstanceTable.h and InstantiationContext.h)
    DECOMP_EMIT_COPY InstanceControl* GetInstance(u32 kind, u32 context); // 0x00361C7C (name is ours)

    // (inline in the original; InstanceTable.cpp)
    bool Register(InstanceControl* pInstance, u32 kind, u32 context)
    {
        if (context == 0) {
            return m_DefaultContext.Register(pInstance, kind);
        }
        if (context < m_pContexts->size()) {
            InstantiationContext* pContext = (*m_pContexts)[context];
            if (pContext == nullptr) {
                SystemError::SignalError(const_cast<wchar_t*>(L"D:/home/jenkins/workspace/Manual-Pack/5129d543/OnlineCore/src/Core/InstanceTable.cpp"), 158,
                                         SystemError::ERROR_NULL, 0);
                return false;
            }
            return pContext->Register(pInstance, kind);
        }
        SystemError::SignalError(const_cast<wchar_t*>(L"D:/home/jenkins/workspace/Manual-Pack/5129d543/OnlineCore/src/Core/InstanceTable.cpp"), 165,
                                 SystemError::ERROR_OUT_OF_RANGE, 0);
        return false;
    }
    // (inline) true if the context exists
    static bool IsValidContext(u32 context)
    {
        ScopedCS lock(s_Lock);
        if (context == 0) {
            return true;
        }
        return context < s_Instance.m_pContexts->size() && (*s_Instance.m_pContexts)[context] != nullptr;
    }
    // (inline) the highest context
    static u32 GetLastContext()
    {
        ScopedCS lock(s_Lock);
        if (s_Instance.m_pContexts == nullptr) {
            return 0;
        }
        return s_Instance.m_pContexts->size() - 1;
    }
    // (inline; no checks)
    InstantiationContext* GetContext(u32 context)
    {
        if (context == 0) {
            return &m_DefaultContext;
        }
        return (*m_pContexts)[context];
    }

    InstantiationContext m_DefaultContext;           // 0x04
    qVector<InstantiationContext*>* m_pContexts;    // 0x48

    static InstanceTable s_Instance;
    // locks the table
    static CriticalSection s_Lock;
};

// (inline in the original, with a copy out of line)
inline InstanceControl* InstanceTable::GetInstance(u32 kind, u32 context)
{
    InstantiationContext* pContext;
    if (context == 0) {
        pContext = &m_DefaultContext;
    } else {
        if (context >= m_pContexts->size()) {
            SystemError::SignalError(const_cast<wchar_t*>(L"D:\\home\\jenkins\\workspace\\Manual-Pack\\5129d543\\.\\OnlineCore/src/Core/InstanceTable.h"), 72,
                                     SystemError::ERROR_OUT_OF_RANGE, 0);
            return reinterpret_cast<InstanceControl*>(-1);
        }
        pContext = (*m_pContexts)[context];
    }
    if (kind >= InstantiationContext::INSTANCE_COUNT) {
        SystemError::SignalError(
            const_cast<wchar_t*>(L"D:\\home\\jenkins\\workspace\\Manual-Pack\\5129d543\\.\\OnlineCore/src/Core/InstantiationContext.h"), 83,
            SystemError::ERROR_OUT_OF_RANGE, 0);
        return reinterpret_cast<InstanceControl*>(-1);
    }
    return pContext->m_Instances[kind];
}
ASSERT_SIZE(InstanceTable, 0x4C);
} // namespace nex
} // namespace nn
