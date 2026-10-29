#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadDualScreenMethodTreeMgr.h"
#include "sead/seadMethodTreeNode.h"
#include "sead/seadTaskBase.h"

namespace sead {
// RTTI N4sead14DualScreenTaskE @ 0x008D16F8
// vtable 0x009057C4 (vptr 0x009057CC), offset_to_top 0, 25 entries
// A task with a calc method and a draw method per screen, each bound to a node of the method trees
// (calc/drawTop/drawBtm are ours).
class DualScreenTask : public ::sead::TaskBase
{
public:
    DualScreenTask(const TaskConstructArg& arg, const char* name); // 0x001259E4
    virtual ~DualScreenTask(); // 0x00544F68 slot 0x00 | nintendogs:bytes
    // 0x00544EFC slot 0x04 (deleting dtor)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074BF14 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return TaskBase::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074BEC8 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x00983474 (guard 0x00983470)
        static const RuntimeTypeInfo::Derive<TaskBase> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual void pauseCalc(bool pause); // 0x00544DB0 slot 0x10
    virtual void pauseDraw(bool pause); // 0x00544E18 slot 0x14 | nintendogs:bytes
    virtual void pauseCalcRec(bool pause); // 0x00544928 slot 0x18
    virtual void pauseDrawRec(bool pause); // 0x00544990 slot 0x1C
    virtual void attachCalcImpl(); // 0x00544A74 slot 0x3C
    virtual void attachDrawImpl(); // 0x00544B00 slot 0x40 | nintendogs:bytes-fuzzy
    virtual void detachCalcImpl(); // 0x00544BD0 slot 0x44
    virtual void detachDrawImpl(); // 0x00544BD8 slot 0x48 | nintendogs:bytes
    virtual const RuntimeTypeInfo::Interface* getCorrespondingMethodTreeMgrTypeInfo() const // 0x0074BFC8 slot 0x4C
    {
        return DualScreenMethodTreeMgr::getTypeInfoStatic();
    }
    virtual MethodTreeNode* getMethodTreeNode(s32 methodType); // 0x00544C34 slot 0x50
    virtual void calc(); // 0x00544DA4 slot 0x58
    virtual void drawTop(); // 0x00544DAC slot 0x5C
    virtual void drawBtm(); // 0x00544DA8 slot 0x60

    MethodTreeNode mCalcNode;    // 0x06C
    MethodTreeNode mDrawTopNode; // 0x0BC
    MethodTreeNode mDrawBtmNode; // 0x10C
};
ASSERT_SIZE(DualScreenTask, 0x15C);
} // namespace sead
