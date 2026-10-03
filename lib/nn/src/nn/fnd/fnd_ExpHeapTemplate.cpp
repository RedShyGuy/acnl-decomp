#include "nn/fnd/fnd_ExpHeapTemplate.h"

// The functions are the instantiation for LockPolicy::Object<CriticalSection> (explicit, at the
// end of the file); check.py puts the argument of that instantiation in for LockPolicyT.

namespace nn {
namespace fnd {

// 0x007D3958 slot 0x00
// 0x007D392C slot 0x04 (deleting dtor)
template <typename LockPolicyT>
ExpHeapTemplate<LockPolicyT>::~ExpHeapTemplate()
{
    // nothing of its own: ~ExpHeapBase and ~HeapBase do the work
}

// 0x007D3870 slot 0x08
template <typename LockPolicyT>
void ExpHeapTemplate<LockPolicyT>::FreeV(void* p)
{
    Free(p);
}

// 0x008279BC slot 0x0C (name is ours)
template <typename LockPolicyT>
void* ExpHeapTemplate<LockPolicyT>::GetHeapStart() const
{
    ScopedLock lock(const_cast<ExpHeapTemplate&>(*this));
    return ExpHeapBase::GetHeapStart();
}

// 0x0082798C slot 0x10 (name is ours)
template <typename LockPolicyT>
size_t ExpHeapTemplate<LockPolicyT>::GetHeapSize() const
{
    ScopedLock lock(const_cast<ExpHeapTemplate&>(*this));
    return ExpHeapBase::GetHeapSize();
}

// 0x008279EC slot 0x14 (name is ours)
template <typename LockPolicyT>
void ExpHeapTemplate<LockPolicyT>::PrintState()
{
    ScopedLock lock(*this);
    ExpHeapBase::PrintState();
}

// 0x00827954 slot 0x18
template <typename LockPolicyT>
bool ExpHeapTemplate<LockPolicyT>::HasAddress(const void* p) const
{
    ScopedLock lock(const_cast<ExpHeapTemplate&>(*this));
    return ExpHeapBase::HasAddress(p);
}

// 0x007D38D0 | fefates:bytes [tier B]
template <typename LockPolicyT>
void* ExpHeapTemplate<LockPolicyT>::Allocator::Allocate(size_t size, s32 alignment)
{
    return mHeap->Allocate(size, alignment, mGroupId, mMode, mUseMargin);
}

// 0x007D38A0 | fefates:bytes [tier B]
template <typename LockPolicyT>
void ExpHeapTemplate<LockPolicyT>::Allocator::Free(void* p)
{
    mHeap->Free(p);
}

// 0x0035244C slot 0x08
// 0x00352448 slot 0x0C (deleting dtor)
template <typename LockPolicyT>
ExpHeapTemplate<LockPolicyT>::Allocator::~Allocator()
{
    // nothing to do
}

template class ExpHeapTemplate<nn::os::LockPolicy::Object<nn::os::CriticalSection> >;
ASSERT_SIZE(ExpHeapTemplate<nn::os::LockPolicy::Object<nn::os::CriticalSection> >, 0x64);

} // namespace fnd
} // namespace nn
