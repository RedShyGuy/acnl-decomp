#pragma once

#include "decomp.h"
#include "Other/dPWorkList.h"

// RTTI 15PWorkDeleteList @ 0x008CC104
// vtable 0x008F1DDC (vptr 0x008F1DE4), offset_to_top 0, 26 entries
class PWorkDeleteList : public ::PWorkList
{
public:
    PWorkDeleteList(); // ctor candidate(s) 0x007F4D50 (unverified)
    virtual ~PWorkDeleteList(); // 0x0029EEFC slot 0x00 | slot vf_0x00 of CatalogBase
    // 0x0029EE90 slot 0x04 | slot vf_0x04 of CatalogBase (deleting dtor)
    void Finalize() override; // 0x0022344C slot 0x0C
    void Update() override; // 0x00222E74 slot 0x10
    void Draw() override; // 0x00222B20 slot 0x14
    s32 GetSelectedId() const override; // 0x0071CA24 slot 0x20
    bool vf_0x28() override; // 0x0071CA1C slot 0x28
    u32 vf_0x2C() override; // 0x0071CA40 slot 0x2C
    bool SetupItems() override; // 0x0029EA84 slot 0x34
    void OnScroll() override; // 0x0029E7D0 slot 0x3C
    void OnSelect(s32 selected, s32 prev) override; // 0x0029E754 slot 0x40
    void OnEnterIn() override; // 0x0029E918 slot 0x4C
    void UpdateIn() override; // 0x0029E864 slot 0x50
    void OnEnterOut() override; // 0x0029EA28 slot 0x54
    void UpdateOut() override; // 0x0029E96C slot 0x58
    void UpdateWait() override; // 0x0029EDFC slot 0x60
};
