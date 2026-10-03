#pragma once

#include "decomp.h"
#include "nn/fnd/fnd_ExpHeapBase.h"
#include "nn/fnd/fnd_IAllocator.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/os/os_LockPolicy_Object.h"

namespace nn {
namespace fnd {
// Instantiations found in the binary:
//   nn::fnd::ExpHeapTemplate<nn::os::LockPolicy::NoLock>  typeinfo 0x008CDE58  vtable 0x008FC078
//   nn::fnd::ExpHeapTemplate<nn::os::LockPolicy::Object<nn::os::CriticalSection> >  typeinfo 0x008CDE84  vtable 0x008FC0B4
//   nn::fnd::ExpHeapTemplate<nn::os::LockPolicy::Object<nn::os::CriticalSection> >::Allocator  typeinfo 0x008CDE78  vtable 0x008FC09C
//
// An ExpHeapBase with a lock (the LockObject base at +0x58). The out-of-line functions are
// instantiated in fnd_ExpHeapTemplate.cpp; member and inline function names are ours.
template <typename LockPolicyT>
class ExpHeapTemplate : public ExpHeapBase, private LockPolicyT::LockObject
{
    typedef typename LockPolicyT::LockObject LockObject;
    typedef typename LockObject::ScopedLock ScopedLock;

public:
    // an IAllocator on the heap
    class Allocator : public ::nn::fnd::IAllocator
    {
    public:
        // inline (in the static initializer of the socket heap)
        Allocator() : mHeap(0) {}

        // inline (in nn::socket::Initialize)
        void Initialize(ExpHeapTemplate* heap, u8 groupId = 0, AllocationMode mode = ALLOCATION_MODE_FIRST_FIT,
                        bool useMargin = false)
        {
            mHeap = heap;
            mGroupId = groupId;
            mUseMargin = useMargin;
            mMode = mode;
        }
        // inline (in nn::socket::Finalize; the original also copies the three bytes of an
        // uninitialized object)
        void Finalize() { mHeap = 0; }

        virtual void* Allocate(size_t size, s32 alignment);
        virtual void Free(void* p);
        virtual ~Allocator();

    private:
        ExpHeapTemplate* mHeap;     // 0x4
        u8 mGroupId;                // 0x8
        AllocationMode mMode;       // 0x9
        bool mUseMargin;            // 0xA
    };

    // inline (in the static initializer of the socket heap)
    ExpHeapTemplate() {}
    virtual ~ExpHeapTemplate();

    // inline (names are ours)
    void Initialize(uptr address, size_t size, bit32 option)
    {
        ExpHeapBase::Initialize(address, size, option);
        LockObject::Initialize();
    }
    void Finalize()
    {
        LockObject::Finalize();
        ExpHeapBase::Finalize();
    }
    void* Allocate(size_t size, s32 alignment, u8 groupId = 0, AllocationMode mode = ALLOCATION_MODE_FIRST_FIT,
                   bool useMargin = false)
    {
        ScopedLock lock(*this);
        return ExpHeapBase::Allocate(size, alignment, groupId, mode, useMargin);
    }
    void Free(void* p)
    {
        ScopedLock lock(*this);
        ExpHeapBase::Free(p);
    }

    virtual void FreeV(void* p);
    virtual void* GetHeapStart() const;
    virtual size_t GetHeapSize() const;
    virtual void PrintState();
    virtual bool HasAddress(const void* p) const;
};
} // namespace fnd
} // namespace nn
