#pragma once

#include "decomp.h"
#include "nn/ro/ro_Module.h"

// RTTI N2nn2ro6Module17EnumerateCallbackE @ 0x008CDE28
// called for every loaded module by Module::Enumerate; false stops (the slot name is ours)
class nn::ro::Module::EnumerateCallback
{
public:
    EnumerateCallback() {}

    virtual bool operator()(nn::ro::Module* module) = 0;
};
