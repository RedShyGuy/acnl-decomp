#include "sead/seadCtrFileStreamFileDevice.h"
#include "sead/seadCtrBackupFileDevice.h"

namespace sead {
// ctor address unknown
sead::CtrBackupFileDevice::CtrBackupFileDevice()
{
}

// 0x0054A160 slot 0x00 | slot vf_0x00 of sead::IDisposer
sead::CtrBackupFileDevice::~CtrBackupFileDevice()
{
}

// 0x0074CD30 slot 0x08 | virtual slot, introduced by sead::FileDevice
void sead::CtrBackupFileDevice::vf_0x08()
{
}

// 0x0074CCE4 slot 0x0C | slot vf_0x0C of sead::FileDevice
void sead::CtrBackupFileDevice::isMatchDevice_(const sead::HandleBase*) const
{
}

// 0x0074CCD4 slot 0x74 | virtual slot, introduced by sead::CtrFileStreamFileDevice
void sead::CtrBackupFileDevice::vf_0x74()
{
}

} // namespace sead
