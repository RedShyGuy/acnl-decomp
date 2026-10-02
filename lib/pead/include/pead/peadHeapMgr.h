#pragma once

#include "decomp.h"
#include "pead/hostio/peadNode.h"

namespace pead {
// RTTI N4pead7HeapMgrE @ 0x008D12F8
// vtable 0x00904D9C (vptr 0x00904DA4), offset_to_top 0, 2 entries
class HeapMgr : public ::pead::hostio::Node
{
public:
    HeapMgr(); // ctor candidate(s) 0x00793140 (unverified)
    virtual void vf_0x00(); // 0x0053D80C slot 0x00 | virtual slot, introduced by pead::HeapMgr
    virtual void vf_0x04(); // 0x0053D808 slot 0x04 | virtual slot, introduced by pead::HeapMgr
};
} // namespace pead
