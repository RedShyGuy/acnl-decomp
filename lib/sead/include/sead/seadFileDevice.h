#pragma once

#include "decomp.h"
#include "sead/seadIDisposer.h"
#include "sead/seadTListNode.h"

namespace sead {
// RTTI N4sead10FileDeviceE @ 0x008D1364
// vtable 0x00904E38 (vptr 0x00904E40), offset_to_top 0, 29 entries
class FileDevice : public ::sead::TListNode<sead::FileDevice*>, public ::sead::IDisposer
{
public:
    struct FileOpenFlag { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct LoadArg { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct SeekOrigin { u32 _unknown; }; // TODO: real type unknown (placeholder)
    FileDevice(); // ctor address unknown
    virtual ~FileDevice(); // 0x0053F01C slot 0x00 | nintendogs:bytes
    // 0x0053EFDC slot 0x04 | nintendogs:bytes (deleting dtor)
    virtual void vf_0x08(); // 0x0074A19C slot 0x08 | virtual slot, introduced by sead::FileDevice
    virtual void isMatchDevice_(const sead::HandleBase*) const; // 0x0074A138 slot 0x0C | slot vf_0x0C of sead::FileDevice
    virtual void vf_0x10(); // 0x0074A054 slot 0x10 | virtual slot, introduced by sead::FileDevice
    virtual void vf_0x14(); // 0x0074A184 slot 0x14 | virtual slot, introduced by sead::FileDevice
    virtual void vf_0x18(); // 0x0074A12C slot 0x18 | virtual slot, introduced by sead::FileDevice
    virtual void vf_0x1C(); // 0x0074A190 slot 0x1C | virtual slot, introduced by sead::FileDevice
    virtual void vf_0x20(); // 0x0074A104 slot 0x20 | slot vf_0x20 of sead::FileDevice (was isMatchDevice_)
    virtual void doIsAvailable_() const; // 0x0011C12F slot 0x24 | slot vf_0x00 of ChangeRentalBase
    virtual void doLoad_(sead::FileDevice::LoadArg&); // 0x0053E944 slot 0x28 | slot vf_0x28 of sead::FileDevice
    virtual void vf_0x2C(); // 0x0053EC10 slot 0x2C | virtual slot, introduced by sead::FileDevice
    virtual void doOpen_(sead::FileHandle*, const sead::SafeStringBase<char>&, sead::FileDevice::FileOpenFlag); // 0x0011C12F slot 0x30 | slot vf_0x00 of ChangeRentalBase
    virtual void doClose_(sead::FileHandle*); // 0x0011C12F slot 0x34 | slot vf_0x00 of ChangeRentalBase
    virtual void doRead_(unsigned*, sead::FileHandle*, unsigned char*, unsigned); // 0x0011C12F slot 0x38 | slot vf_0x00 of ChangeRentalBase
    virtual void doWrite_(unsigned*, sead::FileHandle*, const unsigned char*, unsigned); // 0x0011C12F slot 0x3C | slot vf_0x00 of ChangeRentalBase
    virtual void doSeek_(sead::FileHandle*, int, sead::FileDevice::SeekOrigin); // 0x0011C12F slot 0x40 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x44(); // 0x0011C12F slot 0x44 | slot vf_0x00 of ChangeRentalBase
    virtual void doGetFileSize_(unsigned*, const sead::SafeStringBase<char>&); // 0x0011C12F slot 0x48 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x4C(); // 0x0011C12F slot 0x4C | slot vf_0x00 of ChangeRentalBase
    virtual void doIsExistFile_(bool*, const sead::SafeStringBase<char>&); // 0x0011C12F slot 0x50 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x54(); // 0x0011C12F slot 0x54 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x58(); // 0x0011C12F slot 0x58 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x5C(); // 0x0011C12F slot 0x5C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x60(); // 0x0011C12F slot 0x60 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x64(); // 0x0011C12F slot 0x64 | slot vf_0x00 of ChangeRentalBase
    virtual void doGetLastRawError_() const; // 0x0011C12F slot 0x68 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x6C(); // 0x00749FF8 slot 0x6C | virtual slot, introduced by sead::FileDevice
    virtual void vf_0x70(); // 0x0074A060 slot 0x70 | virtual slot, introduced by sead::FileDevice
    void tryLoad(sead::FileDevice::LoadArg&); // 0x00132D10 | nintendogs:callgraph [tier A]
    void tryRead(unsigned*, sead::FileHandle*, unsigned char*, unsigned); // 0x0053EDD8 | nintendogs:bytes-fuzzy [tier A]
    void tryWrite(unsigned*, sead::FileHandle*, const unsigned char*, unsigned); // 0x0053EF58 | nintendogs:bytes-fuzzy [tier A]
    void isAvailable() const; // 0x00749FD8 | nintendogs:callgraph [tier A]
    void getLastRawError() const; // 0x0074A120 | nintendogs:callgraph [tier A]
};
} // namespace sead
