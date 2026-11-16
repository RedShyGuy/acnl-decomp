#include "ssys/st/dList.h"
#include "fgobj/dDrawList.h"

namespace fgobj {
// 0x005A3108 slot 0x00 | slot vf_0x00 of ssys::st::List
// 0x005A3104 (deleting dtor)
fgobj::DrawList::~DrawList()
{
    // empty (in the original too)
}

} // namespace fgobj
