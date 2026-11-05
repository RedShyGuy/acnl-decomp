#include "Item/dResLoader.h"

namespace item {
// 0x0053788C (name after the RTTI)
item::ResLoader::ResLoader() : unknown0(0), unknown1(0), unknown2(0)
{
}

// 0x005378C8 slot 0x00 (name after the RTTI)
// 0x005378B4 (deleting dtor)
item::ResLoader::~ResLoader()
{
    // (only the base is destroyed)
}
} // namespace item
