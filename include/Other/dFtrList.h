#pragma once

#include "decomp.h"
#include "Other/dInstSelect.h"

// RTTI 7FtrList @ 0x008CD3B4
// vtable 0x008F90F0 (vptr 0x008F90F8), offset_to_top 0, 26 entries
class FtrList : public ::InstSelect<8>
{
public:
    FtrList(); // ctor address unknown
    virtual ~FtrList(); // 0x0060DA4C slot 0x00 | slot vf_0x00 of CatalogBase
    // 0x0060D918 slot 0x04 | slot vf_0x04 of CatalogBase (deleting dtor)
    void Finalize() override; // 0x0060D280 slot 0x0C
    void Update() override; // 0x0060D2FC slot 0x10
    void Draw() override; // 0x0060D254 slot 0x14
    bool IsDecided() const override; // 0x0075E51C slot 0x18
    bool IsDeciding() override; // 0x0075E544 slot 0x1C
    s32 GetSelectedId() const override; // 0x0075E5B8 slot 0x20
    void InitializeContents(ssys::ma::lyt::ArcResAccReader* arcReader) override; // 0x0060D114 slot 0x30
    bool SetupItems() override; // 0x0060D5C4 slot 0x34
    void InitializeScroll() override; // 0x0060D230 slot 0x38
    void UpdateItemNum() override; // 0x0060D1E8 slot 0x44
    void OnEnterIn() override; // 0x0060D578 slot 0x4C
    void OnEnterOut() override; // 0x0060D5A4 slot 0x54
    void UpdateWait() override; // 0x0060D730 slot 0x60
    script::IWord* GetItemWord(s32 index) override; // 0x0075E5F4 slot 0x64
};
