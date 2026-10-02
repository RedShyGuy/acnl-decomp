#pragma once

#include "decomp.h"
#include "pead/peadIDisposer.h"

namespace pead {
// RTTI N4pead15CriticalSectionE @ 0x008D11A8
// vtable 0x00904AB0 (vptr 0x00904AB8), offset_to_top 0, 2 entries
class CriticalSection : public ::pead::IDisposer
{
public:
    CriticalSection(); // ctor candidate(s) 0x00538928 (unverified)
    virtual ~CriticalSection(); // 0x00538970 slot 0x00 | nintendogs:callgraph
    // 0x00538954 slot 0x04 | slot vf_0x04 of pead::IDisposer (deleting dtor)
};
} // namespace pead
