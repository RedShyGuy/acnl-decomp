#pragma once

#include "decomp.h"
#include "pead/hostio/peadReflexible.h"
#include "pead/peadIDisposer.h"
#include "pead/peadINamable.h"

namespace pead {
// RTTI N4pead4HeapE @ 0x008D124C
// vtable 0x00904C68 (vptr 0x00904C70), offset_to_top 0, 22 entries
class Heap : public ::pead::IDisposer, public ::pead::INamable, public ::pead::hostio::Reflexible
{
public:
    Heap(); // ctor address unknown
    virtual ~Heap(); // 0x0053B85C slot 0x00 | nintendogs:bytes
    // 0x0053B838 slot 0x04 | slot vf_0x04 of pead::IDisposer (deleting dtor)
    virtual void vf_0x08(); // 0x007494D4 slot 0x08 | virtual slot, introduced by pead::Heap
    virtual void vf_0x0C(); // 0x00749488 slot 0x0C | virtual slot, introduced by pead::Heap
    virtual void vf_0x10(); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void adjust(); // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
    virtual void tryAlloc(unsigned, int); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    virtual void free(void*); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x20(); // 0x0011C12F slot 0x20 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x24(); // 0x0011C12F slot 0x24 | slot vf_0x00 of ChangeRentalBase
    virtual void freeAll(); // 0x0011C12F slot 0x28 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x2C(); // 0x0011C12F slot 0x2C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x30(); // 0x0011C12F slot 0x30 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x34(); // 0x0011C12F slot 0x34 | slot vf_0x00 of ChangeRentalBase
    virtual void getFreeSize() const; // 0x0011C12F slot 0x38 | slot vf_0x00 of ChangeRentalBase
    virtual void getMaxAllocatableSize(int) const; // 0x0011C12F slot 0x3C | slot vf_0x00 of ChangeRentalBase
    virtual void isInclude(const void*) const; // 0x0011C12F slot 0x40 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x44(); // 0x0011C12F slot 0x44 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x48(); // 0x0011C12F slot 0x48 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x4C(); // 0x0011C12F slot 0x4C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x50(); // 0x00749530 slot 0x50 | virtual slot, introduced by pead::Heap
    virtual void vf_0x54(); // 0x0053B43C slot 0x54 | virtual slot, introduced by pead::Heap
};
} // namespace pead
