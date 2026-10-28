#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadControlDevice.h"

namespace sead {
// RTTI N4sead12CtrHidDeviceE @ 0x008D1444
// vtable 0x00905050 (vptr 0x00905058), offset_to_top 0, 5 entries
class CtrHidDevice : public ::sead::ControlDevice
{
public:
    CtrHidDevice(); // ctor candidate(s) 0x00540D28 (unverified)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074A928 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return ControlDevice::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074A8DC (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009833E4 (guard 0x009833E0)
        static const RuntimeTypeInfo::Derive<ControlDevice> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual void vf_0x08(); // 0x00540E44 slot 0x08 | virtual slot, introduced by sead::CtrHidDevice
    virtual void vf_0x0C(); // 0x00540DFC slot 0x0C | virtual slot, introduced by sead::CtrHidDevice
    virtual void vf_0x10(); // 0x00540C1C slot 0x10 | nintendogs:callseq
    CtrHidDevice(sead::ControllerMgr*); // 0x00540D28 | nintendogs:bytes [tier B]
};
} // namespace sead
