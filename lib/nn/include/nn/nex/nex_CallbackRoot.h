#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// A deferred call (of a member function with an argument, see Callback). The names of the
// functions but CallObjectMethod are ours.
// RTTI N2nn3nex12CallbackRootE @ 0x008CE0B0
class CallbackRoot : public ::nn::nex::RootObject
{
public:
    // (inline)
    CallbackRoot() {}
    virtual ~CallbackRoot() {}
    // makes the call
    virtual void Invoke() = 0;
    virtual void CallObjectMethod() = 0;
};
} // namespace nex
} // namespace nn
