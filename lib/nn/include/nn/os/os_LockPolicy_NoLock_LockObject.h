#pragma once

#include "decomp.h"
#include "nn/os/os_LockPolicy_NoLock.h"

// RTTI N2nn2os10LockPolicy6NoLock10LockObjectE @ 0x008CDDA0
//
// The lock object of a container without a lock: everything does nothing (inline; the functions
// mirror LockPolicy::Object<LockT>::LockObject, their names are ours).
class nn::os::LockPolicy::NoLock::LockObject
{
public:
    void Initialize() {}
    void Finalize() {}
    void Lock() {}
    void Unlock() {}

    class ScopedLock
    {
    public:
        explicit ScopedLock(LockObject&) {}
    };
};
