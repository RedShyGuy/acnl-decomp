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
    void Finalize() override; // 0x001B0304 slot 0x0C
    s32 GetSelectedId() const override; // 0x0070BD48 slot 0x20
    bool SetupItems() override; // 0x001AF814 slot 0x34
    script::IWord* GetItemWord(s32 index) override; // 0x0070BD7C slot 0x64
};
