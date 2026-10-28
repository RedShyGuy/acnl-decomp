#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadDualScreenMethodTreeMgr.h"

namespace sead {
// RTTI N4sead17UlcdMethodTreeMgrE @ 0x008D1C20
// vtable 0x00906080 (vptr 0x00906088), offset_to_top 0, 8 entries
class UlcdMethodTreeMgr : public ::sead::DualScreenMethodTreeMgr
{
public:
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074C9BC (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return DualScreenMethodTreeMgr::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074C970 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009834CC (guard 0x009834C8)
        static const RuntimeTypeInfo::Derive<DualScreenMethodTreeMgr> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~UlcdMethodTreeMgr(); // 0x005492A0 slot 0x08 | slot vf_0x08 of sead::MethodTreeMgr
    // 0x0054921C slot 0x0C | slot vf_0x0C of sead::MethodTreeMgr (deleting dtor)
    virtual void attachMethod(int, sead::MethodTreeNode*); // 0x0054CB44 slot 0x10 | nintendogs:callseq
    virtual void vf_0x14(); // 0x0054CD3C slot 0x14 | virtual slot, introduced by sead::MethodTreeMgr
    virtual void pauseAll(bool); // 0x00548F8C slot 0x18 | nintendogs:bytes
    UlcdMethodTreeMgr(); // 0x00549018 | nintendogs:bytes [tier A]
};
} // namespace sead
