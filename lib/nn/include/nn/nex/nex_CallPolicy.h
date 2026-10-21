#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The base of the policies that check the calls into an object (abstract; it has no vtable of its
// own in the binary). The names of the functions are ours apart from EndCallImpl.
// RTTI N2nn3nex10CallPolicyE @ 0x008CDF3C
class CallPolicy : public ::nn::nex::RootObject
{
public:
    CallPolicy() {} // (inline)
    virtual ~CallPolicy() {}
    virtual void BeginCallImpl() = 0;
    virtual void EndCallImpl() = 0;
};

// (inline; name is ours) a call into the object of the policy for its scope
class ScopedCall
{
public:
    explicit ScopedCall(CallPolicy& policy) : m_pPolicy(&policy) { m_pPolicy->BeginCallImpl(); }
    ~ScopedCall() { m_pPolicy->EndCallImpl(); }

    CallPolicy* m_pPolicy; // 0x00
};
} // namespace nex
} // namespace nn
