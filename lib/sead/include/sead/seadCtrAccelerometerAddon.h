#pragma once

#include "decomp.h"
#include "nn/hid/CTR/hid_Types.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadAccelerometerAddon.h"

namespace sead {
// RTTI N4sead21CtrAccelerometerAddonE @ 0x008D1E84
// vtable 0x00906568 (vptr 0x00906570), offset_to_top 0, 5 entries
// The acceleration of the 3DS from the HID device (member names are ours).
class CtrAccelerometerAddon : public ::sead::AccelerometerAddon
{
public:
    explicit CtrAccelerometerAddon(Controller* controller); // 0x0054B400 | nintendogs:bytes [tier A]
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074D6D8 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return AccelerometerAddon::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074D68C (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x0098351C (guard 0x00983518)
        static const RuntimeTypeInfo::Derive<AccelerometerAddon> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~CtrAccelerometerAddon(); // 0x0054B450 slot 0x08
    // 0x0054B44C slot 0x0C (deleting dtor)
    virtual bool calc(); // 0x0054B31C slot 0x10

    nn::hid::CTR::AccelerometerStatus mStatus; // 0x24, the raw values
};
ASSERT_SIZE(CtrAccelerometerAddon, 0x2C);
} // namespace sead
