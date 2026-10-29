#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"
#include "sead/seadCriticalSection.h"
#include "sead/seadDelegate.h"
#include "sead/seadIDelegate2.h"
#include "sead/seadIDisposer.h"
#include "sead/seadINamable.h"
#include "sead/seadTTreeNode.h"

#include <new>

namespace sead {
// RTTI N4sead14MethodTreeNodeE
// vtable 0x009058D4 (vptr 0x009058DC)
//
// A node of the tree of methods the framework calls each frame: calls its delegate, then the nodes
// below it (unless paused). The nodes of a tree share the critical section of the root. Method names
// are from the reference symbols, member names are ours.
class MethodTreeNode : public ::sead::TTreeNode<sead::MethodTreeNode*>, public ::sead::INamable, public ::sead::IDisposer
{
public:
    // the bits of mPauseFlag
    enum PauseFlag
    {
        PAUSE_FLAG_NONE = 0,
        PAUSE_FLAG_SELF = 1,     // the delegate is not called
        PAUSE_FLAG_CHILDREN = 2, // the nodes below are not called
        PAUSE_FLAG_ALL = 3,
    };

    // (inline in the tasks; the original has a copy)
    DECOMP_EMIT_COPY explicit MethodTreeNode(CriticalSection* criticalSection) // 0x0012C9F4 | nintendogs:bytes [tier A]
        : TTreeNode<MethodTreeNode*>(this), INamable(""), IDisposer(), mCriticalSection(criticalSection),
          mPauseFlag(PAUSE_FLAG_NONE), mPauseEventDelegate(nullptr), field_0x4C(0)
    {
        new (mDelegateHolder) Delegate<MethodTreeNode>();
    }
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
    void pushBackChild(MethodTreeNode* node); // 0x00545154 | nintendogs:callgraph [tier A]
    void pushFrontChild(MethodTreeNode* node); // 0x005451F4 | nintendogs:callseq [tier A]
    // calls the delegate and the nodes below (under the lock)
    void call(); // 0x0054529C | nintendogs:bytes [tier A]
    void callRec_(); // 0x00545314 | nintendogs:bytes [tier A]
    // takes the node out of the tree and unlinks all nodes below it (they lose the critical section)
    void detachAll(); // 0x00545374 | nintendogs:bytes [tier A]
    // gives the node, its children and its later siblings the critical section
    DECOMP_NOINLINE void attachMutexRec_(CriticalSection* criticalSection) const // 0x0074C2D8 | nintendogs:bytes [tier A]
    {
        const_cast<MethodTreeNode*>(this)->mCriticalSection = criticalSection;
        if (child() != nullptr && child()->val() != nullptr) {
            child()->val()->attachMutexRec_(criticalSection);
        }
        if (next() != nullptr && next()->val() != nullptr) {
            next()->val()->attachMutexRec_(criticalSection);
        }
    }

    // sets the pause flag and tells the pause event delegate (name is ours)
    void setPauseFlag(u32 flag)
    {
        lock_();
        if (mPauseEventDelegate != nullptr) {
            mPauseEventDelegate->invoke(this, flag);
        }
        mPauseFlag = flag;
        unlock_();
    }
    // pauses the node and the nodes below it or calls them again (name is ours)
    void pause(bool pause) { setPauseFlag(pause ? PAUSE_FLAG_ALL : PAUSE_FLAG_NONE); }

    // binds the delegate to a method of obj (under the lock; name is ours)
    template <typename T>
    void bind(T* obj, void (T::*method)())
    {
        lock_();
        new (mDelegateHolder) Delegate<T>(obj, method);
        unlock_();
    }

    // the delegate (placement-constructed into the holder)
    IDelegate* getDelegate() { return reinterpret_cast<IDelegate*>(mDelegateHolder); }
    // calls the delegate unless paused
    void callDelegate_()
    {
        if ((mPauseFlag & PAUSE_FLAG_SELF) == 0) {
            getDelegate()->invoke();
        }
    }

    u32 mDelegateHolder[4];                  // 0x2C, a Delegate<T>
    CriticalSection* mCriticalSection;       // 0x3C
    u32 field_0x40;                          // 0x40 (not set here)
    u32 mPauseFlag;                          // 0x44
    IDelegate2<MethodTreeNode*, u32>* mPauseEventDelegate; // 0x48, told about each change of the pause flag
    u32 field_0x4C;                          // 0x4C
};
ASSERT_SIZE(MethodTreeNode, 0x50);
} // namespace sead
