#pragma once

#include "decomp.h"
#include "sead/seadListNode.h"
#include "sead/seadControllerDefine.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"

namespace sead {
class ControllerMgr;

// RTTI N4sead13ControlDeviceE @ 0x008D1600
// An input device of the controller manager (in its list; member names are ours).
class ControlDevice
{
public:
    explicit ControlDevice(ControllerMgr* mgr) : mNode(), mMgr(mgr) {}
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // (name is ours)
    {
        return typeInfo == getTypeInfoStatic();
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x00983414 (guard 0x00983410)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~ControlDevice() {}
    // reads the device
    virtual void calc() = 0; // slot 0x10

    ListNode mNode;                       // 0x04, in ControllerMgr::mDevices
    ControllerDefine::DeviceId mId;       // 0x0C
    ControllerMgr* mMgr;                  // 0x10
};
} // namespace sead
