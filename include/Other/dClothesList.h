#pragma once

#include "decomp.h"
#include "Other/dInstSelect.h"

// RTTI 11ClothesList @ 0x008CB2A8
// vtable 0x008ECA84 (vptr 0x008ECA8C), offset_to_top 0, 26 entries
class ClothesList : public ::InstSelect<8>
{
public:
    ClothesList(); // ctor address unknown
    virtual ~ClothesList(); // 0x001C7DFC slot 0x00 | slot vf_0x00 of CatalogBase
    // 0x001C7C90 slot 0x04 | slot vf_0x04 of CatalogBase (deleting dtor)
    virtual void vf_0x0C(); // 0x001C7474 slot 0x0C | virtual slot, introduced by CatalogBase
    virtual void vf_0x10(); // 0x001C74FC slot 0x10 | virtual slot, introduced by CatalogBase
    virtual void vf_0x14(); // 0x001C7448 slot 0x14 | virtual slot, introduced by CatalogBase
    virtual void vf_0x18(); // 0x0070E59C slot 0x18 | virtual slot, introduced by CatalogBase
    virtual void vf_0x1C(); // 0x0070E5C4 slot 0x1C | virtual slot, introduced by CatalogBase
    virtual void vf_0x20(); // 0x0070E638 slot 0x20 | virtual slot, introduced by CatalogBase
    virtual void vf_0x30(); // 0x001C6F48 slot 0x30 | virtual slot, introduced by CatalogBase
    virtual void vf_0x34(); // 0x001C77CC slot 0x34 | virtual slot, introduced by CatalogBase
    virtual void vf_0x38(); // 0x001C7420 slot 0x38 | virtual slot, introduced by CatalogBase
    virtual void vf_0x44(); // 0x001C73D8 slot 0x44 | virtual slot, introduced by CatalogBase
    virtual void vf_0x4C(); // 0x001C7780 slot 0x4C | virtual slot, introduced by CatalogBase
    virtual void vf_0x54(); // 0x001C77AC slot 0x54 | virtual slot, introduced by CatalogBase
    virtual void vf_0x60(); // 0x001C7A7C slot 0x60 | virtual slot, introduced by CatalogBase
    virtual void vf_0x64(); // 0x0070E670 slot 0x64 | virtual slot, introduced by SelectBase
};
