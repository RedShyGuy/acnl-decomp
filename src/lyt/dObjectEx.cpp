#include "lyt/dObjectEx.h"

namespace lyt {
// 0x0050A3AC slot 0x00
// 0x0050A7EC (deleting dtor)
lyt::ObjectEx::~ObjectEx()
{
    // (only the members are destroyed)
}

} // namespace lyt
