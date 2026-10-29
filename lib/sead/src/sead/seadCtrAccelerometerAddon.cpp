#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadAccelerometerAddon.h"
#include "sead/seadController.h"
#include "sead/seadControllerMgr.h"
#include "sead/seadCtrAccelerometerAddon.h"
#include "sead/seadCtrHidDevice.h"

namespace sead {
// 0x0054B31C slot 0x10
bool sead::CtrAccelerometerAddon::calc()
{
    // (no null test in the original)
    CtrHidDevice* device =
        DynamicCast<CtrHidDevice>(mController->mMgr->getControlDevice(ControllerDefine::DEVICE_ID_CTR_HID));
    bool isValid = (device->mFlags & CtrHidDevice::FLAG_ACCELEROMETER) != 0;
    if (isValid) {
        mAcceleration.x = device->mAcceleration.x;
        mAcceleration.y = device->mAcceleration.y;
        mAcceleration.z = device->mAcceleration.z;
        mStatus = device->mAccelerometerStatus;
    }
    mIsValid = isValid;
    return false;
}

// 0x0054B400 | nintendogs:bytes [tier A]
sead::CtrAccelerometerAddon::CtrAccelerometerAddon(Controller* controller) : AccelerometerAddon(controller)
{
    mStatus.x = 0;
    mStatus.y = 0;
    mStatus.z = 0;
}

// 0x0054B450 slot 0x08
// 0x0054B44C (deleting dtor)
sead::CtrAccelerometerAddon::~CtrAccelerometerAddon()
{
    // empty (in the original too)
}
} // namespace sead

// the type information of the classes below AccelerometerAddon
// 0x0082913C (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::AccelerometerAddon>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
