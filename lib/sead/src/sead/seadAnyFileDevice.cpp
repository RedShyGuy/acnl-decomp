#include "sead/seadFileDevice.h"
#include "sead/seadAnyFileDevice.h"

namespace sead {
// ctor address unknown
sead::AnyFileDevice::AnyFileDevice()
{
}

// 0x00541754 slot 0x00 | nintendogs:bytes
sead::AnyFileDevice::~AnyFileDevice()
{
}

// 0x0074B044 slot 0x08 | virtual slot, introduced by sead::FileDevice
void sead::AnyFileDevice::vf_0x08()
{
}

// 0x0074AEA4 slot 0x0C | slot vf_0x0C of sead::FileDevice
void sead::AnyFileDevice::isMatchDevice_(const sead::HandleBase*) const
{
}

// 0x0074AD94 slot 0x10 | virtual slot, introduced by sead::FileDevice
void sead::AnyFileDevice::vf_0x10()
{
}

// 0x0074AEF0 slot 0x14 | virtual slot, introduced by sead::FileDevice
void sead::AnyFileDevice::vf_0x14()
{
}

// 0x0074AE58 slot 0x18 | virtual slot, introduced by sead::FileDevice
void sead::AnyFileDevice::vf_0x18()
{
}

// 0x0074AF1C slot 0x1C | virtual slot, introduced by sead::FileDevice
void sead::AnyFileDevice::vf_0x1C()
{
}

// 0x0074AE04 slot 0x20 | nintendogs:bytes-fuzzy (was isMatchDevice_)
void sead::AnyFileDevice::vf_0x20()
{
}

// 0x0074ADC0 slot 0x24 | nintendogs:bytes
void sead::AnyFileDevice::doIsAvailable_() const
{
}

// 0x0054167C slot 0x28 | nintendogs:bytes
void sead::AnyFileDevice::doLoad_(sead::FileDevice::LoadArg&)
{
}

// 0x005416A8 slot 0x30 | nintendogs:bytes-fuzzy
void sead::AnyFileDevice::doOpen_(sead::FileHandle*, const sead::SafeStringBase<char>&, sead::FileDevice::FileOpenFlag)
{
}

// 0x0013F654 slot 0x34 | slot vf_0x34 of sead::FileDevice
void sead::AnyFileDevice::doClose_(sead::FileHandle*)
{
}

// 0x005416EC slot 0x38 | slot vf_0x38 of sead::FileDevice
void sead::AnyFileDevice::doRead_(unsigned*, sead::FileHandle*, unsigned char*, unsigned)
{
}

// 0x0054170C slot 0x3C | slot vf_0x3C of sead::FileDevice
void sead::AnyFileDevice::doWrite_(unsigned*, sead::FileHandle*, const unsigned char*, unsigned)
{
}

// 0x0053EEE0 slot 0x40 | slot vf_0x40 of sead::FileDevice
void sead::AnyFileDevice::doSeek_(sead::FileHandle*, int, sead::FileDevice::SeekOrigin)
{
}

// 0x0053E87C slot 0x44 | virtual slot, introduced by sead::FileDevice
void sead::AnyFileDevice::vf_0x44()
{
}

// 0x0054133C slot 0x48 | mk7dlp:bytes
void sead::AnyFileDevice::doGetFileSize_(unsigned*, const sead::SafeStringBase<char>&)
{
}

// 0x0053E644 slot 0x4C | virtual slot, introduced by sead::FileDevice
void sead::AnyFileDevice::vf_0x4C()
{
}

// 0x00541374 slot 0x50 | slot vf_0x50 of sead::FileDevice
void sead::AnyFileDevice::doIsExistFile_(bool*, const sead::SafeStringBase<char>&)
{
}

// 0x0054164C slot 0x54 | virtual slot, introduced by sead::FileDevice
void sead::AnyFileDevice::vf_0x54()
{
}

// 0x005413AC slot 0x58 | virtual slot, introduced by sead::FileDevice
void sead::AnyFileDevice::vf_0x58()
{
}

// 0x00541620 slot 0x5C | virtual slot, introduced by sead::FileDevice
void sead::AnyFileDevice::vf_0x5C()
{
}

// 0x005414BC slot 0x60 | virtual slot, introduced by sead::FileDevice
void sead::AnyFileDevice::vf_0x60()
{
}

// 0x005413A4 slot 0x64 | virtual slot, introduced by sead::FileDevice
void sead::AnyFileDevice::vf_0x64()
{
}

// 0x0074AE90 slot 0x68 | nintendogs:bytes
void sead::AnyFileDevice::doGetLastRawError_() const
{
}

// 0x0011D6F0 | nintendogs:bytes [tier A]
void sead::AnyFileDevice::pushBack(sead::FileDevice*)
{
}

// 0x0074AF54 | nintendogs:callgraph [tier A]
void sead::AnyFileDevice::findFileDeviceByFile_(const sead::SafeStringBase<char>&)
{
}

} // namespace sead
