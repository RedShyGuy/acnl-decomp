#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadMethodTreeMgr.h"

namespace sead {
// RTTI N4sead23DualScreenMethodTreeMgrE @ 0x008D1F38
// vtable 0x009067B8 (vptr 0x009067C0), offset_to_top 0, 8 entries
class DualScreenMethodTreeMgr : public ::sead::MethodTreeMgr
{
public:
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074E178 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return MethodTreeMgr::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074E12C (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x00983564 (guard 0x00983560)
        static const RuntimeTypeInfo::Derive<MethodTreeMgr> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~DualScreenMethodTreeMgr(); // 0x0054D50C slot 0x08 | slot vf_0x08 of sead::MethodTreeMgr
    // 0x0054D4FC slot 0x0C | slot vf_0x0C of sead::MethodTreeMgr (deleting dtor)
    virtual void attachMethod(int, sead::MethodTreeNode*); // 0x0054CBB8 slot 0x10 | nintendogs:callseq
    virtual void vf_0x14(); // 0x0054CDA4 slot 0x14 | virtual slot, introduced by sead::MethodTreeMgr
    virtual void pauseAll(bool); // 0x0054CE84 slot 0x18 | nintendogs:bytes
    virtual void vf_0x1C(); // 0x0054CCD4 slot 0x1C | virtual slot, introduced by sead::MethodTreeMgr
    DualScreenMethodTreeMgr(); // 0x0054CFF8 | nintendogs:bytes [tier A]
};
} // namespace sead
