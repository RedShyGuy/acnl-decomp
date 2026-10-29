#include "pead/peadMutex.h"

namespace pead {
// 0x0053BA64 (name after sead)
void pead::Mutex::lock()
{
    mMutex.Lock();
}

// 0x0053BA8C (name after sead)
void pead::Mutex::unlock()
{
    mMutex.Unlock();
}

// 0x0053BAAC (name after sead)
pead::Mutex::Mutex() : IDisposer()
{
    mMutex.Initialize(false);
}

// 0x0053BB40 slot 0x00 | slot vf_0x00 of pead::IDisposer
// 0x0053BAFC (deleting dtor)
pead::Mutex::~Mutex()
{
    mMutex.Finalize();
}
} // namespace pead
