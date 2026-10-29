#include "nn/hid/CTR/hid_Api.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadControlDevice.h"
#include "sead/seadCtrHidDevice.h"

#include <new>

namespace sead {
// 0x00540C1C slot 0x10 | nintendogs:callseq
void sead::CtrHidDevice::calc()
{
    if (mPadReader->ReadLatest(&mPadStatus)) {
        mFlags |= FLAG_PAD;
    } else {
        mFlags &= ~FLAG_PAD;
    }
    if (mTouchPanelReader->ReadLatest(&mTouchPanelStatus)) {
        mFlags |= FLAG_TOUCH_PANEL;
    } else {
        mFlags &= ~FLAG_TOUCH_PANEL;
    }
    if (mAccelerometerReader != nullptr && mAccelerometerReader->ReadLatest(&mAccelerometerStatus)) {
        mFlags |= FLAG_ACCELEROMETER;
        mAccelerometerReader->ConvertToAcceleration(&mAcceleration, 1, &mAccelerometerStatus);
    } else {
        mFlags &= ~FLAG_ACCELEROMETER;
    }
    if (mGyroscopeReader != nullptr && mGyroscopeReader->ReadLatest(&mGyroscopeStatus)) {
        mFlags |= FLAG_GYROSCOPE;
    } else {
        mFlags &= ~FLAG_GYROSCOPE;
    }
}

// 0x00540D28 | nintendogs:bytes [tier B]
sead::CtrHidDevice::CtrHidDevice(ControllerMgr* mgr)
    : ControlDevice(mgr), mPadReader(nullptr), mTouchPanelReader(nullptr), mAccelerometerReader(nullptr),
      mGyroscopeReader(nullptr), mFlags(0)
{
    mId = ControllerDefine::DEVICE_ID_CTR_HID;
    nn::hid::CTR::Initialize();
    mPadReader = new (mPadReaderBuffer) nn::hid::CTR::PadReader(nn::hid::CTR::GetPad());
    mTouchPanelReader = new (mTouchPanelReaderBuffer) nn::hid::CTR::TouchPanelReader(nn::hid::CTR::GetTouchPanel());
    mAccelerometerReader =
        new (mAccelerometerReaderBuffer) nn::hid::CTR::AccelerometerReader(nn::hid::CTR::GetAccelerometer());
}

// 0x00540E44 slot 0x08
// 0x00540DFC (deleting dtor)
sead::CtrHidDevice::~CtrHidDevice()
{
    if (mAccelerometerReader != nullptr) {
        mAccelerometerReader->~AccelerometerReader();
    }
    if (mGyroscopeReader != nullptr) {
        mGyroscopeReader->~GyroscopeReader();
    }
}
} // namespace sead

// the type information of the classes below ControlDevice
// 0x00828C40 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::ControlDevice>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
