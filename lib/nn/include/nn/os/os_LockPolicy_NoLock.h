#pragma once

#include "decomp.h"
#include "nn/os/os_LockPolicy.h"

class nn::os::LockPolicy::NoLock
{
public:
    class LockObject;
};
