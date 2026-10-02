#pragma once

#include "decomp.h"
#include "Other/dInstSelect.h"

namespace lyt {
// RTTI N3lyt8TourListE @ 0x008D0FA8
// vtable 0x009045B4 (vptr 0x009045BC), offset_to_top 0, 26 entries
class TourList : public ::InstSelect<8>
{
public:
    TourList(); // ctor address unknown
    virtual ~TourList(); // 0x0050C9E0 slot 0x00 | slot vf_0x00 of CatalogBase
    // 0x0050C9A0 slot 0x04 | slot vf_0x04 of CatalogBase (deleting dtor)
    virtual void vf_0x20(); // 0x00747AC4 slot 0x20 | virtual slot, introduced by CatalogBase
    virtual void vf_0x30(); // 0x0050C51C slot 0x30 | virtual slot, introduced by CatalogBase
    virtual void vf_0x34(); // 0x0050C90C slot 0x34 | virtual slot, introduced by CatalogBase
    virtual void vf_0x64(); // 0x00747ACC slot 0x64 | virtual slot, introduced by SelectBase
};
} // namespace lyt
