#pragma once

#include "decomp.h"
#include "Other/dInstSelect.h"

// RTTI 12ExcavateList @ 0x008CB608
// vtable 0x008EE434 (vptr 0x008EE43C), offset_to_top 0, 26 entries
class ExcavateList : public ::InstSelect<8>
{
public:
    ExcavateList(); // ctor address unknown
    virtual ~ExcavateList(); // 0x00200C38 slot 0x00 | slot vf_0x00 of CatalogBase
    // 0x00200B50 slot 0x04 | slot vf_0x04 of CatalogBase (deleting dtor)
    void Finalize() override; // 0x0020054C slot 0x0C
    void Update() override; // 0x002005D4 slot 0x10
    void Draw() override; // 0x00200520 slot 0x14
    bool IsDecided() const override; // 0x00713050 slot 0x18
    bool IsDeciding() override; // 0x00713078 slot 0x1C
    s32 GetSelectedId() const override; // 0x007130EC slot 0x20
    void InitializeContents(ssys::ma::lyt::ArcResAccReader* arcReader) override; // 0x00200038 slot 0x30
    bool SetupItems() override; // 0x002008A8 slot 0x34
    void InitializeScroll() override; // 0x002004FC slot 0x38
    void UpdateItemNum() override; // 0x002004BC slot 0x44
    void OnEnterIn() override; // 0x0020085C slot 0x4C
    void OnEnterOut() override; // 0x00200888 slot 0x54
    void UpdateWait() override; // 0x002009AC slot 0x60
    script::IWord* GetItemWord(s32 index) override; // 0x00713124 slot 0x64
};
