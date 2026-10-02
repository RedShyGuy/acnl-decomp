#pragma once

#include "decomp.h"
#include "pead/peadIDisposer.h"

namespace pead {
// RTTI N4pead5MutexE @ 0x008D1280
// vtable 0x00904CD8 (vptr 0x00904CE0), offset_to_top 0, 2 entries
class Mutex : public ::pead::IDisposer
{
public:
    Mutex(); // ctor candidate(s) 0x0053BAAC (unverified)
    virtual ~Mutex(); // 0x0053BB40 slot 0x00 | slot vf_0x00 of pead::IDisposer
    // 0x0053BAFC slot 0x04 | slot vf_0x04 of pead::IDisposer (deleting dtor)
};
} // namespace pead
