#pragma once

#include "decomp.h"
#include "sead/seadIDisposer.h"

namespace sead {
// RTTI N4sead15CriticalSectionE @ 0x008D1790
// vtable 0x00905954 (vptr 0x0090595C), offset_to_top 0, 2 entries
class CriticalSection : public ::sead::IDisposer
{
public:
    virtual ~CriticalSection(); // 0x00138D4C slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x00545B8C slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    void lock(); // 0x00136474 | nintendogs:callseq-callee [tier A]
    void Exit(); // 0x00138D44 | libgarden [tier A]
    void tryLock(); // 0x0034C024 | nintendogs:callseq-callee [tier A]
    CriticalSection(); // 0x00538928 | nintendogs:callgraph [tier A]
};
} // namespace sead
