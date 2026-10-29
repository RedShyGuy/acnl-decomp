#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"
#include "sead/seadCriticalSection.h"

namespace sead {
class MethodTreeNode;

// RTTI N4sead13MethodTreeMgrE @ 0x008D1674
// vtable 0x009055E0 (vptr 0x009055E8), offset_to_top 0, 8 entries
class MethodTreeMgr
{
public:
    MethodTreeMgr(); // 0x005431C4
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
    virtual ~MethodTreeMgr(); // 0x00543200 slot 0x08
    // 0x005431E0 slot 0x0C (deleting dtor)
    // adds node to the tree of the method type (names of the slots are ours, attachMethod/pauseAll after
    // the reference symbols)
    virtual void attachMethod(s32 methodType, MethodTreeNode* node) = 0; // slot 0x10
    // the node that the method type attaches to (NULL if there is none)
    virtual MethodTreeNode* getRootMethodTreeNode(s32 methodType) = 0; // slot 0x14
    virtual void pauseAll(bool pause) = 0; // slot 0x18
    // pauses the methods of the application
    virtual void pauseAppCalc(bool pause) = 0; // slot 0x1C

    CriticalSection mCriticalSection; // 0x04, the lock of all trees
};
ASSERT_SIZE(MethodTreeMgr, 0x20);
} // namespace sead
