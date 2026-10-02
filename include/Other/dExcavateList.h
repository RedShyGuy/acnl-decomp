#pragma once

#include "decomp.h"
#include "Other/dInstSelect.h"

// RTTI 12ExcavateList @ 0x008CB608
// vtable 0x008EE434 (vptr 0x008EE43C), offset_to_top 0, 26 entries
class ExcavateList : public ::InstSelect<8>
{
public:
    ExcavateList(); // ctor address unknown
    virtual ~ExcavateList(); // 0x00200C38 slot 0x00 | slot vf_0x00 of CatalogBase
    // 0x00200B50 slot 0x04 | slot vf_0x04 of CatalogBase (deleting dtor)
    virtual void vf_0x0C(); // 0x0020054C slot 0x0C | virtual slot, introduced by CatalogBase
    virtual void vf_0x10(); // 0x002005D4 slot 0x10 | virtual slot, introduced by CatalogBase
    virtual void vf_0x14(); // 0x00200520 slot 0x14 | virtual slot, introduced by CatalogBase
    virtual void vf_0x18(); // 0x00713050 slot 0x18 | virtual slot, introduced by CatalogBase
    virtual void vf_0x1C(); // 0x00713078 slot 0x1C | virtual slot, introduced by CatalogBase
    virtual void vf_0x20(); // 0x007130EC slot 0x20 | virtual slot, introduced by CatalogBase
    virtual void vf_0x30(); // 0x00200038 slot 0x30 | virtual slot, introduced by CatalogBase
    virtual void vf_0x34(); // 0x002008A8 slot 0x34 | virtual slot, introduced by CatalogBase
    virtual void vf_0x38(); // 0x002004FC slot 0x38 | virtual slot, introduced by CatalogBase
    virtual void vf_0x44(); // 0x002004BC slot 0x44 | virtual slot, introduced by CatalogBase
    virtual void vf_0x4C(); // 0x0020085C slot 0x4C | virtual slot, introduced by CatalogBase
    virtual void vf_0x54(); // 0x00200888 slot 0x54 | virtual slot, introduced by CatalogBase
    virtual void vf_0x60(); // 0x002009AC slot 0x60 | virtual slot, introduced by CatalogBase
    virtual void vf_0x64(); // 0x00713124 slot 0x64 | virtual slot, introduced by SelectBase
};
