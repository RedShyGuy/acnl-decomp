#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadFramework.h"

namespace sead {
// RTTI N4sead13GameFrameworkE @ 0x008D1668
// vtable 0x0090557C (vptr 0x00905584), offset_to_top 0, 23 entries
class GameFramework : public ::sead::Framework
{
public:
    GameFramework(); // ctor address unknown
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074B89C (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return Framework::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074B850 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x0098343C (guard 0x00983438)
        static const RuntimeTypeInfo::Derive<Framework> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~GameFramework(); // 0x00543170 slot 0x08 | slot vf_0x08 of sead::Framework
    // 0x00543110 slot 0x0C | slot vf_0x0C of sead::Framework (deleting dtor)
    virtual void vf_0x14(); // 0x00542ED4 slot 0x14 | virtual slot, introduced by sead::Framework
    virtual void createControllerMgr(sead::TaskBase*); // 0x00542F78 slot 0x38 | nintendogs:callseq
    virtual void vf_0x3C(); // 0x00542ECC slot 0x3C | virtual slot, introduced by sead::GameFramework
    virtual void vf_0x40(); // 0x0054102C slot 0x40 | virtual slot, introduced by sead::GameFramework
    virtual void vf_0x44(); // 0x00542ED0 slot 0x44 | virtual slot, introduced by sead::GameFramework
    virtual void vf_0x48(); // 0x00543040 slot 0x48 | virtual slot, introduced by sead::GameFramework
    virtual void vf_0x4C(); // 0x0011C12F slot 0x4C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x50(); // 0x00542EC8 slot 0x50 | virtual slot, introduced by sead::GameFramework
    virtual void vf_0x54(); // 0x0074B848 slot 0x54 | virtual slot, introduced by sead::GameFramework
    virtual void waitStartDisplayLoop_(); // 0x00543048 slot 0x58 | nintendogs:bytes-fuzzy
};
} // namespace sead
