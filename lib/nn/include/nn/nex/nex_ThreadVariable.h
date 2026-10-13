#pragma once

#include "decomp.h"
#include "nn/nex/nex_ForcedCriticalSection.h"
#include "nn/nex/nex_ThreadVariableRoot.h"
#include "nn/nex/nex_qMap.h"

namespace nn {
namespace nex {
// A variable with a value per thread (a map from the thread id; a thread gets the default value
// on its first access). Out of line per type. Instantiations in the binary:
//   ThreadVariable<ObjectThreadRoot*>  typeinfo 0x008CE400  vtable 0x008FCDA4
//   ThreadVariable<qList<SecurityContext> >  typeinfo 0x008CE3F4  vtable 0x008FCD88
//   ThreadVariable<unsigned int>  typeinfo 0x008CE40C  vtable 0x008FCDC0
// The names of the members and of the functions other than GetRef are ours.
template <typename T>
class ThreadVariable : public ::nn::nex::ThreadVariableRoot
{
public:
    // (inline)
    explicit ThreadVariable(T defaultValue) : m_DefaultValue(defaultValue), m_Lock(1) { m_Values[GetCurrentThreadId()] = defaultValue; }
    virtual ~ThreadVariable()
    {
        ClearAll();
    }
    virtual void ClearAll()
    {
        ScopedCS lock(m_Lock);
        while (m_Values.size() != 0) {
            m_Values.erase(m_Values.begin());
        }
    }
    virtual void ClearCurrentThread()
    {
        ScopedCS lock(m_Lock);
        typename qMap<u32, T>::iterator it = m_Values.find(GetCurrentThreadId());
        if (it != m_Values.end()) {
            m_Values.erase(it);
        }
    }
    virtual void ClearThread(u32 threadId)
    {
        ScopedCS lock(m_Lock);
        typename qMap<u32, T>::iterator it = m_Values.find(threadId);
        if (it != m_Values.end()) {
            m_Values.erase(it);
        }
    }

    // the value of the current thread
    DECOMP_NOINLINE T& GetRef()
    {
        ScopedCS lock(m_Lock);
        typename qMap<u32, T>::iterator it = m_Values.find(GetCurrentThreadId());
        if (it == m_Values.end()) {
            m_Values[GetCurrentThreadId()] = m_DefaultValue;
            return m_Values[GetCurrentThreadId()];
        }
        return it->second;
    }

    // (a copy of GetRef for const objects; out of line per type)
    DECOMP_NOINLINE const T& GetRef() const
    {
        ScopedCS lock(m_Lock);
        typename qMap<u32, T>::iterator it = m_Values.find(GetCurrentThreadId());
        if (it == m_Values.end()) {
            m_Values[GetCurrentThreadId()] = m_DefaultValue;
            return m_Values[GetCurrentThreadId()];
        }
        return it->second;
    }

    // (inline) sets the value of the current thread
    void Set(const T& value)
    {
        ScopedCS lock(m_Lock);
        m_Values[GetCurrentThreadId()] = value;
    }

    T m_DefaultValue;         // 0x0C
    mutable ForcedCriticalSection m_Lock;  // behind the value
    mutable qMap<u32, T> m_Values;
};
} // namespace nex
} // namespace nn
