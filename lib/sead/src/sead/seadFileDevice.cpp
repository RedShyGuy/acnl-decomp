#include "sead/seadTListNode.h"
#include "sead/seadIDisposer.h"
#include "sead/seadFileDevice.h"

namespace sead {
// ctor address unknown
sead::FileDevice::FileDevice()
{
}

// 0x0053F01C slot 0x00 | nintendogs:bytes
sead::FileDevice::~FileDevice()
{
}

// 0x0074A19C slot 0x08 | virtual slot, introduced by sead::FileDevice
void sead::FileDevice::vf_0x08()
{
}

// 0x0074A138 slot 0x0C | slot vf_0x0C of sead::FileDevice
void sead::FileDevice::isMatchDevice_(const sead::HandleBase*) const
{
}

// 0x0074A054 slot 0x10 | virtual slot, introduced by sead::FileDevice
void sead::FileDevice::vf_0x10()
{
}

// 0x0074A184 slot 0x14 | virtual slot, introduced by sead::FileDevice
void sead::FileDevice::vf_0x14()
{
}

// 0x0074A12C slot 0x18 | virtual slot, introduced by sead::FileDevice
void sead::FileDevice::vf_0x18()
{
}

// 0x0074A190 slot 0x1C | virtual slot, introduced by sead::FileDevice
void sead::FileDevice::vf_0x1C()
{
}

// 0x0074A104 slot 0x20 | slot vf_0x20 of sead::FileDevice (was isMatchDevice_)
void sead::FileDevice::vf_0x20()
{
}

// 0x0011C12F slot 0x24 | slot vf_0x00 of ChangeRentalBase
void sead::FileDevice::doIsAvailable_() const
{
}

// 0x0053E944 slot 0x28 | slot vf_0x28 of sead::FileDevice
void sead::FileDevice::doLoad_(sead::FileDevice::LoadArg&)
{
}

// 0x0053EC10 slot 0x2C | virtual slot, introduced by sead::FileDevice
void sead::FileDevice::vf_0x2C()
{
}

// 0x0011C12F slot 0x30 | slot vf_0x00 of ChangeRentalBase
void sead::FileDevice::doOpen_(sead::FileHandle*, const sead::SafeStringBase<char>&, sead::FileDevice::FileOpenFlag)
{
}

// 0x0011C12F slot 0x34 | slot vf_0x00 of ChangeRentalBase
void sead::FileDevice::doClose_(sead::FileHandle*)
{
}

// 0x0011C12F slot 0x38 | slot vf_0x00 of ChangeRentalBase
void sead::FileDevice::doRead_(unsigned*, sead::FileHandle*, unsigned char*, unsigned)
{
}

// 0x0011C12F slot 0x3C | slot vf_0x00 of ChangeRentalBase
void sead::FileDevice::doWrite_(unsigned*, sead::FileHandle*, const unsigned char*, unsigned)
{
}

// 0x0011C12F slot 0x40 | slot vf_0x00 of ChangeRentalBase
void sead::FileDevice::doSeek_(sead::FileHandle*, int, sead::FileDevice::SeekOrigin)
{
}

// 0x0011C12F slot 0x44 | slot vf_0x00 of ChangeRentalBase
void sead::FileDevice::vf_0x44()
{
}

// 0x0011C12F slot 0x48 | slot vf_0x00 of ChangeRentalBase
void sead::FileDevice::doGetFileSize_(unsigned*, const sead::SafeStringBase<char>&)
{
}

// 0x0011C12F slot 0x4C | slot vf_0x00 of ChangeRentalBase
void sead::FileDevice::vf_0x4C()
{
}

// 0x0011C12F slot 0x50 | slot vf_0x00 of ChangeRentalBase
void sead::FileDevice::doIsExistFile_(bool*, const sead::SafeStringBase<char>&)
{
}

// 0x0011C12F slot 0x54 | slot vf_0x00 of ChangeRentalBase
void sead::FileDevice::vf_0x54()
{
}

// 0x0011C12F slot 0x58 | slot vf_0x00 of ChangeRentalBase
void sead::FileDevice::vf_0x58()
{
}

// 0x0011C12F slot 0x5C | slot vf_0x00 of ChangeRentalBase
void sead::FileDevice::vf_0x5C()
{
}

// 0x0011C12F slot 0x60 | slot vf_0x00 of ChangeRentalBase
void sead::FileDevice::vf_0x60()
{
}

// 0x0011C12F slot 0x64 | slot vf_0x00 of ChangeRentalBase
void sead::FileDevice::vf_0x64()
{
}

// 0x0011C12F slot 0x68 | slot vf_0x00 of ChangeRentalBase
void sead::FileDevice::doGetLastRawError_() const
{
}

// 0x00749FF8 slot 0x6C | virtual slot, introduced by sead::FileDevice
void sead::FileDevice::vf_0x6C()
{
}

// 0x0074A060 slot 0x70 | virtual slot, introduced by sead::FileDevice
void sead::FileDevice::vf_0x70()
{
}

// 0x00132D10 | nintendogs:callgraph [tier A]
void sead::FileDevice::tryLoad(sead::FileDevice::LoadArg&)
{
}

// 0x0053EDD8 | nintendogs:bytes-fuzzy [tier A]
void sead::FileDevice::tryRead(unsigned*, sead::FileHandle*, unsigned char*, unsigned)
{
}

// 0x0053EF58 | nintendogs:bytes-fuzzy [tier A]
void sead::FileDevice::tryWrite(unsigned*, sead::FileHandle*, const unsigned char*, unsigned)
{
}

// 0x00749FD8 | nintendogs:callgraph [tier A]
void sead::FileDevice::isAvailable() const
{
}

// 0x0074A120 | nintendogs:callgraph [tier A]
void sead::FileDevice::getLastRawError() const
{
}

} // namespace sead
