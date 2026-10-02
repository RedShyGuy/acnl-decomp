#pragma once

#include "decomp.h"
#include "Other/dInstSelect.h"

// RTTI 7FtrList @ 0x008CD3B4
// vtable 0x008F90F0 (vptr 0x008F90F8), offset_to_top 0, 26 entries
class FtrList : public ::InstSelect<8>
{
public:
    FtrList(); // ctor address unknown
    virtual ~FtrList(); // 0x0060DA4C slot 0x00 | slot vf_0x00 of CatalogBase
    // 0x0060D918 slot 0x04 | slot vf_0x04 of CatalogBase (deleting dtor)
    virtual void vf_0x0C(); // 0x0060D280 slot 0x0C | virtual slot, introduced by CatalogBase
    virtual void vf_0x10(); // 0x0060D2FC slot 0x10 | virtual slot, introduced by CatalogBase
    virtual void vf_0x14(); // 0x0060D254 slot 0x14 | virtual slot, introduced by CatalogBase
    virtual void vf_0x18(); // 0x0075E51C slot 0x18 | virtual slot, introduced by CatalogBase
    virtual void vf_0x1C(); // 0x0075E544 slot 0x1C | virtual slot, introduced by CatalogBase
    virtual void vf_0x20(); // 0x0075E5B8 slot 0x20 | virtual slot, introduced by CatalogBase
    virtual void vf_0x30(); // 0x0060D114 slot 0x30 | virtual slot, introduced by CatalogBase
    virtual void vf_0x34(); // 0x0060D5C4 slot 0x34 | virtual slot, introduced by CatalogBase
    virtual void vf_0x38(); // 0x0060D230 slot 0x38 | virtual slot, introduced by CatalogBase
    virtual void vf_0x44(); // 0x0060D1E8 slot 0x44 | virtual slot, introduced by CatalogBase
    virtual void vf_0x4C(); // 0x0060D578 slot 0x4C | virtual slot, introduced by CatalogBase
    virtual void vf_0x54(); // 0x0060D5A4 slot 0x54 | virtual slot, introduced by CatalogBase
    virtual void vf_0x60(); // 0x0060D730 slot 0x60 | virtual slot, introduced by CatalogBase
    virtual void vf_0x64(); // 0x0075E5F4 slot 0x64 | virtual slot, introduced by SelectBase
};
