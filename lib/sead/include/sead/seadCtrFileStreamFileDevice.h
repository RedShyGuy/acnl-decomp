#pragma once

#include "decomp.h"
#include "sead/seadFileDevice.h"

namespace sead {
// RTTI N4sead23CtrFileStreamFileDeviceE @ 0x008D1F2C
// vtable 0x00906738 (vptr 0x00906740), offset_to_top 0, 30 entries
class CtrFileStreamFileDevice : public ::sead::FileDevice
{
public:
    CtrFileStreamFileDevice(); // ctor address unknown
    virtual ~CtrFileStreamFileDevice(); // 0x0054CB40 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x0054CB30 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void vf_0x08(); // 0x0074E078 slot 0x08 | virtual slot, introduced by sead::FileDevice
    virtual void isMatchDevice_(const sead::HandleBase*) const; // 0x0074E02C slot 0x0C | slot vf_0x0C of sead::FileDevice
    virtual void doIsAvailable_() const; // 0x0074DF48 slot 0x24 | nintendogs:bytes-fuzzy
    virtual void doOpen_(sead::FileHandle*, const sead::SafeStringBase<char>&, sead::FileDevice::FileOpenFlag); // 0x0054C83C slot 0x30 | nintendogs:bytes-fuzzy
    virtual void doClose_(sead::FileHandle*); // 0x0054CA34 slot 0x34 | slot vf_0x34 of sead::FileDevice
    virtual void doRead_(unsigned*, sead::FileHandle*, unsigned char*, unsigned); // 0x0054C95C slot 0x38 | slot vf_0x38 of sead::FileDevice
    virtual void doWrite_(unsigned*, sead::FileHandle*, const unsigned char*, unsigned); // 0x0054CAA8 slot 0x3C | slot vf_0x3C of sead::FileDevice
    virtual void doSeek_(sead::FileHandle*, int, sead::FileDevice::SeekOrigin); // 0x0054C9D4 slot 0x40 | nintendogs:bytes-fuzzy
    virtual void vf_0x44(); // 0x0054C6D8 slot 0x44 | virtual slot, introduced by sead::FileDevice
    virtual void doGetFileSize_(unsigned*, const sead::SafeStringBase<char>&); // 0x0054BE40 slot 0x48 | mk7dlp:bytes-fuzzy
    virtual void vf_0x4C(); // 0x0054BDE8 slot 0x4C | virtual slot, introduced by sead::FileDevice
    virtual void doIsExistFile_(bool*, const sead::SafeStringBase<char>&); // 0x0054BF40 slot 0x50 | nintendogs:bytes-fuzzy
    virtual void vf_0x54(); // 0x0054C48C slot 0x54 | virtual slot, introduced by sead::FileDevice
    virtual void vf_0x58(); // 0x0054C184 slot 0x58 | virtual slot, introduced by sead::FileDevice
    virtual void vf_0x5C(); // 0x0054C398 slot 0x5C | virtual slot, introduced by sead::FileDevice
    virtual void vf_0x60(); // 0x0054C1FC slot 0x60 | virtual slot, introduced by sead::FileDevice
    virtual void vf_0x64(); // 0x0054C0BC slot 0x64 | virtual slot, introduced by sead::FileDevice
    virtual void doGetLastRawError_() const; // 0x0074E024 slot 0x68 | slot vf_0x68 of sead::FileDevice
    virtual void vf_0x70(); // 0x0074DFD0 slot 0x70 | virtual slot, introduced by sead::FileDevice
    virtual void vf_0x74(); // 0x0011C12F slot 0x74 | slot vf_0x00 of ChangeRentalBase
};
} // namespace sead
