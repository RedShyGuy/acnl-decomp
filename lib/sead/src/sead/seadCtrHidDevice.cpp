#include "sead/seadControlDevice.h"
#include "sead/seadCtrHidDevice.h"

namespace sead {
// ctor candidate(s) 0x00540D28 (unverified)
sead::CtrHidDevice::CtrHidDevice()
{
}

// 0x0074A928 slot 0x00 | virtual slot, introduced by sead::CtrHidDevice
void sead::CtrHidDevice::vf_0x00()
{
}

// 0x0074A8DC slot 0x04 | virtual slot, introduced by sead::CtrHidDevice
void sead::CtrHidDevice::vf_0x04()
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
