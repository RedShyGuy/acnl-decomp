#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadCtrFileStreamFileDevice.h"

namespace sead {
// RTTI N4sead19CtrBackupFileDeviceE @ 0x008D1C60
// vtable 0x0090614C (vptr 0x00906154), offset_to_top 0, 30 entries
//
// The device of the save data archive. (name of getMountName is ours)
class CtrBackupFileDevice : public ::sead::CtrFileStreamFileDevice
{
public:
    // (inline: the constructor of CtrFileStreamFileDevice and the vptr)
    explicit CtrBackupFileDevice(const SafeStringBase<char>& driveName) : CtrFileStreamFileDevice(driveName) {}
    virtual ~CtrBackupFileDevice(); // 0x0054A160 slot 0x00
    // 0x0054A150 slot 0x04 (deleting dtor)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074CD30 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return CtrFileStreamFileDevice::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074CCE4 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009834EC (guard 0x009834E8)
        static const RuntimeTypeInfo::Derive<CtrFileStreamFileDevice> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual const char* getMountName() const; // 0x0074CCD4 slot 0x74
};
} // namespace sead
