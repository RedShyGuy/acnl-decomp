#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"

namespace sead {
// RTTI N4sead7HeapMgrE @ 0x008D2144
// vtable 0x00906C80 (vptr 0x00906C88), offset_to_top 0, 3 entries
class HeapMgr : public ::sead::hostio::Node
{
public:
    HeapMgr(); // ctor candidate(s) 0x00793088 (unverified)
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
    virtual void vf_0x04(); // 0x0055F318 slot 0x04 | virtual slot, introduced by sead::HeapMgr
    virtual void vf_0x08(); // 0x0055F314 slot 0x08 | virtual slot, introduced by sead::HeapMgr
    void setCurrentHeap_(sead::Heap*); // 0x0012CD58 | nintendogs:callseq-callee [tier A]
    void GetCurrentHeap(); // 0x003047C8 | libgarden [tier A]
    void getCurrentHeap() const; // 0x00749CC8 | nintendogs:bytes [tier A]
};
} // namespace sead
