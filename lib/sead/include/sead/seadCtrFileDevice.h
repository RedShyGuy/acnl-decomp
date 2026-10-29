#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadCtrFileStreamFileDevice.h"

namespace sead {
// RTTI N4sead13CtrFileDeviceE @ 0x008D1620
// vtable 0x00905440 (vptr 0x00905448), offset_to_top 0, 30 entries
//
// The device of the rom archive. (name of getMountName is ours)
class CtrFileDevice : public ::sead::CtrFileStreamFileDevice
{
public:
    // (inline: the constructor of CtrFileStreamFileDevice and the vptr)
    explicit CtrFileDevice(const SafeStringBase<char>& driveName) : CtrFileStreamFileDevice(driveName) {}
    virtual ~CtrFileDevice(); // 0x0054224C slot 0x00
    // 0x0054223C slot 0x04 (deleting dtor)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074B744 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return CtrFileStreamFileDevice::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074B6F8 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x00983434 (guard 0x00983430)
        static const RuntimeTypeInfo::Derive<CtrFileStreamFileDevice> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual const char* getMountName() const; // 0x0074B6EC slot 0x74
};
} // namespace sead
