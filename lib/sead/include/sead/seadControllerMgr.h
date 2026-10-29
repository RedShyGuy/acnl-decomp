#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadCalculateTask.h"
#include "sead/seadControlDevice.h"
#include "sead/seadControllerDefine.h"
#include "sead/seadIDelegate1.h"
#include "sead/seadListImpl.h"
#include "sead/seadPtrArrayImpl.h"
#include "sead/seadTaskParameter.h"

namespace sead {
// RTTI N4sead13ControllerMgrE @ 0x008D1608
// vtable 0x009053B0 (vptr 0x009053B8), offset_to_top 0, 23 entries
// The controllers and their devices: calc reads the devices, then the controllers. Without a
// Parameter it makes the HID device and a CtrController with the accelerometer (member names and
// Parameter are ours).
class ControllerMgr : public ::sead::CalculateTask
{
public:
    // what the controller manager is made with (as the TaskParameter of its task)
    class Parameter : public TaskParameter
    {
    public:
        virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const
        {
            if (typeInfo == getTypeInfoStatic()) {
                return true;
            }
            return TaskParameter::isKindOf(typeInfo);
        }
        virtual const RuntimeTypeInfo::Interface* getTypeInfo() const { return getTypeInfoStatic(); }
        // the type information of the class (inline everywhere; name is ours)
        static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
        {
            // 0x00983424 (guard 0x00983420)
            static const RuntimeTypeInfo::Derive<TaskParameter> s_TypeInfo;
            return &s_TypeInfo;
        }

        s32 controllerMax;                     // 0x4
        IDelegate1<ControllerMgr*>* proc;      // 0x8, makes the devices and controllers
    };

    explicit ControllerMgr(const TaskConstructArg& arg); // 0x00541DAC
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074B498 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return CalculateTask::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074B44C (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x0098341C (guard 0x00983418)
        static const RuntimeTypeInfo::Derive<CalculateTask> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~ControllerMgr(); // 0x00541E54 slot 0x00 | nintendogs:bytes
    // 0x00541E14 slot 0x04 (deleting dtor)
    // makes the devices and the controllers
    virtual void prepare(); // 0x00541BF0 slot 0x28 | slot vf_0x28 of sead::TaskBase
    // reads the devices, then the controllers
    virtual void calc(); // 0x00541B78 slot 0x58

    // the instance callback of the task
    static void setInstance_(TaskBase* task); // 0x00541B54 | nintendogs:bytes [tier A]
    // the device of id (NULL if there is none)
    DECOMP_NOINLINE ControlDevice* getControlDevice(ControllerDefine::DeviceId id) const // 0x0074B404 | nintendogs:bytes [tier A]
    {
        for (ControlDevice* device = mDevices.front(); device != mDevices.endObj(); device = mDevices.next(device)) {
            if (device->mId == id) {
                return device;
            }
        }
        return nullptr;
    }

    // the manager (name is ours)
    static ControllerMgr* s_pInstance;

    bool mIsInstance;                   // 0xBC
    OffsetList<ControlDevice> mDevices; // 0xC0
    PtrArray<Controller> mControllers;  // 0xD0
};
ASSERT_SIZE(ControllerMgr, 0xDC);
} // namespace sead
