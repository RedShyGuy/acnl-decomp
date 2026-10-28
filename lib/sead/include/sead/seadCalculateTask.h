#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadTaskBase.h"

namespace sead {
// RTTI N4sead13CalculateTaskE @ 0x008D15F4
// vtable 0x0090534C (vptr 0x00905354), offset_to_top 0, 23 entries
class CalculateTask : public ::sead::TaskBase
{
public:
    CalculateTask(); // TODO: default ctor added so derived stubs compile - may not exist
    virtual ~CalculateTask(); // 0x00541B18 slot 0x00 | nintendogs:bytes
    // 0x00541ADC slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074B304 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return TaskBase::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074B2B8 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x0098340C (guard 0x00983408)
        static const RuntimeTypeInfo::Derive<TaskBase> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual void vf_0x10(); // 0x0054194C slot 0x10 | virtual slot, introduced by sead::TaskBase
    virtual void pauseDraw(bool); // 0x005419B4 slot 0x14 | slot vf_0x14 of sead::TaskBase
    virtual void vf_0x18(); // 0x0012C978 slot 0x18 | virtual slot, introduced by sead::TaskBase
    virtual void vf_0x1C(); // 0x005418C0 slot 0x1C | virtual slot, introduced by sead::TaskBase
    virtual void vf_0x20(); // 0x005418CC slot 0x20 | virtual slot, introduced by sead::TaskBase
    virtual void vf_0x24(); // 0x00541934 slot 0x24 | virtual slot, introduced by sead::TaskBase
    virtual void attachCalcImpl(); // 0x00138CB8 slot 0x3C | slot vf_0x3C of sead::TaskBase
    virtual void attachDrawImpl(); // 0x005418C4 slot 0x40 | slot vf_0x40 of sead::TaskBase
    virtual void detachCalcImpl(); // 0x0054536C slot 0x44 | slot vf_0x44 of sead::TaskBase
    virtual void detachDrawImpl(); // 0x005418C8 slot 0x48 | slot vf_0x48 of sead::TaskBase
    virtual void vf_0x4C(); // 0x0074B3B8 slot 0x4C | virtual slot, introduced by sead::TaskBase
    virtual void getMethodTreeNode(int); // 0x00541938 slot 0x50 | slot vf_0x50 of sead::TaskBase
    virtual void vf_0x58(); // 0x00541948 slot 0x58 | virtual slot, introduced by sead::CalculateTask
    CalculateTask(const sead::TaskConstructArg&, const char*); // 0x005419B8 | nintendogs:bytes [tier A]
};
} // namespace sead
