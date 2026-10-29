#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadController.h"
#include "sead/seadControllerWrapper.h"
#include "sead/seadControllerWrapperBase.h"

#include <cstring>

namespace sead {
// 0x008A4B58
const u8 ControllerWrapper::s_DefaultPadConfig[28] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13,
                                                      14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27};

// 0x00548CA0 slot 0x10
void sead::ControllerWrapper::calc(u32 prevHold, bool prevPointerOn)
{
    if (mIsEnable && mController != nullptr && mController->isConnected()) {
        u32 hold = 0;
        for (s32 i = 0; i < mPadBitMax; i++) {
            if (mController->mPadHold & (1 << mPadConfig[i])) {
                hold |= 1 << i;
            }
        }
        mPadHold = hold;
        mLeftStick = mController->mLeftStick;
        mRightStick = mController->mRightStick;
        mLeftAnalogTrigger = mController->mLeftAnalogTrigger;
        mRightAnalogTrigger = mController->mRightAnalogTrigger;
        bool isTouchKeyHold = mTouchKeyBit >= 0 && (mPadHold & (1 << mTouchKeyBit)) != 0;
        setPointerWithBound_((mController->mPointerFlag & POINTER_ON) != 0, isTouchKeyHold, mController->mPointer);
        u32 prev = 0;
        for (s32 i = 0; i < mPadBitMax; i++) {
            if (prevHold & (1 << mPadConfig[i])) {
                prev |= 1 << i;
            }
        }
        updateDerivativeParams_(prev, prevPointerOn);
    } else {
        setIdle_();
    }
    if (isIdle_()) {
        mIdleFrame++;
    } else {
        mIdleFrame = 0;
    }
}

// 0x00548E10 (name is ours)
sead::ControllerWrapper::ControllerWrapper() : ControllerWrapperBase()
{
    std::memcpy(mPadConfig, s_DefaultPadConfig, sizeof(mPadConfig));
}

// 0x0054B2C0 slot 0x08
// 0x00548E4C (deleting dtor)
// 0x00776450 (thunk 308)
// 0x0077643C (deleting thunk 308)
sead::ControllerWrapper::~ControllerWrapper()
{
    // empty (in the original too)
}
} // namespace sead

// the type information of the classes below ControllerWrapperBase
// 0x0082930C (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::ControllerWrapperBase>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
