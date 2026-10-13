#pragma once

#include "decomp.h"
#include "nn/nex/nex_Job.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_StringStream.h"

namespace nn {
namespace nex {
// A job (Base: Job or PeriodicJob) that calls a member function of an object with an argument.
// Instantiations found in the binary:
//   nn::nex::MethodCallJob<nn::nex::CallContextRegister, int, nn::nex::PeriodicJob>  typeinfo 0x008CE230  vtable 0x008FC7E0
// The names of the members are ours.
template <typename T, typename A, typename Base>
class MethodCallJob : public Base
{
public:
    typedef void (T::*Method)(A);

    // (inline)
    MethodCallJob(Job::JobType type, const DebugString& debugString, T* pObject, Method method, A argument)
        : Base(type, debugString), m_Method(method), m_pObject(pObject), m_Argument(argument)
    {
    }
    virtual ~MethodCallJob() {}

    virtual void Execute() { (m_pObject->*m_Method)(m_Argument); }
    virtual String GetTraceString() const
    {
        StringStream stream;
        stream << Base::GetTraceString();
        stream << L"\tArgument: ";
        stream.StreamNumber(m_Argument);
        return String(stream.CStr());
    }

    Method m_Method;  // (after Base)
    T* m_pObject;
    A m_Argument;
};
} // namespace nex
} // namespace nn
