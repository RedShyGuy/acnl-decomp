#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"
#include "sead/seadIDisposer.h"
#include "sead/seadINamable.h"
#include "sead/seadTTreeNode.h"

namespace sead {
// RTTI N4sead14MethodTreeNodeE @ 0x008D171C
// vtable 0x009058D4 (vptr 0x009058DC), offset_to_top 0, 4 entries
class MethodTreeNode : public ::sead::TTreeNode<sead::MethodTreeNode*>, public ::sead::INamable, public ::sead::IDisposer
{
public:
    MethodTreeNode(); // TODO: default ctor added so derived stubs compile - may not exist
    virtual ~MethodTreeNode(); // 0x00545410 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x005453E4 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074C368 (name is ours)
    {
        return typeInfo == getTypeInfoStatic();
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074C31C (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x00983484 (guard 0x00983480)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }
    void lock_(); // 0x0012C964 | nintendogs:callgraph [tier A]
    void unlock_(); // 0x0012C9E0 | nintendogs:callgraph [tier A]
    MethodTreeNode(sead::CriticalSection*); // 0x0012C9F4 | nintendogs:bytes [tier A]
    void pushBackChild(sead::MethodTreeNode*); // 0x00545154 | nintendogs:callgraph [tier A]
    void pushFrontChild(sead::MethodTreeNode*); // 0x005451F4 | nintendogs:callseq [tier A]
    void call(); // 0x0054529C | nintendogs:bytes [tier A]
    void callRec_(); // 0x00545314 | nintendogs:bytes [tier A]
    void detachAll(); // 0x00545374 | nintendogs:bytes [tier A]
    void attachMutexRec_(sead::CriticalSection*) const; // 0x0074C2D8 | nintendogs:bytes [tier A]
};
} // namespace sead
