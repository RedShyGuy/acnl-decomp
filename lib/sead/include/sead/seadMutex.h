#pragma once

#include "decomp.h"
#include "sead/seadIDisposer.h"

namespace sead {
// RTTI N4sead5MutexE @ 0x008D209C
// vtable 0x00906B50 (vptr 0x00906B58), offset_to_top 0, 2 entries
class Mutex : public ::sead::IDisposer
{
public:
    Mutex(); // ctor candidate(s) 0x0055D158 (unverified)
    virtual ~Mutex(); // 0x0055D1EC slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x0055D1A8 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
} // namespace sead
