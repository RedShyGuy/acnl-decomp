#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadControlDevice.h"
#include "sead/seadCtrHidDevice.h"

namespace sead {
// ctor candidate(s) 0x00540D28 (unverified)
sead::CtrHidDevice::CtrHidDevice()
{
}

// 0x00540E44 slot 0x08 | virtual slot, introduced by sead::CtrHidDevice
void sead::CtrHidDevice::vf_0x08()
{
}

// 0x00540DFC slot 0x0C | virtual slot, introduced by sead::CtrHidDevice
void sead::CtrHidDevice::vf_0x0C()
{
}

// 0x00540C1C slot 0x10 | nintendogs:callseq
void sead::CtrHidDevice::vf_0x10()
{
}

// 0x00540D28 | nintendogs:bytes [tier B]
sead::CtrHidDevice::CtrHidDevice(sead::ControllerMgr*)
{
}

} // namespace sead

// the type information of the classes below ControlDevice
// 0x00828C40 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::ControlDevice>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
