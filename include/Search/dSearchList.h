#pragma once

#include "decomp.h"
#include "Other/dInstSelect.h"

// RTTI 10SearchList @ 0x008CB1C4
// vtable 0x008EC3B0 (vptr 0x008EC3B8), offset_to_top 0, 26 entries
class SearchList : public ::InstSelect<8>
{
public:
    SearchList(); // ctor candidate(s) 0x007F4C18 (unverified)
    virtual ~SearchList(); // 0x001AF8F0 slot 0x00 | slot vf_0x00 of CatalogBase
    // 0x001AF8E0 slot 0x04 | slot vf_0x04 of CatalogBase (deleting dtor)
    virtual void vf_0x0C(); // 0x001B0304 slot 0x0C | virtual slot, introduced by CatalogBase
    virtual void vf_0x20(); // 0x0070BD48 slot 0x20 | virtual slot, introduced by CatalogBase
    virtual void vf_0x34(); // 0x001AF814 slot 0x34 | virtual slot, introduced by CatalogBase
    virtual void vf_0x64(); // 0x0070BD7C slot 0x64 | virtual slot, introduced by SelectBase
};
