#pragma once

#include "decomp.h"
#include "nn/nex/nex_ObjectThreadRoot.h"

namespace nn {
namespace nex {
// A thread that calls a member function of an object with an argument. Instantiations found in
// the binary:
//   nn::nex::ObjectThread<nn::nex::JobNameResolve, int>  typeinfo 0x008CE120  vtable 0x008FC5AC
//   nn::nex::ObjectThread<nn::nex::TransportBufferMultiThread, int>  typeinfo 0x008CE138  vtable 0x008FC5D4
//   nn::nex::ObjectThread<nn::nex::TransportBufferThread, void*>  typeinfo 0x008CE12C  vtable 0x008FC5C0
//   nn::nex::ObjectThread<nn::nex::WorkerThreads, int>  typeinfo 0x008CE114  vtable 0x008FC598
// The names of the members and of SetMethod are ours.
template <typename T, typename A>
class ObjectThread : public ObjectThreadRoot
{
public:
    typedef void (T::*Method)(A);

    // (inline)
    explicit ObjectThread(const String& name) : ObjectThreadRoot(name), m_Method(nullptr), m_pObject(nullptr) {}
    virtual ~ObjectThread() {}

    virtual void Run()
    {
        m_IsRunning = true;
        (m_pObject->*m_Method)(m_Argument);
    }

    // (inline) what Run calls
    void SetMethod(T* pObject, Method method, A argument)
    {
        m_Argument = argument;
        m_Method = method;
        m_pObject = pObject;
    }

    Method m_Method; // 0x24
    T* m_pObject;    // 0x2C
    A m_Argument;    // 0x30
};
} // namespace nex
} // namespace nn
