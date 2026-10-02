#include "sead/seadFileDevice.h"
#include "sead/seadMainFileDevice.h"

namespace sead {
// ctor address unknown
sead::MainFileDevice::MainFileDevice()
{
}

// 0x005450CC slot 0x00 | nintendogs:bytes
sead::MainFileDevice::~MainFileDevice()
{
}

// 0x0074C224 slot 0x08 | virtual slot, introduced by sead::FileDevice
void sead::MainFileDevice::vf_0x08()
{
}

// 0x0074C1B8 slot 0x0C | slot vf_0x0C of sead::FileDevice
void sead::MainFileDevice::isMatchDevice_(const sead::HandleBase*) const
{
}

// 0x0074C188 slot 0x10 | virtual slot, introduced by sead::FileDevice
void sead::MainFileDevice::vf_0x10()
{
}

// 0x0074C204 slot 0x14 | virtual slot, introduced by sead::FileDevice
void sead::MainFileDevice::vf_0x14()
{
}

// 0x0074C1A8 slot 0x18 | virtual slot, introduced by sead::FileDevice
void sead::MainFileDevice::vf_0x18()
{
}

// 0x0074C214 slot 0x1C | virtual slot, introduced by sead::FileDevice
void sead::MainFileDevice::vf_0x1C()
{
}

// 0x0074C198 slot 0x20 | slot vf_0x20 of sead::FileDevice (was isMatchDevice_)
void sead::MainFileDevice::vf_0x20()
{
}

// 0x00749FD0 slot 0x24 | slot vf_0x24 of sead::FileDevice
void sead::MainFileDevice::doIsAvailable_() const
{
}

// 0x00545028 slot 0x30 | slot vf_0x30 of sead::FileDevice
void sead::MainFileDevice::doOpen_(sead::FileHandle*, const sead::SafeStringBase<char>&, sead::FileDevice::FileOpenFlag)
{
}

// 0x00545060 slot 0x34 | slot vf_0x34 of sead::FileDevice
void sead::MainFileDevice::doClose_(sead::FileHandle*)
{
}

// 0x00545040 slot 0x38 | slot vf_0x38 of sead::FileDevice
void sead::MainFileDevice::doRead_(unsigned*, sead::FileHandle*, unsigned char*, unsigned)
{
}

// 0x00545068 slot 0x3C | slot vf_0x3C of sead::FileDevice
void sead::MainFileDevice::doWrite_(unsigned*, sead::FileHandle*, const unsigned char*, unsigned)
{
}

// 0x00545058 slot 0x40 | slot vf_0x40 of sead::FileDevice
void sead::MainFileDevice::doSeek_(sead::FileHandle*, int, sead::FileDevice::SeekOrigin)
{
}

// 0x00545020 slot 0x44 | virtual slot, introduced by sead::FileDevice
void sead::MainFileDevice::vf_0x44()
{
}

// 0x0053E67C slot 0x48 | slot vf_0x48 of sead::FileDevice
void sead::MainFileDevice::doGetFileSize_(unsigned*, const sead::SafeStringBase<char>&)
{
}

// 0x00545000 slot 0x4C | virtual slot, introduced by sead::FileDevice
void sead::MainFileDevice::vf_0x4C()
{
}

// 0x0053E6A8 slot 0x50 | slot vf_0x50 of sead::FileDevice
void sead::MainFileDevice::doIsExistFile_(bool*, const sead::SafeStringBase<char>&)
{
}

// 0x0053E850 slot 0x54 | virtual slot, introduced by sead::FileDevice
void sead::MainFileDevice::vf_0x54()
{
}

// 0x0053E6FC slot 0x58 | virtual slot, introduced by sead::FileDevice
void sead::MainFileDevice::vf_0x58()
{
}

// 0x0053E7E0 slot 0x5C | virtual slot, introduced by sead::FileDevice
void sead::MainFileDevice::vf_0x5C()
{
}

// 0x00545008 slot 0x60 | virtual slot, introduced by sead::FileDevice
void sead::MainFileDevice::vf_0x60()
{
}

// 0x0053E6D4 slot 0x64 | virtual slot, introduced by sead::FileDevice
void sead::MainFileDevice::vf_0x64()
{
}

// 0x0074A118 slot 0x68 | slot vf_0x68 of sead::FileDevice
void sead::MainFileDevice::doGetLastRawError_() const
{
}

} // namespace sead
