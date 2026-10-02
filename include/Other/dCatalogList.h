#pragma once

#include "decomp.h"
#include "Other/dInstCatalog.h"

// RTTI 11CatalogList @ 0x008CB29C
// vtable 0x008ECA18 (vptr 0x008ECA20), offset_to_top 0, 25 entries
class CatalogList : public ::InstCatalog<8>
{
public:
    CatalogList(); // ctor candidate(s) 0x001C6ACC (unverified)
    virtual ~CatalogList(); // 0x001C6D04 slot 0x00 | slot vf_0x00 of CatalogBase
    // 0x001C6CF4 slot 0x04 | slot vf_0x04 of CatalogBase (deleting dtor)
    virtual void vf_0x0C(); // 0x001C6228 slot 0x0C | virtual slot, introduced by CatalogBase
    virtual void vf_0x10(); // 0x001C6300 slot 0x10 | virtual slot, introduced by CatalogBase
    virtual void vf_0x14(); // 0x001C594C slot 0x14 | virtual slot, introduced by CatalogBase
    virtual void vf_0x18(); // 0x0070E53C slot 0x18 | virtual slot, introduced by CatalogBase
    virtual void vf_0x1C(); // 0x0070E554 slot 0x1C | virtual slot, introduced by CatalogBase
    virtual void vf_0x30(); // 0x001C5D94 slot 0x30 | virtual slot, introduced by CatalogBase
    virtual void vf_0x34(); // 0x001C6784 slot 0x34 | virtual slot, introduced by CatalogBase
    virtual void vf_0x38(); // 0x001C6208 slot 0x38 | virtual slot, introduced by CatalogBase
    virtual void vf_0x3C(); // 0x001C6040 slot 0x3C | virtual slot, introduced by CatalogBase
    virtual void vf_0x44(); // 0x001C61D8 slot 0x44 | virtual slot, introduced by CatalogBase
    virtual void vf_0x4C(); // 0x001C64E4 slot 0x4C | virtual slot, introduced by CatalogBase
    virtual void vf_0x54(); // 0x001C650C slot 0x54 | virtual slot, introduced by CatalogBase
    virtual void vf_0x60(); // 0x001C6A84 slot 0x60 | virtual slot, introduced by CatalogBase
};
