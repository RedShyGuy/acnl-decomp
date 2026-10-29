#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadController.h"
#include "sead/seadControllerMgr.h"
#include "sead/seadCtrController.h"
#include "sead/seadCtrHidDevice.h"

namespace sead {
namespace {
// sets or clears the bits of mask
inline void changeBits(u32& flags, u32 mask, bool on)
{
    if (on) {
        flags |= mask;
    } else {
        flags &= ~mask;
    }
}
} // namespace

// 0x00541E84 (name is ours)
void sead::CtrController::setNormalizeStickScaleSettings(f32 scale, s16 threshold)
{
    CtrHidDevice* device = DynamicCast<CtrHidDevice>(mMgr->getControlDevice(ControllerDefine::DEVICE_ID_CTR_HID));
    if (device != nullptr) {
        device->mPadReader->SetNormalizeStickScaleSettings(scale, threshold);
    }
}

// 0x00541F3C slot 0x18 | nintendogs:callseq
void sead::CtrController::calcImpl_()
{
    CtrHidDevice* device = DynamicCast<CtrHidDevice>(mMgr->getControlDevice(ControllerDefine::DEVICE_ID_CTR_HID));
    if (device == nullptr) {
        return;
    }
    if (device->mFlags & CtrHidDevice::FLAG_PAD) {
        // the buttons of the pad to the bits of sead
        u32 hold = device->mPadStatus.hold;
        mPadHold = 0;
        if (hold & 1) {
            mPadHold = 1;
        }
        changeBits(mPadHold, 1 << 1, (hold >> 1) & 1);
        changeBits(mPadHold, 1 << 11, (hold >> 3) & 1);
        changeBits(mPadHold, 1 << 19, (hold >> 4) & 1);
        changeBits(mPadHold, 1 << 18, (hold >> 5) & 1);
        changeBits(mPadHold, 1 << 16, (hold >> 6) & 1);
        changeBits(mPadHold, 1 << 17, (hold >> 7) & 1);
        changeBits(mPadHold, 1 << 14, (hold >> 8) & 1);
        changeBits(mPadHold, 1 << 13, (hold >> 9) & 1);
        changeBits(mPadHold, 1 << 3, (hold >> 10) & 1);
        changeBits(mPadHold, 1 << 4, (hold >> 11) & 1);
        changeBits(mPadHold, 1 << 7, (hold >> 12) & 1);
        changeBits(mPadHold, 1 << 12, (hold >> 2) & 1);
        changeBits(mPadHold, 1 << 23, (hold >> 28) & 1);
        changeBits(mPadHold, 1 << 22, (hold >> 29) & 1);
        changeBits(mPadHold, 1 << 20, (hold >> 30) & 1);
        changeBits(mPadHold, 1 << 21, hold >> 31);
        device->mPadReader->NormalizeStickWithScale(&mLeftStick.x, &mLeftStick.y, device->mPadStatus.stickX,
                                                    device->mPadStatus.stickY);
        mRightStick.x = 0.0f;
        mRightStick.y = 0.0f;
        mLeftAnalogTrigger = 0.0f;
        mRightAnalogTrigger = 0.0f;
    }
    if (device->mFlags & CtrHidDevice::FLAG_TOUCH_PANEL) {
        const nn::hid::CTR::TouchPanelStatus& touch = device->mTouchPanelStatus;
        Vector2<f32> pos(static_cast<f32>(touch.x), static_cast<f32>(touch.y));
        setPointerWithBound_(touch.touch == 1, true, pos);
    }
}

// 0x00542208 | nintendogs:bytes [tier A]
sead::CtrController::CtrController(ControllerMgr* mgr) : Controller(mgr)
{
    mId = ControllerDefine::CONTROLLER_ID_CTR;
    mLeftStickCrossStartBit = -1;
    mRightStickCrossStartBit = -1;
}

// 0x00542238 slot 0x08
// 0x00542234 (deleting dtor)
sead::CtrController::~CtrController()
{
    // empty (in the original too)
}
} // namespace sead

// the type information of the classes below Controller
// 0x00828830 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::Controller>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
