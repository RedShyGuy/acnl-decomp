#pragma once

#include "decomp.h"

namespace nn {
namespace os {
// How a container (e.g. nn::fnd::UnitHeapTemplate) locks: NoLock, or Object<LockT> with a lock
// object of type LockT (os_LockPolicy_Object.h). The names are from RTTI.
class LockPolicy
{
public:
    class NoLock;
    template <typename LockT>
    class Object;
};
} // namespace os
} // namespace nn
