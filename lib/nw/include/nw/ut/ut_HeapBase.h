#pragma once

#include "decomp.h"
#include "nw/ut/ut_HeapNode.h"

namespace nw {
namespace ut {
// RTTI N2nw2ut8HeapBaseE @ 0x008D0530
// vtable 0x00902344 (vptr 0x0090234C), offset_to_top 0, 2 entries
class HeapBase : public ::nw::ut::HeapNode
{
public:
    HeapBase(); // ctor address unknown
    virtual void vf_0x00(); // 0x0048BA90 slot 0x00 | virtual slot, introduced by nw::ut::HeapBase
    virtual void vf_0x04(); // 0x0048BA80 slot 0x04 | virtual slot, introduced by nw::ut::HeapBase
    void Initialize(unsigned, void*, void*, unsigned short); // 0x0013EE80 | nintendogs:bytes [tier A]
    void FillFreeMemory(void*, unsigned); // 0x00140E78 | nintendogs:bytes [tier A]
    void FillAllocMemory(void*, unsigned); // 0x00140EA4 | nintendogs:bytes [tier A]
    void FindContainHeap(nw::ut::LinkList<nw::ut::HeapBase, (long)4>*, const void*); // 0x0014387C | nintendogs:bytes [tier A]
    void Finalize(); // 0x001438D4 | nintendogs:bytes [tier A]
};
} // namespace ut
} // namespace nw
