#include "sead/seadCtrFileStreamFileDevice.h"
#include "Other/dPatchFileDevice.h"

// ctor address unknown
PatchFileDevice::PatchFileDevice(const sead::SafeStringBase<char>& driveName) : CtrFileStreamFileDevice(driveName)
{
}

// 0x0053F018 slot 0x00 | slot vf_0x00 of sead::IDisposer
PatchFileDevice::~PatchFileDevice()
{
}

// 0x0071CB04 slot 0x08
bool PatchFileDevice::isKindOf(const sead::RuntimeTypeInfo::Interface*) const
{
}

// 0x0071CAB8 slot 0x0C
const sead::RuntimeTypeInfo::Interface* PatchFileDevice::getTypeInfo() const
{
}

// 0x0071CAA8 slot 0x74 (name is ours)
const char* PatchFileDevice::getMountName() const
{
    return "patch";
}
