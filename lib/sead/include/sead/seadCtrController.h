#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadController.h"

namespace sead {
// RTTI N4sead13CtrControllerE @ 0x008D1614
// vtable 0x00905414 (vptr 0x0090541C), offset_to_top 0, 9 entries
class CtrController : public ::sead::Controller
{
public:
    CtrController(); // ctor candidate(s) 0x00542208 (unverified)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074B5E8 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return Controller::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074B59C (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x0098342C (guard 0x00983428)
        static const RuntimeTypeInfo::Derive<Controller> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual void vf_0x08(); // 0x00542238 slot 0x08 | virtual slot, introduced by sead::Controller
    virtual void vf_0x0C(); // 0x00542234 slot 0x0C | virtual slot, introduced by sead::Controller
    virtual void vf_0x18(); // 0x00541F3C slot 0x18 | nintendogs:callseq
    CtrController(sead::ControllerMgr*); // 0x00542208 | nintendogs:bytes [tier A]
};
} // namespace sead
