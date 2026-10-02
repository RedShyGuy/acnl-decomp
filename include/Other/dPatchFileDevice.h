#pragma once

#include "decomp.h"
#include "sead/seadCtrFileStreamFileDevice.h"

// RTTI 15PatchFileDevice @ 0x008CC110
// vtable 0x008F1E4C (vptr 0x008F1E54), offset_to_top 0, 30 entries
class PatchFileDevice : public ::sead::CtrFileStreamFileDevice
{
public:
    PatchFileDevice(); // ctor address unknown
    virtual ~PatchFileDevice(); // 0x0053F018 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x0029EF9C slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void vf_0x08(); // 0x0071CB04 slot 0x08 | virtual slot, introduced by sead::FileDevice
    virtual void isMatchDevice_(const sead::HandleBase*) const; // 0x0071CAB8 slot 0x0C | slot vf_0x0C of sead::FileDevice
    virtual void vf_0x74(); // 0x0071CAA8 slot 0x74 | virtual slot, introduced by sead::CtrFileStreamFileDevice
};
