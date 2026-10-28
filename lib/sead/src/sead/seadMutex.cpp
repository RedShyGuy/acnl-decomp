#include "sead/seadMutex.h"

namespace sead {
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
