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
    void Finalize() override; // 0x001C6228 slot 0x0C
    void Update() override; // 0x001C6300 slot 0x10
    void Draw() override; // 0x001C594C slot 0x14
    bool IsDecided() const override; // 0x0070E53C slot 0x18
    bool IsDeciding() override; // 0x0070E554 slot 0x1C
    void InitializeContents(ssys::ma::lyt::ArcResAccReader* arcReader) override; // 0x001C5D94 slot 0x30
    bool SetupItems() override; // 0x001C6784 slot 0x34
    void InitializeScroll() override; // 0x001C6208 slot 0x38
    void OnScroll() override; // 0x001C6040 slot 0x3C
    void UpdateItemNum() override; // 0x001C61D8 slot 0x44
    void OnEnterIn() override; // 0x001C64E4 slot 0x4C
    void OnEnterOut() override; // 0x001C650C slot 0x54
    void UpdateWait() override; // 0x001C6A84 slot 0x60
};
