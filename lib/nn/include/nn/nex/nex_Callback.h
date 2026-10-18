#pragma once

#include "decomp.h"
#include "nn/nex/nex_CallbackRoot.h"

namespace nn {
namespace nex {
// Calls a member function of an object with an argument. The member names are ours.
// Instantiations found in the binary:
//   nn::nex::Callback<nn::nex::StepSequenceJob, nn::nex::StepSequenceJob::Step*>  typeinfo 0x008CF6A4  vtable 0x008FFB98
template <typename T0, typename T1>
class Callback : public ::nn::nex::CallbackRoot
{
public:
    typedef void (T0::*Function)(T1);

    // (inline)
    Callback(T0* pObject, Function pFunction, T1 argument) : m_pObject(pObject), m_pFunction(pFunction), m_Argument(argument) {}
    virtual ~Callback() {}
    virtual void Invoke() { CallObjectMethod(); }
    virtual void CallObjectMethod() { (m_pObject->*m_pFunction)(m_Argument); }

    T0* m_pObject;        // 0x04
    Function m_pFunction; // 0x08
    T1 m_Argument;        // 0x10
};
} // namespace nex
} // namespace nn
