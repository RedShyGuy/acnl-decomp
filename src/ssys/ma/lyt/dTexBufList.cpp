#include "ssys/ma/lyt/dTexBufList.h"

namespace ssys {
namespace ma {
namespace lyt {
// 0x0056860C slot 0x00
// 0x005685A0 (deleting dtor)
ssys::ma::lyt::TexBufList::~TexBufList()
{
    DeleteAll();
}

} // namespace lyt
} // namespace ma
} // namespace ssys
