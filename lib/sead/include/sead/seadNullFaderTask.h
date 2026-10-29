#pragma once

#include "decomp.h"
#include "sead/seadFaderTaskBase.h"
#include "sead/seadMethodTreeMgr.h"

namespace sead {
// RTTI N4sead13NullFaderTaskE @ 0x008D167C
// vtable 0x00905608 (vptr 0x00905610), offset_to_top 0, 26 entries
// The fader of the task manager that changes tasks at once (no fade frames).
class NullFaderTask : public ::sead::FaderTaskBase
{
public:
    explicit NullFaderTask(const TaskConstructArg& arg); // 0x0054323C
    virtual ~NullFaderTask(); // 0x00543288 slot 0x00
    // 0x00543278 slot 0x04 (deleting dtor)
    virtual void pauseDraw(bool pause); // 0x00543238 slot 0x14
    virtual void pauseDrawRec(bool pause); // 0x0054321C slot 0x1C
    virtual void pauseDrawChild(bool pause); // 0x00543228 slot 0x24
    virtual void attachDrawImpl(); // 0x00543220 slot 0x40
    virtual void detachDrawImpl(); // 0x00543224 slot 0x48
    virtual const RuntimeTypeInfo::Interface* getCorrespondingMethodTreeMgrTypeInfo() const // 0x0074B9F8 slot 0x4C
    {
        return MethodTreeMgr::getTypeInfoStatic();
    }
    virtual MethodTreeNode* getMethodTreeNode(s32 methodType); // 0x0054322C slot 0x50
    virtual void vf_0x64(); // 0x00543234 slot 0x64
};
} // namespace sead
