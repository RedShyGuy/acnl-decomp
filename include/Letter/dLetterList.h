#pragma once

#include "decomp.h"
#include "Other/dInstSelect.h"

// RTTI 10LetterList @ 0x008CB0FC
// vtable 0x008EC240 (vptr 0x008EC248), offset_to_top 0, 26 entries
class LetterList : public ::InstSelect<8>
{
public:
    LetterList(); // ctor candidate(s) 0x007F4BB4 (unverified)
    virtual ~LetterList(); // 0x001AD488 slot 0x00 | slot vf_0x00 of CatalogBase
    // 0x001AD448 slot 0x04 | slot vf_0x04 of CatalogBase (deleting dtor)
    virtual void vf_0x20(); // 0x0070BAA4 slot 0x20 | virtual slot, introduced by CatalogBase
    virtual void vf_0x34(); // 0x001AD3EC slot 0x34 | virtual slot, introduced by CatalogBase
    virtual void vf_0x64(); // 0x0070BAC0 slot 0x64 | virtual slot, introduced by SelectBase
};
