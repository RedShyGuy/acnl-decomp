#pragma once

#include "decomp.h"
#include "sead/seadVector3.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadControllerAddon.h"

namespace sead {
// RTTI N4sead18AccelerometerAddonE @ 0x008D1C2C
// The addon of the acceleration of a controller (member names are ours).
class AccelerometerAddon : public ::sead::ControllerAddon
{
public:
    explicit AccelerometerAddon(Controller* controller)
        : ControllerAddon(controller), mIsValid(false), mAcceleration(0.0f, 0.0f, 0.0f)
    {
        mId = ControllerDefine::ADDON_ID_ACCELEROMETER;
    }
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return ControllerAddon::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009834D4 (guard 0x009834D0)
        static const RuntimeTypeInfo::Derive<ControllerAddon> s_TypeInfo;
        return &s_TypeInfo;
    }

    bool mIsValid;               // 0x14
    Vector3<f32> mAcceleration;  // 0x18
};
} // namespace sead
