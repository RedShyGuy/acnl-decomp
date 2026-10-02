#pragma once

#include "decomp.h"
#include "ssys/st/dListNode.h"

namespace ssys {
namespace ma {
// RTTI N4ssys2ma9LoadSplitE @ 0x008D250C
// vtable 0x009073C4 (vptr 0x009073CC), offset_to_top 0, 4 entries
class LoadSplit : public ::ssys::st::ListNode
{
public:
    LoadSplit(); // ctor address unknown
    virtual ~LoadSplit(); // 0x0013CA08 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x0056B85C slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x08(); // 0x0056B854 slot 0x08 | virtual slot, introduced by ssys::ma::LoadSplit
    virtual void vf_0x0C(); // 0x0056B858 slot 0x0C | virtual slot, introduced by ssys::ma::LoadSplit
    void LoadBuffered(sead::SafeStringBase<char> const&, sead::Heap*, unsigned int); // 0x0056B240 | libgarden [tier A]
    void Read(sead::SafeStringBase<char const> const&, sead::Heap*, unsigned long, unsigned long); // 0x0056B484 | libgarden [tier A]
};
} // namespace ma
} // namespace ssys
