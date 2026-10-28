#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"

namespace sead {
// RTTI N4sead13MethodTreeMgrE @ 0x008D1674
// vtable 0x009055E0 (vptr 0x009055E8), offset_to_top 0, 8 entries
class MethodTreeMgr
{
public:
    MethodTreeMgr(); // ctor candidate(s) 0x005431C4 (unverified)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074B99C (name is ours)
    {
        return typeInfo == getTypeInfoStatic();
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074B950 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x00983444 (guard 0x00983440)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~MethodTreeMgr(); // 0x00543200 slot 0x08 | slot vf_0x08 of sead::MethodTreeMgr
    // 0x005431E0 slot 0x0C | slot vf_0x0C of sead::MethodTreeMgr (deleting dtor)
    virtual void attachMethod(int, sead::MethodTreeNode*); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x14(); // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
    virtual void pauseAll(bool); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x1C(); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
};
} // namespace sead
