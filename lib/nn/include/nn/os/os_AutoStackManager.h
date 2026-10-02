#pragma once

#include "decomp.h"

namespace nn {
namespace os {
// RTTI N2nn2os16AutoStackManagerE @ 0x008CDE08
// Allocates and frees the stacks of threads started with
// Thread::TryInitializeAndStartImplUsingAutoStack (the default one is in os_Default.cpp).
class AutoStackManager
{
public:
    AutoStackManager() {} // inline (see __sti___14_os_Default_cpp)
    virtual ~AutoStackManager() {}
    // returns the stack bottom (highest address) of a new stack
    virtual void* Construct(size_t stackSize) = 0; // slot 0x08
    // isError: the thread could not be started (otherwise called by the exiting thread itself)
    virtual void Destruct(void* stackBottom, bool isError) = 0; // slot 0x0C
};
} // namespace os
} // namespace nn
