#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadController.h"
#include "sead/seadControllerBase.h"
#include "sead/seadControllerWrapperBase.h"

namespace sead {
// 0x0053E490 slot 0x10
void sead::Controller::calc()
{
    u32 prevHold = mPadHold;
    bool prevPointerOn = mPointerFlag & POINTER_ON;
    calcImpl_();
    updateDerivativeParams_(prevHold, prevPointerOn);
    bool isIdle = true;
    for (ControllerAddon* addon = mAddons.front(); addon != mAddons.endObj(); addon = mAddons.next(addon)) {
        if (addon->calc()) {
            isIdle = false;
        }
    }
    if (isIdle && isIdle_()) {
        mIdleFrame++;
    } else {
        mIdleFrame = 0;
    }
    for (ControllerWrapperBase* wrapper = mWrappers.front(); wrapper != mWrappers.endObj();
         wrapper = mWrappers.next(wrapper)) {
        wrapper->calc(prevHold, prevPointerOn);
    }
}

// 0x0053E584 slot 0x20
void sead::Controller::setIdle_()
{
    setIdleBase_();
    for (ControllerWrapperBase* wrapper = mWrappers.front(); wrapper != mWrappers.endObj();
         wrapper = mWrappers.next(wrapper)) {
        wrapper->setIdle_();
    }
}

// (out of line: the derived constructors call it)
// 0x0053E5D0
DECOMP_NOINLINE sead::Controller::Controller(ControllerMgr* mgr)
    : ControllerBase(28, 20, 24, 15), mId(ControllerDefine::CONTROLLER_ID_NULL), mMgr(mgr), mAddons(), mWrappers()
{
    mAddons.initOffset(offsetof(ControllerAddon, mNode));
    mWrappers.initOffset(offsetof(ControllerWrapperBase, mNode));
}

// 0x0053E640 slot 0x08
// 0x0053E63C (deleting dtor)
sead::Controller::~Controller()
{
    // empty (in the original too)
}

// 0x005440B8 slot 0x1C
bool sead::Controller::isIdle_()
{
    return isIdleBase_();
}
} // namespace sead

// the type information of the classes below ControllerBase
// 0x00828E10 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::ControllerBase>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
