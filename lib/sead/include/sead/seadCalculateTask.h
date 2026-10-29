#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadMethodTreeMgr.h"
#include "sead/seadMethodTreeNode.h"
#include "sead/seadTaskBase.h"

namespace sead {
// RTTI N4sead13CalculateTaskE @ 0x008D15F4
// vtable 0x0090534C (vptr 0x00905354), offset_to_top 0, 23 entries
// A task with a calc method (slot 0x58, bound to its node of the method tree; names of the slots after
// the reference symbols of TaskBase, calc is ours).
class CalculateTask : public ::sead::TaskBase
{
public:
    CalculateTask(const TaskConstructArg& arg, const char* name); // 0x005419B8 | nintendogs:bytes [tier A]
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
    virtual void pauseCalc(bool pause); // 0x0054194C slot 0x10
    virtual void pauseDraw(bool pause); // 0x005419B4 slot 0x14
    virtual void pauseCalcRec(bool pause); // 0x0012C978 slot 0x18
    virtual void pauseDrawRec(bool pause); // 0x005418C0 slot 0x1C
    virtual void pauseCalcChild(bool pause); // 0x005418CC slot 0x20
    virtual void pauseDrawChild(bool pause); // 0x00541934 slot 0x24
    virtual void attachCalcImpl(); // 0x00138CB8 slot 0x3C
    virtual void attachDrawImpl(); // 0x005418C4 slot 0x40
    virtual void detachCalcImpl(); // 0x0054536C slot 0x44
    virtual void detachDrawImpl(); // 0x005418C8 slot 0x48
    virtual const RuntimeTypeInfo::Interface* getCorrespondingMethodTreeMgrTypeInfo() const // 0x0074B3B8 slot 0x4C
    {
        return MethodTreeMgr::getTypeInfoStatic();
    }
    // the node of the method types 0 and 1 (the calc trees)
    virtual MethodTreeNode* getMethodTreeNode(s32 methodType); // 0x00541938 slot 0x50
    virtual void calc(); // 0x00541948 slot 0x58

    MethodTreeNode mCalcNode; // 0x6C
};
ASSERT_SIZE(CalculateTask, 0xBC);
} // namespace sead
