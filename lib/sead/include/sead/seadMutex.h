#pragma once

#include "decomp.h"
#include "nn/os/os_Mutex.h"
#include "sead/seadIDisposer.h"

namespace sead {
// RTTI N4sead5MutexE @ 0x008D209C
// vtable 0x00906B50 (vptr 0x00906B58), offset_to_top 0, 2 entries
//
// A mutex of the system that its heap destroys (the member name is ours).
class Mutex : public ::sead::IDisposer
{
public:
    Mutex(); // 0x0055D158 (name is ours)
    virtual ~Mutex(); // 0x0055D1EC slot 0x00 | slot vf_0x00 of sead::IDisposer
    void lock(); // 0x0055D110 (name is ours)
    void unlock(); // 0x0055D138 (name is ours)
    // 0x0055D1A8 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)

    nn::os::Mutex mMutex; // 0x10
};
ASSERT_SIZE(Mutex, 0x14);
} // namespace sead
