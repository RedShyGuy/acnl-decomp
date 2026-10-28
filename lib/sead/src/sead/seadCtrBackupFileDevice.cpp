#include "sead/RuntimeTypeInfo/seadDerive.h"
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

// 0x0074CCD4 slot 0x74 | virtual slot, introduced by sead::CtrFileStreamFileDevice
void sead::CtrBackupFileDevice::vf_0x74()
{
}

} // namespace sead

// the type information of the classes below CtrFileStreamFileDevice
// 0x00829380 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::CtrFileStreamFileDevice>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
