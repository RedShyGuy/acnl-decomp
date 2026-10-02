#pragma once

#include "decomp.h"
#include "sead/hostio/seadReflexible.h"
#include "sead/seadIDisposer.h"
#include "sead/seadINamable.h"

namespace sead {
// RTTI N4sead4HeapE @ 0x008D1FDC
// vtable 0x00906A1C (vptr 0x00906A24), offset_to_top 0, 25 entries
// vtable 0x00906A88 (vptr 0x00906A90), offset_to_top -24, 1 entries
class Heap : public ::sead::IDisposer, public ::sead::INamable, public ::sead::hostio::Reflexible
{
public:
    struct HeapDirection { u32 _unknown; }; // TODO: real type unknown (placeholder)
    Heap(); // ctor address unknown
    virtual ~Heap(); // 0x0054E424 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x0054E3F8 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void vf_0x08(); // 0x0074E804 slot 0x08 | virtual slot, introduced by sead::Heap
    virtual void vf_0x0C(); // 0x0074E7B8 slot 0x0C | virtual slot, introduced by sead::Heap
    virtual void vf_0x10(); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x14(); // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x18(); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x1C(); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x20(); // 0x0011C12F slot 0x20 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x24(); // 0x0011C12F slot 0x24 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x28(); // 0x0054E1D8 slot 0x28 | virtual slot, introduced by sead::Heap
    virtual void vf_0x2C(); // 0x0011C12F slot 0x2C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x30(); // 0x0011C12F slot 0x30 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x34(); // 0x0011C12F slot 0x34 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x38(); // 0x0011C12F slot 0x38 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x3C(); // 0x0011C12F slot 0x3C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x40(); // 0x0011C12F slot 0x40 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x44(); // 0x0011C12F slot 0x44 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x48(); // 0x0011C12F slot 0x48 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x4C(); // 0x0011C12F slot 0x4C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x50(); // 0x0011C12F slot 0x50 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x54(); // 0x0074E860 slot 0x54 | virtual slot, introduced by sead::Heap
    virtual void vf_0x58(); // 0x0074E864 slot 0x58 | virtual slot, introduced by sead::Heap
    virtual void vf_0x5C(); // 0x0054E1E4 slot 0x5C | virtual slot, introduced by sead::Heap
    virtual void vf_0x60(); // 0x0054E1E8 slot 0x60 | virtual slot, introduced by sead::Heap
    void appendDisposer_(sead::IDisposer*); // 0x0053B3D0 | nintendogs:bytes [tier A]
    void removeDisposer_(sead::IDisposer*); // 0x0053B440 | nintendogs:bytes [tier A]
    void dispose_(const void*, const void*); // 0x0053B59C | nintendogs:bytes [tier A]
    void destruct_(); // 0x0053B63C | nintendogs:bytes [tier A]
};
} // namespace sead
