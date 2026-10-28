#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"
#include "sead/seadIDisposer.h"
#include "sead/seadINamable.h"
#include "sead/seadTTreeNode.h"

namespace sead {
// RTTI N4sead8TaskBaseE @ 0x008D2210
// vtable 0x00906DAC (vptr 0x00906DB4), offset_to_top 0, 22 entries
class TaskBase : public ::sead::TTreeNode<sead::TaskBase*>, public ::sead::IDisposer, public ::sead::INamable
{
public:
    class CreateArg;
    struct State { u32 _unknown; }; // TODO: real type unknown (placeholder)
    TaskBase(); // ctor address unknown
    virtual ~TaskBase(); // 0x00561508 slot 0x00 | nintendogs:bytes
    // 0x00561478 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074F9D8 (name is ours)
    {
        return typeInfo == getTypeInfoStatic();
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074F98C (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009835D4 (guard 0x009835D0)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual void vf_0x10(); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void pauseDraw(bool); // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x18(); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x1C(); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x20(); // 0x005612B8 slot 0x20 | virtual slot, introduced by sead::TaskBase
    virtual void vf_0x24(); // 0x005612BC slot 0x24 | virtual slot, introduced by sead::TaskBase
    virtual void prepare(); // 0x00561414 slot 0x28 | slot vf_0x28 of sead::TaskBase
    virtual void enterCommon(); // 0x0056119C slot 0x2C | slot vf_0x2C of sead::TaskBase
    virtual void enter(); // 0x0056140C slot 0x30 | slot vf_0x30 of sead::TaskBase
    virtual void exit(); // 0x00561408 slot 0x34 | slot vf_0x34 of sead::TaskBase
    virtual void onEvent(const sead::TaskEvent&); // 0x00561410 slot 0x38 | slot vf_0x38 of sead::TaskBase
    virtual void attachCalcImpl(); // 0x0011C12F slot 0x3C | slot vf_0x00 of ChangeRentalBase
    virtual void attachDrawImpl(); // 0x0011C12F slot 0x40 | slot vf_0x00 of ChangeRentalBase
    virtual void detachCalcImpl(); // 0x0011C12F slot 0x44 | slot vf_0x00 of ChangeRentalBase
    virtual void detachDrawImpl(); // 0x0011C12F slot 0x48 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x4C(); // 0x0011C12F slot 0x4C | slot vf_0x00 of ChangeRentalBase
    virtual void getMethodTreeNode(int); // 0x0011C12F slot 0x50 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x54(); // 0x00561188 slot 0x54 | virtual slot, introduced by sead::TaskBase
    void attachMethodWithCheck(int, sead::MethodTreeNode*); // 0x00561350 | nintendogs:callseq-callee [tier A]
    void adjustHeapWithSlackWithoutLock_(int, unsigned); // 0x00561368 | nintendogs:bytes-fuzzy [tier A]
};
} // namespace sead
