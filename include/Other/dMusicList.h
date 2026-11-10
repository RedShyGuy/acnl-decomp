#pragma once

#include "decomp.h"
#include "Other/dInstSelect.h"

// RTTI 9MusicList @ 0x008CD7B4
// vtable 0x008FA6D4 (vptr 0x008FA6DC), offset_to_top 0, 26 entries
class MusicList : public ::InstSelect<8>
{
public:
    MusicList(); // ctor candidate(s) 0x007F4E10 (unverified)
    virtual ~MusicList(); // 0x006E7890 slot 0x00 | slot vf_0x00 of CatalogBase
    // 0x006E7850 slot 0x04 | slot vf_0x04 of CatalogBase (deleting dtor)
    s32 GetSelectedId() const override; // 0x00770BC4 slot 0x20
    bool SetupItems() override; // 0x006E77FC slot 0x34
    script::IWord* GetItemWord(s32 index) override; // 0x00770BE0 slot 0x64
};
