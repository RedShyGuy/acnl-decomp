#pragma once

// nn::os::LockPolicy::Object<LockT>: the container derives from LockObject, which holds the lock
// (nn::fnd::UnitHeapTemplate<LockPolicy::Object<CriticalSection> > has it at +0x30). The class
// names are from RTTI, the member names are ours; everything is inline.

#include "decomp.h"
#include "nn/os/os_LockPolicy.h"

template <typename LockT>
class nn::os::LockPolicy::Object
{
public:
    class LockObject
    {
    public:
        void Initialize() { mLock.Initialize(); }
        void Lock() { mLock.Enter(); }
        void Unlock() { mLock.Exit(); }

        // locks for the lifetime of the object
        class ScopedLock
        {
        public:
            explicit ScopedLock(LockObject& object) : mObject(object) { mObject.Lock(); }
            ~ScopedLock() { mObject.Unlock(); }

        private:
            LockObject& mObject;
        };

    private:
        LockT mLock;
    };
};
