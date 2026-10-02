#pragma once

#include "decomp.h"
#include "nn/fnd/fnd_HeapBase.h"

namespace nn {
namespace fnd {
// RTTI N2nn3fnd12UnitHeapBaseE @ 0x008CDE4C
// vtable 0x008FC054 (vptr 0x008FC05C), offset_to_top 0, 7 entries
class UnitHeapBase : public ::nn::fnd::HeapBase
{
public:
    UnitHeapBase(); // ctor address unknown
    virtual void vf_0x00(); // 0x00352428 slot 0x00 | virtual slot, introduced by nn::fnd::HeapBase
    virtual void vf_0x04(); // 0x003523FC slot 0x04 | virtual slot, introduced by nn::fnd::HeapBase
    virtual void FreeV(void*); // 0x003523E0 slot 0x08 | mk7dlp:bytes
    virtual void vf_0x0C(); // 0x00729750 slot 0x0C | virtual slot, introduced by nn::fnd::HeapBase
    virtual void vf_0x10(); // 0x00729748 slot 0x10 | virtual slot, introduced by nn::fnd::HeapBase
    virtual void vf_0x14(); // 0x00729758 slot 0x14 | virtual slot, introduced by nn::fnd::HeapBase
    virtual void HasAddress(const void*) const; // 0x00729720 slot 0x18 | slot vf_0x18 of nn::fnd::HeapBase
    void Initialize(unsigned, unsigned, unsigned, int, unsigned); // 0x001369A4 | nintendogs:bytes [tier A]
};
} // namespace fnd
} // namespace nn
