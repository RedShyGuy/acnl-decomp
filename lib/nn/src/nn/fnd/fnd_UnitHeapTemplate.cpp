#include "nn/fnd/fnd_UnitHeapTemplate.h"

// The functions are the instantiation for LockPolicy::Object<CriticalSection> (explicit, at the
// end of the file); check.py puts the argument of that instantiation in for LockPolicyT.

namespace nn {
namespace fnd {

// 0x0013A334 slot 0x00
// 0x007D39BC slot 0x04 (deleting dtor)
template <typename LockPolicyT>
UnitHeapTemplate<LockPolicyT>::~UnitHeapTemplate()
{
    // nothing of its own: ~UnitHeapBase and ~HeapBase do the work
}

// 0x00135AA0 | nintendogs:bytes [tier A]
template <typename LockPolicyT>
void* UnitHeapTemplate<LockPolicyT>::Allocate()
{
    ScopedLock lock(*this);
    return AllocateUnit();
}

// 0x007D3980 slot 0x08
template <typename LockPolicyT>
void UnitHeapTemplate<LockPolicyT>::FreeV(void* p)
{
    Free(p);
}

// 0x00827A44 slot 0x0C (name is ours)
template <typename LockPolicyT>
void* UnitHeapTemplate<LockPolicyT>::GetHeapStart() const
{
    return UnitHeapBase::GetHeapStart();
}

// 0x00827A3C slot 0x10 (name is ours)
template <typename LockPolicyT>
size_t UnitHeapTemplate<LockPolicyT>::GetHeapSize() const
{
    return UnitHeapBase::GetHeapSize();
}

// 0x00827A4C slot 0x14 (name is ours)
template <typename LockPolicyT>
void UnitHeapTemplate<LockPolicyT>::PrintState()
{
    ScopedLock lock(*this);
    UnitHeapBase::PrintState();
}

// 0x00827A14 slot 0x18
template <typename LockPolicyT>
bool UnitHeapTemplate<LockPolicyT>::HasAddress(const void* p) const
{
    return UnitHeapBase::HasAddress(p);
}

template class UnitHeapTemplate<nn::os::LockPolicy::Object<nn::os::CriticalSection> >;
ASSERT_SIZE(UnitHeapTemplate<nn::os::LockPolicy::Object<nn::os::CriticalSection> >, 0x3C);

} // namespace fnd
} // namespace nn
