#pragma once

#include "decomp.h"

namespace nn {
namespace fnd {
// RTTI N2nn3fnd8HeapBaseE @ 0x008CDEF4
// vtable 0x008FC0FC (vptr 0x008FC104), offset_to_top 0, 7 entries
class HeapBase
{
public:
    HeapBase(); // ctor candidate(s) 0x0013B4C4 (unverified)
    virtual void vf_0x00(); // 0x0011C12F slot 0x00 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x04(); // 0x0011C12F slot 0x04 | slot vf_0x00 of ChangeRentalBase
    virtual void FreeV(void*); // 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x0C(); // 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x10(); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x14(); // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
    virtual void HasAddress(const void*) const; // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    void FillMemory32(unsigned, unsigned, unsigned); // 0x00136C28 | nintendogs:bytes [tier A]
    ~HeapBase(); // 0x0013B4C4 | nintendogs:bytes [tier A]
};
} // namespace fnd
} // namespace nn
