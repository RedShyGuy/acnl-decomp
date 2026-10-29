#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadControllerBase.h"
#include "sead/seadControllerWrapperBase.h"

#include <cstring>

namespace sead {
// 0x0054415C slot 0x14
void sead::ControllerWrapperBase::setIdle_()
{
    setIdleBase_();
}

// 0x0054B17C (name is ours)
void sead::ControllerWrapperBase::setController(Controller* controller, bool copyRepeat)
{
    if (mController != nullptr) {
        mController->mWrappers.erase(this);
        mController = nullptr;
    }
    controller->mWrappers.pushBack(this);
    mController = controller;
    if (copyRepeat) {
        std::memcpy(mPadRepeatDelays, controller->mPadRepeatDelays, sizeof(mPadRepeatDelays));
        std::memcpy(mPadRepeatPulses, controller->mPadRepeatPulses, sizeof(mPadRepeatPulses));
    }
}

// 0x0054B20C slot 0x18
bool sead::ControllerWrapperBase::isIdle_()
{
    return isIdleBase_();
}

// (out of line: the derived constructors call it)
// 0x0054B210 (name is ours)
DECOMP_NOINLINE sead::ControllerWrapperBase::ControllerWrapperBase()
    : ControllerBase(28, -1, -1, 15), IDisposer(), mController(nullptr), mIsEnable(true), mNode()
{
}

// 0x0054B2C4 slot 0x08
// 0x0054B264 (deleting dtor)
// 0x007764B4 (thunk 308)
// 0x00776458 (deleting thunk 308)
sead::ControllerWrapperBase::~ControllerWrapperBase()
{
    if (mController != nullptr) {
        mController->mWrappers.erase(this);
        mController = nullptr;
    }
}
} // namespace sead
