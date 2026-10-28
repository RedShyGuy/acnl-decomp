#include "sead/seadCtrFileStreamFileDevice.h"
#include "sead/seadCtrFileDevice.h"

namespace sead {
// ctor address unknown
sead::CtrFileDevice::CtrFileDevice()
{
}

// 0x0054224C slot 0x00 | slot vf_0x00 of sead::IDisposer
sead::CtrFileDevice::~CtrFileDevice()
{
}

// 0x0074B6EC slot 0x74 | virtual slot, introduced by sead::CtrFileStreamFileDevice
void sead::CtrFileDevice::vf_0x74()
{
}

} // namespace sead
