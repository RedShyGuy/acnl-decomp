#pragma once

#include "decomp.h"
#include "Other/dInstSelect.h"

// RTTI 10TicketList @ 0x008CB208
// vtable 0x008EC584 (vptr 0x008EC58C), offset_to_top 0, 26 entries
class TicketList : public ::InstSelect<8>
{
public:
    TicketList(); // ctor candidate(s) 0x007F4C90 (unverified)
    virtual ~TicketList(); // 0x001B7890 slot 0x00 | slot vf_0x00 of CatalogBase
    // 0x001B7834 slot 0x04 | slot vf_0x04 of CatalogBase (deleting dtor)
    void Finalize() override; // 0x001B74F8 slot 0x0C
    s32 GetSelectedId() const override; // 0x0070CE18 slot 0x20
    bool vf_0x28() override; // 0x0070CE10 slot 0x28
    u32 vf_0x2C() override; // 0x0070CE38 slot 0x2C
    bool SetupItems() override; // 0x001B7524 slot 0x34
    void UpdateWait() override; // 0x00692554 slot 0x60
    script::IWord* GetItemWord(s32 index) override; // 0x0070CE20 slot 0x64
};
