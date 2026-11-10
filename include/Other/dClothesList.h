#pragma once

#include "decomp.h"
#include "Other/dInstSelect.h"

// RTTI 11ClothesList @ 0x008CB2A8
// vtable 0x008ECA84 (vptr 0x008ECA8C), offset_to_top 0, 26 entries
class ClothesList : public ::InstSelect<8>
{
public:
    ClothesList(); // ctor address unknown
    virtual ~ClothesList(); // 0x001C7DFC slot 0x00 | slot vf_0x00 of CatalogBase
    // 0x001C7C90 slot 0x04 | slot vf_0x04 of CatalogBase (deleting dtor)
    void Finalize() override; // 0x001C7474 slot 0x0C
    void Update() override; // 0x001C74FC slot 0x10
    void Draw() override; // 0x001C7448 slot 0x14
    bool IsDecided() const override; // 0x0070E59C slot 0x18
    bool IsDeciding() override; // 0x0070E5C4 slot 0x1C
    s32 GetSelectedId() const override; // 0x0070E638 slot 0x20
    void InitializeContents(ssys::ma::lyt::ArcResAccReader* arcReader) override; // 0x001C6F48 slot 0x30
    bool SetupItems() override; // 0x001C77CC slot 0x34
    void InitializeScroll() override; // 0x001C7420 slot 0x38
    void UpdateItemNum() override; // 0x001C73D8 slot 0x44
    void OnEnterIn() override; // 0x001C7780 slot 0x4C
    void OnEnterOut() override; // 0x001C77AC slot 0x54
    void UpdateWait() override; // 0x001C7A7C slot 0x60
    script::IWord* GetItemWord(s32 index) override; // 0x0070E670 slot 0x64
};
