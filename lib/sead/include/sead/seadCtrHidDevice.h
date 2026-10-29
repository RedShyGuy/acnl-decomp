#pragma once

#include "decomp.h"
#include "nn/hid/CTR/hid_GyroscopeReader.h"
#include "nn/hid/CTR/hid_AccelerometerReader.h"
#include "nn/hid/CTR/hid_TouchPanelReader.h"
#include "nn/hid/CTR/hid_PadReader.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadControlDevice.h"

namespace sead {
// RTTI N4sead12CtrHidDeviceE @ 0x008D1444
// vtable 0x00905050 (vptr 0x00905058), offset_to_top 0, 5 entries
// The input of the 3DS: the readers of nn::hid (placed in the device) and their last states (member
// names are ours).
class CtrHidDevice : public ::sead::ControlDevice
{
public:
    explicit CtrHidDevice(ControllerMgr* mgr); // 0x00540D28 | nintendogs:bytes [tier B]
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
    virtual ~CtrHidDevice(); // 0x00540E44 slot 0x08
    // 0x00540DFC slot 0x0C (deleting dtor)
    // reads the states (the bits of mFlags tell which ones are valid)
    virtual void calc(); // 0x00540C1C slot 0x10 | nintendogs:callseq

    // the bits of mFlags
    enum Flag
    {
        FLAG_PAD = 1,
        FLAG_TOUCH_PANEL = 2,
        FLAG_ACCELEROMETER = 4,
        FLAG_GYROSCOPE = 8,
    };

    u32 mPadReaderBuffer[sizeof(nn::hid::CTR::PadReader) / 4];                     // 0x0014
    u32 mTouchPanelReaderBuffer[sizeof(nn::hid::CTR::TouchPanelReader) / 4];       // 0x0054
    u32 mAccelerometerReaderBuffer[sizeof(nn::hid::CTR::AccelerometerReader) / 4]; // 0x0064
    u32 mGyroscopeReaderBuffer[sizeof(nn::hid::CTR::GyroscopeReader) / 4];         // 0x00BC (not used here)
    nn::hid::CTR::PadReader* mPadReader;                     // 0x15BC
    nn::hid::CTR::PadStatus mPadStatus;                      // 0x15C0
    nn::hid::CTR::TouchPanelReader* mTouchPanelReader;       // 0x15D0
    nn::hid::CTR::TouchPanelStatus mTouchPanelStatus;        // 0x15D4
    nn::hid::CTR::AccelerometerReader* mAccelerometerReader; // 0x15DC
    nn::hid::CTR::AccelerometerStatus mAccelerometerStatus;  // 0x15E0
    nn::hid::CTR::AccelerationFloat mAcceleration;           // 0x15E8
    nn::hid::CTR::GyroscopeReader* mGyroscopeReader;         // 0x15F4
    nn::hid::CTR::GyroscopeStatus mGyroscopeStatus;          // 0x15F8
    u32 mFlags;                                              // 0x1634
};
ASSERT_SIZE(CtrHidDevice, 0x1638);
} // namespace sead
