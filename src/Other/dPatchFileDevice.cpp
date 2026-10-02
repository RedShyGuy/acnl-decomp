#include "sead/seadCtrFileStreamFileDevice.h"
#include "Other/dPatchFileDevice.h"

// ctor address unknown
PatchFileDevice::PatchFileDevice()
{
}

// 0x0053F018 slot 0x00 | slot vf_0x00 of sead::IDisposer
PatchFileDevice::~PatchFileDevice()
{
}

// 0x0071CB04 slot 0x08 | virtual slot, introduced by sead::FileDevice
void PatchFileDevice::vf_0x08()
{
}

// 0x0071CAB8 slot 0x0C | slot vf_0x0C of sead::FileDevice
void PatchFileDevice::isMatchDevice_(const sead::HandleBase*) const
{
}

// 0x0071CAA8 slot 0x74 | virtual slot, introduced by sead::CtrFileStreamFileDevice
void PatchFileDevice::vf_0x74()
{
}

