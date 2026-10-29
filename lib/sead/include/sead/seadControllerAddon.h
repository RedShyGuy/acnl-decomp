#pragma once

#include "decomp.h"
#include "sead/seadListNode.h"
#include "sead/seadControllerDefine.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"

namespace sead {
class Controller;

// RTTI N4sead15ControllerAddonE @ 0x008D1788
// An addon of a controller (in its list; member names are ours).
class ControllerAddon
{
public:
    explicit ControllerAddon(Controller* controller) : mNode(), mController(controller) {}
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
        // 0x00983494 (guard 0x00983490)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~ControllerAddon() {}
    // reads the addon (true: the controller is not idle)
    virtual bool calc() = 0; // slot 0x10

    ListNode mNode;                    // 0x04, in Controller::mAddons
    ControllerDefine::AddonId mId;     // 0x0C
    Controller* mController;           // 0x10
};
} // namespace sead
