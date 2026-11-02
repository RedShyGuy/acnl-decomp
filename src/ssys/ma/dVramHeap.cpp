#include "ssys/ma/dVramHeap.h"

namespace ssys {
namespace ma {
// 0x0056B1F0 slot 0x24
// 0x0056B1D0 (deleting dtor)
ssys::ma::VramHeap::~VramHeap()
{
    // (only the members are destroyed)
}

} // namespace ma
} // namespace ssys
