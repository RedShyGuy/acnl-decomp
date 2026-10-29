#pragma once

#include "decomp.h"
#include "sead/seadListImpl.h"
#include "sead/seadControllerDefine.h"
#include "sead/seadControllerAddon.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadControllerBase.h"

namespace sead {
class ControllerMgr;
class ControllerWrapperBase;

// RTTI N4sead10ControllerE @ 0x008D1358
// vtable 0x00904E0C (vptr 0x00904E14), offset_to_top 0, 9 entries
// A controller of the manager: calcImpl_ reads the device, then the addons and the wrappers follow
// (member names and the unnamed slots are ours).
class Controller : public ::sead::ControllerBase
{
public:
    explicit Controller(ControllerMgr* mgr); // 0x0053E5D0
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x00749F1C (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return ControllerBase::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x00749ED0 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009833A4 (guard 0x009833A0)
        static const RuntimeTypeInfo::Derive<ControllerBase> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~Controller(); // 0x0053E640 slot 0x08
    // 0x0053E63C slot 0x0C (deleting dtor)
    virtual void calc(); // 0x0053E490 slot 0x10
    virtual bool isConnected() const { return true; } // 0x00749EC8 slot 0x14
    // reads the state of the device
    virtual void calcImpl_() = 0; // slot 0x18
    // (the original falls into isIdleBase_ with a nop)
    virtual bool isIdle_(); // 0x005440B8 slot 0x1C
    virtual void setIdle_(); // 0x0053E584 slot 0x20

    ControllerDefine::ControllerId mId;               // 0x134
    ControllerMgr* mMgr;                              // 0x138
    OffsetList<ControllerAddon> mAddons;              // 0x13C
    OffsetList<ControllerWrapperBase> mWrappers;      // 0x14C
};
ASSERT_SIZE(Controller, 0x15C);
} // namespace sead
