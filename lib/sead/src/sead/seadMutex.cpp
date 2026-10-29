#include "sead/seadMutex.h"

namespace sead {
// 0x0055D110 (name is ours)
void sead::Mutex::lock()
{
    mMutex.Lock();
}

// 0x0055D138 (name is ours)
void sead::Mutex::unlock()
{
    mMutex.Unlock();
}

// 0x0055D158 (name is ours)
sead::Mutex::Mutex() : IDisposer()
{
    mMutex.Initialize(false);
}

// 0x0055D1EC slot 0x00 | slot vf_0x00 of sead::IDisposer
// 0x0055D1A8 (deleting dtor)
sead::Mutex::~Mutex()
{
    mMutex.Finalize();
}

} // namespace sead
