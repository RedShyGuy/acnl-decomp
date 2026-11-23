#include "Item/dResLoader.h"

namespace item {
// 0x0053788C (name after the RTTI)
item::ResLoader::ResLoader() : m_pDesign(NULL)
{
}

// 0x005378C8 slot 0x00 (name after the RTTI)
// 0x005378B4 (deleting dtor)
item::ResLoader::~ResLoader()
{
    // (only the base is destroyed)
}
} // namespace item
