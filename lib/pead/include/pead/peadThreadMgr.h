#pragma once

#include "decomp.h"
#include "pead/hostio/peadNode.h"

namespace pead {
// RTTI N4pead9ThreadMgrE @ 0x008D1340
// vtable 0x00904DE4 (vptr 0x00904DEC), offset_to_top 0, 2 entries
class ThreadMgr : public ::pead::hostio::Node
{
public:
    class SingletonDisposer_;
    ThreadMgr(); // ctor candidate(s) 0x0053DE74 (unverified)
    virtual ~ThreadMgr(); // 0x0053DEBC slot 0x00 | nintendogs:bytes
    // 0x0053DEAC slot 0x04 | slot vf_0x04 of pead::ThreadMgr (deleting dtor)
    static ThreadMgr* s_pInstance; // 0x0097E43C
};
} // namespace pead
