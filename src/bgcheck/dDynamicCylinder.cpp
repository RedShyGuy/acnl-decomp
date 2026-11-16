#include "bgcheck/dDynamicCylinder.h"

namespace bgcheck {
// 0x00615040
bgcheck::DynamicCylinder::DynamicCylinder()
{
    // (only the members are constructed)
}

// 0x00615084 slot 0x00
// 0x00615060 (deleting dtor)
bgcheck::DynamicCylinder::~DynamicCylinder()
{
    // (only the members are destroyed)
}

} // namespace bgcheck
