#pragma once

#include "decomp.h"
#include "Other/dInstSelect.h"

// RTTI 9PWorkList @ 0x008CD7E4
// vtable 0x008FA7C4 (vptr 0x008FA7CC), offset_to_top 0, 26 entries
class PWorkList : public ::InstSelect<8>
{
public:
    PWorkList(); // ctor candidate(s) 0x007F4EE8 (unverified)
    virtual ~PWorkList(); // 0x006EF868 slot 0x00 | slot vf_0x00 of CatalogBase
    // 0x006EF828 slot 0x04 | slot vf_0x04 of CatalogBase (deleting dtor)
    virtual void vf_0x0C(); // 0x006EF6F4 slot 0x0C | virtual slot, introduced by CatalogBase
    virtual void vf_0x20(); // 0x00770CD8 slot 0x20 | virtual slot, introduced by CatalogBase
    virtual void vf_0x28(); // 0x00770CD0 slot 0x28 | virtual slot, introduced by CatalogBase
    virtual void vf_0x2C(); // 0x00770D0C slot 0x2C | virtual slot, introduced by CatalogBase
    virtual void vf_0x34(); // 0x006EF714 slot 0x34 | virtual slot, introduced by CatalogBase
    virtual void vf_0x40(); // 0x006EF6B0 slot 0x40 | virtual slot, introduced by CatalogBase
    virtual void vf_0x54(); // 0x006EF704 slot 0x54 | virtual slot, introduced by CatalogBase
    virtual void vf_0x5C(); // 0x006EF824 slot 0x5C | virtual slot, introduced by CatalogBase
    virtual void vf_0x60(); // 0x006EF7AC slot 0x60 | virtual slot, introduced by CatalogBase
    virtual void vf_0x64(); // 0x00770CF4 slot 0x64 | virtual slot, introduced by SelectBase
};
