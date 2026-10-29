#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadCalculateTask.h"
#include "sead/seadController.h"
#include "sead/seadControllerMgr.h"
#include "sead/seadCtrAccelerometerAddon.h"
#include "sead/seadCtrController.h"
#include "sead/seadCtrHidDevice.h"
#include "sead/seadNew.h"
#include "sead/seadTaskBase_CreateArg.h"

namespace sead {
// 0x009762F4
ControllerMgr* ControllerMgr::s_pInstance = nullptr;

// 0x00541B54 | nintendogs:bytes [tier A]
void sead::ControllerMgr::setInstance_(TaskBase* task)
{
    if (s_pInstance == nullptr) {
        s_pInstance = static_cast<ControllerMgr*>(task);
        s_pInstance->mIsInstance = true;
    }
}

// 0x00541B78 slot 0x58
void sead::ControllerMgr::calc()
{
    for (ControlDevice* device = mDevices.front(); device != mDevices.endObj(); device = mDevices.next(device)) {
        device->calc();
    }
    for (Controller** it = mControllers.begin(); it != mControllers.end(); it++) {
        (*it)->calc();
    }
}

// 0x00541BF0 slot 0x28 | slot vf_0x28 of sead::TaskBase
void sead::ControllerMgr::prepare()
{
    Parameter* parameter = DynamicCast<Parameter>(mParameter);
    if (parameter != nullptr) {
        mControllers.allocBuffer(parameter->controllerMax, nullptr, 4);
        if (parameter->proc != nullptr) {
            parameter->proc->invoke(this);
        }
        return;
    }
    mControllers.allocBuffer(1, nullptr, 4);
    mDevices.pushBack(new (nullptr, 4) CtrHidDevice(this));
    CtrController* controller = new (nullptr, 4) CtrController(this);
    mControllers.pushBack(controller);
    controller->mAddons.pushBack(new (nullptr, 4) CtrAccelerometerAddon(controller));
}

// 0x00541DAC
DECOMP_NOINLINE sead::ControllerMgr::ControllerMgr(const TaskConstructArg& arg)
    : CalculateTask(arg, "sead::ControllerMgr"), mIsInstance(false), mDevices(), mControllers()
{
    mDevices.initOffset(offsetof(ControlDevice, mNode));
}

// 0x00541E54 slot 0x00 | nintendogs:bytes
// 0x00541E14 (deleting dtor)
sead::ControllerMgr::~ControllerMgr()
{
    if (mIsInstance) {
        s_pInstance = nullptr;
    }
}
} // namespace sead

// the factory of the controller manager (name is ours)
// 0x00541D80
template sead::TaskBase* sead::TaskFactory<sead::ControllerMgr>(const sead::TaskConstructArg&);


// the type information of the classes below CalculateTask
// 0x00828BCC (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::CalculateTask>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
