#pragma once

#include "decomp.h"
#include "sead/seadCtrFileStreamFileDevice.h"

namespace sead {
// RTTI N4sead13CtrFileDeviceE @ 0x008D1620
// vtable 0x00905440 (vptr 0x00905448), offset_to_top 0, 30 entries
class CtrFileDevice : public ::sead::CtrFileStreamFileDevice
{
public:
    CtrFileDevice(); // ctor address unknown
    virtual ~CtrFileDevice(); // 0x0054224C slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x0054223C slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void vf_0x08(); // 0x0074B744 slot 0x08 | virtual slot, introduced by sead::FileDevice
    virtual void isMatchDevice_(const sead::HandleBase*) const; // 0x0074B6F8 slot 0x0C | slot vf_0x0C of sead::FileDevice
    virtual void vf_0x74(); // 0x0074B6EC slot 0x74 | virtual slot, introduced by sead::CtrFileStreamFileDevice
};
} // namespace sead
