#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadCalculateTask.h"

namespace sead {
// RTTI N4sead13ControllerMgrE @ 0x008D1608
// vtable 0x009053B0 (vptr 0x009053B8), offset_to_top 0, 23 entries
class ControllerMgr : public ::sead::CalculateTask
{
public:
    ControllerMgr(); // ctor candidate(s) 0x00541DB0 (unverified)
    virtual ~ControllerMgr(); // 0x00541E54 slot 0x00 | nintendogs:bytes
    // 0x00541E14 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
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
    virtual void prepare(); // 0x00541BF0 slot 0x28 | slot vf_0x28 of sead::TaskBase
    virtual void vf_0x58(); // 0x00541B78 slot 0x58 | virtual slot, introduced by sead::CalculateTask
    void setInstance_(sead::TaskBase*); // 0x00541B54 | nintendogs:bytes [tier A]
    void getControlDevice(sead::ControllerDefine::DeviceId) const; // 0x0074B404 | nintendogs:bytes [tier A]
};
} // namespace sead
