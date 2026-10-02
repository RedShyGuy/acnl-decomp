#pragma once

#include "decomp.h"
#include "sead/hostio/seadReflexible.h"
#include "sead/seadIDisposer.h"
#include "sead/seadINamable.h"

namespace sead {
// RTTI N4sead6ThreadE @ 0x008D20B0
// vtable 0x00906B7C (vptr 0x00906B84), offset_to_top 0, 18 entries
// vtable 0x00906BCC (vptr 0x00906BD4), offset_to_top -24, 1 entries
class Thread : public ::sead::IDisposer, public ::sead::INamable, public ::sead::hostio::Reflexible
{
public:
    Thread(); // ctor address unknown
    virtual ~Thread(); // 0x0055D9A0 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x0055D98C slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void vf_0x08(); // 0x0055D938 slot 0x08 | virtual slot, introduced by sead::Thread
    virtual void vf_0x0C(); // 0x00540F8C slot 0x0C | virtual slot, introduced by sead::Thread
    virtual void vf_0x10(); // 0x0055D67C slot 0x10 | virtual slot, introduced by sead::Thread
    virtual void vf_0x14(); // 0x0074EE58 slot 0x14 | virtual slot, introduced by sead::Thread
    virtual void vf_0x18(); // 0x0055D83C slot 0x18 | virtual slot, introduced by sead::Thread
    virtual void vf_0x1C(); // 0x0055D7C0 slot 0x1C | virtual slot, introduced by sead::Thread
    virtual void vf_0x20(); // 0x0055D944 slot 0x20 | virtual slot, introduced by sead::Thread
    virtual void vf_0x24(); // 0x0055D788 slot 0x24 | virtual slot, introduced by sead::Thread
    virtual void vf_0x28(); // 0x0055D794 slot 0x28 | virtual slot, introduced by sead::Thread
    virtual void vf_0x2C(); // 0x0055D698 slot 0x2C | virtual slot, introduced by sead::Thread
    virtual void vf_0x30(); // 0x0074EE40 slot 0x30 | virtual slot, introduced by sead::Thread
    virtual void vf_0x34(); // 0x0074EE48 slot 0x34 | virtual slot, introduced by sead::Thread
    virtual void vf_0x38(); // 0x0074EE50 slot 0x38 | virtual slot, introduced by sead::Thread
    virtual void vf_0x3C(); // 0x0074EE60 slot 0x3C | virtual slot, introduced by sead::Thread
    virtual void vf_0x40(); // 0x0011C12F slot 0x40 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x44(); // 0x0074EE68 slot 0x44 | virtual slot, introduced by sead::Thread
    void ctrThreadFunc_(unsigned); // 0x0053C04C | nintendogs:bytes [tier A]
    void run_(); // 0x0053C138 | nintendogs:bytes [tier A]
};
} // namespace sead
