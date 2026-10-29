#pragma once

#include "decomp.h"
#include "nn/os/os_Mutex.h"
#include "pead/peadIDisposer.h"

namespace pead {
// RTTI N4pead5MutexE @ 0x008D1280
// vtable 0x00904CD8 (vptr 0x00904CE0), offset_to_top 0, 2 entries
//
// pia's copy of sead::Mutex: a mutex of the system that its heap destroys (the member name is ours).
class Mutex : public ::pead::IDisposer
{
public:
    Mutex(); // 0x0053BAAC (name after sead)
    virtual ~Mutex(); // 0x0053BB40 slot 0x00 | slot vf_0x00 of pead::IDisposer
    // 0x0053BAFC slot 0x04 | slot vf_0x04 of pead::IDisposer (deleting dtor)

    void lock(); // 0x0053BA64 (name after sead)
    void unlock(); // 0x0053BA8C (name after sead)

    nn::os::Mutex mMutex; // 0x10
};
ASSERT_SIZE(Mutex, 0x14);
} // namespace pead
