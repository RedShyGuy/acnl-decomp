#pragma once

#include "decomp.h"
#include "sead/seadCtrFileStreamFileDevice.h"

namespace sead {
// RTTI N4sead19CtrBackupFileDeviceE @ 0x008D1C60
// vtable 0x0090614C (vptr 0x00906154), offset_to_top 0, 30 entries
class CtrBackupFileDevice : public ::sead::CtrFileStreamFileDevice
{
public:
    CtrBackupFileDevice(); // ctor address unknown
    virtual ~CtrBackupFileDevice(); // 0x0054A160 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x0054A150 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void vf_0x08(); // 0x0074CD30 slot 0x08 | virtual slot, introduced by sead::FileDevice
    virtual void isMatchDevice_(const sead::HandleBase*) const; // 0x0074CCE4 slot 0x0C | slot vf_0x0C of sead::FileDevice
    virtual void vf_0x74(); // 0x0074CCD4 slot 0x74 | virtual slot, introduced by sead::CtrFileStreamFileDevice
};
} // namespace sead
